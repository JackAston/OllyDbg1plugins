#pragma once
/////////-------------------------string functions--------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//----------String functions-----------------------

void ultoa(int x,char* Out,int radix)
{
	    int i,c;
		if(!x) *Out=0;
		else
		{
			i=0;
			while(x)
			{
				int rem=x%radix;
				if(rem>9) 
				{
					rem-=10;
					Out[i++]=rem+'A';
				}
				else
				{
					Out[i++]=rem+'0';
				}
				x/=radix;
			}
			Out[i--]=0;
			c=0;
			while(c!=i&&c<i)
			{
				Out[c]^=Out[i];
				Out[i]^=Out[c];
				Out[c++]^=Out[i--];
			}
		}
}

void strcpy(unsigned char* dst,unsigned char* src)
{
	int i=0;
	while(src[i])		dst[i]=src[i++];
}