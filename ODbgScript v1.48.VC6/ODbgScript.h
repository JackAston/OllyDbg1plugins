#pragma once

using namespace std;

#define VERSIONHI      1               // High plugin version
#define VERSIONLO      48              // Low plugin version


///long
static char      wndprogclass[32];	   // Name of progress window class
static char      wndlogclass[32];	   // Name of log window class
static char      wndwatchclass[32];

static int       focusonstop;            // Foreground on pause

static bool	 dbgfocus=false;

static void*	 pmemforexec;


#define STRING_READSIZE 256

#include "mru.h"

//For unique vars btw processes
//#pragma data_seg( ".GLOBALS" )
//#pragma data_seg()