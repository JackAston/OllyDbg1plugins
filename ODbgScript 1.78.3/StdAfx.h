// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

//remove debug build warnings (debug names too long)
#ifdef _DEBUG
#pragma warning (disable : 4786)
#endif

// Insert your headers here
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <iostream>
#include <fstream>
#include <sstream>
#include <tchar.h>

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>

#include <stdio.h>
#include <fcntl.h>
//#include <io.h>
//#include <stdlib.h> 

#include <windows.h>
#include <Commdlg.h>
#include <Winuser.h>
#include <shlwapi.h>
#include <shellapi.h>

#ifdef _DEBUG
#include <io.h>
#endif 

#include "var.h"
#include "plugin.h"

#include "HelperFunctions.h"
#include "Search.h"
#include "guicon.h"

#include "OllyLang.h"
#include "ODbgScript.h"

#include "resource.h"


