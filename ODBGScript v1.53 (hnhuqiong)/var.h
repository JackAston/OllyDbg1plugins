#pragma once

using namespace std;

enum vtype { EMP, DW, STR, FLT };

class var
{
public:
	DWORD dw;
	string str;
	long double flt;
	vtype vt;

	var();
	var(const var& rhs); 
	var(string& rhs); 
	var(DWORD rhs); 
	var(int rhs); 
	var(long double rhs); 

	// less than zero this < rhs
	// zero this == rhs 
	// greater than zero this > rhs 
	int compare(const var& rhs) const; 
	int compare(const string& rhs) const; 
	int compare(const DWORD& rhs) const; 
	int compare(const int& rhs) const; 
	int compare(const long double& rhs) const; 

	var& operator=(const var& rhs);
	var& operator=(const string& rhs);
	var& operator=(const DWORD& rhs);
	var& operator=(const int& rhs);
	var& operator=(const long double& rhs);

	var& operator+=(const var& rhs);
	var& operator+=(const string& rhs);
	var& operator+=(const DWORD& rhs);
	var& operator+=(const int& rhs);
	var& operator+=(const long double& rhs);

};
