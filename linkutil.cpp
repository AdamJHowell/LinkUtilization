/*
File Prologue
Name: Adam Howell
Project: SNMP Link Utilization
Date: 2014-11-11
Description: This project will calculate link utilization from two SNMP Walks.

I declare that the following source code was written by me, or taken from public domain sources.
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
