#include "var.h"

var::var()
{
	vt = EMP;
	dw = 0;
	flt = 0;
	str = "";
	size = 0;
	isbuf = false;
}

var::var(const var& rhs)
{
	vt = rhs.vt;
	dw = rhs.dw;
	flt = rhs.flt;
	str = rhs.str;
	size = rhs.size;
	isbuf = rhs.isbuf;
}

var::var(string& rhs)
{
	vt = STR;
	dw = 0;
	flt = 0;
	str  = rhs;
	string s=rhs;
	if (UnquoteString(s,'#','#')) {
		size = s.length()/2;
		int (*pf)(int) = toupper; 
		transform(str.begin(), str.end(), str.begin(), pf);
		isbuf = true;
	} else {
		size = rhs.length();
		isbuf = false;
	}
}

var::var(ulong rhs)
{
	vt = DW;
	dw = rhs;
	flt = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
}

var::var(int rhs)
{
	vt = DW;
	dw = (ulong)rhs;
	flt = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
}

var::var(long double rhs)
{
	vt = FLT;
	flt = rhs;
	dw = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
}


var& var::operator=(const var& rhs)
{
	vt = rhs.vt;
	dw = rhs.dw;
	str = rhs.str;
	flt = rhs.flt;
	size = rhs.size;
	isbuf = rhs.isbuf;
	return *this;
}

var& var::operator=(const string& rhs)
{
	vt = STR;
	dw = 0;
	flt = 0;
	str = rhs;
	string s=rhs;
	if (UnquoteString(s,'#','#')) {
		size = s.length()/2;
		int (*pf)(int) = toupper; 
		transform(str.begin(), str.end(), str.begin(), pf);
		isbuf = true;
	} else {
		size = rhs.length();
		isbuf = false;
	}
	return *this;
}

var& var::operator=(const ulong& rhs)
{
	vt = DW;
	dw = rhs;
	flt = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
	return *this;
}

var& var::operator=(const int& rhs)
{
	vt = DW;
	dw = (ulong)rhs;
	flt = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
	return *this;
}

var& var::operator=(unsigned short& rhs)
{
	vt = DW;
	dw = rhs;
	flt = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
	return *this;
}

var& var::operator=(unsigned char& rhs)
{
	vt = DW;
	dw = rhs;
	flt = 0;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
	return *this;
}

var& var::operator=(const long double& rhs)
{
	vt = FLT;
	dw = 0;
	flt = (long double)rhs;
	str = "";
	size = sizeof(rhs);
	isbuf = false;
	return *this;
}

var& var::operator+=(const var& rhs)
{
	if(rhs.vt == DW)
		*this+=rhs.dw;
	else if(rhs.vt == FLT)
		*this+=rhs.flt;
	else if(rhs.vt == STR) {
		//operator+=(const string& rhs)
		*this+=rhs.str;
	}
	return *this;
}

var& var::operator+=(const string& rhs)
{
	string s;
	if(vt == STR) {
		string s=rhs;
		if (UnquoteString(s,'#','#')) {
			if (!isbuf) {
				//String + buf Hex Buffer to String ok
				size_t len=s.length()/2;
				char* buf = (char*)malloc(len+1);
				Str2Rgch(s,buf,len+1);
				s.assign(buf,len);
				str  += s;
				size += len;
				free(buf);
			} else { 
				// Buffer + Buffer
				str = "#"+str.substr(1,str.length()-2)+s+"#";
				size += s.length()/2;
			}
		} else {
			if (!isbuf) {
				//str + str
				str += rhs;
				size += rhs.length();
			} else {
				//buf + str
				string Hex;
				Str2Hex(s,Hex,s.length());
				str = "#"+str.substr(1,str.length()-2)+Hex+"#";
				size += s.length();
			}
		}

	} else if(vt == DW) {
		var v=(string)rhs;

		char dwbuf[12];
		if (v.isbuf) {
			//ulong + BUFFER >> CONCATE HEX
			s = strbuffhex();
			sprintf(dwbuf, "%08X",dw);
			*this = "#"+((string)dwbuf)+s+"#";
		} else {
			//ulong + STRING >> CONCATE ultoa+str
			s = strupr(ultoa(dw, dwbuf, 16));
			*this = s+v.str;
		}
	}

	return *this;
}

var& var::operator+=(const ulong& rhs)
{
	if(vt == DW)
		dw += rhs;
	else if(vt == FLT)
		flt += rhs;
	else if(vt == STR) {
		string s;
		char dwbuf[12];
		if (isbuf) {
			//Concate Num ulong to a buffer (4 octets)
			s = strbuffhex();
			sprintf(dwbuf, "%08X",rev(rhs));
			*this = "#"+s+dwbuf+"#";
		} else {
			//Add Number to a String
			s = strupr(ultoa(rhs, dwbuf, 16));
			str += s;
			size += s.length();
		}
	}
	return *this;
}

var& var::operator+=(const int& rhs)
{
	if(vt == DW)
		dw += (ulong)rhs;
	else if(vt == FLT)
		flt += rhs;
	else if(vt == STR)
		*this+=(ulong) rhs;

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
		return -2;
	
	if(vt == DW)
	{
		if(dw < rhs.dw) return -1;
		if(dw == rhs.dw) return 0;
		if(dw > rhs.dw) return 1;
	} 	
	else if(vt == FLT)
	{
		if(flt < rhs.flt) return -1;
		if(flt == rhs.flt) return 0;
		if(flt > rhs.flt) return 1;
	}
	else if(vt == STR) {
		if (isbuf == rhs.isbuf)
			return str.compare(rhs.str);
		else {
			string Hex;
			if (isbuf) {
				//Buffer / String
				string s=str;
				UnquoteString(s,'#','#');
				Str2Hex((string)rhs.str,Hex,rhs.size);
				return s.compare(Hex);
			} else { 
				//String / Buffer
				string s=rhs.str;
				UnquoteString(s,'#','#');
				Str2Hex((string)str,Hex,size);
				return Hex.compare(s);
			}
		}
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

int var::compare(const ulong& rhs) const
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

string var::strclean(void) 
{
	return CleanString(strbuff());
}

string var::strbuffhex(void) 
{
	if (isbuf)
		//#001122# to "001122"
		return str.substr(1,str.length()-2);
	else {
		string s;
		Str2Hex(str,s,size);
		return s;
	}
}

string var::strbuff(void) 
{
	if (isbuf) {
		//#303132# to "012"
		string s=strbuffhex();
		string tmp=s.substr(0,size);
		Str2Rgch(s,(char*)tmp.c_str(),size);
		s=tmp;
		return s;
	} else
		return str;
}

void var::resize(ulong newsize)
{
	if (vt==DW){
		if (newsize==1) {
			dw&=0xFF;
		}
		else if (newsize==2) {
			dw&=0xFFFF;
		}
	} 
	else if (vt==STR){
		if (size > newsize) {
			if (isbuf) {
				*this = "#"+strbuff().substr(0,newsize)+"#";
			} else {
				*this = strbuff().substr(0,newsize);
			}
		}
	}
}