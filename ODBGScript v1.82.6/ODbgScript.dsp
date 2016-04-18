# Microsoft Developer Studio Project File - Name="ODbgScript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ODbgScript - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ODbgScript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ODbgScript.mak" CFG="ODbgScript - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ODbgScript - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ODbgScript - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ODbgScript - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ODBGSCRIPT_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Zp4 /MD /O2 /Ob0 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "ODBGSCRIPT_EXPORTS" /D "_AFXDLL" /Fr /J /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 mfcs42.lib mfc42.lib msvcprt.lib msvcrt.lib ollydbgvc7.lib kernel32.lib user32.lib shlwapi.lib Comdlg32.lib shell32.lib /nologo /dll /pdb:none /machine:I386
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ODbgScript - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ODBGSCRIPT_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /Zp1 /MDd /Gi /GX- /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "ODBGSCRIPT_EXPORTS" /D "_AFXDLL" /FR /J /FD /GZ /c
# SUBTRACT CPP /Gy /u /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mfcs42d.lib mfc42d.lib msvcprtd.lib msvcrt.lib ollydbgvc7.lib kernel32.lib user32.lib shlwapi.lib Comdlg32.lib shell32.lib /nologo /dll /incremental:no /debug /machine:I386 /nodefaultlib:"msvcrtd.lib" /out:"Debug/ODbgScript.dll " /pdbtype:sept
# SUBTRACT LINK32 /verbose /profile /map /nodefaultlib /force
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copydbg.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ODbgScript - Win32 Release"
# Name "ODbgScript - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Dialogs.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\guicon.cpp

!IF  "$(CFG)" == "ODbgScript - Win32 Release"

# PROP Exclude_From_Build 1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ODbgScript - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HelperFunctions.cpp

!IF  "$(CFG)" == "ODbgScript - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ODbgScript - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IniReader.cpp
# End Source File
# Begin Source File

SOURCE=.\LogWindows.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mru.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ODbgScript.cpp

!IF  "$(CFG)" == "ODbgScript - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ODbgScript - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OllyLang.cpp

!IF  "$(CFG)" == "ODbgScript - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "ODbgScript - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OllyLangCommands.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Progress.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Search.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\var.cpp
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\dumper.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\guicon.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\HelperFunctions.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\IniReader.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\LogWindows.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mru.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ODbgScript.h
# End Source File
# Begin Source File

SOURCE=.\OllyLang.h
# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\Progress.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Search.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\var.h
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\log.ico
# End Source File
# Begin Source File

SOURCE=.\ODbgScript.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\script.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\copy.bat
# End Source File
# Begin Source File

SOURCE=.\copydbg.bat
# End Source File
# Begin Source File

SOURCE=.\doc\ODbgScript.txt
# End Source File
# Begin Source File

SOURCE=.\doc\Plugins.chm
# End Source File
# Begin Source File

SOURCE=.\doc\sample.osc
# End Source File
# End Target
# End Project
