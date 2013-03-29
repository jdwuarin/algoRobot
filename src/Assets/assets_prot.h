/*
 * Assets_prot.h
 *
 *  Created on: Apr 17, 2012
 *      Author: john-d
 */

#ifndef ASSETS_PROT_H_
#define ASSETS_PROT_H_
#include <iostream>
#include <fstream> // file I/O support
#include <cstdlib> // support for exit()
#include <deque>
#include <string>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include <sys/stat.h>
#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <climits>
#include <vector>
#include <boost/bind.hpp>
#include <boost/function.hpp>

using namespace std;



struct colInfo{
	vector<string>names;
	unsigned int length;
	int columnNbr;
};


class colData{
public:
	colInfo info;

	colData(){}
	virtual ~colData(){}
	virtual const double  getCurrValue(){this->calledBadObj();exit(EXIT_FAILURE);} //although all values stored might not be double,
	virtual void setCurrValue(const double& _currValue){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual void setCurrValue(const long& _currValue){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual void writeCurrValue(){this->calledBadObj();exit(EXIT_FAILURE);}			//all returned values are of type double by convention to
																				//allow for computation with any value.
	virtual const double getValue_at(const size_t& pos){this->calledBadObj();exit(EXIT_FAILURE);} //get value in double column deque at position pos.

	inline virtual const double getLastValue(){this->calledBadObj();exit(EXIT_FAILURE);}

	virtual void push_back(const double& value){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual void push_back(const long& value){this->calledBadObj();exit(EXIT_FAILURE);}

	virtual void eraseEntry(const size_t& pos){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual void eraseRange(const size_t& start,const size_t& finish){
		this->calledBadObj();exit(EXIT_FAILURE);
	}


	virtual const size_t getColSize(){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual const double findValue(const long& value){this->calledBadObj();exit(EXIT_FAILURE);} //return row number of first found value.

	virtual deque<double>* getDblColumnPtr(){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual deque<long>* getLgColumnPtr(){this->calledBadObj();exit(EXIT_FAILURE);}
	virtual void reOrder(const deque<size_t>* index){this->calledBadObj();exit(EXIT_FAILURE);} //used for sorting

	void calledBadObj(){
		cout << "Wrong object called, object " << this->info.names.at(0) << " must be initialized to\n"
					"colDataDouble or colDataLong, or other if exists\n";
	}
};


class colDataDouble: public colData{

	deque<double>*column;
	double currValue;
public:

	colDataDouble(){column = new deque<double>; currValue = 0;}
	virtual ~colDataDouble(){delete column;}
	const double getCurrValue(){return this->currValue;}
	void setCurrValue(const double& _currValue){this->currValue = _currValue;}
	void writeCurrValue(){this->column->push_back(currValue);}
	const double getValue_at(const size_t& pos){return this->column->at(pos);}
	const double getLastValue(){
		if(this->column->size() == 0)
			return LONG_MAX;
		else
			return(this->column->back());
	}

	void push_back(const double& value){this->column->push_back(value);}
	void eraseEntry(const size_t& pos){this->column->erase(this->column->begin() + pos);}
	virtual void eraseRange(const size_t& start,const size_t& finish){
		this->column->erase(this->column->begin()+ start,
				this->column->begin()+finish);
	}

	const size_t getColSize(){return column->size();}

	inline const double findValue(const long& value) {
		return((find(this->column->begin(), this->column->end(), value)) - this->column->begin());}

	deque<double>* getDblColumnPtr(){return this->column;} //returns address of column vector to use in sorting

	virtual void reOrder(const deque<size_t>* index){
		deque<double>*copy = new deque<double> (this->column->size()); //copy vector.

		size_t i = 0;
		for(i = 0; i < this->column->size(); ++i)
			copy->at(i) = this->column->at(index->at(i)); //essentially sorts the deque according to index

		delete this->column;
		this->column = copy; //update the address of the column containing deque to the sorted values.
							  //this effectively frees the memory held by column before.
	}


};

class colDataLong: public colData{

	deque<long>*column;
	long currValue;
public:

	colDataLong(){column = new deque<long>; currValue = 0;}
	virtual ~colDataLong(){delete column;}
	const double getCurrValue(){return double (this->currValue);}
	void setCurrValue(const long& _currValue){this->currValue = _currValue;}
	void setCurrValue(const double& _currValue){this->currValue = long(_currValue);}
	void writeCurrValue(){this->column->push_back(currValue);}
	const double getValue_at(const size_t& pos){return double (this->column->at(pos));}
	const double getLastValue(){
		if(this->column->size() == 0)
			return LONG_MAX;
		else
			return double (this->column->back());
	}


	void push_back(const long& value){this->column->push_back(value);}
	void eraseEntry(const size_t& pos){this->column->erase(this->column->begin()+ pos);}
	virtual void eraseRange(const size_t& start,const size_t& finish){
		this->column->erase(this->column->begin()+ start,
				this->column->begin()+finish);
	}

	const size_t getColSize(){return column->size();}

	inline const double findValue(const long& value) {
		return((find(this->column->begin(), this->column->end(), value)) - this->column->begin());}

	deque<long>* getLgColumnPtr(){return this->column;}

	virtual void reOrder(const deque<size_t>* index){
		deque<long>*copy = new deque<long> (this->column->size()); //copy vector.

		size_t i = 0;
		for(i = 0; i < this->column->size(); ++i)
			copy->at(i) = this->column->at(index->at(i)); //essentially sorts the deque according to index

		delete this->column;
		this->column = copy; //update the address of the column containing deque to the sorted values.
							  //this effectively frees the memory held by column before.

	}
};



#endif /* ASSETS_PROT_H_ */
