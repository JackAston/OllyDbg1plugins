// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma once

//remove debug build warnings (debug names too long)
#ifdef _DEBUG
#pragma warning (disable : 4786)
#endif

//remove SEH missing warning
#pragma warning (disable : 4530)

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <iostream>

using namespace std;

#include <fstream>
#include <sstream>

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>


#include <afx.h>
//#include <windows.h>
#include <Commdlg.h>
//#include <Winuser.h>
#include <shlwapi.h>
#include <shellapi.h>



#include "var.h"
#include "plugin.h"

#include "HelperFunctions.h"
#include "IniReader.h"

#include "OllyLang.h"
#include "ODbgScript.h"
#include "Search.h"

#ifdef _DEBUG
#include "guicon.h"
#endif

#include "resource.h"
