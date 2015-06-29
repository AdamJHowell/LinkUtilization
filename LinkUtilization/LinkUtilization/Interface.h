#pragma once

#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <boost/filesystem.hpp>

using namespace std;

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
	void setSpeed1( const int );				// setSpeed1() will set the ifSpeed1 for this interface.
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
