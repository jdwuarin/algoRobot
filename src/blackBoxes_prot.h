/*
 * prototypes.h
 *
 *  Created on: Mar 21, 2012
 *      Author: john-d
 */

//centralization of most of the preprocessor directives and prototypes, so as not to repeat myself at the beginning of each file.

#ifndef BLACKBOXES_PROT_H_
#define BLACKBOXES_PROT_H_
#include <iostream>
#include <fstream> // file I/O support
#include <cstdlib> // support for exit()
#include <vector>
#include <deque>
#include <map>
#include <utility>
#include <string>
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <typeinfo>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <ctime>
#include <cstring>
#include <climits>
#include <boost/bind.hpp>
#include <boost/function.hpp>
//#include <ALGLIB/specialfunctions.h>
#include "Assets/Equity.h"
#include "Assets/Columns.h"
#include "Strategies/strategies.h"

const double FILE_MX_SZ = 9.9E14;
typedef boost::function<long(void)> stratFunction; //create pointer to function used in runStrat using boost
//typedef long (*stratFunction) (vector<Asset*>&); //create pointer to function used in runStrat


void scanBtCfg(const char*);
void scanForFiles(const char*, bool, vector<string> = vector<string>());
void tryOpenFolder (const char*);
const char* searchFileNonRec (const char*, const char*);
void updateData (const char*);
string getSubFolderName(string subFolder, const string& baseDir);
void statError(const char* fileName, struct stat& fStatus);
vector<string> getNames(string folderString);

void detectData(const string& dataFile, Asset*& pAsset,
		const string& writeTo, long lastDate = 0, double lastByte = 0);
void detectData(const string& dataFile, Asset*& calls, Asset*& puts,
		const string& writeTo, long lastDate = 0, double lastByte = 0);
char getDelim(string& row, const string& dataFile);


long DMYToExcelSerialDate(string);

void toOpen (const string& dataFile, const string& writeTo,
		vector<string>&stratNames);
void toOpen (vector<string>&dataFiles, const string& writeTo,
		vector<string>&stratNames);
bool containsCpFlag(const string&);
long getFilesSize(vector<string>&dataFiles);

vector<stratFunction> getStrats(vector<Asset*>& Assets, //has to take Assets as argument.
		vector<string>& stratNames, bool runDirect);




#endif /* BLACKBOXES_PROT_H_ */
