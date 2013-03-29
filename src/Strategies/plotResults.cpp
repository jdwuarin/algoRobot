/*
 * plotResults.cpp
 *
 *  Created on: Sep 27, 2012
 *      Author: john-d
 */


#include "strategies.h"
#include "../gnuplot-iostream.h"



void plotResults(deque<double>*returnS){
	// -persist option makes the window not disappear when program exits
	Gnuplot gp("gnuplot -persist");

	gp << "plot '-' with lines title 'returns'\n";
	gp.send(returnS->begin(), returnS->end());

}


void plotResults(deque<long>*dateS, deque<double>*returnS){
	// -persist option makes the window not disappear when program exits
	Gnuplot gp("gnuplot -persist");

	gp << "plot '-' with lines title 'returns'\n";
	gp.send(dateS->begin(), dateS->end(), returnS->begin(), returnS->end());

}

void plotLogResults(deque<double>*returnS){
	// -persist option makes the window not disappear when program exits
	Gnuplot gp("gnuplot -persist");

	gp << "set logscale y\n";
	gp << "plot '-' with lines title 'log returns'\n";
	gp.send(returnS->begin(), returnS->end());

}


void plotLogResults(deque<long>*dateS, deque<double>*returnS){
	// -persist option makes the window not disappear when program exits
	Gnuplot gp("gnuplot -persist");

	gp << "set logscale y\n";
	gp << "plot '-' with lines title 'log returns'\n";
	gp.send(dateS->begin(), dateS->end(), returnS->begin(), returnS->end());

}


