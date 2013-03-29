/*
 * Columns.h
 *
 *  Created on: Jun 13, 2012
 *      Author: john-d
 */

#ifndef COLUMNS_H_
#define COLUMNS_H_
#include "Options.h"
#include "Equity.h"


class Columns {
	vector<colInfo*>possibleColsInfo;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Right there is where I can add columns. Also change Columns.cpp file.
	//in order for the detection to be possible later. In order
	//to make it possible to read data with more columns, just add the column
	//in Asset* header and include accessor.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	colInfo* dateInfo;
	colInfo* closeInfo;
	colInfo* openInfo;
	colInfo* highInfo;
	colInfo* lowInfo;
	colInfo* exDateInfo;
	colInfo* strikeInfo;
	colInfo* bBidInfo;
	colInfo* bOfferInfo;
	colInfo* volumeInfo;
	colInfo* ooiInfo;
	colInfo* iVolInfo;
	colInfo* deltaInfo;
	colInfo* gammaInfo;
	colInfo* vegaInfo;
	colInfo* thetaInfo;
	colInfo* cpInfo;
	colInfo* optionIdInfo; //option identifier
	colInfo* settleFlagInfo; //special settlement flag
	colInfo* exchDesInfo; //exchange designator (where it was traded)
	colInfo* exerStyleInfo; //american or european or other.
	colInfo* secidInfo; //security id, used for equities
	colInfo* indGroupInfo; //see ivyDB for meaning of industry group info
	colInfo* cfadjInfo; //cumulative adjustment factor
	colInfo* shroutInfo; //shares outstanding

public:
	Columns();
	virtual ~Columns();


	void setColNbr(stringstream& srow, char& delimiter);
	void checkValid(const string&);
	void initiateAsset(vector<Asset**>&, const string& dataFile);
	int getCpColNbr(){return this->cpInfo->columnNbr;}
	friend class Asset;
	friend class Equity;
	friend class Options;
	friend class OptionsImpVol;
	friend class OptionsDelta;
	friend class OptionsGreeks;
};

#endif /* COLUMNS_H_ */
