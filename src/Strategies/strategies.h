/*
 * strategies.h
 *
 *  Created on: May 26, 2012
 *      Author: john-d
 */

#ifndef STRATEGIES_H_
#define STRATEGIES_H_
#include <string>
#include "../Assets/Options.h"
typedef boost::function<long(void)> stratFunction; //create pointer to function used in runStrat using boost
//typedef long (*stratFunction) (vector<Asset*>&); //create pointer to function used in runStrat

long printData(vector<Asset*>& assets);
long stopIt(vector<Asset*>&); // just an indicator function to tell toOpen to stop scanning for function names.
long something(vector<Asset*>&);
long simpleMA(vector<Asset*>&);
long optionsVolumeComp1(vector<Asset*>&, double startMoney);
void plotResults(deque<long>*, deque<double>*);
void plotResults(deque<double>*returnS);
void plotLogResults(deque<long>*dateS, deque<double>*returnS);
void plotLogResults(deque<double>*returnS);




#endif /* STRATEGIES_H_ */
