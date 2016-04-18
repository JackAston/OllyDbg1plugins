#pragma once

using namespace std;

enum vtype { EMP, DW, STR, FLT };

typedef unsigned long ulong;

class var
{
typedef unsigned long ulong;
public:
	ulong dw;
	string str;
	long double flt;
	vtype vt;
	bool isbuf;
	int size;

	var();
	var(const var& rhs); 
	var(string& rhs); 
	var(ulong rhs); 
	var(int rhs); 
	var(long double rhs); 

	// less than zero this < rhs
	// zero this == rhs 
	// greater than zero this > rhs 
	int compare(const var& rhs) const; 
	int compare(const string& rhs) const; 
	int compare(const ulong& rhs) const; 
	int compare(const int& rhs) const; 
	int compare(const long double& rhs) const; 

	string strclean(void);
	string strbuffhex(void);
	string strbuff(void);

	var& operator=(const var& rhs);
	var& operator=(const string& rhs);
	var& operator=(const ulong& rhs);
	var& operator=(const int& rhs);
	var& operator=(unsigned short& rhs);
	var& operator=(unsigned char& rhs);
	var& operator=(const long double& rhs);

	var& operator+=(const var& rhs);
	var& operator+=(const string& rhs);
	var& operator+=(const ulong& rhs);
	var& operator+=(const int& rhs);
	var& operator+=(const long double& rhs);

	void resize(ulong newsize);

};
