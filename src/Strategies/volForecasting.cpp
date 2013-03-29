/*
 * volForecasting.cpp
 *
 *  Created on: Oct 20, 2012
 *      Author: john-d
 */


#include <string>
#include "strategies.h"
#include <climits>

long volForecasting1(vector<Asset*>& assets, double startMoney){ //based on 2010 paper


	return 0;
}


long outOfMoneyOption(vector<Asset*>& assets, double startMoney){ //based on what

	Asset* calls;
	Asset* puts;
	Asset* underlyer;
	double tCRatio = 0.01; //records the transaction cost ratio
	if (assets.size() != 3){
		cerr << " Wrong number of objects to test outOfMoneyOption" << endl;
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

	//we have the required types of data: perform back-test.
	//Simple strat that just keeps cash adn trades 20% of portfolio in out of the money straddle.
	//Equity information is contained is separate object.

	struct stat fStatus; //where all the output will be in for further analysis
	string outputFile = assets.at(0)->getWriteTo()+ "/" + "outOfMoneyOption.csv";
	int thereOrNot = stat(outputFile.c_str(), &fStatus);
	ofstream dataOut(outputFile.c_str(), ios_base::out | ios_base::app); //append to file only will create file if not there
	if (thereOrNot == -1) //file not already created.
		dataOut << "Date," << "Capital," << "Cash," << "calls," << "puts" << endl;
	else{}//do nothing


	double currCash = startMoney;
	deque<double>results;
	results.push_back(startMoney);
	double nbrCalls = 0; //in practice, very hard to get a perfect straddle, so numbers will vary.
	double nbrPuts = 0;
	double callID = 0; //ID of currently held call contracts
	double putID = 0; //ID of currently held put contracts
	size_t equityIndex = 0;
	long currDate = underlyer->getDate(equityIndex);
	dataOut << currDate << "," << currCash
			<< "," << currCash << "," << nbrCalls << "," << nbrPuts << endl;

	size_t callsPos = 0;
	size_t putsPos = 0;
	double oufOfMoneyRatio = 0.2; // certainly need to data mine this value.

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	//////////////////// find option closest to outOfMoneyRatio ////////////////////////
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


	return 0;
}
