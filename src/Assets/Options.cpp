/*
 * Options.cpp
 *
 *  Created on: Mar 21, 2012
 *      Author: john-d
 */

#include "../blackBoxes_prot.h"


template<typename T>
class Comp{
	const deque<T>* _values; //first pointer is const.
public:
  Comp( const deque<T>* values) : _values(values) {}

  bool operator()(size_t i, size_t j){
        return (_values)->at(i) < (_values)->at(j);
  }
};


Options::Options(Columns& columnHeaders):Asset(columnHeaders){

	open = new colDataDouble;
	high = new colDataDouble;
	low = new colDataDouble;
	exDate = new colDataLong;
	strike = new colDataDouble;
	bBid = new colDataDouble;
	bOffer = new colDataDouble;
	opVolume = new colDataDouble;
	ooi = new colDataDouble;
	secid = new colDataDouble;
	optionId = new colDataDouble;
	settleFlag = new colDataLong;
	exchDes = new colDataLong;
	exerStyle = new colDataLong;

	this->open->info = *columnHeaders.openInfo;
	this->high->info = *columnHeaders.highInfo;
	this->low->info = *columnHeaders.lowInfo;
	this->exDate->info = *columnHeaders.exDateInfo;
	this->strike->info = *columnHeaders.strikeInfo;
	this->bBid->info = *columnHeaders.bBidInfo;
	this->bOffer->info = *columnHeaders.bOfferInfo;
	this->opVolume->info = *columnHeaders.volumeInfo;
	this->ooi->info = *columnHeaders.ooiInfo;
	this->secid->info = *columnHeaders.secidInfo;
	this->optionId->info = *columnHeaders.optionIdInfo;
	this->settleFlag->info = *columnHeaders.settleFlagInfo;
	this->exchDes->info = *columnHeaders.exchDesInfo;
	this->exerStyle->info = *columnHeaders.exerStyleInfo;


	if (open->info.columnNbr !=0)allData.push_back(open);
	if (high->info.columnNbr !=0)allData.push_back(high);
	if (low->info.columnNbr !=0)allData.push_back(low);
	if (exDate->info.columnNbr !=0)allData.push_back(exDate);
	if (strike->info.columnNbr !=0)allData.push_back(strike);
	if (bBid->info.columnNbr !=0)allData.push_back(bBid);
	if (bOffer->info.columnNbr !=0)allData.push_back(bOffer);
	if (opVolume->info.columnNbr !=0)allData.push_back(opVolume);
	if (ooi->info.columnNbr !=0)allData.push_back(ooi);
	if (secid->info.columnNbr !=0)allData.push_back(secid);
	if (optionId->info.columnNbr !=0)allData.push_back(optionId);
	if (settleFlag->info.columnNbr !=0)allData.push_back(settleFlag);
	if (exchDes->info.columnNbr !=0)allData.push_back(exchDes);
	if (exerStyle->info.columnNbr !=0)allData.push_back(exerStyle);

	isCall = -1; //set default value to isCall
	strikeDivisor = 0; //set strikeDivisor to default error value.

	std::sort(allData.begin(), allData.end(), columnNbrComp);
}


Options::~Options(){
	delete open;
	delete high;
	delete low;
	delete exDate;
	delete strike;
	delete bBid;
	delete bOffer;
	delete opVolume;
	delete ooi;
	delete optionId;
	delete settleFlag;
	delete exchDes;
	delete exerStyle;
}


void Options::setStrikeDivisor(){
	int checkSize;
	if (this->getNbrRows() > 20)
		checkSize = 20;
	else
		checkSize = (int) this->getNbrRows();

	int i;
	int strikex1000 = 0; //check if we are using Ivydb data

	for (i = 0; i < checkSize; ++i){
		if (int(this->strike->getValue_at(i)) % 100 == 0) //account for strikes that end in .5
				++strikex1000;
		else
			--strikex1000;
	}

	if (strikex1000 == checkSize) //if all strikes were way bigger than closes.
		this->strikeDivisor = 1000;	//assume I was using Ivydb
	else if (strikex1000 == -checkSize)
		this->strikeDivisor = 1;
	else
		cout << "Strike data format for: " << this->dataFile << endl <<
		"could not be detected. Please enter strikeDivisor manually:" << endl <<
		">";
		while (this->strikeDivisor == 0){
			cin >> this->strikeDivisor; //strike divisor is set here.
			if (!cin){
				cin.clear();
				while(cin.get() != '\n'){continue;} //discard rest of line.
				this->strikeDivisor = 0;
				cout << "Wrong data type entered. please try again:" << endl <<
						">";
			}
		}
}

//for datafile name.
void Options::setDataFile(string dataFile){
	size_t found;
	size_t foundPoint;
	found=dataFile.find_last_of("/");
	foundPoint = dataFile.find_last_of(".");
	dataFile = dataFile.substr(found+1, foundPoint-(found+1));
	if(this->isCall == true)
		dataFile = dataFile + "_call";
	else
		dataFile = dataFile + "_put";

	this->dataFile = dataFile;
}

unsigned int Options::getExDateIndex(){
	unsigned int i = 0;
	unsigned int exDateIndex = 0;
	for(i = 0; i < this->allData.size(); ++i){
		if (allData.at(i)->info.names.at(0) == "exDate"){
			exDateIndex = i;
		}
	}
	return exDateIndex;
}

unsigned int Options::getStrikeIndex(){
	unsigned int i = 0;
	unsigned int strikeIndex = 0;
	for(i = 0; i < this->allData.size(); ++i){
		if (allData.at(i)->info.names.at(0) == "strike"){
			strikeIndex = i;
		}
	}
	return strikeIndex;
}



void Options::setCallStatus(){
	unsigned int checkSize;
	if (this->getNbrRows() > 200)
		checkSize = 200;
	else
		checkSize = (unsigned int) this->getNbrRows();

	unsigned int i;
	int callStatus = 0; //check if we are using Ivydb data
	double diff = 0; //stores difference between close and strike.

	if(this->close->info.columnNbr != 0){ //if options object contains close column
		for (i = 0; i < checkSize; ++i){
			diff = this->getClose(i)-this->getStrike(i);
			//option type becomes obvious for deep in or out of the money options.
			if((diff/this->getClose(i) > 0.1) || (diff/this->getClose(i) < -0.1)){
				if (diff  > 0)   //close higher than strike
					if(this->getBBid(i) < diff)
						--callStatus;
					else if (this->getBBid(i) > diff)
						++callStatus;
					else
						continue; //unclear what type of option, should normally never happen

				else //diff is negative (never = 0, as we have our main condition)
					if(this->getBBid(i) < -diff) //diff is negative, prices are never, so inverse diff
						--callStatus;
					else if (this->getBBid(i) > -diff)
						++callStatus;
					else
						continue; //unclear what type of option, should normally never happen
			}
		}

		if (callStatus > 0) //if it looks like a call.
			this->isCall = true;	//assume I have calls
		else if (callStatus < 0)
			this->isCall = false;
		else
			cout << "Options data type for: " << this->dataFile << endl <<
			"could not be detected. Please enter isCall manually:" << endl <<
			"Enter 0 if data contains puts and 1 if data contains calls or -1 if "
			<< endl << "unsure or unimportant."
			">";
			while (this->isCall == 0){
				cin >> this->isCall; //strike divisor is set here.
				if (!cin){
					cin.clear();
					while(cin.get() != '\n'){continue;} //discard rest of line.
					cout << "Wrong data type entered. please try again:" << endl << ">";
					this->isCall = -1;
				}
				else if((this->isCall != -1) && (this->isCall != 0) &&(this->isCall != 1)){
					cout << " Please enter either 1, 0 or -1." << endl << ">";
					this->isCall = -1;
				}
			}

	}

	else// do nothing, isCall stays at -1. User has to change that in strat function
		cerr << "Warning: Could not set callStatus, user will have to do so in strat funciton" << endl;


}

long Options::getNbrBytes(){
	long nbrBytes = Asset::getNbrBytes();

	nbrBytes += (long)this->getNbrRows()*(sizeof(long)+ 5*sizeof(double));

	return(nbrBytes);

}


void reOrder(const deque<size_t>& indexBase, deque<size_t>** indexToSort){
	deque<size_t>*copy = new deque<size_t> (indexBase.size()); //copy vector.

	size_t i = 0;
	for(i = 0; i < indexBase.size(); ++i)
		copy->at(i) = (*indexToSort)->at(indexBase.at(i)); //essentially sorts the deque according to index

	delete *indexToSort; //free the memory held by column before.
	*indexToSort = copy; //update the address of the column containing deque to the sorted values.

}

void Options::sortData(){
	//first create index deque

	size_t i = 0;
	deque<size_t>*indexEx = new deque<size_t>;
	deque<size_t>indexStrike;
	for (i = 0; i < this->getNbrRows(); ++i){
		indexEx->push_back(i);
		indexStrike.push_back(i);
	}



	size_t begDateIndex = 0, endDateIndex = 0, begExIndex = 0, endExIndex = 0, dataSize = this->getNbrRows(); //variables used to run through the deque
	while (begDateIndex < dataSize){ //while I am not out of the date bounds

		while(this->getDate(endDateIndex) == this->getDate(begDateIndex)){ //while I am on one date
			++endDateIndex;
			if(endDateIndex == dataSize) //if data read was the end of the deque
				break; // break out of loop
		}

		sort(indexEx->begin() + begDateIndex, indexEx->begin() + endDateIndex, Comp<long>(this->exDate->getLgColumnPtr()));

		begDateIndex = endDateIndex; //update begDateIndex to start at first of next date.


	}

	this->strike->reOrder(indexEx);  //reorder our vectors according to sorted index.
	this->exDate->reOrder(indexEx);
	begDateIndex = 0, endDateIndex = 0, begExIndex = 0, endExIndex = 0; //reset all parameters


	while (begDateIndex < dataSize){ //while I am not out of the date bounds
		//getDate to code
		while(this->getDate(endDateIndex) == this->getDate(begDateIndex)){ //while I am on one date
			++endDateIndex;
			if(endDateIndex == dataSize) //if data read was the end of the deque
				break; // break out of loop
		}
		//got the endDate value set.
		// do not sort last of row which has different date if exception is true


		begDateIndex = endDateIndex; //update begDateIndex to start at first of next date.

		while (begExIndex < begDateIndex){// while I am not out of the expiration date bounds.
			//getExDate to code.
			while(this->getExDate(endExIndex) == this->getExDate(begExIndex)){ //while I am on one expiration date
				++endExIndex;
				if(endExIndex == begDateIndex)
					break; //break out of loop in order not to try to access an out of bound object/row
			}

			sort(indexStrike.begin() + begExIndex, indexStrike.begin() + endExIndex, Comp<double>(this->strike->getDblColumnPtr()));
			begExIndex = endExIndex; //update begExIndex to start at first of next expiration date
		}

	}

	reOrder(indexStrike, &indexEx);

	for (i = 0; i < allData.size(); ++i){
		if((allData[i]->info.names.at(0) == "exDate") || (allData[i]->info.names.at(0) == "strike"))
			allData[i]->reOrder(&indexStrike);
		else
			allData[i]->reOrder(indexEx);  //reorder our vectors according to sorted index.
	}


	delete indexEx;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//For OptionsImpVol
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionsImpVol::OptionsImpVol(Columns& columnHeaders):Options(columnHeaders){
	impVol = new colDataDouble;
	this->impVol->info = *columnHeaders.iVolInfo;
	allData.push_back(impVol); //no condition as impVol objects have impVol column

	std::sort(allData.begin(), allData.end(), columnNbrComp);
}

OptionsImpVol::~OptionsImpVol(){
	delete impVol;
}



long OptionsImpVol::getNbrBytes(){
	long nbrBytes = Options::getNbrBytes();

	nbrBytes += (long)this->getNbrRows()*(sizeof(double));

	return(nbrBytes);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//For OptionsDelta
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OptionsDelta::OptionsDelta(Columns& columnHeaders):OptionsImpVol(columnHeaders){
	delta = new colDataDouble;
	this->delta->info = *columnHeaders.deltaInfo;
	allData.push_back(delta);

	std::sort(allData.begin(), allData.end(), columnNbrComp);
}

OptionsDelta::~OptionsDelta(){
	delete delta;
}

void OptionsDelta::setCallStatus(){ //using deltas to figure out call status.
	unsigned int i = 0;
	while (this->getDelta(i) == 0)
		{++i; continue;} //just skip all rows with 0 value delta.
	if (this->getDelta(i) > 0) //delta is positive
		this->isCall = 1; //we have calls
	else
		this->isCall = 0; //we have puts
}

long OptionsDelta::getNbrBytes(){
	long nbrBytes = OptionsImpVol::getNbrBytes();

	nbrBytes += (long)this->getNbrRows() * (sizeof(double));

	return(nbrBytes);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//For OptionGreeks
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


OptionsGreeks::OptionsGreeks(Columns& columnHeaders):OptionsDelta(columnHeaders){
	gamma = new colDataDouble;
	vega = new colDataDouble;
	theta = new colDataDouble;

	this->gamma->info = *columnHeaders.gammaInfo;
	this->vega->info = *columnHeaders.vegaInfo;
	this->theta->info = *columnHeaders.thetaInfo;

	if (gamma->info.columnNbr !=0)allData.push_back(gamma);
	if (vega->info.columnNbr !=0)allData.push_back(vega);
	if (theta->info.columnNbr !=0)allData.push_back(theta);

	std::sort(allData.begin(), allData.end(), columnNbrComp);
}

OptionsGreeks::~OptionsGreeks(){
	delete gamma;
	delete vega;
	delete theta;
}


long OptionsGreeks::getNbrBytes(){
	long nbrBytes = OptionsDelta::getNbrBytes();

	nbrBytes += (long)this->getNbrRows() * (3*sizeof(double));

	return(nbrBytes);

}

