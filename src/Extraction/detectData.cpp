


/*
 *  detectData.cpp
 *
 *  Created on: Mar 18, 2012
 *      Author: john-d
 *	In this
 */


#include "../blackBoxes_prot.h"


void detectData(const string& dataFile, Asset*& calls, Asset*& puts, const string& writeTo,
		long lastDate, double lastByte){ //function used to detect data if data contains the cp_flag
										 //function returns number of bytes extracted.

	ifstream inFile(dataFile.c_str()); // object for handling file input
	string row;
	stringstream srow;
	getline(inFile,row);
	srow.str(row); //replace old row with new row;
	//used to find out what column I am in, and thus what deque to extract the data to.
	Columns columnHeaders; //instantiate an object of Columns class.

	//find out if we are using a european or american style csv file. Following conditions
	char delimiter = getDelim(row, dataFile);


	columnHeaders.setColNbr(srow, delimiter); //set column numbers
	columnHeaders.checkValid(dataFile); //check if data is valid according to column numbers
	int cpColNbr = columnHeaders.getCpColNbr();
	string callOrPutRow; //used to determine what type of option is contained in row
	int i = 0; //used for collOrPutRow

	if ((calls == NULL)){ // we initialize here the proper options objects to contain our data. This is done only at the beginning of a file.


		vector<Asset**>dummyAssets; dummyAssets.push_back(&calls); dummyAssets.push_back(&puts);//just a dummy assets used for initiateAsset.
		columnHeaders.initiateAsset(dummyAssets, dataFile); //find what the Assets object will point to depending on the rows identity.
		//uses pointer to pointer, so that the copy made automatically is not a copy of our pointer to initialize, but a copy to
		//the pointer to those addresses.

		calls->setExFromByte((double)inFile.tellg());  //skip first line for rest of code as I am starting at beginning of file.
													   //!!!!information has to be set in both objects.
		puts->setExFromByte((double)inFile.tellg());
		calls->setWriteTo(writeTo); //setting folder to write to, using data passed as argument to detectData.
		puts->setWriteTo(writeTo);
	}


	//we first detect what type of options object we are dealing with
	//and save all the data to two single deques of row objects in order to sort columns in parallel.

	bool optionsOrNot = calls->getIsOption();

	////////////////////////////////////////////////////////////////////
	//if Options data was entered, find out what kind of options it was.
	////////////////////////////////////////////////////////////////////
	if (optionsOrNot == true) {

		//Skip to wanted position, after first line if new file, and as of last accessed byte
		//if return file.
		inFile.seekg(calls->getExFromByte(), ios_base::beg);
		double before = time(0);
		while ((inFile.eof() != 1)){ //while I am in the file

			getline(inFile, row); //get next line.
			srow.clear(); //clear eofbit set from last line.
			srow.str(row); //replace old row with new row.

			for(i = 0; i < cpColNbr; ++i)
				getline(srow, callOrPutRow, delimiter);

			srow.seekg(0); //reset row for srow

			if(callOrPutRow.find("C") != string::npos){ //we have a call
				calls->extractRow(srow, delimiter); //row is extracted and put in temporary values
				//line is now completely extracted. Now dealing with file size and multiple files part
				if(calls->fileSizeManagement(lastDate, lastByte, inFile) == true){ //file read status set only for calls.
					puts->setExToDate(calls->getExToDate());
					puts->setExFromByte(calls->getExFromByte());
					break; //break only if member function returned to break, otherwise, do nothing.
				}
				calls->writeCurrValues();
			}
			else{
				puts->extractRow(srow, delimiter); //row is extracted and put in temporary values
				//line is now completely extracted. Now dealing with file size and multiple files part
				if(puts->fileSizeManagement(lastDate, lastByte, inFile) == true){ //file read status set only for calls.
					calls->setExToDate(puts->getExToDate());
					calls->setExFromByte(puts->getExFromByte());
					break; //break only if member function returned to break, otherwise, do nothing.
				}
				puts->writeCurrValues();
			}

			//Done with dealing with filesize and multiple files part.



		}
		cout << "It took " << time(0) - before << " seconds to extract " << endl;

		before = time(0);

		calls->addExRows();
		puts->addExRows();

		before = time(0);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Program will quickly sort the data. This is currently done whether data needs sorting or not.
		//So that all the times where data is already sorted, there is no need to even go through the deque of objects.
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		calls->sortData();
		puts->sortData();

		cout << "It took " << time(0) - before << " seconds to sort " << endl;
		before = time(0);
		//just setting the strikeDivisor here, should run just fine.
		if(calls->getStrikeDivisor() == 0)
			calls->setStrikeDivisor();
		if(puts->getStrikeDivisor() == 0)
			puts->setStrikeDivisor();

		calls->setIsCall(true); //or 1
		puts->setIsCall(false);	//or 0
		calls->setDataFile(dataFile); //setting path to file data will be extracted form.
		puts->setDataFile(dataFile); //has to be done after setIsCall is done.
	}



	else{  //This is not an *Options object. Undefined for now.

	}

}



void detectData(const string& dataFile, Asset*& pAsset, const string& writeTo,
		long lastDate, double lastByte){ //function used to detect data if data contains the cp_flag
																  //function returns number of bytes extracted.

	ifstream inFile(dataFile.c_str()); // object for handling file input
	string row;
	stringstream srow;
	getline(inFile,row);
	srow.str(row); //replace old row with new row;
	//used to find out what column I am in, and thus what deque to extract the data to.
	Columns columnHeaders; //instantiate an objects of Columns class.

	//find out if we are using a european or american style csv file. Following conditions
	char delimiter = getDelim(row, dataFile);


	columnHeaders.setColNbr(srow, delimiter); //set column numbers
	//columnHeaders.checkValid(dataFile); //check if data is valid according to column numbers


	if ((pAsset == NULL)){ // we initialize here the proper options objects to contain our data. This is done only at the beginning of a file.

		vector<Asset**>dummyAssets; dummyAssets.push_back(&pAsset);//just a dummy assets used for initiateAsset.
		columnHeaders.initiateAsset(dummyAssets, dataFile); //find what the Asset object will point to depending on the rows identity.

		pAsset->setExFromByte((double)inFile.tellg());  //skip first line for rest of code as I am starting at beginning of file.
													   //!!!!information is arbitrarily set in the pAsset object
		pAsset->setWriteTo(writeTo); //setting folder to write to, using data passed as argument to detectData.

	}


	//we first detect what type of options object we are dealing with
	//and save all the data to two single deques of row objects in order to sort columns in parallel.

	bool optionsOrNot = pAsset->getIsOption();

	////////////////////////////////////////////////////////////////////
	//if Options data was entered, find out what kind of options it was.
	////////////////////////////////////////////////////////////////////
	if (optionsOrNot == true) {

		//Skip to wanted position, after first line if new file, and as of last accessed byte
		//if return file.
		inFile.seekg(pAsset->getExFromByte(), ios_base::beg);


		while ((inFile.eof() != 1)){ //while I am in the file

			getline(inFile, row); //get next line.
			srow.clear(); //clear eofbit set from last line.
			srow.str(row); //replace old row with new row.

			pAsset->extractRow(srow, delimiter); //row is extracted and put in temporary
												//values of of calls object (arbitrarily), could as well be puts.

			if(pAsset->fileSizeManagement(lastDate, lastByte, inFile)) 	//Now dealing with file size and multiple files part
				break; //only break if returned to.


			pAsset->writeCurrValues();




		} //end of while(inFile.eof() != 1) loop.

		pAsset->addExRows();
		//Now that deques are filled, sort them as wanted.
		pAsset->sortData();


		if(pAsset->getStrikeDivisor() == 0)
			pAsset->setStrikeDivisor();

		pAsset->setCallStatus();
	}




	else{  //This is not an *Options object.

		//Skip to wanted position, after first line if new file, and as of last accessed byte
		//if return file.
		inFile.seekg(pAsset->getExFromByte(), ios_base::beg);
		while ((inFile.eof() != 1)){ //while I am in the file

			getline(inFile, row); //get next line.
			srow.clear(); //clear eofbit set from last line.
			srow.str(row); //replace old row with new row.

			pAsset->extractRow(srow, delimiter); //row is extracted and put in temporary
												//values of object

			//////////////////////////////////////////////////////////////////////////////////////
			//line is now completely extracted. Now dealing with file size and multiple files part
			//////////////////////////////////////////////////////////////////////////////////////

			if(pAsset->fileSizeManagement(lastDate, lastByte, inFile))
				break; //only break if returned to

			pAsset->writeCurrValues(); //push_back all deques containing our data in the structure of our objects

		} //end of while(inFile.eof() != 1) loop.

	}
	pAsset->addExRows();
	pAsset->setDataFile(dataFile); //setting path to file data will be extracted form.
	//extraction is finished

}

//can easily be accommodated to use any other delimiter.
char getDelim(string& row, const string& dataFile){

	char delimiter;
	if(row.find(",") != string::npos) //we have an american style csv file, the delimiter is a comma ','.
		delimiter = ',';
	else if(row.find(";") != string::npos) //we have a european style csv file, the delimiter is a semicolon ';'.
		delimiter = ';';
	else{ // no delimiter was found. return error
		cerr << "no suitable delimiter was found in file: " << dataFile << ".\nProgram terminating.\n";
		exit(EXIT_FAILURE);
	}

	return delimiter;

}




/*
void copyToFiles(const string& dataFile, const deque<OptionsRow*>&OCalls, const deque<OptionsRow*>&OPuts, const char& delim){

	string copyToFileOrNot;
	cout << "Should data be copied to two files in order to avoid sorting next time. Enter yes(y) or no(n) to save to files or not: ";
	getline(cin, copyToFileOrNot); cout << endl;

	while ((copyToFileOrNot!= "yes") && (copyToFileOrNot!= "no") && (copyToFileOrNot!= "y") && (copyToFileOrNot!= "n")){
				cout << "The value you entered is not valid. Please enter yes(y) or no(n):" << endl;
				cin >> copyToFileOrNot;
	}

	if ((copyToFileOrNot == "yes") || (copyToFileOrNot == "y")) { 		//copying the data, this might take some time.
		cout << "copying data, this might take some time."<< endl;

	size_t found; //create size_t object to manage position in string.
	found=dataFile.find_last_of("."); //find last point.
	string callsFile = dataFile.substr(0,found); callsFile.append("_bCalls.csv"); //create new string with new name just for calls.
	string putsFile = dataFile.substr(0,found); putsFile.append("_bPuts.csv"); //create new string with new name just for puts.

	ofstream outCalls(callsFile.c_str());
	outCalls.precision(10);

	ofstream outPuts(putsFile.c_str());
	outPuts.precision(10);

	OCalls[0]->printHeaders(outCalls, delim); //print calls header
	OPuts[0]->printHeaders(outPuts, delim); //print put header

	unsigned int i;
	for (i=0; i<OCalls.size(); ++i)
		OCalls[i]->printToFile(outCalls, delim);
	cout << "calls data was saved to: " << callsFile << endl;
	for (i=0; i<OPuts.size(); ++i)
		OPuts[i]->printToFile(outPuts, delim);
	cout << "puts data was saved to: " << putsFile << endl << endl;

	}


	else {} //do nothing

}

void copyToSingleFile(const string& dataFile, const double& nbrRows, const deque<OptionsRow*>&options, const char& delim){

	string copyToFileOrNot;
	cout << "Should data be copied to a file in order to avoid sorting next time. Enter yes(y) or no(n) to save to files or not: ";
	cin >> copyToFileOrNot; cout << endl;

	while ((copyToFileOrNot!= "yes") && (copyToFileOrNot!= "no") && (copyToFileOrNot!= "y") && (copyToFileOrNot!= "n")){
				cout << "The value you entered is not valid. Please enter yes(y) or no(n):" << endl;
				cin >> copyToFileOrNot;
	}

	if ((copyToFileOrNot == "yes") || (copyToFileOrNot == "y")) { 		//copying the data, this might take some time.
		cout << "copying data, this might take some time."<< endl;

	size_t found; //create size_t object to manage position in string.
	found=dataFile.find_last_of("."); //find last point.
	string optionsFile = dataFile.substr(0,found); optionsFile.append("_sorted.csv"); //create new string with new name for sorted data.

	ofstream outOptions(optionsFile.c_str());
	//outOptions.open("realdata.csv");
	outOptions.precision(10);

	options[0]->printHeaders(outOptions, delim); //print option header

	unsigned int i;
	for (i=0; i<nbrRows; ++i)
		options[i]->printToFile(outOptions, delim);

	cout << "data was saved to: " << optionsFile << endl;

	}
	else {} //do nothing

}
*/
