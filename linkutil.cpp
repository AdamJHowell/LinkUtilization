/*
File Prologue
Name: Adam Howell
Project: SNMP Link Utilization
Date: 2014-11-11
Description: This project will calculate link utilization from two SNMP Walks.

I declare that the following source code was written by me, or taken from public domain sources.
*/

/*
Pseudocode:
Open the first walk.
Open the second walk.
Locate all links in the first walk.
Locate all links in the second walk.
Discard links that do not exist in both walks.  Optionally, print a list of these discareded links to the screen.
Print to screen a list of links that exist in both walks and prompt the user which link to run utilization on.

For the selected link, read from the first walk: sysUpTime (.1.3.6.1.2.1.1.3.0), ifSpeed (.1.3.6.1.2.1.2.2.1.5.13), ifInOctets (.1.3.6.1.2.1.2.2.1.10.13) with its counter size, and ifOutOctets (.1.3.6.1.2.1.2.2.1.16.13) with its counter size.
For the selected link, read from the second walk: sysUpTime (.1.3.6.1.2.1.1.3.0), ifSpeed (.1.3.6.1.2.1.2.2.1.5.13), ifInOctets (.1.3.6.1.2.1.2.2.1.10.13) with its counter size, and ifOutOctets (.1.3.6.1.2.1.2.2.1.16.13) with its counter size.
Counter32 max value: 4294967295
Counter64 max value: 18446744073709551615
If the second sysUpTime is not greater than the first, print an error and exit.
If the ifSpeed values or counter sizes do not match, print an error and exit.
Subtract the first sysUpTime from the second sysUpTime to get the timeDelta
Divide the timeDelta by the ifInSpeed to get maxRate

If the second ifInOctets is not greater than the first ifInOctets, add the counter size to the second ifInOctets.
Subtract the first ifInOctets from the second ifInOctets to get the ifInDelta.
Multiply the ifInDelta by 8, then by 100, then divide by maxRate to get inLinkUtilization.
Display inLinkUtilization (along with other variables).

If the second ifOutOctets is not greater than the first ifOutOctets, add the counter size to the second ifOutOctets.
Subtract the first ifOutOctets from the second ifOutOctets to get the ifOutDelta.
Multiply the ifOutDelta by 8, then by 100, then divide by maxRate to get outLinkUtilization.
Display outLinkUtilization (along with other variables).

Sum ifInDelta with ifOutDelta to get ifTotalDelta.
Multiply the ifTotalDelta by 8, then by 100, then divide by maxRate to get TotalLinkUtilization.
Display TotalLinkUtilization (along with other variables).
*/

//#include <dos.h>
#include <windows.h>	// For directory listing
#include <iostream>
#include <string>
#include <fstream>
#include <array>


using namespace std;


const string HEADER1 = "Please select the link you wish to calculate utilization for:\n";


int main()
{
	cout << HEADER1 << endl;
	
	// Pause so the user can read the screen.
	system( "PAUSE" );
	return 0;
}// End main().
