

#include "../blackBoxes_prot.h"

using namespace std;


void scanBtCfg(const char* cfgFileName){ //scan back-test config file.


	struct stat fileNameStatus; //records status of fileName.
	statError(cfgFileName, fileNameStatus); //check if file exists.
	bool multi = true; //boolean to be set in file.
	vector<string>strategies;
	ifstream inFile(cfgFileName); // object for handling file input
	string input;
	while (input == "\0"){getline(inFile, input);}
	if ((input.find("backtest")== string::npos) &&
			(input.find("back-test")== string::npos) &&
			(input.find("Backtest")== string::npos) &&
			(input.find("Back-test")== string::npos)){
		cerr << "First input of file must contain the word Back-test. Exiting" << endl;
		exit(EXIT_FAILURE);
	}

	while (inFile.peek() == '\n'){inFile.get();}
	getline(inFile, input);

	if (input != "Option:"){
		cerr << "Next input in file should have contained Option:\n"
				"but was " << input << " Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	while (inFile.peek() == '\n'){inFile.get();}
	getline(inFile, input);

	if (input == "-m") // multiFile,
		multi = true;
	else if (input == "-b") //non multiFile
		multi = false;
	else{
		cerr << "No option found. Possible options are -m or -b:\n"
				"and must reside on same input as Option:." << endl;
		exit(EXIT_FAILURE);
	}

	while (inFile.peek() == '\n'){inFile.get();}
	getline(inFile, input);

	if (input != "Data:"){
		cerr << "Next input in file should have contained Data:\n"
				"but was " << input << " Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	while (inFile.peek() == '\n'){inFile.get();}
	getline(inFile, input);  //get fileName
	string fileName = input.c_str(); //found non-empty input, assume it is fileName.

	statError(fileName.c_str(), fileNameStatus); //check if file exists.

	while (inFile.peek() == '\n'){inFile.get();}
	getline(inFile, input);

	if ((input != "Strategy:") && (input != "Strategies:")){
		cerr << "Next input in file should have contained Strategy(ies):\n"
				"but was " << input << " Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	while (inFile.peek() == '\n'){inFile.get();}

	while (getline(inFile, input)){
		while (inFile.peek() == '\n'){inFile.get();}
		strategies.push_back(input); //add strategy name to list of strats.

	}

	if (strategies.size() < 1){
		cerr << "You must specify at least one strategy in config file.\n"
				"Exiting" << endl;
		exit(EXIT_FAILURE);
	}

	scanForFiles(fileName.c_str(), multi, strategies); //run scanForFiles.

}

void scanForFiles(const char* fileName, bool multi, vector<string>stratNames){ //function for back-tests only. Bool used to identify if files are independent or not.

	struct stat fStatus;
	struct stat dStatus; //records writeTo status
	int dirStatus;

	statError(fileName, fStatus); //find out if there was an error.

    string writeTo = fileName; writeTo += "_strategies_results";
    string baseResDir = writeTo;

    if (stat(writeTo.c_str(), &dStatus) == -1){ //if folder does not alread exist
        dirStatus = mkdir(writeTo.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //create directory to write to.
        if(dirStatus == -1)															//is the base folder required to print solutions.
        	cerr << "could not create: " << writeTo << " error: " << errno << endl; //return error report to standard error output.
    }


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//If we are dealing with a directory, program will detect whether
	//we are dealing with a normal backtest or a backtest of type -m,
	//using the multi bool.
	//If multi bool is set to false, program will read through the folder trying to detect all files
	//and calling toOpen function on all of them. The strategies to be compared will be set
	//while reading the first file.
	//If however multi bool is set to true, the program will try to detect if the folder
	//contains files or folders.
	//	If it contains folder, it will run through all folders, setting the strategies to be
	//		back-tested while in the first folder.
	//	If however, it does not contains plain data files, it will extract them at the same time
	//		and try to run strategies on them as required by user or program setting them.
	//		Note: other programs, as the toOpen function can be called with already predetermined
	//		strategies to be called upon a particular file or folder.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (S_ISDIR(fStatus.st_mode)){

		DIR *directoryp; //declare directory pointer
		directoryp = opendir(fileName);
		struct dirent *internFP; //declare internal file pointer
	    internFP = readdir(directoryp); //read first file of folder.
	    const string folderString = fileName;


	    while ((internFP == NULL) || (strcmp(internFP->d_name, ".") == 0) ||
	    		(strcmp(internFP->d_name, "..") == 0)){ //abort, folder is empty.

		    if (internFP == NULL){
		    	cerr << "Folder is empty. Aborting.\n ";
		    	exit(EXIT_FAILURE);
		    }
	    	internFP = readdir(directoryp); //if any file has a different name, it will not end up to internFP == NULL
	    }



	    if (multi == false){ //if files are read independently from one another

			cout << "You entered a path to a directory with \'-b\' option" << endl;
			cout << "Program will try going through all files.\n";

		    string dataFile;
		    dataFile = folderString + "/" + internFP->d_name;


		    cout << dataFile << " will soon be extracted" << endl;

    		writeTo = getSubFolderName(dataFile, baseResDir); //get proper writeTo name.
    	    if (stat(writeTo.c_str(), &dStatus) == -1){ //if folder does not alread exist
    	        dirStatus = mkdir(writeTo.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //create directory to write to.
    	        if(dirStatus == -1)
    	        	cerr << "could not create: " << writeTo << " error: " << errno << endl; //return error report to standard error output.
    	    }

		    toOpen(dataFile, writeTo, stratNames);
		    //run default mode on first file and return strategies to use for all files in stratNames if necessary.

		    while ((internFP = readdir(directoryp)) != NULL){

		    	dataFile = folderString + "/" + internFP->d_name; //update dataFile.
		    	writeTo = dataFile + "_strategies_results";
		    	if((dataFile == folderString + "/.") || (dataFile == folderString + "/..")) //if readdir returned . or ..
		    		continue;
		    	else{
		    		writeTo = getSubFolderName(dataFile, baseResDir); //get proper writeTo name.
		    	    if (stat(writeTo.c_str(), &dStatus) == -1){ //if folder does not alread exist
		    	        dirStatus = mkdir(writeTo.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //create directory to write to.
		    	        if(dirStatus == -1)
		    	        	cerr << "could not create: " << writeTo << " error: " << errno << endl; //return error report to standard error output.
		    	    }
		    		toOpen(dataFile, writeTo, stratNames);
		    	}

		    }

		    cout << "Back-test completed for all files. \n"
		    		"refer to created files for more information." << endl;
		    exit(EXIT_SUCCESS);
	    }



	    else{ //if files are dependent from one another,

		    string currFileOrFolder;
		    vector<string>dataFiles;
		    currFileOrFolder = folderString + "/" + internFP->d_name;

	    	struct stat firstFileStatus;

	    	statError(currFileOrFolder.c_str(), firstFileStatus);

	    	if (S_ISDIR(firstFileStatus.st_mode)){ //we have a folder of folder(s), do backtests on all folders containing dependent files.
	    										   //writeTo will have to be updated at every iteration of while loop.

				cout << "You entered a path to a directory with \'-m\' option" << endl;
				cout << "Program will try going through all folders.\n";

				writeTo = getSubFolderName(currFileOrFolder, baseResDir); //get proper writeTo name.
	    	    if (stat(writeTo.c_str(), &dStatus) == -1){ //if folder does not alread exist
	    	        dirStatus = mkdir(writeTo.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //create directory to write to.
	    	        if(dirStatus == -1)
	    	        	cerr << "could not create: " << writeTo << " error: " << errno << endl; //return error report to standard error output.
	    	    }

			    dataFiles = getNames(currFileOrFolder);
			    toOpen(dataFiles, (const string&) writeTo, stratNames); //get stratsToRun on first folder.

			    while ((internFP = readdir(directoryp)) != NULL){

			    	currFileOrFolder = folderString + "/" + internFP->d_name; //update currFileOrFolder

			    	if((currFileOrFolder == folderString + "/.") || (currFileOrFolder == folderString + "/..")) //if readdir returned . or ..
			    		continue;
			    	else{
			    		dataFiles = getNames(currFileOrFolder); //update dataFiles to files in current folder.
			    		writeTo = getSubFolderName(currFileOrFolder, baseResDir); //get proper writeTo name.
			    	    if (stat(writeTo.c_str(), &dStatus) == -1){ //if folder does not alread exist
			    	        dirStatus = mkdir(writeTo.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);  //create directory to write to.
			    	        if(dirStatus == -1)
			    	        	cerr << "could not create: " << writeTo << " error: " << errno << endl; //return error report to standard error output.
			    	    }
			    		toOpen(dataFiles, writeTo, stratNames); //run strategies on folder.
			    	}

			    }


	    	}



	    	else if (S_ISREG(firstFileStatus.st_mode)){ //we have a folder of data files.
				cout << "You entered a path to a directory with \'-m\' option" << endl;
				cout << "Program will try going through all files.\n";
				dataFiles = getNames(folderString);
				toOpen(dataFiles, writeTo, stratNames); //toOpen will quite simply take care of the rest here.
	    	}


		    cout << "Back-test completed for all files. \n"
		    		"refer to created file(s) for more information." << endl;
		    exit(EXIT_SUCCESS);

	    }





	}



	else if (S_ISREG(fStatus.st_mode)){
		cout << "You entered a path to a regular file," << endl <<
				"Program will try extracting it now." << endl;
		toOpen(fileName, writeTo, stratNames);

	    cout << "Back-test completed for file. \n"
	    		"refer to created file(s) for more information." << endl;
	    exit(EXIT_SUCCESS);
	}

	else{
		cout << "\"-b\" and \"-m\" option takes either a folder or a file as argument.\n"
				"File type unrecognized. Exiting" << endl;
		exit(EXIT_FAILURE);
	}


}



void updateData (const char* dataFolder= "/run/media/john-d/Iomega_Ext_Drive/Data"){ //default folder can be changed.

	tryOpenFolder(dataFolder);
	cout << "\nupdating data:\n";

}

void tryOpenFolder (const char* folder){

	struct stat fStat;

	if (stat(folder, &fStat) == -1) {
		perror("stat");
	    exit(EXIT_FAILURE);
	}

	if (!S_ISDIR(fStat.st_mode)){
		cerr << "You have not entered a path to a directory. Exiting" << endl;
		exit(EXIT_FAILURE);
	}


	DIR *folderp; //declare directory pointer
	if((folderp = opendir(folder)) == NULL){ // if folder could not be open for any reason
		cout << "Folder could not be opened.\nYou might not have read privileges to the folder Exiting" << endl;
		exit(EXIT_FAILURE);
	}
}

const char* searchFileNonRec (const char* folder, const char* searchString){  //search folder for a specific filename.
																			  //Return file only if it is the only one in folder.
	DIR *directoryp; //declare directory pointer
	directoryp = opendir(folder);
	int fileTypeCount = 0;
	const string folderString = folder;
	string newFile;
	string foundFile;

	struct dirent *internFP; //declare internal file pointer
    while ((internFP = readdir(directoryp)) != NULL) {
        newFile = folderString + "/" + internFP->d_name;
        if((newFile == folderString + "/.") || (newFile == folderString + "/..")) //if readdir returned . or ..
        	continue;

        else if((newFile.find(searchString)!= string::npos) && (newFile.find("~") == string::npos )){ //file found, discard recovery file.
        	foundFile = newFile;
        	++fileTypeCount;
        }

    }
    closedir(directoryp);

    if (fileTypeCount > 1){
        cerr << "Fatal error, multiple \'" << searchString << "\' files not. Exiting" << endl;
        exit (EXIT_FAILURE);
    }
    else if (fileTypeCount == 0){
        cerr << "Fatal error \'" << searchString << "\' file not found. Exiting" << endl;
        exit (EXIT_FAILURE);
    }
    else
    	return foundFile.c_str();

}

string getSubFolderName(string subFolder, const string& baseDir){

	size_t found;
	string writeTo;
	found=subFolder.find_last_of("/");
	subFolder = subFolder.substr(found+1); //subFolder is equal only to the name of the folder and not full path

	writeTo = baseDir + "/" + subFolder;

	return writeTo;
}

void statError(const char* fileName, struct stat& fStatus){

	string fileNameString = fileName;
	if (stat(fileName, &fStatus) == -1) {
		cerr << "File not found: " << fileNameString << endl;
	    exit(EXIT_FAILURE);
	}
}

vector<string> getNames(string folderString){

	DIR *directoryp;
	directoryp = opendir(folderString.c_str());
	struct dirent *internFP; //declare internal file pointer
	struct stat fStatus;
	string currFile;
	vector<string>dataFiles;
    while ((internFP = readdir(directoryp)) != NULL){

    	currFile = folderString + "/" + internFP->d_name; //update dataFile.
    	statError(currFile.c_str(), fStatus);

    	if((currFile == folderString + "/.") || (currFile == folderString + "/..")) //if readdir returned . or ..
    		continue;

    	else{
        	if (!S_ISREG(fStatus.st_mode)){
        		cerr << "file: " << currFile << " could not be opened. Exiting." << endl;
        		exit(EXIT_FAILURE);
        	}
    		dataFiles.push_back(currFile);
    	}
    }

    return dataFiles;
}
