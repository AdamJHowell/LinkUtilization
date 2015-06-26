#pragma once
class Interface
{
private:
	int ifIndex;					// The Interface Index.
	int ifSpeed1;					// The Interface Speed from the first walk.
	int ifSpeed2;					// The Interface Speed from the second walk.
	int ifInOctets1;				// The Interface Inbound Octet Count from the first walk.
	int ifInOctets2;				// The Interface Inbound Octet Count from the second walk.
	int ifOutOctets1;				// The Interface Outbound Octet Count from the first walk.
	int ifOutOctets2;				// The Interface Outbound Octet Count from the second walk.
	int ifOctetCounterSize1;			// The Interface Octet Size from the first walk.
	int ifOctetCounterSize2;			// The Interface Octet Size from the second walk.
public:
	Interface();					// Default constructor.
	Interface( int, int, int, int, int, int, int, int, int );	// Parameterized constructor.
	void setIndex();				// setIndex() will set the ifIndex for this interface.
	void setSpeed1();				// setSpeed1() will set the ifSpeed1 for this interface.
	void setSpeed2();				// setSpeed2() will set the ifSpeed2 for this interface.
	void setInOctets1();			// setInOctets1() will set the ifInOctets1 for this interface.
	void setInOctets2();			// setInOctets2() will set the ifInOctets2 for this interface.
	void setOutOctets1();			// setInOctets1() will set the ifOutOctets1 for this interface.
	void setOutOctets2();			// setInOctets2() will set the ifOutOctets2 for this interface.
	void setOctetCounterSize1();		// setOctetCounterSize1() will set the ifOctetCounterSize1 for this interface.
	void setOctetCounterSize2();		// setOctetCounterSize2() will set the ifOctetCounterSize2 for this interface.
	
	~Interface();					// Destructor.
};
