#include "Interface.h"


Interface::Interface()
{
	ifIndex = 0;
	ifDescr = "";
	ifSpeed1 = 0;
	ifSpeed2 = 0;
	ifInOctets1 = 0;
	ifInOctets2 = 0;
	ifOutOctets1 = 0;
	ifOutOctets2 = 0;
	ifOctetCounterSize1 = 0;
	ifOctetCounterSize2 = 0;
}



// Function name:	setIndex()
// Purpose:		This function will set the ifIndex of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Index.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setIndex( const int _index )
{
	ifIndex = _index;
}


// Function name:	setDescr()
// Purpose:		This function will set the description of the Interface to what is passed as a parameter.
// Parameters:		String descrtiption for the Interface.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setDescr( const string& _description )
{
	ifDescr = _description;
}



// Function name:	setSpeed1()
// Purpose:		This function will set the ifSpeed of the first walk of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Speed from the first walk.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setSpeed1( int _speed1 )
{
	ifSpeed1 = _speed1;
}


// Function name:	setSpeed2()
// Purpose:		This function will set the ifSpeed of the second walk of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Speed from the second walk.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setSpeed2( const int _speed2 )
{
	ifSpeed2 = _speed2;
}


// Function name:	setInOctets1()
// Purpose:		This function will set the ifInOctets of the first walk of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Inbound Octet count from the first walk.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setInOctets1( const unsigned long _inOctets1 )
{
	ifInOctets1 = _inOctets1;
}


// Function name:	setInOctets2()
// Purpose:		This function will set the ifInOctets of the second walk of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Inbound Octet count from the second walk.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setInOctets2( const unsigned long _inOctets2 )
{
	ifInOctets2 = _inOctets2;
}


// Function name:	setOutOctets1()
// Purpose:		This function will set the ifOutOctets of the first walk of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Outbound Octet count from the first walk.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setOutOctets1( const unsigned long _outOctets1 )
{
	ifOutOctets1 = _outOctets1;
}


// Function name:	setOutOctets2()
// Purpose:		This function will set the ifOutOctets of the second walk of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Outbound Octet count from the second walk.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setOutOctets2( const unsigned long _outOctets2 )
{
	ifOutOctets2 = _outOctets2;
}


// Function name:	setOctetCounterSize1()
// Purpose:		This function will set the Inteface Counter size of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Counter size.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setOctetCounterSize1( const int _OctetCounterSize1 )
{
	ifOctetCounterSize1 = _OctetCounterSize1;
}


// Function name:	setOctetCounterSize2()
// Purpose:		This function will set the Inteface Counter size of the Interface to what is passed as a parameter.
// Parameters:		Integer value for the Interface Counter size.
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::setOctetCounterSize2( const int _OctetCounterSize2 )
{
	ifOctetCounterSize2 = _OctetCounterSize2;
}


// Function name:	getInterface()
// Purpose:		This function will print all variables in the Interface class object.
// Parameters:		none
// Returns:		none
// Preconditions:	none
// Postconditions:	none
void Interface::getInterface( void )
{
	cout << "ifIndex: " << ifIndex << endl;
	cout << "ifDescr: " << ifDescr << endl;
	cout << "ifSpeed1: " << ifSpeed1 << endl;
	cout << "ifSpeed2: " << ifSpeed2 << endl;
	cout << "ifInOctets1: " << ifInOctets1 << endl;
	cout << "ifInOctets2: " << ifInOctets2 << endl;
	cout << "ifOutOctets1: " << ifOutOctets1 << endl;
	cout << "ifOutOctets2: " << ifOutOctets2 << endl;
	cout << "ifOctetCounterSize1: " << ifOctetCounterSize1 << endl;
	cout << "ifOctetCounterSize2: " << ifOctetCounterSize2 << endl;
}


Interface::~Interface()
{
}
