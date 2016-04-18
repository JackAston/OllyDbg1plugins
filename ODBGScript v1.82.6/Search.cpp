#include <math.h>

bool CompareChar(const char src, char* cmp)
{
	if(strstr(cmp, "??")) // ??
		return true;

	if(strstr(cmp, "?") == cmp) // ?6
	{
		char low = src % 0x10;
		char val = (char)strtoul(cmp + 1, 0, 16);
		if(val == low)
			return true;
		return false;
	}
	else if(strstr(cmp, "?") == cmp + 1) // 5?
	{
		cmp[1] = 0;
		char high = (src - src % 0x10) / 0x10;
		char val = (char)strtoul(cmp, 0, 16);
		if(val == high)
			return true;
		return false;
	}
	else // 56
	{
		char high = (src - src % 0x10) / 0x10;
		char low = src % 0x10;
		char val2 = (char)strtoul(cmp + 1, 0, 16);
		cmp[1] = 0;
		char val1 = (char)strtoul(cmp, 0, 16);
		if(high == val1 && low == val2)
			return true;
		return false;
	}
}

int FindWithWildcards(const char* source, const char* findstring, size_t len)
{
	char cmp[3] = {0};
	int findlen = ceil(((double)strlen(findstring) / 2));
	if(len < findlen)
		return -1;

	for(int i = 0; i < len; i++)
	{
		for(int j = 0; j < findlen; j++)
		{
			strncpy(cmp, findstring + j * 2, 2);
			if(!CompareChar(source[i+j], cmp))
				break;
			else if(j == (findlen - 1))
				return i;
		}
	}
	return -1;
}

char * HexString2BinArray(const char * s)
{
	char HexBuf[3];
	char * result=new char[(strlen(s) /2)+1];
	result[0]=0;

	int i=0;
	while (strlen(s))
	{
		strncpy(HexBuf,s,2);
		HexBuf[2]=0;
		result[i]=(char)strtoul(HexBuf,0,16);
		i++;
		s+=2;
	}
	i++;

	return result;
}

char WildSymbolToChar(const char* x)
{
	char n;

	if (x[0]=='?')
		n=x[1];
	else
		n=x[0];

	if (n>='0' && n<='9')
		n-='0';
	else
	{
		if (n>='a' && n<='f')
			n-='a';
		else
		{
			if (n>='A' && n<='F')
				n-='A';
		}
	}
	if (x[1]=='?')
		n=n<<4;

	return n;
}
char HexString2BinChar(const char *s)
{
	char HexBuf[3];
	strncpy(HexBuf,s,2);
	HexBuf[2]=0;
	return (char)strtoul(HexBuf,0,16);	
}

bool Replace(char * s, const char * searchstr, const char * replstring, size_t length)
{
	char * ps=s;
	bool WasReplaced=false;
	if (strlen(searchstr)!=strlen(replstring))
		throw("Search string and replace strings should be of equal length");

	if (strlen(searchstr)%2)
		throw("Bad search string");

	if ((strlen(searchstr)/2)>length)
		throw("The size of data is too small");

	size_t ReplaceLength=strlen(replstring)/2;

	size_t offs=0;

	bool equal=true;
	unsigned i=0;
	unsigned j=0;

	//Wildcard Search
	while (i<strlen(replstring))
	{
		if (strncmp("??",&searchstr[i],2)) // if searchstr[i] != "??"
		{
			if ((searchstr[i]=='?') || (searchstr[i+1]=='?'))
			{
				unsigned char mask=0;

				char maskbuf[3];

				if (searchstr[i]=='?')
				{
					maskbuf[0]=searchstr[i+1]; // "?5"
					maskbuf[1]=0;

					if (s[j]>0xF)
						equal=false;

					mask=0x0F;

				}
				else
				{
					maskbuf[0]=searchstr[i]; // "5?"
					maskbuf[1]='0';
					maskbuf[2]=0;
					mask=0xF0;
				}


				if ((char)(s[j]&mask)-(char)strtoul(maskbuf,0,16))
					equal=false;

				if (!equal)
					break;
			}
			else
				if (HexString2BinChar(&searchstr[i])!=s[j])
				{
					equal=false;
					break;
				}

		}
		else
		{
			i+=2;
			i-=2;
		}
		i+=2;
		j++;

	}

	//Wildcard Replacment
	if (equal)
	{
		if (!strstr(replstring,"?")) // no wildcards
		{
			char * replacestring;
			try
			{
				replacestring=HexString2BinArray(replstring);
			}
			catch (...)
			{
				throw("Bad replace string");
			}
			memcpy(s,replacestring,ReplaceLength);
			delete replacestring;
		}
		else //replace by wildcard
			for (i=0;i<ReplaceLength;i++)
			{
				if (strncmp("??",&replstring[i*2],2)) // if searchstr[i] != "??"
				{
					if ((replstring[i*2]=='?') || (replstring[i*2+1]=='?'))
					{

						if (replstring[i*2]=='?')	//?5
							ps[i]=(ps[i]& 0xF0) | WildSymbolToChar(&replstring[i*2]);
						else						//5?
							ps[i]=(ps[i]& 0x0F) | WildSymbolToChar(&replstring[i*2]);
					}
					else
						ps[i]=HexString2BinChar(&replstring[i*2]); //not wildcard replacement
				}
			}
			WasReplaced=true;
	}
	return WasReplaced;
}

char * Byte2Hex(char b)
{
	char * buf=new char[3];
	sprintf(buf,"%02X",(unsigned char)b);
	return buf;
}