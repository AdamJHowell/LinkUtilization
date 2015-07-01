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
1. Print the introduction
2. List the PWD
3. Read first walk into array1
4. Read second walk into array2
5. List all interfaces to the user
6. Ask which interface to show stats for
7. Scan both arrays for the chosen interface, and create an Interface class object
8. Display stats from the Interface class object created for the chosen interface
9. Loop back to step 5.

Open the first walk.
Read the first sysUpTime (.1.3.6.1.2.1.1.3.0).
Locate all links in the first walk (1.3.6.1.2.1.2.2.1.1.*).
Open the second walk.
Read the second sysUpTime (.1.3.6.1.2.1.1.3.0).
Locate all links in the second walk (1.3.6.1.2.1.2.2.1.1.*).
If the second sysUpTime is not greater than the first, print an error and exit.
Subtract the first sysUpTime from the second sysUpTime to get the timeDelta.
Discard links that do not exist in both walks.  Optionally, print a list of these discarded links to the screen.
Print to screen a list of links that exist in both walks and prompt the user which link to run utilization on (ifIndex).

For the selected link, read from the both walks:
	ifSpeed (.1.3.6.1.2.1.2.2.1.5.13)
	ifInOctets (.1.3.6.1.2.1.2.2.1.10.13) with its counter size
	ifOutOctets (.1.3.6.1.2.1.2.2.1.16.13) with its counter size
Counter32 max value: 4294967295
Counter64 max value: 18446744073709551615
If the ifSpeed values or counter sizes do not match, print an error and exit.
Divide the timeDelta by the ifInSpeed to get maxRate.

To deal with counter roll-over:
	If the second ifInOctets (or ifOutOctets) is not greater than the first, add the counter size to the second ifInOctets (or ifOutOctets).
Subtract the first ifInOctets (or ifOutOctets) from the second ifInOctets (or ifOutOctets) to get the ifInDelta (or ifOutDelta).
Multiply the delta by 8, then by 100, then divide by maxRate to get inLinkUtilization.
Display link utilization (along with other stats).

Sum ifInDelta with ifOutDelta to get ifTotalDelta.
Multiply the ifTotalDelta by 8, then by 100, then divide by maxRate to get TotalLinkUtilization.
Display TotalLinkUtilization (along with other stats).
*/


#include "Interface.h"


using namespace boost::filesystem;


void greeting1( void );
int fileRead( ifstream& _handle, string _array[] );									// My file reading function.
void oidRead( string _array[], int _ifIndex, Interface& _interface, int _arrayLength, int _walkNumber );	// My interface populating function.
void splitString( const string _str, string& _orig, string& _dest, int& _mile, int& _cost );	// My string splitting function.


int main()
{
	string ifIndex = "";			// This is a string, because it will be taken as the users choice and suffixed to the "base" OIDs.
	string ifDescr = "";			// This string will contain the link description.
	string readTemp = "";			// Temporary string to hold data, usually before converting it to an int.
	string ifIndexArray[MAXINTERFACE];	// An array to hold all discovered interface descriptions, the ifIndex will be the array index.
	int tempInt = 0;				// A temporary integer for random math needs.
	int stringOffset = 0;			// The offest within the line where the search text was found.
	int sysUpTime1 = 0;				// The system up time from the first walk.
	int ifSpeed1 = 0;				// The interface speed for the selected link from the first walk.
	unsigned long ifInOctets1 = 0;	// The inbound octet count for the selected link from the first walk.
	unsigned long ifOutOctets1 = 0;	// The outbound octet count for the selected link from the first walk.
	int sysUpTime2 = 0;				// The system up time from the second walk.
	int ifSpeed2 = 0;				// The interface speed for the selected link in the second walk
	unsigned long ifInOctets2 = 0;	// The inbound octet count for the selected link in the second walk.
	unsigned long ifOutOctets2 = 0;	// The outbound octet count for the selected link in the second walk.
	int timeDelta = 0;				// The difference between the first and second up times.
	int maxRate = 0;				// The maximum rate of the links.
	int ifInDelta = 0;				// The difference between the first and second inbound octet counts.
	int inLinkUtilization = 0; 		// The inbound link utilization.
	int ifOutDelta = 0;				// The difference between the first and second outbound octet counts.
	int outLinkUtilization = 0;		// The outbound link utilization.
	int ifTotalDelta = 0;			// The sum of the inbound and outbound octet deltas.
	int TotalLinkUtilization = 0;		// The total link utilization.
	int fileCount1 = 0;				// The number of lines in input file 1.
	int fileCount2 = 0;				// The number of lines in input file 2.
	int ifIndexOffset = 0;			// The offset in characters of the ifIndex.
	int indexCount = 0;				// The number of interfaces located.

	// Set the decimal precision for float output.
	cout.precision( PRECISION );

	// Print the header and the PWD to the screen.
	greeting1();

	// Create arrays to hold each file.
	string* walk1Array = new string[ARRAYSIZE];
	string* walk2Array = new string[ARRAYSIZE];

	// Create arrays to hold each ifIndex and ifDescr.
	string* walk1IndexDescr = new string[MAXINTERFACE];
	string* walk2IndexDescr = new string[MAXINTERFACE];

#pragma region Walk1

	// Open a filestream for the first SNMP walk.
	ifstream dataFile1( INFILE1 );

	// Test for file error.
	if ( dataFile1.fail() )
	{
		// Announce that we could not open the file.
		cout << "Could not open file \"" << INFILE1 << "\" for reading.\n" << endl;
		// Pause so the user can read the screen.
		system( "PAUSE" );
		return 1;
	}
	// If the file opens properly.
	else
	{
		// Test code.
		//cout << "Opened \"" << INFILE1 << "\" for reading." << endl;

		// Read the entire file into the array.
		fileCount1 = fileRead( dataFile1, walk1Array );
	}
	dataFile1.close();

	// Create an Interface class object.
	Interface interface1;

	// Loop through the first array until we find sysUpTime, all ifIndexes, all ifDescrs.
	for ( int i = 0; i < fileCount1; i++ )
	{
		// Search for sysUpTime (.1.3.6.1.2.1.1.3.0).
		if( walk1Array[i].find( SYSUPTIMEOID ) != string::npos )
		{
			// The actual uptime ticks will be at offest 32, and will not be a fixed length, so read to EOL.
			sysUpTime1 = stoi( walk1Array[i].substr( 32 ) );
		}

		// Search for ifIndex (.1.3.6.1.2.1.2.2.1.1.x).
		// The ifIndex is the numeric portion after the last dot in the OID, or the value after "INTEGER: "
		if( walk1Array[i].find( IFINDEXOID ) != string::npos )
		{
			// Since we found the OID, search for the text INTEGER: since we know the actual index is 9 characters ater this.
			if( ( ifIndexOffset = walk1Array[i].find( "INTEGER: " ) ) != string::npos )
			{
				// The actual index should be 9 characters after INTEGER:
				ifIndex = ( walk1Array[i].substr( ifIndexOffset + 9 ) );
				indexCount++;

				// Set the Interface object ifIndex to the newly found index.
				//interface1.setIndex( stoi( ifIndex ) );
			}
		}

		// Search for ifDescr (.1.3.6.1.2.1.2.2.1.2.x).
		// The ifDescr is the text portion after " = STRING: ".
		if( walk1Array[i].find( IFDESCROID ) != string::npos )
		{
			// Since we found the OID, search for the text STRING: since we know the actual index is 9 characters ater this.
			if( ( stringOffset = walk1Array[i].find( " = STRING: " ) ) != string::npos )
			{
				// The actual description should be 11 characters after " = STRING: ".
				//ifDescr = ( walk1Array[i].substr( stringOffset + 11 ) );
				
				// Put the description text into walk1IndexDescr[].
				walk1IndexDescr[stoi( walk1Array[i].substr( 21, stringOffset ) )] = ( walk1Array[i].substr( stringOffset + 11 ) );

				//Test code.
				//cout << "ifDescr in the file array: " << walk1Array[i].substr( stringOffset + 11 ) << endl;
				//cout << "ifIndex in the file array: " << stoi( walk1Array[i].substr( 21, stringOffset ) ) << endl;

				//Test code.
				//cout << "walk1IndexDescr: " << walk1IndexDescr[stoi( walk1Array[i].substr( 21, stringOffset ) )] << endl;
				
				// Set the Interface object ifIndex to the newly found index.
				//interface1.setDescr( ( ifDescr ) );
			}
		}
	}
	//Test code.
	oidRead( walk1Array, 2, interface1, fileCount1, 1 );

	// Display the resultant Interface-class object.
	cout << "Class walk 1:\n\tifIndex: " << interface1.getIndex() << "\n\tifDescr: " << interface1.getDescr();
	cout << "\n\tifSpeed: " << interface1.getSpeed1() << "\n\tifInOctets: " << interface1.getInOctets1();
	cout << "\n\tifOutOctets: " << interface1.getOutOctets1() << endl;
	cout << endl;

	//Test code.
	cout << "Found " << indexCount << " interfaces in walk 1." << endl;

#pragma endregion Walk1
#pragma region Walk2

	// Open a filestream for the first SNMP walk.
	ifstream dataFile2( INFILE2 );

	// Test for file error.
	if ( dataFile2.fail() )
	{
		// Announce that we could not open the file.
		cout << "Could not open file \"" << INFILE2 << "\" for reading." << endl << endl;
	}
	// If the file opens properly.
	else
	{
		// Test code.
		//cout << "Opened \"" << INFILE1 << "\" for reading." << endl;

		// Read the entire file into the array.
		fileCount2 = fileRead( dataFile2, walk2Array );

		// Loop through the first array until we find sysUpTime(.1.3.6.1.2.1.1.3.0).
		for (int i = 0; i < fileCount2; i++)
		{
			// Search for sysUpTime (.1.3.6.1.2.1.1.3.0).
			if( walk2Array[i].find( SYSUPTIMEOID ) != string::npos )
			{
				// The actual uptime ticks will be at offest 32, and will not be a fixed length, so read to EOL.
				sysUpTime2 = stoi( walk2Array[i].substr( 32 ) );
			}

			// Search for ifIndex (.1.3.6.1.2.1.2.2.1.1.x).
			// The ifIndex is the numeric portion after the last dot in the OID, or the value after "INTEGER: "
			if( walk2Array[i].find( IFINDEXOID ) != string::npos )
			{
				// Since we found the OID, search for the text INTEGER: since we know the actual index is 9 characters ater this.
				if( ( ifIndexOffset = walk2Array[i].find( "INTEGER: " ) ) != string::npos )
				{
					// The actual index should be 9 characters after INTEGER:
					ifIndex = ( walk2Array[i].substr( ifIndexOffset + 9 ) );
					indexCount++;

					// Set the Interface object ifIndex to the newly found index.
					//interface1.setIndex( stoi( ifIndex ) );
				}
			}

			// Search for ifDescr (.1.3.6.1.2.1.2.2.1.2.x).
			// The ifDescr is the text portion after " = STRING: ".
			if( walk2Array[i].find( IFDESCROID ) != string::npos )
			{
				// Since we found the OID, search for the text STRING: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = walk2Array[i].find( " = STRING: " ) ) != string::npos )
				{
					// The actual description should be 11 characters after " = STRING: "
					//ifDescr = ( walk2Array[i].substr( stringOffset + 11 ) );
					
					// Put the description text into walk2IndexDescr[].
					walk2IndexDescr[stoi( walk2Array[i].substr( 21, stringOffset ) )] = ( walk2Array[i].substr( stringOffset + 11 ) );

					//Test code.
					//cout << "ifDescr in the file array: " << walk2Array[i].substr( stringOffset + 11 ) << endl;
					//cout << "ifIndex in the file array: " << stoi( walk2Array[i].substr( 21, stringOffset ) ) << endl;

					//Test code.
					//cout << "walk2IndexDescr: " << walk2IndexDescr[stoi( walk2Array[i].substr( 21, stringOffset ) )] << endl;

					// Set the Interface object ifIndex to the newly found index.
					//interface1.setDescr( ( ifDescr ) );
				}
			}
		}
		cout << endl;
	}
	dataFile2.close();
	//Test code.
	oidRead( walk2Array, 2, interface1, fileCount1, 2 );

	// Display the resultant Interface-class object.
	cout << "Class walk 2:\n\tifIndex: " << interface1.getIndex() << "\n\tifDescr: " << interface1.getDescr();
	cout << "\n\tifSpeed: " << interface1.getSpeed2() << "\n\tifInOctets: " << interface1.getInOctets2();
	cout << "\n\tifOutOctets: " << interface1.getOutOctets2() << endl;
	cout << endl;

#pragma endregion Walk2

	// Store the difference in upTimes into ifInDelta, which we use for several calculations.
	if( sysUpTime1 > sysUpTime2 )
	{
		cout << "The second walk had an earlier time than the first." << endl;
	}
	else
	{
		ifInDelta = ( ( sysUpTime2 - sysUpTime1 ) / 100 );
		cout << "The time delta was:\n\t" << ifInDelta << " second";
		if( ifInDelta > 1 )
		{
			cout << "s";
		}
		cout << "." << endl;
		if( ifInDelta > 60 )
		{
			cout << '\t' << ( ( float ) ifInDelta / 60.0 ) << " minutes." << endl;
		}
		if( ifInDelta > 360 )
		{
			cout << '\t' << ( ( float ) ifInDelta / 60.0 / 60.0 ) << " hours." << endl;
		}
	}

	// Print the program header.
	cout << HEADER2 << endl;

	// Pause so the user can read the screen.
	system( "PAUSE" );
	return 0;
}// End main().


// Function name:	greeting1()
// Purpose:		This function will print greeting information and the present working directory to the user.
// Parameters:		none
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void greeting1( void )
{
	// Print the program header.
	cout << HEADER1 << endl;

	// Print the PWD to the screen.
	boost::filesystem::path new_full_path( boost::filesystem::current_path() );
	cout << "The SNMP walk files should be in this directory:\n" << new_full_path << '\n' << endl;
}


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


// Function name:	oidRead()
// Purpose:		This function will read OID information into an Interface-class object.
// Parameters:		An array of strings, the ifIndex to collect data for, and an Interface-class object to load the data into.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void oidRead( string _array[], int _ifIndex, Interface& _interface, int _arrayLength, int _walkNumber )
{
	// Read in ifSpeed, ifInOctets, and ifOutOctets.
	// Later I may add discards and errors.
	int stringOffset = 0;

	// Loop through the first array until we find several OIDs.
	for ( int i = 0; i < _arrayLength; i++ )
	{
		if( _walkNumber == 1 )
		{
			// Search for ifSpeed (.1.3.6.1.2.1.2.2.1.5.*).
			if( _array[i].find( IFSPEEDOID + to_string( _ifIndex ) ) != string::npos )
			{
				if( ( stringOffset = _array[i].find( "GAUGE32: " ) ) != string::npos )
				{
					// The actual interface speed will be 9 characters after "GAUGE32: " at offest 32, and will not be a fixed length, so read to EOL.
					_interface.setSpeed1( stoi( _array[i].substr( stringOffset + 9 ) ) );
				}
			}
			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.10.x).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFINOCTETSOID ) != string::npos )
			{
				// Since we found the OID, search for the text COUNTER32: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( "COUNTER32: " ) ) != string::npos )
				{
					// The actual speed should be 11 characters after INTEGER:
					_interface.setInOctets1( ( stoul( ( _array[i].substr( stringOffset + 11 ) ) ) ) );
				}
			}
			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.16.x).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFOUTOCTETSOID ) != string::npos )
			{
				// Since we found the OID, search for the text COUNTER32: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( "COUNTER32: " ) ) != string::npos )
				{
					// The actual speed should be 11 characters after INTEGER:
					_interface.setOutOctets1( ( stoul( ( _array[i].substr( stringOffset + 11 ) ) ) ) );
				}
			}
		}
		else if( _walkNumber == 2 )
		{
			// Search for ifSpeed (.1.3.6.1.2.1.2.2.1.5.*).
			if( _array[i].find( IFSPEEDOID + to_string( _ifIndex ) ) != string::npos )
			{
				if( ( stringOffset = _array[i].find( "GAUGE32: " ) ) != string::npos )
				{
					// The actual interface speed will be 9 characters after "GAUGE32: " at offest 32, and will not be a fixed length, so read to EOL.
					_interface.setSpeed2( stoi( _array[i].substr( stringOffset + 9 ) ) );
				}
			}
			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.10.x).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFINOCTETSOID ) != string::npos )
			{
				// Since we found the OID, search for the text COUNTER32: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( "COUNTER32: " ) ) != string::npos )
				{
					// The actual speed should be 11 characters after INTEGER:
					_interface.setInOctets2( ( stoul( ( _array[i].substr( stringOffset + 11 ) ) ) ) );
				}
			}
			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.16.x).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFOUTOCTETSOID ) != string::npos )
			{
				// Since we found the OID, search for the text COUNTER32: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( "COUNTER32: " ) ) != string::npos )
				{
					// The actual speed should be 11 characters after INTEGER:
					_interface.setOutOctets2( ( stoul( ( _array[i].substr( stringOffset + 11 ) ) ) ) );
				}
			}
		}
		else
		{
			cout << "The wrong walk number was passed to oidRead()." << endl;
		}
	}
}


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
