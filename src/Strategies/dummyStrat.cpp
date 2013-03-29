/*
 * dummyStrat.cpp
 *
 *  Created on: May 26, 2012
 *      Author: john-d
 */


#include <string>
#include "../Assets/Options.h"
#include <climits>


long something(vector<Asset*>& object){
	cout << "passed this hurdle..." << endl;

	return object.at(0)->getLastDate();
}



long stopIt(vector<Asset*>& object){
	return 0;
}

long testcalls30Days(vector<Asset*>& object){

	int callObjectIdent;

	if (object.size() > 3){
		exit (EXIT_FAILURE);
		cerr << " Too many objects to test testcalls30Days" << endl;
	}
	if (object.at(0)->getIsCall() != 1)
	{}//callObjectIdent;
return 0;
}
