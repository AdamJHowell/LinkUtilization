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
int presentIndexes( string _array[] );
void locateInterfaces( string _array1[], string _array2[], string _array3[], int fileCount1, int fileCount2 );


int main()
{
	string ifIndex = "";			// This is a string, because it will be taken as the users choice and suffixed to the "base" OIDs.
	string ifDescr = "";			// This string will contain the link description.
	string readTemp = "";			// Temporary string to hold data, usually before converting it to an int.
	string ifIndexArray[MAXINTERFACE];	// An array to hold all discovered interface descriptions, the ifIndex will be the array index.
	int selectedIfIndex = 0;			// The user-chosen interface to run stats on.
	int stringOffset = 0;			// The offest within the line where the search text was found.
	int timeDelta = 0;				// The difference between the first and second up times.
	int maxRate = 0;				// The maximum rate of the links.
	int fileCount1 = 0;				// The number of lines in input file 1.
	int fileCount2 = 0;				// The number of lines in input file 2.
	int ifIndexOffset = 0;			// The offset in characters of the ifIndex.

	// Set the decimal precision for decimal output.
	cout.precision( PRECISION );
	cout.fixed;

	// Print the header and the PWD to the screen.
	greeting1();

	// Create arrays to hold each file.
	string* walk1Array = new string[ARRAYSIZE];
	string* walk2Array = new string[ARRAYSIZE];

	// Create arrays to hold each ifIndex and ifDescr.
	string* walk1IndexDescr = new string[MAXINTERFACE];
	string* walk2IndexDescr = new string[MAXINTERFACE];

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

		// Open a filestream for the first SNMP walk.
		ifstream dataFile2( INFILE2 );

		// Test for file error.
		if( dataFile2.fail() )
		{
			// Announce that we could not open the file.
			cout << "Could not open file \"" << INFILE2 << "\" for reading." << endl << endl;
			// Pause so the user can read the screen.
			system( "PAUSE" );
			return 2;
		}
		// If the file opens properly.
		else
		{
			// Test code.
			//cout << "Opened \"" << INFILE2 << "\" for reading." << endl;

			// Read the entire file into the array.
			fileCount2 = fileRead( dataFile2, walk2Array );
		}
		dataFile2.close();
	}
	dataFile1.close();

	// Discover all interfaces in both walks, and store in walk1IndexDescr.
	locateInterfaces( walk1Array, walk2Array, walk1IndexDescr, fileCount1, fileCount2 );

	// Display all discovered interfaces from walk1IndexDescr.
	selectedIfIndex = presentIndexes( walk1IndexDescr );

	cout << "\nRunning stats on interface index " << selectedIfIndex << "..." << endl;

	// Create an Interface class object.
	Interface interface1;

	// Populate that interface class object.
	oidRead( walk1Array, selectedIfIndex, interface1, fileCount1, 1 );
	oidRead( walk2Array, selectedIfIndex, interface1, fileCount2, 2 );

	// Store the difference in upTimes into ifInDelta, which we use for several calculations.
	if( interface1.getSysUpTime1() > interface1.getSysUpTime2() )
	{
		cout << "The second walk had an earlier time than the first." << endl;
	}
	else
	{
		// Subtract the time from the first walk from the time from the second walk, and convert into seconds.
		timeDelta = ( ( interface1.getSysUpTime2() - interface1.getSysUpTime1() ) / 100 );
		cout << "The time delta was:\n\t" << timeDelta << " second";
		if( timeDelta > 1 )
		{
			cout << "s";
		}
		cout << "." << endl;
		if( timeDelta > 60 )
		{
			cout << '\t' << ( ( double ) timeDelta / 60.0 ) << " minutes." << endl;
		}
		if( timeDelta > 360 )
		{
			cout << '\t' << ( ( double ) timeDelta / 60.0 / 60.0 ) << " hours." << endl;
		}
	}

	//Test code.
	interface1.getInterface();
	interface1.calculateUtilization();

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
} // End greeting1().


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

	_interface.setIndex( _ifIndex );

	// Loop through the first array until we find several OIDs.
	for ( int i = 0; i < _arrayLength; i++ )
	{
		if( _walkNumber == 1 )
		{
			// Search for sysUpTime (.1.3.6.1.2.1.1.3.0).
			// The sysUptime is the numerical portion after " = GAUGE32: ".
			if( _array[i].find( SYSUPTIMEOID ) != string::npos )
			{
				// The actual uptime ticks will be at offest 32, and will not be a fixed length, so read to EOL.
				_interface.setSysUpTime1( stoi( _array[i].substr( 32 ) ) );
			}

			// Search for ifSpeed (.1.3.6.1.2.1.2.2.1.5._ifIndex).
			// The ifSpeed is the numerical portion after " = GAUGE32: ".
			if( _array[i].find( IFSPEEDOID + to_string( _ifIndex ) ) != string::npos )
			{
				if( ( stringOffset = _array[i].find( "GAUGE32: " ) ) != string::npos )
				{
					// The actual interface speed will be 9 characters after "GAUGE32: " at offest 32, and will not be a fixed length, so read to EOL.
					_interface.setSpeed1( stoi( _array[i].substr( stringOffset + 9 ) ) );
				}
			}

			// Search for ifDescr (.1.3.6.1.2.1.2.2.1.2._ifIndex).
			// The ifDescr is the text portion after " = STRING: ".
			if( _array[i].find( IFDESCROID + to_string( _ifIndex ) ) != string::npos )
			{
				// Since we found the OID, search for the text STRING: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( " = STRING: " ) ) != string::npos )
				{
					// The actual description should be 11 characters after " = STRING: "
					//ifDescr = ( _array[i].substr( stringOffset + 11 ) );

					// Put the description text into walk2IndexDescr[].
					_interface.setDescr( _array[i].substr( stringOffset + 11 ) );

					//Test code.
					//cout << "ifDescr in the file array: " << _array[i].substr( stringOffset + 11 ) << endl;
					//cout << "ifIndex in the file array: " << stoi( _array[i].substr( 21, stringOffset ) ) << endl;

					//Test code.
					//cout << "walk2IndexDescr: " << walk2IndexDescr[stoi( _array[i].substr( 21, stringOffset ) )] << endl;
				}
			}

			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.10._ifIndex).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFINOCTETSOID + to_string( _ifIndex ) ) != string::npos )
			{
				// Since we found the OID, search for the text COUNTER32: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( "COUNTER32: " ) ) != string::npos )
				{
					// The actual speed should be 11 characters after INTEGER:
					_interface.setInOctets1( ( stoul( ( _array[i].substr( stringOffset + 11 ) ) ) ) );
				}
			}

			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.16._ifIndex).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFOUTOCTETSOID + to_string( _ifIndex ) ) != string::npos )
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
			// Search for sysUpTime (.1.3.6.1.2.1.1.3.0).
			// The sysUptime is the numerical portion after " = GAUGE32: ".
			if( _array[i].find( SYSUPTIMEOID ) != string::npos )
			{
				// The actual uptime ticks will be at offest 32, and will not be a fixed length, so read to EOL.
				_interface.setSysUpTime2( stoi( _array[i].substr( 32 ) ) );
			}

			// Search for ifSpeed (.1.3.6.1.2.1.2.2.1.5._ifIndex).
			// The ifSpeed is the numerical portion after " = GAUGE32: ".
			if( _array[i].find( IFSPEEDOID + to_string( _ifIndex ) ) != string::npos )
			{
				if( ( stringOffset = _array[i].find( "GAUGE32: " ) ) != string::npos )
				{
					// The actual interface speed will be 9 characters after "GAUGE32: " at offest 32, and will not be a fixed length, so read to EOL.
					_interface.setSpeed2( stoi( _array[i].substr( stringOffset + 9 ) ) );
				}
			}

			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.10._ifIndex).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFINOCTETSOID + to_string( _ifIndex ) ) != string::npos )
			{
				// Since we found the OID, search for the text COUNTER32: since we know the actual index is 9 characters ater this.
				if( ( stringOffset = _array[i].find( "COUNTER32: " ) ) != string::npos )
				{
					// The actual speed should be 11 characters after INTEGER:
					_interface.setInOctets2( ( stoul( ( _array[i].substr( stringOffset + 11 ) ) ) ) );
				}
			}

			// Search for ifInOctets (.1.3.6.1.2.1.2.2.1.16._ifIndex).
			// The ifInOctets is the numeric portion after "COUNTER32: ".  This number rolls to zero when it hits COUNTER32MAX.
			if( _array[i].find( IFOUTOCTETSOID + to_string( _ifIndex ) ) != string::npos )
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
} // End oidRead().


// Function name:	presentIndexes()
// Purpose:		This function will display all known interfaces and prompt the user to select one.
// Parameters:		An array of strings, containing each ifDescr laoded into the array index that matches the SNMP ifIndex.
// Returns:		An integer representing the selected ifIndex.
// Preconditions:	none
// Postconditions:	none
int presentIndexes( string _array[] )
{
	int menuAnswer = 0;
	// Prompt the user for the interface to run stats on.
	cout << "\n" << HEADER2 << endl;

	// Cycle through the ifDescr array, printing each ifDescr.
	for( size_t i = 0; i <= sizeof(_array); i++ )
	{
		if( _array[i] != "" )
		{
			cout << i << ": " << _array[i] << endl;
		}
	}
	cout << "Choice: ";
	// Read the user's response.
	cin >> menuAnswer;
	// Flush the input buffer.
	cin.ignore( IGNORE, '\n' );

	return menuAnswer;
} // End presentIndexes().


// Function name:	locateInterfaces()
// Purpose:		This function will scan the provided arrays for interfaces, and store the discovered interfaces in an array.
// Parameters:		Two arrays of strings, each containing one walk, and an array to load ifDescr into (the array index will match the SNMP ifIndex).
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void locateInterfaces( string _array1[], string _array2[], string _array3[], int fileCount1, int fileCount2 )
{
	int tempOffset = 0;
	// Loop through the first array until we find sysUpTime, all ifIndexes, all ifDescrs.
	for( int i = 0; i < fileCount1; i++ )
	{
		// Search for ifDescr (.1.3.6.1.2.1.2.2.1.2.*).
		// The ifDescr is the text portion after " = STRING: ".
		if( _array1[i].find( IFDESCROID ) != string::npos )
		{
			// Since we found the OID, search for the text STRING: since we know the actual index is 9 characters ater this.
			if( ( tempOffset = _array1[i].find( " = STRING: " ) ) != string::npos )
			{
				// Put the description text into walk1IndexDescr[] at the location of ifIndex.
				_array3[stoi( _array1[i].substr( 21, tempOffset ) )] = ( _array1[i].substr( tempOffset + 11 ) );

				//Test code.
				//cout << "ifDescr in the file array: " << walk1Array[i].substr( stringOffset + 11 ) << endl;
				//cout << "ifIndex in the file array: " << stoi( walk1Array[i].substr( 21, stringOffset ) ) << endl;

				//Test code.
				//cout << "walk1IndexDescr: " << walk1IndexDescr[stoi( walk1Array[i].substr( 21, stringOffset ) )] << endl;
			}
		}
	}
} // End locateInterfaces().