////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   StdAfx.h
//  Version     :   1.0
//  Creater     :   Luo Cong
//  Date        :   2006-7-28 11:04:32
//  Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STDAFX_H__
#define __STDAFX_H__

#include <windows.h>
#include <stdio.h>
#include <assert.h>

#ifdef _DEBUG
#include <crtdbg.h>
#endif

#ifndef ASSERT
#define ASSERT assert
#endif

#ifndef PROCESS_ERROR
#define PROCESS_ERROR(Condition) if (!(Condition)) goto Exit0
#endif

#endif  // __STDAFX_H__