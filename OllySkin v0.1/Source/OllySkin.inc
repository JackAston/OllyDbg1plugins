	
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
	
	include Libs\Plugin.inc
	includelib Libs\OllyDbg.lib
	
	; GetProcLib
	; Written by Petroizki             ; Benchmark
	include Libs\GetProcLib.inc        ; GetProcAddress == 733
	includelib Libs\GetProcLib.lib     ; GetProcOffset  == 184

.data
	szPluginTitle CHAR "OllySkin",0
	szEnable      CHAR "Enable",0
	szSelect      CHAR "Select",0
	szClose       CHAR "Close",0
	szSkinFile    CHAR "Skin file",0
	szPluginInfo  CHAR "Version 0.1 alpha",0
	szMenu        CHAR "0 Configuration|1 About",0
	szAboutInfo   CHAR "OllySkin 0.1 alpha",13,10,13,10,"Written by maluc",13,10,"skinengine.dll - Copyright (c) by KSDev",0
	szIni         CHAR "ollydbg.ini",0
	szHistory     CHAR "History",0
	szPluginPath  CHAR "Plugin path",0
	szNoFile      CHAR "Skin?",0
	szSkins       CHAR "\skins",0
	szZero        CHAR "0",0
	szOne         CHAR "1",0
	szFilter      CHAR "Skin Files",0,"*.skin;*.msstyles",0,"All Files",0,"*",0,0
	
	szDll         CHAR "\skinengine.dll",0
	szInit        CHAR "InitSkinEngine",0
	szLoadSkin    CHAR "LoadSkinFromFile",0
	szApplySkin   CHAR "ApplySkinEngine",0
	szDisableSkin CHAR "DisableSkinEngine",0
;	szFreeSkin    CHAR "FreeSkinEngine",0
	szAboutSkin   CHAR "AboutSkinEngine",0

	bEnable       BOOL FALSE
	bInit         BOOL FALSE

.data?
	hInstance     HINSTANCE ?
	hWin          HWND ?
	hDll          HMODULE ?
;	hFileMenu     HMENU ?    
	
	_InitSkin     dd ?
	_LoadSkin     dd ?
	_ApplySkin    dd ?
	_DisableSkin  dd ?
;	_FreeSkin     dd ?
;	_AboutSkin    dd ?
	
	ofn           OPENFILENAME <>
;	rc            RECT <>
	Ollydbg_Ini   BYTE MAX_PATH+1 dup (?)
	PluginPath    BYTE MAX_PATH+1 dup (?)
	SkinFile      BYTE MAX_PATH+1 dup (?)
	Buffer        BYTE MAX_PATH+1 dup (?)
	TempBuffer    BYTE MAX_PATH+1 dup (?)
	
.code