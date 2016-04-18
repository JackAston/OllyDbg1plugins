#include "StdAfx.h"
#include "var.h"

var::var()
{
	vt = EMP;
	dw = 0;
	flt = 0;
	str = "";
}

var::var(const var& rhs)
{
	vt = rhs.vt;
	dw = rhs.dw;
	flt = rhs.flt;
	str = rhs.str;
}

var::var(string& rhs)
{
	vt = STR;
	dw = 0;
	flt = 0;
	str = rhs;
}

var::var(DWORD rhs)
{
	vt = DW;
	dw = rhs;
	flt = 0;
	str = "";
}

var::var(int rhs)
{
	vt = DW;
	dw = (DWORD)rhs;
	flt = 0;
	str = "";
}

var::var(long double rhs)
{
	vt = FLT;
	flt = rhs;
	dw = 0;
	str = "";
}


var& var::operator=(const var& rhs)
{
	vt = rhs.vt;
	dw = rhs.dw;
	str = rhs.str;
	flt = rhs.flt;
	return *this;
}

var& var::operator=(const string& rhs)
{
	vt = STR;
	dw = 0;
	flt = 0;
	str = rhs;
	return *this;
}

var& var::operator=(const DWORD& rhs)
{
	vt = DW;
	dw = rhs;
	flt = 0;
	str = "";
	return *this;
}

var& var::operator=(const int& rhs)
{
	vt = DW;
	dw = (DWORD)rhs;
	flt = 0;
	str = "";
	return *this;
}

var& var::operator=(const long double& rhs)
{
	vt = FLT;
	dw = 0;
	flt = (long double)rhs;
	str = "";
	return *this;
}

var& var::operator+=(const var& rhs)
{
	if(vt == rhs.vt)
	{
		if(vt == DW)
			dw += rhs.dw;
		if(vt == FLT)
			flt += rhs.flt;
		if(vt == STR)
			str += rhs.str;
	}
	return *this;
}

var& var::operator+=(const string& rhs)
{
	if(vt == STR)
		str += rhs;
	return *this;
}

var& var::operator+=(const DWORD& rhs)
{
	if(vt == DW)
		dw += rhs;
	return *this;
}

var& var::operator+=(const int& rhs)
{
	if(vt == DW)
		dw += (DWORD)rhs;
	return *this;
}

var& var::operator+=(const long double& rhs)
{
	if(vt == FLT)
		flt += (long double)rhs;
	return *this;
}

int var::compare(const var& rhs) const
{
	// less than zero this < rhs
	// zero this == rhs 
	// greater than zero this > rhs 
	if(vt != rhs.vt || vt == EMP || rhs.vt == EMP)
		return 0;

	if(vt == STR)
        return str.compare(rhs.str);

	if(vt == DW)
	{
		if(dw < rhs.dw) return -1;
		if(dw == rhs.dw) return 0;
		if(dw > rhs.dw) return 1;
	}

	if(vt == FLT)
	{
		if(flt < rhs.flt) return -1;
		if(flt == rhs.flt) return 0;
		if(flt > rhs.flt) return 1;
	}

	return 0;
}

int var::compare(const string& rhs) const
{
	var tmp;
	tmp.vt = STR;
	tmp.str = rhs;
    return compare(tmp);
}

int var::compare(const DWORD& rhs) const
{
	var tmp(rhs);
	return compare(tmp);
}

int var::compare(const int& rhs) const
{
	var tmp(rhs);
	return compare(tmp);
}

int var::compare(const long double& rhs) const
{
	var tmp(rhs);
	return compare(tmp);
}