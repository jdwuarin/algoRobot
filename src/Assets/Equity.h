/*
 * Equity.h
 *
 *  Created on: Apr 18, 2012
 *      Author: john-d
 */

#ifndef EQUITY_H_
#define EQUITY_H_
#include "Asset.h"

class Equity: public Asset {

protected:
	colData* open;
	colData* high;
	colData* low;
	colData* volume;
	colData* secid; //identifier for security. Only one per security, no other company can ever have the same.
	colData* indGroup; //industry group, see ivyDB manual for meaning
	colData* cfadj; //cumulative adjustment factor. 1 at start, 2 if 1:2 split etc... look at value at beginning of dataset.
	colData* shrout; //shares outstanding
public:
	Equity(Columns& columnHeaders);
	virtual ~Equity();

	bool getIsEquity(){return true;}
	virtual const double getOpen(const size_t& pos){return(this->open->getValue_at(pos));}
	virtual const double getHigh(const size_t& pos){return(this->high->getValue_at(pos));}
	virtual const double getLow(const size_t& pos){return(this->low->getValue_at(pos));}
	virtual const double getVolume(const size_t& pos){return(this->volume->getValue_at(pos));}
	virtual const double getSecid(const size_t& pos){return(this->secid->getValue_at(pos));}
	virtual const double getIndGroup(const size_t& pos){return(this->indGroup->getValue_at(pos));}
	virtual const double getCfadj(const size_t& pos){return(this->cfadj->getValue_at(pos));}
	virtual const double getShrout(const size_t& pos){return(this->shrout->getValue_at(pos));}

	virtual long getNbrBytes();
};

#endif /* EQUITY_H_ */
