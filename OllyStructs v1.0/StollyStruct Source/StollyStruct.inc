;include macros.asm

include windows.inc
include kernel32.inc
include user32.inc
include plugin.inc
include masm32.inc
include comdlg32.inc
include shlwapi.inc
include gdi32.inc

includelib kernel32.lib
includelib user32.lib
includelib Comctl32.lib
includelib shell32.lib
includelib ollydbg.lib
includelib shlwapi.lib
includelib gdi32.lib
;-----------------------------------------------------SYMBOLS-----------------------------------------------------
Mandatory   = 0
Optional    = 1
NotFound    = 0
Found       = 1
Installed   = 2

MaxBytes =   8

; Structure Selection form
IDD_DLG_STRUCTSELECT    equ     2000
IDC_CBO_STRUCTS         equ     2004
IDC_EDT_STRUCTSTART     equ     2005
IDC_EDT_INSTANCE        equ     2006
IDC_BTN_LABEL           equ     1005
IDC_BTN_OK	            equ		1003

IDD_DLG_ABOUT           equ     3000
IDC_IMG_ABOUT           equ     3001

IniAppSize              equ     30*1024    ; 30 kb buffer for list of possible structures (should be enough)

;-----------------------------------------------------STRUCTURES----------------------------------------------------- 

Patch struct
    ImageOffSet     DWORD   0                   ; OffSet in image
    NumberOfBytes   DWORD   0                   ; Number Of Bytes for this patch
    Expected        byte    MaxBytes   Dup(0)   ; Expected bytes (max 8)
    PatchData       byte    MaxBytes   Dup(0)   ; Replacement bytes
Patch ends

; OllyDbg supports and makes extensive use of special kind of data collections
; called sorted tables. A table consists of descriptor (t_table) and data. All
; data elements has same size and begin with a 3-dword header: address, size
; and type. Table automatically sorts items by address, overlapping is not
; allowed. Our bookmark table consists of elements of type t_StollyStruct.

t_StollyStruct struc 1
    index           dword   ?           ; Structure index
    size_           dword   ?           ; Size of index, always 1 in our case
    type_           dword   ?           ; Type of entry, always 0
    struct_addr     dword   ?
    struct_size     dword   ?           ; Size of index, always 1 in our case
    struct_name     db  TEXTLEN dup(0)      ; Name of the entry
    struct_type     db  TEXTLEN dup(0)      ; Entry Type (int, dword, ...)
t_StollyStruct ends

;-----------------------------------------------------PROTOTYPES-----------------------------------------------------

StructSelectDlgProc	PROTO	:HWND,:UINT,:WPARAM,:LPARAM
StructAboutDlgProc	PROTO	:HWND,:UINT,:WPARAM,:LPARAM

ParseHex PROTO  s:ptr byte
ParseSizeString PROTO  s:ptr byte
StollyStructWinProc proto hw:HWND, msg:UINT, wp:WPARAM, lp:LPARAM
StollyStructGetText proto C s:ptr byte, mask_:ptr byte, select:ptr dword, ph:ptr t_sortheader, column:dword

CreateStollyStructWindow proto
LoadStructure proto szType:ptr byte, dwAddress:DWORD, szName:ptr byte
GetPEBAddress proto
GetTEBAddress proto
LabelSortedStructure proto
ClearLabelSortedStructure proto
IsStructureAvailable PROTO szName:PTR BYTE

;-----------------------------------------------------Globals   -----------------------------------------------------

.data
StollyStructTable       t_table         <>          ; Main Structure table

hLogo                   HBITMAP 0
szPlugin                db "StollyStruct",0
szMainMenu              db "0 Select structure|1 Show PEB,2 Show TEB|3 &About",0
szStructMenu            db "0 &Structure",0
szInit                  db "StollyStruct plugin v1.0 by jstorme",0
szBadInit               db "StollyStruct was unable to initialize :(",0
;szAbout                 db "StollyStruct plugin v1.0 by jstorme",13,10,"Helps to visualize and label structures.",0

szStructTableName       db "Structure",0
szStollyStructWinClass  db "StollyStructClass",0    ; Name of StollyStruct window class

szDbFile                db "\StollyStructs.ini",0    ; Name of file containing all structure information
szDbPath                db MAX_PATH dup(0)
szKeyDefault            db "...",0                  ; Default 
;szBigBuffer             db 1024 dup(0)              ; Buffer to read line from ini-file
szCountBuffer           db 4 dup(0)                 ; Counter buffer

szBarAddress            db "Address",0              ; Title bar in Olly MDI window
szBarName               db "Name",0     
szBarSize               db "Bytes",0     
szBarType               db "Type",0
szBarValue              db "Value",0
szBarDump               db "Hex Dump",0


szMenuEdit              db "&Edit",0
szMenuLabel             db "&Label",0
szMenuLabelTmp          db "&Temp Labels (only valid until this window is closed)",0
szMenuFollow            db "&Follow",0
szMenuFollowInDump      db "address in dump",0
szMenuFolValInDump      db "value in dump",0
szMenuFollowInStack     db "address in stack",0
szMenuFolValInStack     db "value in stack",0
szMenuNewStruct         db "Open new structure at value",0

szFormatDWORD           db "%08X",0                 ; Format string for wsprintf Hex
szFormatString          db "%s",0                   ; Format string for wsprintf String
szFormatNumber          db "%d",0                   ; Format string for wsprintf Decimal
szFormatByte            db "%02X",0                 ; Format string for wsprintf Hex Byte
szFormatWORD            db "%04X",0                 ; Format string for wsprintf Hex Byte

szByteFormatBuf         db 8 dup(0)
szDelimiter             db ".",0

szPEB                   db "_PEB",0                  ; Fixed Key in ini file for PEB Structure
szTEB                   db "_TEB",0                  ; Fixed Key in ini file for PEB Structure


szStructAddressBuffer   db 12 dup(0)                ; TextBuffer 4 StructureAddress
dwStructAddress         DWORD 0 
szStructType            db 64 dup(0)                ; Name of the structure 
szStructName            db 16 dup(0)                ; Instance name of the structure
szValidHEX              db "FEDCBA9876543210",0     ; Hex conversion
                                      
.data?
hDllInst                HINSTANCE       ?           ; DLL instance
hOllyInst               HINSTANCE       ?           ; Olly Inst
hwMain                  HWND            ?           ; Handle of main OllyDbg window
dw_buffer               DWORD           ?
hHeap                   DWORD           ?
szStructListPtr         DWORD           ?
bTempLabels             BYTE            ?
