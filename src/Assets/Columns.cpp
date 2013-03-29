/*
 * Columns.cpp
 *
 *  Created on: Jun 13, 2012
 *      Author: john-d
 */

#include "Columns.h"

Columns::Columns(){

	dateInfo = new colInfo;
	closeInfo = new colInfo;
	openInfo = new colInfo;
	highInfo = new colInfo;
	lowInfo = new colInfo;
	exDateInfo = new colInfo;
	strikeInfo = new colInfo;
	bBidInfo = new colInfo;
	bOfferInfo = new colInfo;
	volumeInfo = new colInfo;
	ooiInfo = new colInfo;
	iVolInfo = new colInfo;
	deltaInfo = new colInfo;
	gammaInfo = new colInfo;
	vegaInfo = new colInfo;
	thetaInfo = new colInfo;
	cpInfo = new colInfo;
	optionIdInfo = new colInfo;
	settleFlagInfo = new colInfo;
	exchDesInfo = new colInfo;
	exerStyleInfo = new colInfo;
	secidInfo = new colInfo;
	indGroupInfo = new colInfo;
	cfadjInfo = new colInfo;
	shroutInfo = new colInfo;

	dateInfo->names.push_back("date");
	dateInfo->names.push_back("Date");
	dateInfo->length = 4;
	dateInfo->columnNbr = 0;
	possibleColsInfo.push_back(dateInfo);

	closeInfo->names.push_back("close");
	closeInfo->names.push_back("Close");
	closeInfo->length = INT_MAX;
	closeInfo->columnNbr = 0;
	possibleColsInfo.push_back(closeInfo);

	openInfo->names.push_back("open");
	openInfo->names.push_back("Open");
	openInfo->length = 4;
	openInfo->columnNbr = 0;
	possibleColsInfo.push_back(openInfo);

	highInfo->names.push_back("high");
	highInfo->names.push_back("High");
	highInfo->length = INT_MAX;
	highInfo->columnNbr = 0;
	possibleColsInfo.push_back(highInfo);

	lowInfo->names.push_back("low");
	lowInfo->names.push_back("Low");
	lowInfo->length = INT_MAX;
	lowInfo->columnNbr = 0;
	possibleColsInfo.push_back(lowInfo);

	exDateInfo->names.push_back("exDate");
	exDateInfo->names.push_back("exdate");
	exDateInfo->names.push_back("Exdate");
	exDateInfo->length = INT_MAX;
	exDateInfo->columnNbr = 0;
	possibleColsInfo.push_back(exDateInfo);

	strikeInfo->names.push_back("strike");
	strikeInfo->names.push_back("Strike");
	strikeInfo->length = INT_MAX;
	strikeInfo->columnNbr = 0;
	possibleColsInfo.push_back(strikeInfo);

	bBidInfo->names.push_back("bid");
	bBidInfo->names.push_back("Bid");
	bBidInfo->length = INT_MAX;
	bBidInfo->columnNbr = 0;
	possibleColsInfo.push_back(bBidInfo);

	bOfferInfo->names.push_back("offer");
	bOfferInfo->names.push_back("Offer");
	bOfferInfo->names.push_back("ask");
	bOfferInfo->names.push_back("Ask");
	bOfferInfo->length = INT_MAX;
	bOfferInfo->columnNbr = 0;
	possibleColsInfo.push_back(bOfferInfo);

	volumeInfo->names.push_back("volume");
	volumeInfo->names.push_back("Volume");
	volumeInfo->names.push_back("oVolume");
	volumeInfo->length = INT_MAX;
	volumeInfo->columnNbr = 0;
	possibleColsInfo.push_back(volumeInfo);

	ooiInfo->names.push_back("ooi");
	ooiInfo->names.push_back("Ooi");
	ooiInfo->names.push_back("interest");
	ooiInfo->names.push_back("Interest");
	ooiInfo->length = INT_MAX;
	ooiInfo->columnNbr = 0;
	possibleColsInfo.push_back(ooiInfo);

	iVolInfo->names.push_back("iVol");
	iVolInfo->names.push_back("impl");
	iVolInfo->names.push_back("Impl");
	iVolInfo->names.push_back("implVol");
	iVolInfo->length = INT_MAX;
	iVolInfo->columnNbr = 0;
	possibleColsInfo.push_back(iVolInfo);

	deltaInfo->names.push_back("delta");
	deltaInfo->names.push_back("Delta");
	deltaInfo->length = INT_MAX;
	deltaInfo->columnNbr = 0;
	possibleColsInfo.push_back(deltaInfo);

	gammaInfo->names.push_back("gamma");
	gammaInfo->names.push_back("Gamma");
	gammaInfo->length = INT_MAX;
	gammaInfo->columnNbr = 0;
	possibleColsInfo.push_back(gammaInfo);

	vegaInfo->names.push_back("vega");
	vegaInfo->names.push_back("Vega");
	vegaInfo->length = INT_MAX;
	vegaInfo->columnNbr = 0;
	possibleColsInfo.push_back(vegaInfo);

	thetaInfo->names.push_back("theta");
	thetaInfo->names.push_back("Theta");
	thetaInfo->length = INT_MAX;
	thetaInfo->columnNbr = 0;
	possibleColsInfo.push_back(thetaInfo);

	cpInfo->names.push_back("cp_flag");
	cpInfo->names.push_back("cpInfo");
	cpInfo->names.push_back("CP_Flag");
	cpInfo->length = INT_MAX;
	cpInfo->columnNbr = 0;
	possibleColsInfo.push_back(cpInfo);

	optionIdInfo->names.push_back("optionid");
	optionIdInfo->names.push_back("optionId");
	optionIdInfo->length = INT_MAX;
	optionIdInfo->columnNbr = 0;
	possibleColsInfo.push_back(optionIdInfo);

	settleFlagInfo->names.push_back("ss_flag");
	settleFlagInfo->names.push_back("ssFlag");
	settleFlagInfo->length = INT_MAX;
	settleFlagInfo->columnNbr = 0;
	possibleColsInfo.push_back(settleFlagInfo);

	exchDesInfo->names.push_back("exchange");
	exchDesInfo->names.push_back("exchangeD");
	exchDesInfo->length = INT_MAX;
	exchDesInfo->columnNbr = 0;
	possibleColsInfo.push_back(exchDesInfo);

	exerStyleInfo->names.push_back("exercise_style");
	exerStyleInfo->length = INT_MAX;
	exerStyleInfo->columnNbr = 0;
	possibleColsInfo.push_back(exerStyleInfo);

	secidInfo->names.push_back("secid");
	secidInfo->length = INT_MAX;
	secidInfo->columnNbr = 0;
	possibleColsInfo.push_back(secidInfo);

	indGroupInfo->names.push_back("industry_group");
	indGroupInfo->names.push_back("Industry_Group");
	indGroupInfo->length = INT_MAX;
	indGroupInfo->columnNbr = 0;
	possibleColsInfo.push_back(indGroupInfo);

	cfadjInfo->names.push_back("cfadj");
	cfadjInfo->names.push_back("cumulative_adjustment");
	cfadjInfo->names.push_back("cum_adj_fact");
	cfadjInfo->length = INT_MAX;
	cfadjInfo->columnNbr = 0;
	possibleColsInfo.push_back(cfadjInfo);

	shroutInfo->names.push_back("shrout");
	shroutInfo->names.push_back("shares_outstanding");
	shroutInfo->length = INT_MAX;
	shroutInfo->columnNbr = 0;
	possibleColsInfo.push_back(shroutInfo);

}


Columns::~Columns() {
	delete dateInfo;
	delete closeInfo;
	delete openInfo;
	delete highInfo;
	delete lowInfo;
	delete exDateInfo;
	delete strikeInfo;
	delete bBidInfo;
	delete bOfferInfo;
	delete volumeInfo;
	delete ooiInfo;
	delete iVolInfo;
	delete deltaInfo;
	delete gammaInfo;
	delete vegaInfo;
	delete thetaInfo;
	delete cpInfo;
	delete optionIdInfo;
	delete settleFlagInfo;
	delete exchDesInfo;
	delete exerStyleInfo;
	delete secidInfo;
	delete indGroupInfo;
	delete cfadjInfo;
	delete shroutInfo;
}

void Columns::setColNbr(stringstream& srow, char& delimiter){
	int columnNbr=0;
	string column;
	unsigned int i = 0;
	while (getline(srow, column, delimiter)){
		++columnNbr;

		unsigned int j; //used to run through name vector.
		for (i = 0; i < possibleColsInfo.size(); ++i){ //run through all possibleColsInfo object
			j = 0;
			for (j = 0; j < possibleColsInfo.at(i)->names.size(); ++j){ //run through all variations of name.
				if ((column.find(possibleColsInfo.at(i)->names.at(j).c_str()) != string::npos) &&
						(column.length() <= possibleColsInfo.at(i)->length))
					possibleColsInfo.at(i)->columnNbr = columnNbr;
			}

		}


	}
}


void Columns::checkValid(const string& dataFile){

	/*if ((this->dateInfo->columnNbr==0) || (this->closeInfo->columnNbr==0) || (this->exDateInfo->columnNbr==0)
			|| (this->strikeInfo->columnNbr==0) || (this->bBidInfo->columnNbr==0) || (this->bOfferInfo->columnNbr==0) || (this->oVolumeInfo->columnNbr==0)
			|| (this->ooiInfo->columnNbr==0)) {//if I have insufficient data to use any strategy for options
		cerr << "Data in " << dataFile << " has been detected to be insufficient " << endl <<
				"to back-test or use any useful trading strategy." << endl <<
				"This can be because the first row of your data file does not contain headers or because they" << endl <<
				"are wrongly named. If so, change them as required to use this program. " << endl;
		cerr << "Program terminating.\n";
		exit(EXIT_FAILURE);
	}

	else{}*/
}


void Columns::initiateAsset(vector<Asset**>&dummyAssets, const string& dataFile){ //only multiple valued vector if we have both calls and puts

	unsigned int i = 0;	//run through vector.

	for (i = 0; i < dummyAssets.size(); ++i){
		if (this->gammaInfo->columnNbr != 0) //we have an OptionsGreeks object.
			*dummyAssets.at(i) = new OptionsGreeks(*this);
		else if ((this->gammaInfo->columnNbr == 0) && (this->deltaInfo->columnNbr != 0)) //we have an OptionsDelta object.
			*dummyAssets.at(i) = new OptionsDelta(*this);
		else if ((this->gammaInfo->columnNbr == 0) && (this->deltaInfo->columnNbr == 0)
				&& (this->iVolInfo->columnNbr != 0)) //we have an OptionsImpVol object.
			*dummyAssets.at(i) = new OptionsImpVol(*this);
		else if ((this->gammaInfo->columnNbr == 0) && (this->deltaInfo->columnNbr == 0)
				&& (this->iVolInfo->columnNbr == 0) && (this->shroutInfo->columnNbr == 0)
				&& (this->ooiInfo->columnNbr != 0))
			*dummyAssets.at(i) = new Options(*this);
		else if ((this->gammaInfo->columnNbr == 0) && (this->deltaInfo->columnNbr == 0)
				&& (this->iVolInfo->columnNbr == 0) && (this->shroutInfo->columnNbr != 0)
				&& (this->ooiInfo->columnNbr == 0))
			*dummyAssets.at(i) = new Equity(*this);

		else {
			cout << "could not determine type of object from Columns in \n"
					<< dataFile << endl;
			exit(EXIT_FAILURE);
		}

			//best guess is...
			;
		//else{
		//"Could not find appropriate object for: " << Assets.at(i)->getDataFile() << " Exiting." << endl
		//exit(Exit_Failure);
		//}
	}

}
