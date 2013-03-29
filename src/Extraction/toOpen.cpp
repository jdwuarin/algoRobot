 /*
 * toOpen.cpp
 *  * Contains all the functions used after user selected what kind of data
 * he/she wanted to extract, the number of files, whether there will be
 * joining files with macro indicators or others, etc.
 *
 *  Created on: Apr 25, 2012
 *      Author: John_David Wuarin
 */

#include "../blackBoxes_prot.h"
const double STD_EX_SIZE = 200000000; // standard size of blocks to be extracted with large files.
double getNbrRows(vector<Asset*>&assets);

void toOpen (const string& dataFile, const string& writeTo,
		vector<string>&stratNames){
	vector<stratFunction>stratsToRun;
	struct stat fStatus;
	stratFunction currStrat; //records current strategy to add to test list
	double before;
	before = time(0);
	vector<Asset*>assets; assets.push_back(NULL); //create a pointer to an Asset object containing usually sorted data.
    unsigned int i = 0; //used to run through stratToRun.
    bool runDirect = true; //boolean indicating if strats can be run directly in getStrats.
    					   //Only files smaller than 1.2*STD_EX_SIZE can be tested directly.


	statError(dataFile.c_str(), fStatus);

	if (fStatus.st_size > 1.2*STD_EX_SIZE){ //if data file is larger than STD_EX_SIZE + 100MB

		runDirect = false; //can't run strat directly
	    long firstDateToKeep = LONG_MAX;
	    long newDate = LONG_MAX;

		/*******************************If The File contains a c-p flag using containsCpFlag function to test condition.*************************************/

		if (containsCpFlag(dataFile)){
			vector<string>dataFiles;
			dataFiles.push_back(dataFile);
			toOpen(dataFiles, writeTo, stratNames); //run all cpflag containing files in other toOpen function.
			return;
		}

		else{
			cout << "Extracting and running strategies to write to " << writeTo << endl;
			cout << "File size: " << fStatus.st_size/1000000 << "MB" << endl;

			detectData(dataFile, assets.at(0), writeTo, 0, STD_EX_SIZE); //pointers are initialized.

			stratsToRun = getStrats(assets, stratNames, runDirect); //get list of strategies to run.

			while (assets.at(0)->getExFromByte() > -1){

				double untilByte; //used to record what byte to extract file until
				firstDateToKeep = assets.at(0)->getLastDate();//set first Date To Keep at last date by default.
																		 //if not specified otherwise, program will erase all but last date.
				//run strategies update newDate

				for (i=0; i<stratsToRun.size(); ++i){
					newDate = stratsToRun.at(i)(); //run stratFunction, get earliest date to keep.
					if (newDate < firstDateToKeep)
						firstDateToKeep = newDate;

				}


				if ((firstDateToKeep < assets.at(0)->getDate(0)) ||
						(firstDateToKeep > assets.at(0)->getLastDate())){
					cout << "One of the strategies returned an error value as a date, report bug. Exiting:" << endl;
					exit(EXIT_FAILURE);
				}

				//erase already used rows.
				double keepFromRow = assets.at(0)->findDate(firstDateToKeep);
				cout << firstDateToKeep << " " << assets.at(0)->getNbrRows() << " Rows; Keep from row: " << keepFromRow << endl;
				assets.at(0)->eraseRows(0, keepFromRow);

				//get next rows.

				untilByte = assets.at(0)->getExFromByte() + STD_EX_SIZE - assets.at(0)->getNbrBytes();
				detectData(dataFile, assets.at(0), writeTo, 0, untilByte);


			}

			for (i=0; i<stratsToRun.size(); ++i){
				newDate = stratsToRun.at(i)(); //run stratFunction, get earliest date to keep.
				if (newDate < firstDateToKeep)
					firstDateToKeep = newDate;

			}

		}



	}



	else { //file is small
		if (containsCpFlag(dataFile)){
			vector<string>dataFiles;
			dataFiles.push_back(dataFile);
			toOpen(dataFiles, writeTo, stratNames); //run all cpflag containing files in other toOpen function.
			return;
		}

		else{ //does not contain the cp flag.

			cout << "Extracting and running strategies to write to " << writeTo << endl;
			cout << "File size: " << fStatus.st_size/1000 << "KB" << endl;

			detectData(dataFile, assets.at(0), writeTo); //pointer is initialized.

			stratsToRun = getStrats(assets, stratNames, runDirect); //get list of strategies to run.

			//run strategies

			for (i=0; i<stratsToRun.size(); ++i)
				stratsToRun.at(i)(); //run stratFunction, get earliest date to keep.


		}

	}

	cout << "It took " << time(0) - before << " seconds to extract " <<
			getNbrRows(assets)<< " lines" <<endl;

	for (unsigned int i = 0; i< assets.size(); ++i){
		delete assets[i];
	}

}




void toOpen (vector<string>&dataFiles, const string& writeTo,
		vector<string>&stratNames){
	vector<stratFunction>stratsToRun;
	double dataFilesSize = getFilesSize(dataFiles); // get total size of
	struct stat currFileStatus; //records the size of the current file.
	stratFunction currStrat; //records current strategy to add to test list
	double before;
	before = time(0);
	vector<Asset*>assets; //vector containing all the assets.
	assets.push_back(NULL); //initialize assets[0] to NULL.
	unsigned int i = 0; //run through dataFiles
	unsigned int j = 0; //run through assets
	unsigned int k = 0; //run through stratsToRun
    bool runDirect = true; //boolean indicating if strats can be run directly in getStrats.
    					   //Only files smaller than 1.2*STD_EX_SIZE can be tested directly.

	cout << "Extracting and running strategies to write to " << writeTo << endl;
	cout << "File(s) size: " << dataFilesSize/1000000 << "MB" << endl;

	if (dataFilesSize > 1.2*STD_EX_SIZE ){ //if data files are jointly larger than 600 MB.

		runDirect = false;
		double firstSizeRatio; //records the first file size ratio with respect to total files size
	    long firstDateToKeep = LONG_MAX;
	    long newDate = LONG_MAX;

		statError(dataFiles.at(i).c_str(), currFileStatus); //assign currFileStatus to file and check for error.
		firstSizeRatio = ((double) currFileStatus.st_size)/dataFilesSize;
		if(containsCpFlag(dataFiles.at(0))){
			assets.push_back(NULL); //add Asset* object for puts.
			detectData(dataFiles.at(0), assets.at(0), assets.at(1), writeTo, 0, floor(firstSizeRatio*STD_EX_SIZE)); //pointers are initialized.
			j+=2; //increment j by two for two assets.
		}
		else{
			detectData(dataFiles.at(0), assets.at(0), writeTo, 0, floor(firstSizeRatio*STD_EX_SIZE)); //pointer is initialized.
			j++; // just add one to j.
		}

		for(i=1; i < dataFiles.size(); ++i){ //only go in if there are more than one file in dataFiles vector.

			assets.push_back(NULL); //there is at least one new Asset.
			if(containsCpFlag(dataFiles.at(i))){
				assets.push_back(NULL); //add Asset* object for puts.
				detectData(dataFiles.at(i), assets.at(j), assets.at(j+1), writeTo, assets.at(0)->getExToDate()); //pointers are initialized.
				j+=2; //increment j by two for two assets.
			}
			else{
				detectData(dataFiles.at(i), assets.at(j), writeTo, assets.at(0)->getExToDate()); //pointer is initialized.
				j++; // just add one to j.
			}

		}

		stratsToRun = getStrats(assets, stratNames, runDirect); //get list of strategies to run.

		while (assets.at(0)->getExFromByte() > -1){ //default starts from here, as stratsToRun would contain something.

			double untilByte; //used to record what byte to extract first file until
			firstDateToKeep = assets.at(0)->getLastDate(); //set first Date To Keep at last date of first file by default.
																	 //if not specified otherwise, program will erase all but last date.
			//run strategies update newDate

			for (k=0; k<stratsToRun.size(); ++k){

				newDate = stratsToRun.at(k)(); //run stratFunction, get earliest date to keep.
				if (newDate < firstDateToKeep)
					firstDateToKeep = newDate;

			}


			if ((firstDateToKeep < assets.at(0)->getDate(0)) ||
					(firstDateToKeep > assets.at(0)->getLastDate())){
				cout << "One of the strategies returned an error value as a date, report bug. Exiting:" << endl;
				exit(EXIT_FAILURE);
			}

			//erase already used rows.

			double keepFromRow;
			for (j=0; j < assets.size(); ++j){
				keepFromRow = assets.at(j)->findDate(firstDateToKeep);
				cout << firstDateToKeep << " " << assets.at(j)->getNbrRows() << " Rows; Keep from row: " << keepFromRow << endl;
				assets.at(j)->eraseRows(0, keepFromRow);
			}
			//get next rows.

			j = 0; //reset j to 0.
			untilByte = assets.at(0)->getExFromByte() + floor(firstSizeRatio*STD_EX_SIZE)
					- assets.at(0)->getNbrBytes(); //set untilByte for forst file.
			if(containsCpFlag(dataFiles.at(0))){
				detectData(dataFiles.at(0), assets.at(0), assets.at(1), writeTo, 0, untilByte); //continue extracting first file
				j+=2; //increment j by two for two assets.
			}
			else{
				detectData(dataFiles.at(0), assets.at(0), writeTo, 0, untilByte); //pointer is initialized.
				j++; // just add one to j.
			}

			for(i=1; i < dataFiles.size(); ++i){ //only go in if there are more than one file in dataFiles vector.
				if(containsCpFlag(dataFiles.at(i))){
					detectData(dataFiles.at(i), assets.at(j), assets.at(j+1), writeTo, assets.at(0)->getExToDate()); //continue extracting other files
					j+=2; //increment j by two for two assets.
				}
				else{
					detectData(dataFiles.at(i), assets.at(j), writeTo, assets.at(0)->getExToDate()); //continue extracting other files
					j++; // just add one to j.
				}

			}

		}


		for (k=0; k<stratsToRun.size(); ++k){

			newDate = stratsToRun.at(k)(); //run stratFunction, get earliest date to keep.
			if (newDate < firstDateToKeep)
				firstDateToKeep = newDate;

		}



	}

	else{ //files are jointly smaller than what was tested.

		statError(dataFiles.at(i).c_str(), currFileStatus); //assign currFileStatus to file and check for error.
		if(containsCpFlag(dataFiles.at(0))){
			assets.push_back(NULL); //add Asset* object for puts.
			detectData(dataFiles.at(0), assets.at(0), assets.at(1), writeTo); //pointers are initialized.
			j+=2; //increment j by two for two assets.
		}
		else{
			detectData(dataFiles.at(0), assets.at(0), writeTo); //pointer is initialized.
			j++; // just add one to j.
		}
		for(i=1; i < dataFiles.size(); ++i){ //only go in if there are more than one file in dataFiles vector.

			assets.push_back(NULL); //there is at least one new Asset.
			if(containsCpFlag(dataFiles.at(i))){
				assets.push_back(NULL); //add Asset* object for puts.
				detectData(dataFiles.at(i), assets.at(j), assets.at(j+1), writeTo); //pointers are initialized.
				j+=2; //increment j by two for two assets.
			}
			else{
				detectData(dataFiles.at(i), assets.at(j), writeTo); //pointer is initialized.
				j++; // just add one to j.
			}

		}

		stratsToRun = getStrats(assets, stratNames, runDirect); //get list of strategies to run.

		for (k=0; k<stratsToRun.size(); ++k)
			stratsToRun.at(k)(); //run stratFunction

	}

	cout.precision(10);
	cout << "It took " << time(0) - before << " seconds to extract " <<
			getNbrRows(assets)<< " lines" <<endl;
	cout.precision(6);

	for (unsigned int i = 0; i< assets.size(); ++i){
		delete assets.at(i);
	}
}





bool containsCpFlag(const string& dataFile){

	ifstream inFile(dataFile.c_str()); // object for handling file input
	if (!inFile.is_open()) // failed to open file
	{
		cerr << "Could not open the file " << endl;
		cerr << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	string row;
	getline(inFile,row);
	if ((row.find("cp") != string::npos) || (row.find("Cp") != string::npos))
		return(true);
	else
		return(false);
}

long getFilesSize(vector<string>&dataFiles){

	unsigned int i;
	long totalSize = 0;
	for (i = 0; i<dataFiles.size(); ++i){
		struct stat fStatus;

		statError(dataFiles.at(i).c_str(), fStatus);

		totalSize+= fStatus.st_size;
	}

	return (totalSize);

}

double getNbrRows(vector<Asset*>&assets){

	double nbrRows = 0;

	unsigned int i;

	for (i = 0; i< assets.size(); ++i){
		nbrRows+= assets.at(i)->getExRows();
	}

	return nbrRows;
}
