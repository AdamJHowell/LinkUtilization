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
	int ifIndex;					// The Interface Index: .1.3.6.1.2.1.2.2.1.1.*
	string ifDescr;				// The Interface Description: .1.3.6.1.2.1.2.2.1.2.*
	int ifSpeed1;					// The Interface Speed from the first walk.
	int ifSpeed2;					// The Interface Speed from the second walk.
	int ifInOctets1;				// The Interface Inbound Octet Count from the first walk.
	int ifInOctets2;				// The Interface Inbound Octet Count from the second walk.
	int ifOutOctets1;				// The Interface Outbound Octet Count from the first walk.
	int ifOutOctets2;				// The Interface Outbound Octet Count from the second walk.
	int ifOctetCounterSize1;			// The Interface Octet Size from the first walk.
	int ifOctetCounterSize2;			// The Interface Octet Size from the second walk.
public:
	Interface();						// Default constructor.
	Interface( int, string, int, int, int, int, int, int, int, int );	// Parameterized constructor.
	void setIndex( const int );			// setIndex() will set the ifIndex for this interface.
	void setDescr( const string& );		// setDescr() will set the ifDescr for this interface.
	void setSpeed1( const int );			// setSpeed1() will set the ifSpeed1 for this interface.
	void setSpeed2( const int );			// setSpeed2() will set the ifSpeed2 for this interface.
	void setInOctets1( const int );		// setInOctets1() will set the ifInOctets1 for this interface.
	void setInOctets2( const int );		// setInOctets2() will set the ifInOctets2 for this interface.
	void setOutOctets1( const int );		// setInOctets1() will set the ifOutOctets1 for this interface.
	void setOutOctets2( const int );		// setInOctets2() will set the ifOutOctets2 for this interface.
	void setOctetCounterSize1( const int );	// setOctetCounterSize1() will set the ifOctetCounterSize1 for this interface.
	void setOctetCounterSize2( const int );	// setOctetCounterSize2() will set the ifOctetCounterSize2 for this interface.
	
	~Interface();					// Destructor.
};
