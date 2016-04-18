	
	include \masm32\include\windows.inc
	include \masm32\include\dialogs.inc
	include \masm32\macros\macros.asm

	uselib masm32
	uselib user32
	uselib kernel32
	uselib shell32
	uselib gdi32
	uselib comctl32
	uselib comdlg32
	uselib winmm
	uselib msvcrt
	uselib shlwapi
	uselib oleaut32
	uselib advapi32
	uselib riched20
	
	include Libs\Plugin.inc
	includelib Libs\OllyDbg.lib

.data
	szPluginTitle CHAR "TransOlly",0
	szEnable      CHAR "Enable Transparent",0
	szPluginInfo  CHAR "Version 1.0",0
	szMenu        CHAR "0 Options|1 About",0
	szAboutInfo   CHAR "TransOlly 1.0",0
	szIni         CHAR "ollydbg.ini",0
	szNull        CHAR "0",0
	szOne         CHAR "1",0
	sz255         CHAR "255",0
	szAlpha       CHAR "Alpha",0
	
	bEnable       BOOL FALSE
	Alpha         dd   255/2

.data?
	hInstance     HINSTANCE ?
	hWin          HWND ?
	
	Settings      BYTE 10 dup (?)
	Buffer        BYTE MAX_PATH+1 dup (?)
	

.code