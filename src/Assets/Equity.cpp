/*
 * Equity.cpp
 *
 *  Created on: Apr 18, 2012
 *      Author: john-d
 */

#include "../blackBoxes_prot.h"

Equity::Equity(Columns& columnHeaders):Asset(columnHeaders){
	open = new colDataDouble;
	high = new colDataDouble;
	low = new colDataDouble;
	volume = new colDataDouble;
	secid = new colDataDouble;
	indGroup = new colDataDouble;
	cfadj = new colDataDouble;
	shrout = new colDataDouble;

	this->open->info = *columnHeaders.openInfo;
	this->high->info = *columnHeaders.highInfo;
	this->low->info = *columnHeaders.lowInfo;
	this->volume->info = *columnHeaders.volumeInfo;
	this->secid->info = *columnHeaders.secidInfo;
	this->indGroup->info = *columnHeaders.indGroupInfo;
	this->cfadj->info = *columnHeaders.cfadjInfo;
	this->shrout->info = *columnHeaders.shroutInfo;

	allData.push_back(open);
	allData.push_back(high);
	allData.push_back(low);
	allData.push_back(volume);
	if (secid->info.columnNbr !=0)allData.push_back(secid);
	if (indGroup->info.columnNbr !=0)allData.push_back(indGroup);
	if (cfadj->info.columnNbr !=0)allData.push_back(cfadj);
	if (shrout->info.columnNbr !=0)allData.push_back(shrout);

	std::sort(allData.begin(), allData.end(), columnNbrComp);
}

long Equity::getNbrBytes(){
	long nbrBytes = Asset::getNbrBytes();

	nbrBytes += long(this->getNbrRows()*(7*sizeof(double)));

	return(nbrBytes);

}

Equity::~Equity() {
	delete open;
	delete high;
	delete low;
	delete volume;
	delete secid;
	delete indGroup;
	delete cfadj;
	delete shrout;
}
