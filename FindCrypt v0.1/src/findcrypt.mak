# Microsoft Developer Studio Generated NMAKE File, Based on findcrypt.dsp
!IF "$(CFG)" == ""
CFG=findcrypt - Win32 Debug
!MESSAGE No configuration specified. Defaulting to findcrypt - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "findcrypt - Win32 Release" && "$(CFG)" != "findcrypt - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "findcrypt.mak" CFG="findcrypt - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "findcrypt - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "findcrypt - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "findcrypt - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\findcrypt.dll" "$(OUTDIR)\findcrypt.bsc"


CLEAN :
	-@erase "$(INTDIR)\findcrypt.obj"
	-@erase "$(INTDIR)\findcrypt.pch"
	-@erase "$(INTDIR)\findcrypt.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\findcrypt.bsc"
	-@erase "$(OUTDIR)\findcrypt.dll"
	-@erase "$(OUTDIR)\findcrypt.exp"
	-@erase "$(OUTDIR)\findcrypt.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FINDCRYPT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\findcrypt.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\findcrypt.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\findcrypt.sbr"

"$(OUTDIR)\findcrypt.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=ollydbg.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\findcrypt.pdb" /machine:I386 /out:"$(OUTDIR)\findcrypt.dll" /implib:"$(OUTDIR)\findcrypt.lib" 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\findcrypt.obj" \
	".\ollydbg.lib"

"$(OUTDIR)\findcrypt.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "findcrypt - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\findcrypt.dll"


CLEAN :
	-@erase "$(INTDIR)\findcrypt.obj"
	-@erase "$(INTDIR)\findcrypt.pch"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\findcrypt.dll"
	-@erase "$(OUTDIR)\findcrypt.exp"
	-@erase "$(OUTDIR)\findcrypt.lib"
	-@erase "$(OUTDIR)\findcrypt.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FINDCRYPT_EXPORTS" /Fp"$(INTDIR)\findcrypt.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\findcrypt.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\findcrypt.pdb" /debug /machine:I386 /out:"$(OUTDIR)\findcrypt.dll" /implib:"$(OUTDIR)\findcrypt.lib" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\findcrypt.obj" \
	".\ollydbg.lib"

"$(OUTDIR)\findcrypt.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("findcrypt.dep")
!INCLUDE "findcrypt.dep"
!ELSE 
!MESSAGE Warning: cannot find "findcrypt.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "findcrypt - Win32 Release" || "$(CFG)" == "findcrypt - Win32 Debug"
SOURCE=.\findcrypt.cpp

!IF  "$(CFG)" == "findcrypt - Win32 Release"


"$(INTDIR)\findcrypt.obj"	"$(INTDIR)\findcrypt.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\findcrypt.pch"


!ELSEIF  "$(CFG)" == "findcrypt - Win32 Debug"


"$(INTDIR)\findcrypt.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\findcrypt.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "findcrypt - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FINDCRYPT_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\findcrypt.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\findcrypt.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "findcrypt - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FINDCRYPT_EXPORTS" /Fp"$(INTDIR)\findcrypt.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /J /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\findcrypt.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

