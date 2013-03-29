/*
 * Menu.cpp
 *
 *  Created on: Jun 10, 2012
 *      Author: john-d
 */


#include "../Assets/Options.h"
#include "strategies.h"

vector<string> getParam(stringstream& sstratString, unsigned int nbrParam, bool& failParamParse);

vector<stratFunction> getStrats(vector<Asset*>& assets,
		vector<string>& stratNames, bool runDirect){

	vector<stratFunction>stratsToRun;
	unsigned int i = 0; //run through stratNames if it isn't empty.
	bool failParamParse = false; //used to know if args list was usable
	vector<string>parameters; //will contain the potential parameter list of strategy.
	string stratString; //records the string containing all information concerning
							//the strategy to be added to list
	vector<string>stratNames_2;
	string stratIdent; //records name of strategy without parameters.
	stringstream sstratString; //used to parse the line.

	if(stratNames.size() == 0){
		cout << "Please enter a list of strategies to back-test\n"
				"separated by ENTER.\n"
				"Press done to stop entering names.\n";
	}



	while ((stratString != "done") && (stratString != "exit")){
		if(stratNames.size() == 0){ //we had no config file
			cout << ">";
			getline(cin, stratString); //put line in stratString
		}
		else{
			if(i >= stratNames.size()){ stratString = "done";} //force stratString to stop parsing vector
			else {stratString = stratNames.at(i);} //set stratString
			++i;

		}


		if ((stratString == "done") || (stratString == "exit")){} // do nothing

		else if(stratString == "run"){
			unsigned int j = 0; //run through stratsToRun
			if (stratNames.size() != 0 ){ //I had a config file.
				cerr<< "You may not use the run command in a config file.\n"
						"Exiting:\n"; exit(EXIT_FAILURE);
			}
			else if (runDirect == false){
				cout << "File or Folder is too large to allow direct run."
						"\nPlease enter name of strategy or done to stop.";
				continue;
			}

			for (j = 0; j < stratsToRun.size(); ++j)
				stratsToRun.at(j)(); //run all strategies

			stratsToRun = vector<stratFunction>(); //reset stratsToRun
			stratNames_2 = vector<string>(); //reset stratNames_2
		}


		else{ //we have the actual name of a strategy with arguments.
			sstratString.clear(); //clear potential eofbit
			sstratString.str(stratString); // update sstratString
			getline(sstratString, stratIdent, '(');

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Add conditions for coded functions below.
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (stratIdent == "something"){
				//number of param extracted very important when writing own function.
				parameters = getParam(sstratString, 0, failParamParse);
				if(failParamParse){continue;} //add exception handling here.
				stratsToRun.push_back(boost::bind(something, assets));
				//if function has params:
				//stratsToRun.push_back(boost::bind(something, Assets, atof(parameters.at(0).c_str()),
				//atof(parameters.at(0).c_str())...and so on.); //atof: string to double.
			}

			else if (stratIdent == "printData"){
				//number of param extracted very important when writing own function.
				parameters = getParam(sstratString, 0, failParamParse);
				if(failParamParse){continue;}
				stratsToRun.push_back(boost::bind(printData, assets));
			}

			else if (stratIdent == "optionsVolumeComp1"){
				//number of param extracted very important when writing own function.
				parameters = getParam(sstratString, 1, failParamParse);
				if(failParamParse){continue;}
				stratsToRun.push_back(boost::bind(optionsVolumeComp1, assets,
						atof(parameters.at(0).c_str()) ));
			}

			else{
				cout << "Unrecognized strategy name: " << stratIdent << endl;
				if(stratNames.size() == 0)
						cout << "\nPlease specify strategy to test.\n";
				else
					cout << "Strategy " << stratIdent << " will not be back-tested" <<  endl;
				continue;
			}

			stratNames_2.push_back(stratString);


		}
	}

	if(stratsToRun.empty() && stratNames.size() == 0){ //user pressed done or exit straight away.
		cout << "No strategy was specified. please try again:" << endl;
		stratsToRun = getStrats(assets, stratNames, runDirect); //running recursion

	}

	if(stratNames.empty())
		stratNames = stratNames_2; //return vector of stratNames used.

	return stratsToRun; //else, do nothing, just return the stratsToRun, even if vector is empty


}

vector<string> getParam(stringstream& sstratString, unsigned int nbrParam, bool& failParamParse){
	vector<string>parameters;
	parameters.push_back("\0");
	 //initialize element 0.
	string stratIdent;
	char addchar = ' '; //char to add
	unsigned int i;
	sstratString.seekg(0);
	getline(sstratString, stratIdent, '(');


	if (nbrParam == 0){
		if(sstratString.peek() != ')')
			cerr << "Too many arguments were specified.\n" <<
					stratIdent << " will not be back-tested" << endl;
			failParamParse = true;
			parameters = vector<string>();
	}

	for (i = 0; i < nbrParam; ++i){

		addchar = ' '; // reinitialize addchar.


		if(sstratString.eof() || sstratString.peek() == ')'){ // too few parameters entered.
			failParamParse = true;
			parameters = vector<string>(); //so as not to also return empty argument error
			cerr << "Too few arguments were specified.\n" <<
							stratIdent << " will not be back-tested" << endl;
		}

		while ((!sstratString.eof()) //while I am reading a single param
				&& (addchar != ')') && (addchar != ',')){

			addchar = sstratString.get();
			//only push_back if I have comma or if I am at the
			//first param and there actually is a param.
			if(addchar == ','){
				parameters.push_back("\0");}
			else if (addchar == ')')
				continue;
			else if(addchar != ' '){
				parameters.at(i)+= addchar;}

		}

		//last read element should be ) otherwise, assume too many arguments.
		if ((i == nbrParam-1) && (addchar != ')')){
			cerr << "Too many arguments were specified.\n" <<
					stratIdent << " will not be back-tested" << endl;
			failParamParse = true;
			parameters = vector<string>();
		}

	}


	//quickly check if one of the parameters is empty.
	for (i = 0; i < parameters.size(); ++i){
		if ((nbrParam != 0) && (parameters.at(i) == "\0")){
			cerr << "At least one argument was unspecified.\n"
					"(two commas next to one another?)\n" <<
					stratIdent << " will not be back-tested" << endl;
			failParamParse = true;
		}
	}


	return (parameters);
}
