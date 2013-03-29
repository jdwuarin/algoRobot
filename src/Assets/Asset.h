/*
 * Asset.h
 *
 *  Created on: Apr 2, 2012
 *      Author: john-d
 */

#ifndef ASSET_H_
#define ASSET_H_
#include "assets_prot.h"


using namespace std;
class Columns;
class Asset;
void detectData(const string&, Asset*&, const string&, long, double);
void detectData(const string&, Asset*&, Asset*&, const string&, long, double);

class Asset { //this class is the base class of all assets we will be running our backtesting on. Automated trades are written in the strategies
			  //sub_folder.

protected: //declaration of all the family protected members
	colData* date;
	colData* close; //those are the two primary objects of each asset. Although some options file don't have the close column
	vector<colData*>allData;



	//information concerning up to what date to keep data in large file management.
	long exToDate;
	double exFromByte;
	unsigned int testFromRow; //used for large files that keep data with dates already passed at do data mine parameters.
					  //indicates the row to test from.
	unsigned int nbrObjectCuts; //keeps count of how many time large files were segmented while extracting

	long exRows; //number of extracted rows.
	string writeTo; //contains name of folder to write to.
	string dataFile; //contains name of file data was extracted from.




public:
	Asset(Columns& columnHeaders);
	virtual ~Asset();

	void addRow(vector<long>&, vector<double>&);
	void extractRow(stringstream&, char&);
	void writeCurrValues();
	void eraseRow(const size_t& row);
	void eraseRows(const size_t&  firstRow, const size_t&  lastRow);


	size_t getNbrRows(){return(this->date->getColSize());}
	void addExRows(){this->exRows += this->getNbrRows() - this->testFromRow;}
	//as testFromRow saves row number of first new row of data.
	long getExRows(){return(this->exRows);}

	bool fileSizeManagement(const long& lastDate, const double& lastByte,
			ifstream& inFile);

	virtual void sortData(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}

	virtual long getNbrBytes();
	inline const double findDate(const long& date)
	{return(this->date->findValue(date));} //returns row number of first found date.

	void setExToDate(long exToDate){this->exToDate = exToDate;}
	long getExToDate(){return(this->exToDate);}
	void setExFromByte(double exFromByte){this->exFromByte = exFromByte;}
	double getExFromByte(){return(this->exFromByte);}
	void setTestFromRow(){this->testFromRow = this->getNbrRows();}
	unsigned int getTestFromRow(){return(this->testFromRow);}

	unsigned int getNbrObjectCuts(){return(this->nbrObjectCuts);}
	unsigned int getDateCol(); //get column index inside allLongData, used for sorting.
	virtual unsigned int getExDateIndex(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual unsigned int getStrikeIndex(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}

	string getWriteTo(){return (this->writeTo);}
	void setWriteTo(string writeTo){this->writeTo = writeTo;}
	string getDataFile(){return (this->dataFile);}
	virtual void setDataFile(string dataFile);

	void printHeaders(ofstream& output);
	void printData(ofstream& output);


	//by convention, all values are returned as double
	//to allow user to do computation with them without having to type cast.
	const double getDate(const size_t& pos){return(this->date->getValue_at(pos));}
	deque<long>* getDateColumnPtr(){return this->date->getLgColumnPtr();}
	inline const double getLastDate(){return this->date->getLastValue();}
	inline const double getCurrDate(){return(this->date->getCurrValue());}
	const double getClose(const size_t& pos){return(this->close->getValue_at(pos));}
	deque<double>* getCloseColumnPtr(){return this->close->getDblColumnPtr();}


	virtual const double getOpen(const size_t& pos){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getHigh(const size_t& pos){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getLow(const size_t& pos){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getVolume(const size_t& pos){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}

	//For Options.
	virtual const double getExDate(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getStrike(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getBBid(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getBOffer(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getUndVolume(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getOoi(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getImpVol(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getDelta(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getGamma(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getVega(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getTheta(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getOptionId(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getSettleFlag(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getExchDes(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getExerStyle(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual void setIsCall(bool isCall) {cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual int getIsCall(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual int getStrikeDivisor(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual void setStrikeDivisor(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual void setCallStatus(){cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}

	//For Equity.
	virtual const double getSecid(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getIndGroup(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getCfadj(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual const double getShrout(const size_t& pos){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}

	//Options methods used for strats.
	//Method uses assetRowPointer to go from date to Date.
	virtual double getDayVolume(){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}
	virtual double getDayOoi(){ cout << "Wrong object type called" << endl; exit(EXIT_FAILURE);}

	//class type identifier
	virtual bool getIsEquity(){return false;}
	virtual bool getIsOption(){return false;}
	virtual bool getIsOptionsImpVol(){return false;}
	virtual bool getIsOptionsDelta(){return false;}
	virtual bool getIsOptionGreeks(){return false;}

	static bool columnNbrComp (colData* a, colData* b){return (a->info.columnNbr < b->info.columnNbr);}

	friend void detectData(const string&, Asset*&, const string&, long, double);
	friend void detectData(const string&, Asset*&, Asset*&, const string&, long, double);

};

#endif /* ASSET_H_ */
