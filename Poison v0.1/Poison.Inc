include windows.inc
include kernel32.inc
include user32.inc
include Libs\Plugin.inc
include comdlg32.inc
include masm32.inc

includelib kernel32.lib
includelib user32.lib
includelib Libs\OllyDbg.lib
includelib comdlg32.lib
includelib masm32.lib

GetAPIAddress PROTO :DWORD,:DWORD,:DWORD,:DWORD
DlgOptionProc PROTO :DWORD,:DWORD,:DWORD,:DWORD
HeapFlag PROTO
EPBreakPoint PROTO
Aggresive PROTO 
Hide PROTO 
TEBAddress PROTO
PEBAddress PROTO
PluginRead PROTO
FABreakPoints PROTO


literal MACRO quoted_text:VARARG
        LOCAL local_text
        .data
          local_text db quoted_text,0
        .code
        EXITM <local_text>
      ENDM

CTEXT MACRO quoted_text:VARARG
        EXITM <offset literal(quoted_text)>
      ENDM

m2m MACRO M1, M2
        push M2
        pop  M1
      ENDM

return MACRO arg
        mov eax, arg
        ret
      ENDM

.data
Process32entry						PROCESSENTRY32 <>
explorer							db "explorer.exe",0
AboutText 							db "Thanks go to Avatar and the creator of IsDuggerPresent for", 13,10
									db "releasing there Ollydbg plugin source codes and Eviloid for", 13, 10
									db "the Masm32 Plugin Inc. Greetz go out to everyone in SND Forum.", 13, 10
									db "---------------------------------------------------------------------", 13, 10
									db "Just some Test Code by What", 13,0 
AboutCaption 						TCHAR "About",0
sPluginTitle 						TCHAR "Poison",0
sPluginInfo 						TCHAR "What (SND Forum)",0
sMenu 								TCHAR "0 Hide|1 Options|2 About",0
sKernel32 							TCHAR "kernel32.dll",0
sUser32 							TCHAR "user32.dll",0
sntdll 								TCHAR "ntdll.dll",0
sIsDebuggerPresent 					TCHAR "IsDebuggerPresent",0
sZwQueryInformationProcess 			TCHAR "ZwQueryInformationProcess",0
sZwSetInformationThread 			TCHAR "ZwSetInformationThread",0
sOutputDebugStringA 				TCHAR "OutputDebugStringA",0
sCheckRemoteDebuggerPresent 		TCHAR "CheckRemoteDebuggerPresent",0
sBlockInput 						TCHAR "BlockInput",0
sFindWindow 						TCHAR "FindWindowA",0
sDebugBreak							TCHAR "DebugBreak",0
sOpenProcess						TCHAR "ZwOpenProcess",0
sCreateThread						TCHAR "CreateThread",0
sGetTickCount						TCHAR "GetTickCount",0
sFindWindowExA						TCHAR "FindWindowExA",0
sODBG_Pluginreset					TCHAR "_ODBG_Pluginreset",0
sIsDebug							TCHAR "Poison",0
sKiRaiseUserExceptionDispatcher		TCHAR "KiRaiseUserExceptionDispatcher",0
sZwYieldExecution					TCHAR "ZwYieldExecution",0
sModule32Next						TCHAR "Module32NextW",0
sProcess32Next						TCHAR "Process32NextW",0
sEnumWindows						TCHAR "EnumWindows",0
sZwQueryObject						TCHAR "ZwQueryObject",0
sAggresive							TCHAR "Aggresive",0
sZwQueryInformationThread			TCHAR "ZwQueryInformationThread",0
sBreakPoints						TCHAR "BreakPoints",0
sQueryPerformanceFrequency			TCHAR "QueryPerformanceFrequency",0
sQueryPerformanceCounter			TCHAR "QueryPerformanceCounter",0
sZwQueryPerformanceFrequency		TCHAR "ZwQueryPerformanceFrequency",0
sZwQueryPerformanceCounter			TCHAR "ZwQueryPerformanceCounter",0

sIsDebuggerPresentOffset				db 2h
sNtGlobalFlagOffset						db 68h


sBlockPatch 							db 90h, 0C2h, 04h, 00
sIsDebuggerPresentPatch					db 00
sCheckRemoteDebuggerPresentPatch 		db 0B8h, 00, 00, 00, 00, 0C2h, 08h, 00, 90h
sFindWindowPatch 						db 0B8h, 00, 00, 00, 00, 0C2h, 08h, 00, 90h
sOutputDebugStringAPatch 				db 0B8h, 01h, 00h, 00h, 00h, 0C2h, 04h, 00
sHeapFlagPatch 							db 00, 00, 00, 00
sFPUBugPatch 							db 0DBh
sZwSetInformationThreadPatch 			db 83h, 7Ch, 24h, 08h, 11h, 75h, 03h, 0C2h, 10h, 00h, 0B8h, 0E5h, 00h, 00h, 00h, 0E9h, 33h, 0E6h, 59h, 7Ch
sJump 									db 0E9h
sGetStartUpInfoPatch 					db 8Dh, 0BDh, 04h, 0E5h, 0FFh, 0FFh, 57h, 33h, 0C0h, 0B9h, 44h, 00h, 00h, 00h, 0F3h, 0AAh, 5Fh, 0C7h, 07h, 44h,00h,00h,00h,90h,90h,90h,90h,90h,90h,90h,90h,90h,90h,90h,0C7h,85h, 30h,0E5h,0FFh,0FFh, 81h,00h,00h,00h
sDebugBreakPatch						db 0C3h
sOpenProcessPatch						db 81h, 7Ch, 24h, 30h, 78h, 56h, 34h, 12h, 74h, 0Ah, 0C7h, 44h, 24h, 30h, 00, 00, 00, 00, 48h, 48h, 0B8h, 7Ah, 00h, 00h, 00h, 0E9h
sCreateThreadPatch						db 0C2h, 18h, 00h
sPatch									db 0FFh, 15h, 44h, 0F6h, 4Ah, 00h, 0EBh, 14h
sCall									db 0e8h
sGetTickCountPatch						db 0B8h, 00h, 04h, 00h, 00h, 0BAh, 05h, 00h, 00h, 00h, 0C3h, 00h
sFindWindowExAPatch						db 0B8h, 00, 00, 00, 00, 0C2h, 10h, 00, 90h
sKiRaiseUserExceptionDispatcherPatch 	db 3Dh, 08h, 00h, 00h, 0C0h, 75h, 03h, 33h, 0C0h, 0C3h, 55h, 8Bh, 0ECh, 83h, 0ECh, 50h	
sPlacementPatch							db 0EBh, 37h
sCPUWindowPatch							db 0EBh
sEntryPointAlertPatch					db 83h,0C4h,10h,90h,90h
sHeapFlagPatch2							db 02h, 00h, 00h, 00h
sCaptionPatch1							db 0B8h, 70h, 0F6h, 4Ah, 00h, 90h
sCaptionPatch2							db 50h, 4fh, 49h, 53h, 4fh, 4eh
sCaptionPatch3							db 68h, 80h, 0F6h, 4Ah, 00h
sCaptionPatch4							db 50h, 72h, 6Fh, 66h, 65h, 73h, 73h, 69h, 6Fh, 6Eh, 61h, 6Ch, 20h, 45h, 64h, 69h, 74h, 69h, 6Fh, 6Eh, 00h
sCaptionPatch5							db 68h, 70h, 0F6h, 4Ah, 00h
sZwYieldExecutionPatch					db 0B8h, 01h, 00h, 00h, 00h, 0C3h, 90h
sUnknownFormatPatch						db 0EBh
sModule32NextPatch						db 33h, 0C0h, 0C2h, 08h, 00h
sProcess32NextPatch						db 33h, 0C0h, 0C2h, 08h, 00h
sGetTickCountv2Patch					db 0BAh, 00h, 00h, 0FEh, 7Fh, 0Fh, 0B6h, 02h, 0Fh, 0B6h, 12h, 05h, 31h, 04h, 00h, 00h, 0C3h
sEnumWindowsPatch						db 33h, 0C0h, 0C2h, 08h, 00h
sCompressedCodePatch					db 0B8h, 06h, 00h, 00h, 00h, 83h, 0C4h, 04h, 90h, 90h, 90h, 90h
sZwQueryInformationProcessPatch			db 83h, 7Ch, 24h, 08h, 07h, 75h, 0Fh, 8Bh, 44h, 24h, 0Ch, 0C7h, 00h, 00h, 00h, 00h, 00h, 33h, 0C0h, 0C2h, 14h, 00h, 83h, 7Ch, 24h, 08h, 00h, 75h, 0Fh, 60h, 90h, 0B8h, 18h, 07h, 00h, 00h, 8Bh, 4Ch, 24h, 2Ch, 89h, 41h, 14h, 61h, 0C2h, 14h, 00h
sZwQueryObjectPatch						db 60h, 83h, 7Ch, 24h, 28h, 03h, 75h, 0Ch, 8Bh, 7Ch, 24h, 2Ch, 8Bh, 4Ch, 24h, 30h, 33h, 0C0h, 0F3h, 0AAh, 61h, 0C2h, 14h, 00h
sHidePebPatch							db 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,00,00,00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 
sRetByteFix								db 01h
sQueryPerformancePatch					db 33h, 0C0h, 0C2h, 04h, 00h
sZwQueryPerformancePatch				db 33h, 0C0h, 0C2h, 08h, 00h
sProcess32NextWHook						db 60h, 8Bh, 45h, 0Ch, 8Bh, 58h, 08h, 81h, 0FBh, 78h, 56h, 34h, 12h, 75h, 07h, 0C7h, 40h, 18h, 78h, 56h, 34h, 12h, 81h, 0FBh, 78h, 56h, 34h, 12h, 75h, 04h, 0C6h, 40h, 24h, 00h, 61h, 0C2h, 08h, 00h








BlockInput_    							db "pBlockInput",0
IsDebuggerPresent_   					db "pIsDebuggerPresent",0
NtGlobalFlag_							db "pNtGlobalFlag",0
CheckRemoteDebuggerPresent_ 			db "pCheckRemoteDebuggerPresent", 0
FindWindow_ 							db "pFindWindow",0
AutoHide_								db "pAutoHide",0
OutputDebugStringA_						db "pOutputDebugStringA",0
HeapFlag_								db "pHeapFlag",0
FPUBug_									db "pFPUBug",0
ZwSetInformationThread_					db "pZwSetInformationThread",0
GetStartUpInfo_							db "pGetStartUpInfo",0
Aggressive_								db "pAggressive",0
ZwQueryInformationProcess_  			db "pZwQueryInformationProcessPatch",0
DebugBreak_								db "pDebugBreak",0
OpenProcess_							db "pOpenProcess",0
CreateThread_							db "pCreateThread",0
EPBreak_								db "pEPBreak",0
TLSCallBack_							db "pTLSCallBack",0
GetTickCount_							db "pGetTickCount",0
InvalidHandle_							db "pInvalidHandle",0
CaptionPatch_							db "pCaptionPatch",0
ZwYieldExecution_						db "pZwYieldExecution",0
Process32Next_							db "pProcess32Next",0
Module32Next_							db "pModule32Next",0
GetTickCountv2_							db "pGetTickCountv2",0
EnumWindows_							db "pEnumWindows",0
ZwQueryObject_							db "pZwQueryObject",0
QueryPerformanceFrequency_				db "pQueryPerformanceFrequency",0
QueryPerformanceCounter_				db "pQueryPerformanceCounter",0
Process32NextW_							db "pProcess32NextW",0

.data?
hinst               		HINSTANCE       ?  
hwmain              		HWND            ?  
SVWinClass					dd ?
LetsGo						dd ?
Handle						dd ?
GoodOffset					dd ?
hInstance 					dd ?
hWin 						dd ?
pKernel32 					dd ?
pUser32 					dd ?
pIsDebuggerPresent 			dd ?
pBlockInput 				dd ?
pIsDebuggerPresent2 		dd ?
pCheckRemoteDebuggerPresent dd ?
pFindWindow 				dd ?
pOutputDebugStringA			dd ?
pHeapFlag					dd ?
pZwSetInformationThread		dd ?
pntdll						dd ?
pZwQueryInformationProcess  dd ?
pDebugBreak					dd ?
pOpenProcess				dd ?
pCreateThread				dd ?
pGetTickCount				dd ?
pKiRaiseUserExceptionDispatcher dd ?
pZwYieldExecution			dd ?
pModule32Next				dd ?
pProcess32Next				dd ?
pEnumWindows				dd ?
pZwQueryObject				dd ?
pQueryPerformanceCounter   	dd ?
pQueryPerformanceFrequency	dd ?
pZwQueryPerformanceCounter  dd ?
pZwQueryPerformanceFrequency	dd ?


JumpLocation 				dd ?
NewOffset					dd ?
MoveBack					dd ?
Virtual						dd ?
Location 					dd ?
Old 						dd ?
dw_buffer 					dd ?
Range1						dd ?
Range2						dd ?
Saved						dd ?
NumberHeaps					dd ?
LocationHeaps				dd ?
ThreadInfo					dd 30 dup (?)
TEBOffset					dd ?
ProcID						dd ?


.const
IDD_DLG									equ 100
IDD_OPTION								equ 101
IDC_GRP1								equ 1003
IDC_CHK1								equ 1001
IDC_CHK3 								equ 1004
IDC_CHK4								equ 1005
IDC_BTN1								equ 1006
IDC_BTN2								equ 1007
IDC_CHK2								equ 1002
IDC_CHK5								equ 1008
IDC_GRP5								equ 1009
IDC_CHK7								equ 1010
IDC_CHK8								equ 1011
IDC_CHK9								equ 1012
IDC_CHK10								equ 1013
IDC_CHK11								equ 1014
IDC_CHK12								equ 1015
IDC_CHK13								equ 1016
IDC_CHK14								equ 1017
IDC_CHK15								equ 1018
IDC_CHK16								equ 1019
IDC_GRP2								equ 1020
IDC_GRP3								equ 1021
IDC_GRP4								equ 1022
IDC_CHK6								equ 1023
IDC_CHK17								equ 1024
IDC_CHK18								equ 1025
IDC_CHK19								equ 1026
IDC_CHK20								equ 1028
IDC_CHK21								equ 1029
IDC_CHK23								equ 1031
IDC_CHK24								equ 1032
IDC_CHK25								equ 1033
IDC_CHK26								equ 1034
IDC_CHK27								equ 1035
IDC_CHK28								equ 1036
IDC_CHK29								equ 1037
IDC_CHK30								equ 1038
IDC_CHK31								equ 1039
CHK_1									equ 1030


.data?


IsDebuggerPresent1					dd 				?
NtGlobalFlag1       				dd 				?
CheckRemoteDebuggerPresent1 		dd 				?
FindWindow1							dd 				?
BlockInput1							dd				?
AutoHide1							dd				?
OutputDebugStringA1					dd				?
HeapFlag1							dd				?
FPUBug1								dd				?
sHeapFlag2 							dd				?
ZwSetInformationThread1				dd				?
GetStartUpInfo1						dd				?
Aggressive1							dd				?
ZwQueryInformationProcess1  		dd			    ?
DebugBreak1							dd				?
OpenProcess1						dd				?
CreateThread1						dd				?
EPBreak1							dd				?
TLSCallBack1						dd				?
GetTickCount1						dd				?
InvalidHandle1						dd				?
MaximizeWindow1						dd				?
EntryPointAlert1					dd				?
CaptionPatch1						dd				?
ZwYieldExecution1					dd				?
UnknownFormat1						dd				?
Process32Next1						dd				?
Module32Next1						dd				?
GetTickCountv2						dd				?
EnumWindows1						dd				?
CompressedCode1						dd				?
ZwQueryObject1						dd				?
QueryPerformanceCounter1			dd				?
QueryPerformanceFrequency1			dd				?
Process32NextW1						dd 				?

ZwSetInformationThread2				dd				?
ZwQueryInformationProcess2			dd				?
OpenProcess2						dd				?
InvalidHandle2						dd				?
ZwQueryObject2						dd				?
GetTickCount2						dd 				?
Process32NextW2						dd 				?
Completed							dd				?




