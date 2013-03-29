/*
 * DateConverter.cpp
 *
 *  Created on: Mar 6, 2012
 *      Author: john-d
 */

#include <iostream>
#include <fstream> // file I/O support
#include <cstdlib> // support for exit()
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#include <stdlib.h>

using namespace std;

long DMYToExcelSerialDate(string date)
{
	if (date.size() < 10) // the date is already in excel format. Return string transformed to long using atoi.
		return(atoi(date.c_str()));
	else{ // we have a date in human readable format. Convert as required.
		int nYear, nMonth, nDay;

			//cin >> date;
			int y, e, a, r, m,o, d,s;
			y = date[0] -48; e = date[1]-48; a = date[2]-48; r = date[3]-48; //substracting 48, because numbers start at that level in the ASCII library
			nYear = 1000*y + 100*e + 10*a + r;
			m = date[5]-48; o = date[6]-48;
			nMonth = 10*m + o;
			d = date[8]-48; s = date[9]-48;
			nDay = 10*d + s;



		    // Excel/Lotus 123 have a bug with 29-02-1900. 1900 is not a

		    // leap year, but Excel/Lotus 123 think it is...

		    if (nDay == 29 && nMonth == 02 && nYear==1900)
		        return 60;

		    // DMY to Modified Julian calculatie with an extra substraction of 2415019.

		    long nSerialDate =
		            int(( 1461 * ( nYear + 4800 + int(( nMonth - 14 ) / 12) ) ) / 4) +
		            int(( 367 * ( nMonth - 2 - 12 * ( ( nMonth - 14 ) / 12 ) ) ) / 12) -
		            int(( 3 * ( int(( nYear + 4900 + int(( nMonth - 14 ) / 12) ) / 100) ) ) / 4) +
		            nDay - 2415019 - 32075;

		    if (nSerialDate < 60)
		    {
		        // Because of the 29-02-1900 bug, any serial date

		        // under 60 is one off... Compensate.

		        nSerialDate--;
		    }

		    return nSerialDate;
	}

}
