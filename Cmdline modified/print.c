// this code below was written by anonymouse
// any bugs comments reviews criticisms are welcome
// i have attached a precompiled cmdline.dll along with this source
// for those who dont wish to compile :) in the zip file
// ollydbgs native evaluate expression and log expression 
// allows evaluation of a single expression only
// this code adds printing functionality to log window
// to ollydbg plugin cmdline.dll
// so we can log multiple expresssions on one single 
// conditional breakpoint 
// i added it to original comdline plugin so that it can also be used 
// with .notation (pass command to plugin when paused)
// like 
// .print eax
// .print string[403000]
// .print hex 403000 5
// .print eax
// .print unicode[403040]
// .print byte ptr ds:[403000]
// .run
// which would result in logs like this
/*
Log data
Address    Message
00401009   Breakpoint at LOGREG1.00401009 (<ModuleEntryPoint>+9)
           eax = 2
           string[403000] = logging registers in ollydbg
            dump 403000: 6c 6f 67 67 69
           eax = 2
           unicode[403040] = my dumb unicode string
           byte ptr ds:[403000] = 6c
00401009   Breakpoint at LOGREG1.00401009 (<ModuleEntryPoint>+9)
           eax = 3
           string[403000] = logging registers in ollydbg
            dump 403000: 6c 6f 67 67 69
           eax = 3
           unicode[403040] = my dumb unicode string
           byte ptr ds:[403000] = 6c
00401009   Breakpoint at LOGREG1.00401009 (<ModuleEntryPoint>+9)
           eax = 4
           string[403000] = logging registers in ollydbg
            dump 403000: 6c 6f 67 67 69
           eax = 4
           unicode[403040] = my dumb unicode string
           byte ptr ds:[403000] = 6c 
*/

/* how to compile this code
save this code in a seperate file Print.c
open the origianl cmdexec.c (that comes in plug110.zip sdk)
add a declaration to the function 

Comparing files Cmdexec.c and cmdexecorig.c
****** Cmdexec.c
#include "plugin.h"
int Print(char *answer,ulong parm);  // declaration of our new function


****** cmdexecorig.c
#include "plugin.h"

******

make the static char string[TEXTLEN] global by removing the static keyword

****** Cmdexec.c
static t_result  value;                // Operands V/v (expression)
// remove static keyword to make it global so that we can access it from our source file
char      string[TEXTLEN];      // Operand  S   (any string)   
static int       count;                // Operands N/n (count)
****** cmdexecorig.c
static t_result  value;                // Operands V/v (expression)
static char      string[TEXTLEN];      // Operand  S   (any string)
static int       count;                // Operands N/n (count)
******

****** Cmdexec.c

register the function to t_cmdlist structure

//register our function to t_cmdlist structure

****** Cmdexec.c
  { "HELP",  "S",  0,                 Heeeelp },   // Help on API function
  { "H",     "S",  0,                 Heeeelp },   // Ditto
  { "PRINT", "S",  0,                 Print   } 
};
****** cmdexecorig.c
  { "HELP",  "S",  0,                 Heeeelp },   // Help on API function
  { "H",     "S",  0,                 Heeeelp }    // Ditto
};
******


open the makefile and add the print.obj 
OBJFILES = Cmdexec.obj Command.obj print.obj
and you are set to compile the new plugin 
replace the compiled dll in your plugin path

USAGE

alt+f1 to get the commandline plugin active 
type you expression like below in a comma seperated values
print eax,string[403005],hex 403000 5,eax,string[403000],unicode[403040],byte ptr ds:[403000],eax,ecx,edx
hit enter you will get the expressions logged to log window
Log data, item 0
 Message=eax = 6 string[403005] = ng registers in ollydbg  dump 403000: 6c 6f 67 67 69 eax = 6 string[403000] = logging registers in ollydbg unicode[403040] = my dumb unicode string byte ptr ds:[403000] = 6c eax = 6 ecx = 8160f8a8 edx = fffffffa

or shift+f4 (log conditional breakpoint
and enter your expression to log prepended with a . (dot) 
see above
*/

 
#define STRICT                         // Avoids some type mismatches
#include <windows.h>
#include <stdio.h>
#include "plugin.h"

extern char string[TEXTLEN];  // defined in cmdexec.c

int Print(char *answer,ulong parm)
{
int i,j=0,strln,n,k,l=0,strln1,p;
char buf[TEXTLEN]={0},resstring[TEXTLEN]={0},tempstring[TEXTLEN]={0},unistr[TEXTLEN]={0};
char hexdump[TEXTLEN]={0},tempdump[TEXTLEN]={0},tempdump1[TEXTLEN]={0},tempexpr[TEXTLEN]={0};
char *splithexdump[TEXTLEN]={0},*arr[100]={0};
long hexadr,hexnum;
t_result myres;


strncpy(buf,string,(TEXTLEN-1));

// thanks to 0xf001 for making my ugly goto style splitter into a compact code
// splits the comma seperated string into its components

for(i=0; buf[i]; strln=i++)
	{
	if((buf[i] == ','))
		{
		buf[i]=0;
		}
	}
for(i=0; i<strln+1; i++)
	{
	if(buf[i])
		{
		arr[j++]=&buf[i];
		while(buf[++i]);
		}
	}


for(i=0;i<j;i++)
	{
	snprintf(tempstring,(TEXTLEN-1),"%s",arr[i]);
	if(strnicmp(tempstring,"hex",3)==0)
		{
		strncpy(hexdump,tempstring,(TEXTLEN-1));

// if the substring contains space split it into further components
// needed for expressions like "hex 403000 5"

		for(k=0; hexdump[k]; strln1=k++)
		if((hexdump[k] == ' ') || (hexdump[k] == 0x9))
			hexdump[k]=0; 
		for(k=0; k<strln1+1; k++)
		if(hexdump[k])
			{
			splithexdump[l++]=&hexdump[k];
			while(hexdump[++k]);
			}
		hexadr=strtol(splithexdump[1],0,16);
		hexnum=strtol(splithexdump[2],0,16);
		snprintf(tempdump,(TEXTLEN-1)," dump %X: ",hexadr);
		strncat(tempdump1,tempdump,(TEXTLEN-1));
		
		for(p=0;p<hexnum;p++)
			{
			snprintf(tempexpr,(TEXTLEN-1),"byte ptr ds:[%x]",(hexadr+p));
			n=Expression(&myres,tempexpr,0,0,NULL,0,0,Getcputhreadid());
			if((!n<0) || (myres.type!=DEC_UNKNOWN))
				snprintf(tempdump,(TEXTLEN-1),"%2x ",myres.u);
				strncat(tempdump1,tempdump,(TEXTLEN-1));
				
			}
		strncat(resstring,tempdump1,(TEXTLEN-1));
		
		}
	else
		{
		n=Expression(&myres,tempstring,0,0,NULL,0,0,Getcputhreadid());
		if((!n<0) || (myres.type!=DEC_UNKNOWN))
		if(myres.type == DEC_STRING)
			{
			snprintf(tempstring,(TEXTLEN-1),"%s = %s ",arr[i],myres.value);
			}
		else if(myres.type == DEC_UNICODE)
			{
			wcstombs(unistr,myres.wvalue,sizeof(myres.wvalue));
			snprintf(tempstring,(TEXTLEN-1),"%s = %s ",arr[i],unistr);
			}
		else
			{
			snprintf(tempstring,(TEXTLEN-1),"%s = %x ",arr[i],myres.u);
			}
		strncat(resstring,tempstring,(TEXTLEN-1));
		}
	}
strncpy(answer,resstring,(TEXTLEN-1));
Addtolist (0,1,answer);
return 0;
};
