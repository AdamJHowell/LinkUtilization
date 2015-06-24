/*
File Prologue
Name: Adam Howell
Project: SNMP Link Utilization
Date: 2014-11-11
Description: This project will calculate link utilization from two SNMP Walks.
A great deal of information about OIDs can be found here: http://tools.cisco.com/Support/SNMP/do/BrowseOID.do?local=en

I declare that the following source code was written by me, or is common knowledge, or was taken from the public domain.
*/

/*
Pseudocode:
Open the first walk.
Read the first sysUpTime (.1.3.6.1.2.1.1.3.0).
Locate all links in the first walk (1.3.6.1.2.1.2.2.1.1).
Open the second walk.
Read the second sysUpTime (.1.3.6.1.2.1.1.3.0).
Locate all links in the second walk (1.3.6.1.2.1.2.2.1.1).
Subtract the first sysUpTime from the second sysUpTime to get the timeDelta.
Discard links that do not exist in both walks.  Optionally, print a list of these discareded links to the screen.
If the second sysUpTime is not greater than the first, print an error and exit.
Print to screen a list of links that exist in both walks and prompt the user which link to run utilization on (ifIndex).

For the selected link, read from the first walk:
	sysUpTime (.1.3.6.1.2.1.1.3.0)
	ifSpeed (.1.3.6.1.2.1.2.2.1.5.13)
	ifInOctets (.1.3.6.1.2.1.2.2.1.10.13) with its counter size
	ifOutOctets (.1.3.6.1.2.1.2.2.1.16.13) with its counter size
For the selected link, read from the second walk:
	ifSpeed (.1.3.6.1.2.1.2.2.1.5.13)
	ifInOctets (.1.3.6.1.2.1.2.2.1.10.13) with its counter size
	ifOutOctets (.1.3.6.1.2.1.2.2.1.16.13) with its counter size
Counter32 max value: 4294967295
Counter64 max value: 18446744073709551615
If the ifSpeed values or counter sizes do not match, print an error and exit.
Divide the timeDelta by the ifInSpeed to get maxRate.

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
#include <boost/filesystem.hpp>


using namespace std;
using namespace boost::filesystem;


const string HEADER1 = "Please select the link you wish to calculate utilization for:\n";
const string INFILE1 = "walk1.txt";			// Hard-coded input file 1.
const string INFILE2 = "walk2.txt";			// Hard-coded input file 2.
const int COUNTER32MAX = 4294967295;			// The maximum value a Counter32 can hold.
//const long COUNTER64MAX = 18446744073709551615;	// The maximum value a Counter64 can hold.
const int ARRAYSIZE = 100000;					// The size of the array for the files.


int fileRead( ifstream& _handle, string _array[] );									// My file reading function.
void splitString( const string _str, string& _orig, string& _dest, int& _mile, int& _cost );	// My string splitting function.


int main()
{
	string ifIndex = "";		// This is a string, because it will be taken as the users choice and suffixed to the "base" OIDs.
	string ifDescr = "";		// This string will contain the link description.
	string readTemp = "";		// Temporary string to hold data, usually before converting it to an int.
	string sysUpTimeOID = ".1.3.6.1.2.1.1.3.0";	// Search string.
	int sysUpTime1 = 0;			// The system up time from the first walk.
	int ifSpeed1 = 0;			// The interface speed for the selected link from the first walk.
	int ifInOctets1 = 0;		// The inbound octet count for the selected link from the first walk.
	int ifOutOctets1 = 0;		// The outbound octet count for the selected link from the first walk.
	int sysUpTime2 = 0;			// The system up time from the second walk.
	int ifSpeed2 = 0;			// The interface speed for the selected link in the second walk
	int ifInOctets2 = 0;		// The inbound octet count for the selected link in the second walk.
	int ifOutOctets2 = 0;		// The outbound octet count for the selected link in the second walk.
	int timeDelta = 0;			// The difference between the first and second up times.
	int maxRate = 0;			// The maximum rate of the links.
	int ifInDelta = 0;			// The difference between the first and second inbound octet counts.
	int inLinkUtilization = 0; 	// The inbound link utilization.
	int ifOutDelta = 0;			// The difference between the first and second outbound octet counts.
	int outLinkUtilization = 0;	// The outbound link utilization.
	int ifTotalDelta = 0;		// The sum of the inbound and outbound octet deltas.
	int TotalLinkUtilization = 0;	// The total link utilization.
	int fileCount1 = 0;			// The number of lines in input file 1.
	int ifIndexOffset = 0;		// The offset in characters of the ifIndex.

	// Print the PWD to the screen.
	boost::filesystem::path new_full_path( boost::filesystem::current_path() );
	std::cout << "Current path is:\n" << new_full_path << std::endl;

	// Open a filestream for the first SNMP walk.
	ifstream dataFile1( INFILE1 );

	// Test for file error.
	if ( dataFile1.fail() )
	{
		// Announce that we could not open the file.
		cout << "Could not open file \"" << INFILE1 << "\" for reading." << endl << endl;
	}
	// If the file opens properly.
	else
	{
		// Test code.
		//cout << "Opened \"" << INFILE1 << "\" for reading." << endl;

		// Create arrays to hold each file.
		string* walk1Array = new string[ARRAYSIZE];
//		string* walk2Array = new string[ARRAYSIZE];

		// Read the file into the array
		fileCount1 = fileRead( dataFile1, walk1Array );

		// Loop through the first array until we find sysUpTime(.1.3.6.1.2.1.1.3.0)
		// The actual uptime ticks will be at offest 32, and will not be a fixed length, so search to EOL.
		for (int i = 0; i < fileCount1; i++)
		{
			// Search for OID .1.3.6.1.2.1.1.3.0
			ifIndexOffset = walk1Array[i].find(sysUpTimeOID);
		}
		cout << endl;

		// Loop through the first array, locating each link.
		for ( int i = 0; i < fileCount1; i++ )
		{
			// Search for OID .1.3.6.1.2.1.2.2.1.1.x
//			ifIndexOffset = walk1Array.find( "1.3.6.1.2.1.2.2.1.1.", 0 );
		}
		cout << endl;
	}
	// Print the program header.
	cout << HEADER1 << endl;

	// Pause so the user can read the screen.
	system( "PAUSE" );
	return 0;
}// End main().


// Function name:	fileRead()
// Purpose:		This function will read the content of the provided file handle into an array.
// Parameters:		The file handle to read, and an array to write to.
// Returns:		The length of the file read.
// Preconditions:	none
// Postconditions:	none
int fileRead( ifstream& _handle, string _array[] )
{
	string fileStr = "";	// Variable to temporarily hold the contents read from the file.
	int length = 0;		// Variable to count the lines in the file.

	// Loop until End Of File, reading one number at a time.
	while ( !_handle.eof() )
	{
		// Test code.
		//cout << "Entered the while() loop." << endl;

		// Read one line from dataFile1 into fileStr.
		getline( _handle, fileStr );

		// Test code.
		//cout << "Read line as " << fileStr << endl;

		// Insert the line read from the file into the array.
		_array[length] = fileStr;

		// Increment length.
		length++;

		// Test code.
		//cout << "Total values read so far: " << length << endl;
	}
	return length;
} // End fileRead().


// Function name:	splitString()
// Purpose:		This function will split a string based on whitespace.
// Parameters:		A string to split, a string for the origination city, a string for the destination city, an integer for the distance, and an integer for the cost.
// Returns:		none
// Preconditions:	The first parameter MUST be a string of "alpha whitespace alpha whitespace integer whitespace integer".
// Postconditions:	none
void splitString( const string _str, string& _orig, string& _dest, int& _mile, int& _cost )
{
	// A typical line from a SNMP Walk will have spaces between objects.  For example, ifDescr looks like this: .1.3.6.1.2.1.2.2.1.2.1 = STRING: "lo"
	string origination = "";		// This will temporarily hold mileage characters before assigning them to _orig.
	string destination = "";		// This will temporarily hold mileage characters before assigning them to _dest.
	string mileStr = "";		// This will temporarily hold mileage character numbers before stoi converts them to true integers.
	string costStr = "";		// This will temporarily hold cost character numbers before stoi converts them to true integers.
	int whiteSpaceCount = 0;		// This will keep track of what segments have been processed.
	bool prevWhite = false;		// A temp variable to keep track of consecutive whitespace.

	// Test code.
	//cout << "String length: " << _str.length() << endl;

	// Split _str into segments.
	for ( unsigned int i = 0; i < _str.length(); i++ )
	{
		// Check for letters.
		if ( ( _str.at( i ) >= 'A' && _str.at( i ) <= 'Z' ) || ( _str.at( i ) >= 'a' && _str.at( i ) <= 'z' ) )
		{
			// Test code.
			//cout << _str.at(i);
			if ( whiteSpaceCount < 1 )
			{
				// Append the char at i to _orig.
				origination += _str.at( i );
				prevWhite = false;
			}
			else
			{
				// Append the char at i to _dest.
				destination += _str.at( i );
				prevWhite = false;
			}
		}
		// Check for double-quotes, used to delimit strings in an SNMP Walk.  This will be used to retrieve the ifDescr, and possibly other strings.
		else if ( _str.at( i ) == '\"' )
		{
			// If we are not yet to the 4th column.
			if ( whiteSpaceCount < 2 )
			{
				//more work to do here~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			}
		}
		// Check for numbers.
		else if ( _str.at( i ) >= '0' && _str.at( i ) <= '9' )
		{
			// If we are not yet to the 4th column.
			if ( whiteSpaceCount < 3 )
			{
				// Append the char at i to mileStr.
				mileStr += _str.at( i );
				prevWhite = false;
			}
			else
			{
				// Append the char at i to costStr.
				costStr += _str.at( i );
				prevWhite = false;
			}
		}
		// Check for white space.
		else if ( _str.at( i ) == ' ' || _str.at( i ) == '\t' )
		{
			// This is how I track which segments have been dealt with.
			// If the previous character was whitespace.
			if ( prevWhite == false )
			{
				whiteSpaceCount++;
				prevWhite = true;
			}

			// Test code.
			//cout << "White spaces encountered thus far: " << whiteSpaceCount << endl;
		}
		// Check for dollar sign.
		else if ( _str.at( i ) == '$' )
		{
			prevWhite = false;

			// Test code.
			//cout << "Found a dollar sign ($)." << endl;
		}
		// Catch all else.
		else
		{
			cout << "Found an unknown character: " << _str.at( i ) << endl;
			prevWhite = false;
		}
	}
	// Assign all our temp variables to the input parameters.
	_orig = origination;
	_dest = destination;
	_mile = stoi( mileStr );
	_cost = stoi( costStr );
} // End splitString().
