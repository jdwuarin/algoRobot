/*
 * Options.h
 *
 *  Created on: Mar 21, 2012
 *      Author: john-d
 *      Options Data with less columns than those cited in this header will not be taken into account, as backtesting without any of those values is
 *      relatively useless.
 */


#ifndef OPTIONS_H_
#define OPTIONS_H_
#include "Asset.h"

using namespace std;

class Options: public Asset {
protected:
	colData* open;
	colData* high;
	colData* low;
	colData* exDate;
	colData* strike; //save strike. Divide by 1000 when using ivydb. CHange code if other database doesn't include the x1000
	colData* bBid; //save best bid
	colData* bOffer; //save best offer for day.
	colData* opVolume; // save volume for options
	colData* ooi; //save open interest for option
	colData* optionId; //option identifier
	colData* secid; //identifier for security. Only one per security, no other company can ever have the same.


	colData* settleFlag; //special settlement flag
	//= 48 (0) if standard (100 shares)
	//49 (1) if special
	//69 (E) if there was an error

	colData* exchDes; //exchange designator (where it was traded)
	//00000 if delisted
	//00001 if NYSE
	//00002 if AMEX
	//00004 if NASDAQ naional market system
	//00008 if NASDAQ small cap
	//00016 if OTC Bulletin Board
	//32768 if security is index (-1 for standard 16 bit ints)

	colData* exerStyle; //american or european or other.
	// = 65 (A) if American
	// = 69 (E) if european
	// = 63 (?) if unknown or not classified



	int isCall; //member variable indicating if the object contains call data.
	int strikeDivisor; //what number to divide strike by.

public:
	Options(Columns& columnHeaders);

	virtual ~Options();


	//list of accessors
	const double getExDate(const size_t& pos){return(this->exDate->getValue_at(pos));}
	const double getStrike(const size_t& pos){return(this->strike->getValue_at(pos)/this->strikeDivisor);}
	const double getBBid(const size_t& pos){return(this->bBid->getValue_at(pos));}
	const double getBOffer(const size_t& pos){return(this->bOffer->getValue_at(pos));}
	const double getVolume(const size_t& pos){return(this->opVolume->getValue_at(pos));}
	const double getOoi(const size_t& pos){return(this->ooi->getValue_at(pos));}
	const double getOptionId(const size_t& pos){return(this->optionId->getValue_at(pos));}
	const double getSecid(const size_t& pos){return(this->secid->getValue_at(pos));}
	const double getSettleFlag(const size_t& pos){return(this->settleFlag->getValue_at(pos));}
	const double getExchDes(const size_t& pos){return(this->exchDes->getValue_at(pos));}
	const double getExerStyle(const size_t& pos){return(this->exerStyle->getValue_at(pos));}

	void setIsCall(bool isCall) {this->isCall = isCall;} //mutator for isCall.
	int getIsCall(){return isCall;}
	bool getIsOption(){return true;}
	void setDataFile(string dataFile);
	unsigned int getExDateIndex(); //get index within allData, might be useful for strategies
	unsigned int getStrikeIndex();

	int getStrikeDivisor(){return (this->strikeDivisor);}
	void setStrikeDivisor();
	virtual void setCallStatus(); //finds out if data contains calls or puts and sets isCall

	virtual long getNbrBytes();

	virtual void sortData();

	//methods for strategies
	double getNextDayVolume();
	double getNextDayOoi();
};

class OptionsImpVol: public Options{
protected:
	colData* impVol;

public:
	OptionsImpVol(Columns& columnHeaders);

	virtual ~OptionsImpVol();

	const double getImpVol(const size_t& pos){return(this->impVol->getValue_at(pos));}

	virtual long getNbrBytes();

	bool getIsOptionsImpVol(){return true;}


};

class OptionsDelta: public OptionsImpVol{
protected:
	colData* delta;

public:
	OptionsDelta(Columns& columnHeaders);

	virtual ~OptionsDelta();


	const double getDelta(const size_t& pos){return(this->delta->getValue_at(pos));}
	void setCallStatus();

	virtual long getNbrBytes();

	bool getIsOptionsDelta(){return true;}


};

class OptionsGreeks: public OptionsDelta{
protected:
	colData* gamma;
	colData* vega;
	colData* theta;
public:
	OptionsGreeks(Columns& columnHeaders);

	virtual ~OptionsGreeks();


	const double getGamma(const size_t& pos){return(this->gamma->getValue_at(pos));}
	const double getVega(const size_t& pos){return(this->vega->getValue_at(pos));}
	const double getTheta(const size_t& pos){return(this->theta->getValue_at(pos));}

	virtual long getNbrBytes();

	bool getIsOptionGreeks(){return true;}

};

#endif /* OPTIONS_H_ */
