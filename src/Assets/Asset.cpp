/*
 * Asset.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: john-d
 */

#include "../blackBoxes_prot.h"


Asset::Asset(Columns& columnHeaders){
	this->exToDate = 0;
	this->exFromByte = 0;
	this->exRows = 0;
	this->testFromRow = 0;
	this->nbrObjectCuts = 0;
	date = new colDataLong;
	close = new colDataDouble;
	this->date->info = *columnHeaders.dateInfo;
	this->close->info = *columnHeaders.closeInfo;
	allData.push_back(date);
	if (close->info.columnNbr !=0)allData.push_back(close);

	std::sort(allData.begin(), allData.end(), columnNbrComp);

}

Asset::~Asset() {
	delete date;
	delete close;
}


void Asset::extractRow(stringstream& srow, char& delimiter){

	int columnNbr=0;
	string column;
	unsigned int i = 0;
	int h = 0; //records inside columnNbr we are at for allData deque
	int count = 0;

	//columnNbr is now = allData[0]->info.columnNbr
	for(i = 0; i < allData.size(); ++i){

		h = allData[i]->info.columnNbr; //h knows the next row number.
		++columnNbr; 	getline(srow, column, delimiter);
		while (columnNbr < h) // While I haven't reached the next of my ordered columns.
			{++columnNbr; 	getline(srow, column, delimiter);}

		if(allData[i]->info.names[0] == "date" || allData[i]->info.names[0] == "exDate")
			{allData[i]->setCurrValue(DMYToExcelSerialDate(column)); ++count;}

		else if(allData[i]->info.names[0] == "ss_flag" || allData[i]->info.names[0] == "exchange"
				|| allData[i]->info.names[0] == "exercise_style")
			allData[i]->setCurrValue(double(*column.c_str()));

		else
			allData[i]->setCurrValue(atof(column.c_str()));

		if (count == 2)
			break;
	}

	for(unsigned int j = i+1 ;j < allData.size(); ++j){

		h = allData[j]->info.columnNbr; //h knows the next row number.
		++columnNbr; 	getline(srow, column, delimiter);
		while (columnNbr < h) // I haven't reached the next of my ordered columns.
			{++columnNbr; 	getline(srow, column, delimiter);}

		if(allData[j]->info.names[0] == "ss_flag" || allData[j]->info.names[0] == "exchange"
				|| allData[j]->info.names[0] == "exercise_style")
			allData[j]->setCurrValue(double(*column.c_str()));
		else
			allData[j]->setCurrValue(atof(column.c_str()));
	}



/*

	while (getline(srow, column, delimiter)){ //while we are not at the end of the line.

		//to optimize as now allData object is sorted.
		++columnNbr;


		for (i = 0; i < allData.size(); ++i){ //run through all colDataLong objects
			if(columnNbr == allData[i]->info.columnNbr){
				if(allData[i]->info.names[0] == "date" || allData[i]->info.names[0] == "exDate")
					allData[i]->setCurrValue(DMYToExcelSerialDate(column));
				else
					allData[i]->setCurrValue(atof(column.c_str()));
			}
			else{}
		}

	}

	*/

}

void Asset::writeCurrValues(){
	unsigned int i = 0; //used to run through allRowsInfo

	for (i = 0; i < allData.size(); ++i) //run through all colDataLong objects
			allData.at(i)->writeCurrValue(); //add currValue to used deque
}

void Asset::eraseRow(const size_t& row){
	unsigned int i;
	for (i = 0; i < this->allData.size();++i)
		allData.at(i)->eraseEntry(row);
}

void Asset::eraseRows(const size_t& firstRow, const size_t& lastRow){
	unsigned int i;
	for (i = 0; i < this->allData.size();++i)
		allData.at(i)->eraseRange(firstRow, lastRow);

	this->setTestFromRow(); //set testFromRow to row number to be extracted.
	++this->nbrObjectCuts;

}


bool Asset::fileSizeManagement(const long& lastDate, const double& lastByte,
		ifstream& inFile){
	bool breakSignal = false; //tells function whether to return break signal or not.

	if ((lastDate == 0) && (lastByte != 0)){ //file size is big, dealing with first of file.



		if(inFile.eof()) {
			this->setExFromByte(-1);  //used for first file to indicate to stop while loop in toOpen.
			this->setExToDate(-1);  //serves as an indicator to other files to go till the eof is reached.
			breakSignal = true;//break if end of file was just read
		}

		else if (((double)inFile.tellg() >= lastByte) && ((this->getCurrDate() > this->getLastDate()))){
			//if I have passed last byte to extract
			//and I am at a new date.
			inFile.unget().unget(); //decrement get pointer by two, putting back \n character and the one before.
			while (inFile.peek() != '\n'){
				inFile.unget(); //put pointer back by one.
			}
			inFile.get(); //next char is now \n, program thus discards it then breaks.
			 this->setExFromByte((double)inFile.tellg()); //set byte to start extracting from next time.
			 this->setExToDate(this->getCurrDate()-1); //for multiple files strategy.

			breakSignal = true;
		}
	}

	else if((lastDate != 0) && (lastByte == 0)){ //first time program opens any file after first one.

		if(inFile.eof()){
			this->setExToDate(-1);
			breakSignal = true;//break if end of file was just read...
		}

		else if(lastDate == -1){}// do nothing, just extract until eof is reached.

		else if ((this->getCurrDate() > lastDate)){
			//if I have passed last date to extract
			//and I am at a new date.
			inFile.unget().unget(); //decrement get pointer by two, putting back \n character and the one before.
			while (inFile.peek() != '\n'){
				inFile.unget(); //put pointer back by one.
			}
			inFile.get(); //next char is now \n, program thus discards it.
			this->setExFromByte((double)inFile.tellg()); //set byte to start extracting from next time.
			breakSignal = true;
		}
	}


	else{ //one or more files of small size. Just do nothing.
		if(inFile.eof()){ //just break at end of file as in other conditions.
			this->setExToDate(-1);
			breakSignal = true;
			}
	}

	return breakSignal;

}



long Asset::getNbrBytes(){
	long nbrBytes = (long)this->getNbrRows()*(sizeof(long)+ sizeof(double));
	return(nbrBytes);
}

unsigned int Asset::getDateCol(){
	unsigned int i = 0;
	unsigned int dateIndex = 0;
	for(i = 0; i < this->allData.size(); ++i){
		if (allData.at(i)->info.names.at(0) == "date"){
			dateIndex = i;
		}
	}
	return dateIndex;
}

void Asset::setDataFile(string dataFile){
	size_t found;
	size_t foundPoint;
	found=dataFile.find_last_of("/");
	foundPoint = dataFile.find_last_of(".");
	dataFile = dataFile.substr(found+1, foundPoint-(found+1));
	this->dataFile = dataFile;

}


void Asset::printHeaders(ofstream& output){

	unsigned int i = 0; //used to run through allRowsInfo

	for (i = 0; i < allData.size(); ++i){ //run through all colDataDouble object
		output << allData.at(i)->info.names.at(0);
		if(i != allData.size() -1)
			output << ','; //add currValue to used deque
	}
	output << endl;

}
void Asset::printData(ofstream& output){
	unsigned int h; //go through rows
	unsigned int i; //go through columns

	for (h = this->getTestFromRow(); h < allData.at(0)->getColSize(); ++h){ // going through all rows.

		for (i = 0; i < allData.size(); ++i){ //run through all colDataDouble object
			output << allData[i]->getValue_at(h);
			if(i != allData.size() -1)
				output << ','; //add currValue to used deque
		}
		output << endl;
	}

}
