#pragma once


#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <boost/filesystem.hpp>


using namespace std;


const string HEADER1 = "This program will search two SNMP walks and provide information from them.\n";
const string HEADER2 = "Please select the link you wish to calculate utilization for:\n";
const string INFILE1 = "walk1.txt";					// Hard-coded input file 1.
const string INFILE2 = "walk2.txt";					// Hard-coded input file 2.
const string SYSUPTIMEOID = ".1.3.6.1.2.1.1.3.0";			// The OID for sysUpTime.
const string IFINDEXOID = ".1.3.6.1.2.1.2.2.1.1.";		// The OID for ifIndex.
const string IFDESCROID = ".1.3.6.1.2.1.2.2.1.2.";		// The OID for ifDescr.
const string IFSPEEDOID = ".1.3.6.1.2.1.2.2.1.5.";		// The OID for ifSpeed.
const string IFINOCTETSOID = ".1.3.6.1.2.1.2.2.1.10.";		// The OID for ifInOctets.
const string IFINDISCARDSOID = ".1.3.6.1.2.1.2.2.1.13.";	// The OID for ifInDiscards.
const string IFINERRORSOID = ".1.3.6.1.2.1.2.2.1.14.";		// The OID for ifInErrors.
const string IFOUTOCTETSOID = ".1.3.6.1.2.1.2.2.1.16.";	// The OID for ifOutOctets.
const string IFOUTDISCARDSOID = ".1.3.6.1.2.1.2.2.1.19.";	// The OID for ifOutDiscards.
const string IFOUTERRORSOID = ".1.3.6.1.2.1.2.2.1.20.";	// The OID for ifOutErrors.
const int PRECISION = 3;								// The number of decimal places to show for floats.
const unsigned int COUNTER32MAX = 4294967295;			// The maximum value a Counter32 can hold.
//const long COUNTER64MAX = 18446744073709551615;			// The maximum value a Counter64 can hold.
const int ARRAYSIZE = 100000;							// The size of the array for the files.
const int	MAXINTERFACE = 2000;						// The maximum number of interfaces this program can deal with.


class Interface
{
private:
	int ifIndex;				// The Interface Index: .1.3.6.1.2.1.2.2.1.1.*
	string ifDescr;			// The Interface Description: .1.3.6.1.2.1.2.2.1.2.*
	int ifSpeed1;				// The Interface Speed from the first walk.
	int ifSpeed2;				// The Interface Speed from the second walk.
	unsigned long ifInOctets1;	// The Interface Inbound Octet Count from the first walk.
	unsigned long ifInOctets2;	// The Interface Inbound Octet Count from the second walk.
	unsigned long ifOutOctets1;	// The Interface Outbound Octet Count from the first walk.
	unsigned long ifOutOctets2;	// The Interface Outbound Octet Count from the second walk.
	int ifOctetCounterSize1;		// The Interface Octet Size from the first walk.
	int ifOctetCounterSize2;		// The Interface Octet Size from the second walk.
public:
	Interface();							// Default constructor.
	Interface( int, string, int, int, int, int, int, int, int, int );	// Parameterized constructor.
	void setIndex( const int );				// setIndex() will set the ifIndex for this interface.
	void setDescr( const string& );			// setDescr() will set the ifDescr for this interface.
	void setSpeed1( int );				// setSpeed1() will set the ifSpeed1 for this interface.
	void setSpeed2( const int );				// setSpeed2() will set the ifSpeed2 for this interface.
	void setInOctets1( const unsigned long );	// setInOctets1() will set the ifInOctets1 for this interface.
	void setInOctets2( const unsigned long );	// setInOctets2() will set the ifInOctets2 for this interface.
	void setOutOctets1( const unsigned long );	// setInOctets1() will set the ifOutOctets1 for this interface.
	void setOutOctets2( const unsigned long );	// setInOctets2() will set the ifOutOctets2 for this interface.
	void setOctetCounterSize1( const int );		// setOctetCounterSize1() will set the ifOctetCounterSize1 for this interface.
	void setOctetCounterSize2( const int );		// setOctetCounterSize2() will set the ifOctetCounterSize2 for this interface.

	int getIndex() { return ifIndex; };					// getIndex() will return the ifIndex for this interface.
	string getDescr() { return ifDescr; };					// getDescr() will return the ifDescr for this interface.
	int getSpeed1() { return ifSpeed1; };					// getSpeed1() will return the ifSpeed1 for this interface.
	int getSpeed2() { return ifSpeed2; };					// getSpeed2() will return the ifSpeed2 for this interface.
	unsigned long getInOctets1() { return ifInOctets1; };		// getInOctets1() will return the ifInOctets1 for this interface.
	unsigned long getInOctets2() { return ifInOctets2; };		// getInOctets2() will return the ifInOctets2 for this interface.
	unsigned long getOutOctets1() { return ifOutOctets1; };	// getInOctets1() will set the ifOutOctets1 for this interface.
	unsigned long getOutOctets2() { return ifOutOctets2; };	// getInOctets1() will set the ifOutOctets1 for this interface.
	int getOctetCounterSize1() { return ifOctetCounterSize1; };	// getOctetCounterSize1() will set the ifOctetCounterSize1 for this interface.
	int getOctetCounterSize2() { return ifOctetCounterSize2; };	// getOctetCounterSize2() will set the ifOctetCounterSize2 for this interface.

	~Interface();				// Destructor.
};
