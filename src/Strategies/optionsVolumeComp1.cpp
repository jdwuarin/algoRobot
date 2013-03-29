/*
 * optionsVolumeComp1.cpp;
 *
 *  Created on: Aug 16, 2012
 *      Author: john-d
 */


#include <string>
#include "strategies.h"
#include <climits>


long optionsVolumeComp1(vector<Asset*>& assets, double startMoney){

	Asset* calls;
	Asset* puts;
	Asset* underlyer;
	const double RATIODIFF = 0.05;
	double tCRatio = 0.01; //records the transaction cost ratio
	if (assets.size() != 3){
		cerr << " Wrong number of objects to test optionsVolumeComp1" << endl;
		exit (EXIT_FAILURE);
	}

	if (assets.at(0)->getIsEquity() == 1){ //first object is Equity
		underlyer = assets.at(0);
		if(assets.at(1)->getIsCall() == 1){ //second object is calls
			calls = assets.at(1);
			puts = assets.at(2);
		}
		else{
			calls = assets.at(2);
			puts = assets.at(1);
		}
	}
	else if (assets.at(0)->getIsOption() == 1){
		if(assets.at(0)->getIsCall() == 1){
			calls = assets.at(0);
			if(assets.at(1)->getIsEquity() == 1){
				underlyer = assets.at(1);
				puts = assets.at(2);
			}
			else{
				underlyer = assets.at(2);
				puts = assets.at(1);
			}

		}
		else{
			puts = assets.at(0);
			if(assets.at(1)->getIsEquity() == 1){
				underlyer = assets.at(1);
				calls = assets.at(2);
			}
			else{
				underlyer = assets.at(2);
				calls = assets.at(1);
			}
		}
	}

	else{
		cerr << " Functions requires an equity, a  put and a call dataset to do analysis."
				"At least one is missing." << endl; //write using exception throwing rather than exit.
		exit (EXIT_FAILURE);
	}



	//we have the required types of data: perform back-test.
	//ooi is lagged by one day. Do not forget, that for any day. we get the
	//info on the following day, thus trading strategy must rely on
	//one day old information.
	//Equity information is contained in separate object.

	struct stat fStatus; //where all the output will be i for further analysis
	string outputFile = assets.at(0)->getWriteTo()+ "/" + "optionsVolumeComp1.csv";
	int thereOrNot = stat(outputFile.c_str(), &fStatus);
	ofstream dataOut(outputFile.c_str(), ios_base::out | ios_base::app); //append to file only will create file if not there
	if (thereOrNot == -1) //file not already created.
		dataOut << "Date," << "Capital," << "Cash," << "LongPos," << "ShortPos" << endl;
	else{}//do nothing


	double currCash = startMoney;
	deque<double>results;
	results.push_back(startMoney);
	double currLongPos = 0; //number of short positions
	double nbrToLong = 0; //number of shares to buy first thing in morning.
	double currShortPos = 0; //number of long positions (trying long only for now)
	double nbrToShort = 0; //number of shares to short first thing in morning.
	size_t equityIndex = 0;
	long currDate = underlyer->getDate(equityIndex);
	dataOut << currDate << "," << currCash + currLongPos* underlyer->getClose(equityIndex)
			<< "," << currCash << "," << currLongPos << "," << 0 << endl;

	size_t callsPos = 0;
	size_t putsPos = 0;
	double callsDailyVol = 0;
	double callsDailyOoi = 0;
	double putsDailyVol = 0;
	double putsDailyOoi = 0;
	double prevPcVolRatio = 0;
	double currPcVolRatio = 0;
	double prevPcOoiRatio = 0;
	double currPcOoiRatio = 0;

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////////// get first day data on volumes /////////////////////////////
	while (calls->getDate(callsPos) == currDate){
		callsDailyVol += calls->getVolume(callsPos);
		callsDailyOoi += calls->getOoi(callsPos);
		++callsPos;
		if(callsPos == calls->getNbrRows()) break;
	}

	//get puts daily volume and open interest for all options on given day.
	while (puts->getDate(putsPos) == currDate){ //while I am on one date
		putsDailyVol += puts->getVolume(putsPos);
		putsDailyOoi += puts->getOoi(putsPos);
		++putsPos;
		if(putsPos == puts->getNbrRows()) break;
	}

	prevPcVolRatio = putsDailyVol/(callsDailyVol + putsDailyVol);
	prevPcOoiRatio = putsDailyOoi/(callsDailyOoi + putsDailyOoi);

	//////////////////////// get first day data on volumes /////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////


	while(++equityIndex < underlyer->getNbrRows()){ //increment before comparing, to start comparing from row 1.

		currDate = underlyer->getDate(equityIndex);

		if (nbrToLong - currLongPos > 0){ //I have to buy some stock

			if(currCash - (1+tCRatio)*(nbrToLong-currLongPos)*underlyer->getOpen(equityIndex) >= 0){ // I've go enough cash to commit
				currCash -= (1+tCRatio)*(nbrToLong-currLongPos)*underlyer->getOpen(equityIndex);
				currLongPos = nbrToLong;
			}
			else{
				nbrToLong = currLongPos + floor(currCash/((1+tCRatio)*underlyer->getOpen(equityIndex))); //buy as much as I can
				currCash -= (1+tCRatio)*(nbrToLong-currLongPos)*underlyer->getOpen(equityIndex);
				currLongPos = nbrToLong;
			}

		}
		else if (nbrToLong - currLongPos < 0){ // I have to sell some stock, currently only doing long only.
			currCash += (1-tCRatio)*(currLongPos - nbrToLong)*underlyer->getOpen(equityIndex);
			currLongPos = nbrToLong;
		}
		else{} // do nothing.

		//re-initialize values on new date
		callsDailyVol = 0;
		callsDailyOoi = 0;
		putsDailyVol = 0;
		putsDailyOoi = 0;
		//get calls daily volume and open interest for all options on given day.
		while (calls->getDate(callsPos) == currDate){ //while I am on one date
			callsDailyVol += calls->getVolume(callsPos);
			callsDailyOoi += calls->getOoi(callsPos);
			++callsPos;
			if(callsPos == calls->getNbrRows()) break;
		}

		//get puts daily volume and open interest for all options on given day.
		while (puts->getDate(putsPos) == currDate){ //while I am on one date
			putsDailyVol += puts->getVolume(putsPos);
			putsDailyOoi += puts->getOoi(putsPos);
			++putsPos;
			if(putsPos == puts->getNbrRows()) break;
		}

		currPcVolRatio = putsDailyVol/(callsDailyVol + putsDailyVol); //update pcVolRatio, wrong, actually want difference in volumes from one day to the other.
		currPcOoiRatio = putsDailyOoi/(callsDailyOoi + putsDailyOoi);

		//make trade decision based on pcVolRatio
		//start easy, only long, cannot short.

		if (currPcVolRatio - prevPcVolRatio < RATIODIFF){
			nbrToLong += floor((1-currPcVolRatio)*(currCash/((1+tCRatio)*underlyer->getClose(equityIndex))));
		}
		else if (currPcVolRatio - prevPcVolRatio > RATIODIFF)
			if(currLongPos > 0)
				nbrToLong = floor((currPcVolRatio)*currLongPos); //diminish part of stocks
			else{}
		else{
			if(currLongPos*underlyer->getClose(equityIndex) > currCash) //no clear sign and lots of cash in stocks
				nbrToLong = nbrToLong * 0.9; //diminish part of equity. 0.9 picked random
			else{} //do nothing.
		}


		results.push_back(currCash + currLongPos * underlyer->getClose(equityIndex));
		dataOut << currDate << "," << currCash + currLongPos* underlyer->getClose(equityIndex)
				<< "," << currCash << "," << currLongPos << "," << 0 << endl;
	}

	plotResults(underlyer->getDateColumnPtr(), &results);
	plotLogResults(underlyer->getDateColumnPtr(), &results);
	return 0;
}



long optionsVolumeComp2(vector<Asset*>& assets, double startMoney){

	return 0;
}

