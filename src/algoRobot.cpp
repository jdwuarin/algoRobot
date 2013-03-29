

//============================================================================
// Name        : AlgoRobot.cpp
// Author      : John-David Wuarin
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : This program allows a user to easily test and implement any trading strategy he/she
//				 has in mind. It is supposed to be clean and easy to edit. And allows a user to easily implement any
//				 trading strategy by writing just the few lines of code required for the strategy.
//============================================================================


#include "blackBoxes_prot.h"


using namespace std;
#include <climits>

const double version = 1.0;

void help(char *argv0);

int main(int argc, char *argv[])
{

	string dataFile;
	string option;

	if (argc == 1){ //no parameters passed and no file to read...
		cout << "No file or folder was specified. The program will be run in default mode\n"
				"Please specify a file to open:\n>  \n";
		dataFile = "/home/john-d/Data/test.cfg";  //getline(cin, dataFile);
		scanBtCfg(dataFile.c_str());

	}

	else if ((argc == 2) || (argc > 3)){
		cerr << "\nThe Program requires an option AND 1 Path to file/folder\n\n";
		help(argv[0]);
	    exit(EXIT_FAILURE);

	}

	else{

		option = argv[1];
		if ((option == "-a") || (option == "--add")){
			cout << "\nOpening portfolio folder " << argv[2] << endl;
			//addStrategy(argv[2]);
		}

		else if((option == "-b") || (option == "--back-test")){
			scanForFiles(argv[2], false);
		}

		else if((option == "-cb") || (option == "--config-back-test")){
			scanBtCfg(argv[2]);
		}

		else if((option == "-m") || (option == "--multifile-back-test")){
			scanForFiles(argv[2], true);
		}

		else if ((option == "-r") || (option == "--run")){
			//scanForFiles(argv[2]);
			cout << "running strategies on portfolio " << argv[2] << ":\n";
			//runMock();
			//runReal;

		}
		else if ((option == "-u") || (option == "--update")){
			updateData(argv[3]);
		}

		else if ((option == "-h") || (option == "--help")){
			cout << endl;
			help(argv[0]);
			exit(EXIT_FAILURE);
		}
		else{
			cerr << "\nUnrecognized parameter: " << argv[1] << endl;
			help(argv[0]);
			exit(EXIT_FAILURE);
		}

	}


	//scanForFiles();



	return 0;
}

void help(char *argv0){
	cout.setf(ios_base::fixed, ios_base::floatfield); cout.precision(2);
	cout << "AlgoRobot version " << version << ':' << endl;
	cout << "Usage: " << argv0 << " [OPTION] [PATH_TO_FIE/FOLDER]\n\n";
	cout << "This program allows a user to test, implement and trade electronically any strategy\n"
			"he/she has in mind. New strategies can easily be hard coded and edited in the source code.\n"
			"And allows a user to easily implement any trading strategy\n\n";
	cout.setf(ios_base::left, ios_base::adjustfield);

	cout.width(13);
	cout << "Option";
	cout.width(23);
	cout << "Long Option";
	cout << "Meaning" << endl << endl;

	cout.width(13);
	cout << "  -a";
	cout.width(23);
	cout << "--add";
	cout << "Allows a user to add a strategy to" << endl;
	cout.width(36); cout << " ";
	cout << "automated scheme. Takes folder as argument" << endl;

	cout.width(13);
	cout << "  -b";
	cout.width(23);
	cout << "--back-test";
	cout << "Allows a user to back-test one or multiple" << endl;
	cout.width(36); cout << " ";
	cout << "strategy(ies) to be determined. " << endl;
	cout.width(36); cout << " ";
	cout  << "Takes data file or folder containing"<< endl;
	cout.width(36); cout << " ";
	cout << "only data files as argument" << endl;

	cout.width(13);
	cout << "  -cb";
	cout.width(23);
	cout << "--config-back-test";
	cout << "Allows a user to back-test strategies" << endl;
	cout.width(36); cout << " ";
	cout << "based on a configuration file. " << endl;
	cout.width(36); cout << " ";
	cout  << "Takes configuration file as"<< endl;
	cout.width(36); cout << " ";
	cout << "argument" << endl;

	cout.width(13);
	cout << "  -m";
	cout.width(23);
	cout << "--multifile-back-test";
	cout << "Allows a user to back-test one or multiple" << endl;
	cout.width(36); cout << " ";
	cout << "strategy(ies) to be determined" <<endl;
	cout.width(36); cout << " ";
	cout << "on files related to one another." << endl;
	cout.width(36); cout << " ";
	cout <<"Takes a folder of data or a"<< endl;
	cout.width(36); cout << " ";
	cout << "folder of folders containing only" << endl;
	cout.width(36); cout << " ";
	cout << "the related data files as argument" << endl;

	cout.width(13);
	cout << "  -r";
	cout.width(23);
	cout << "--run";
	cout.width(40);
	cout << "Update all data, then run real and " << endl;
	cout.width(36); cout << " ";
	cout << "mock strategies for portfolio." << endl;
	cout.width(36); cout << " ";
	cout << "Takes folder as argument" << endl;

	cout.width(13);
	cout << "  -u";
	cout.width(23);
	cout << "--update";
	cout.width(40);
	cout << "Update all data without running strategies." << endl;

	cout.width(13);
	cout << "<no option>";
	cout.width(23);
	cout << "<no argument>";
	cout.width(90);
	cout << "Enter user-mode. Allows to back-test" << endl;
	cout.width(36); cout << " ";
	cout << "and create configuration folders with" << endl ;
	cout.width(36); cout << " ";
	cout << "accompanying files" << endl;

	cout.width(13);
	cout << "  -h";
	cout.width(23);
	cout << "--help";
	cout.width(90);
	cout << "Print out this help"<< endl;

	cout.unsetf(ios_base::floatfield); cout.precision(6); //resetting cout to default after changing it to print version.

}
