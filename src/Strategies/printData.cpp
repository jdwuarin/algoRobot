/*
 * dummyStrat.cpp
 *
 *  Created on: May 26, 2012
 *      Author: john-d
 */


#include <string>
#include "../blackBoxes_prot.h"
#include "../Assets/Options.h"
#include <climits>


long printData(vector<Asset*>& assets){


	if (assets.at(0)->getNbrObjectCuts() == 0)
		cout << "printing data, this might take some time. " << endl;
	unsigned int i; //run through assets.
	for (i = 0; i < assets.size(); ++i){


		string outputFile = assets.at(i)->getWriteTo()+ "/" +
				assets.at(i)->getDataFile() + ".csv";
		struct stat fStatus;

		if ((stat(outputFile.c_str(), &fStatus) == -1) || (assets.at(0)->getNbrObjectCuts() == 0)){ //file not already created. Or first call
			ofstream output(outputFile.c_str());
			output.precision(10);
			assets.at(i)->printHeaders(output); //print option header
			assets.at(i)->printData(output);
		}
		else{
			ofstream output(outputFile.c_str(), ios_base::out | ios_base::app);
			output.precision(10);
			assets.at(i)->printData(output);
		}

		if(assets.at(0)->getExToDate() == -1) //if end of file was reached.
			cout << "data was saved to: " << outputFile << endl;
	}

	//see if plots work...(only for equity data.)
	if(assets.at(0)->getIsEquity()){
		plotResults(assets.at(0)->getDateColumnPtr(), assets.at(0)->getCloseColumnPtr());
		plotLogResults(assets.at(0)->getDateColumnPtr(), assets.at(0)->getCloseColumnPtr());
	}


return assets.at(0)->getLastDate();
}
