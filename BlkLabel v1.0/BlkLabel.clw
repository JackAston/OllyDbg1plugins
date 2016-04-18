 PROGRAM
    INCLUDE('Equates.clw')
    INCLUDE('Builtins.clw')
!
! Global Definitions:
!
! (1) Type Re-definitions:
!
ASCIIsz                               EQUATE(CSTRING)
HANDLE                                EQUATE(LONG)
hWnd                                  EQUATE(LONG)
hInstance                             EQUATE(LONG)
hFile                                 EQUATE(LONG)
hFindFile                             EQUATE(LONG)
hTemplateFile                         EQUATE(LONG)
DWORD                                 EQUATE(LONG)
WORD                                  EQUATE(SHORT)
wParam                                EQUATE(WORD)
lParam                                EQUATE(DWORD)
OFS_MAXPATHNAME                       EQUATE(128)
MAX_PATH                              EQUATE(260)
INVALID_HANDLE_VALUE                  EQUATE(-1)
GENERIC_READ                          EQUATE(080000000h)
GENERIC_WRITE                         EQUATE(040000000h)
CREATE_ALWAYS                         EQUATE(2)
OPEN_EXISTING                         EQUATE(3)
FILE_SHARE_READ                       EQUATE(01h)
FILE_ATTRIBUTE_ARCHIVE                EQUATE(000000020h)
FILE_ATTRIBUTE_NORMAL                 EQUATE(000000080h)
HH_DISPLAY_TOPIC                      EQUATE(00000h)
HH_CLOSE_ALL                          EQUATE(00012h)
!
! (2) 'Windows in general':
!
!
WindowsMaximumPathNameLength          EQUATE(MAX_PATH)
WindowsMaximumFileNameLength          EQUATE(256)
!
S_OK                                  EQUATE(0)
S_False                               EQUATE(-1)
WM_USER                               EQUATE(0400h)
!
! Long File Dialgue Control:
!
FILE_DIALOGUE_Open                    EQUATE(0000000b)
FILE_DIALOGUE_Save                    EQUATE(0000001b)
FILE_DIALOGUE_HoldCurrentPath         EQUATE(0000010b)
FILE_DIALOGUE_DontReportErrors        EQUATE(0000100b)
FILE_DIALOGUE_AllowMultipleSelections EQUATE(0001000b)
FILE_DIALOGUE_LongFileNames           EQUATE(0010000b)
FILE_DIALOGUE_DirectorySelection      EQUATE(0100000b)
!
! Open/Save File Name Flags:
!
OFN_READONLY                          EQUATE(000000001h)
OFN_OVERWRITEPROMPT                   EQUATE(000000002h)
OFN_HIDEREADONLY                      EQUATE(000000004h)
OFN_NOCHANGEDIR                       EQUATE(000000008h)
OFN_SHOWHELP                          EQUATE(000000010h)
OFN_ENABLEHOOK                        EQUATE(000000020h)
OFN_ENABLETEMPLATE                    EQUATE(000000040h)
OFN_ENABLETEMPLATEHANDLE              EQUATE(000000080h)
OFN_NOVALIDATE                        EQUATE(000000100h)
OFN_ALLOWMULTISELECT                  EQUATE(000000200h)
OFN_EXTENSIONDIFFERENT                EQUATE(000000400h)
OFN_PATHMUSTEXIST                     EQUATE(000000800h)
OFN_FILEMUSTEXIST                     EQUATE(000001000h)
OFN_CREATEPROMPT                      EQUATE(000002000h)
OFN_SHAREAWARE                        EQUATE(000004000h)
OFN_NOREADONLYRETURN                  EQUATE(000008000h)
OFN_NOTESTFILECREATE                  EQUATE(000010000h)
OFN_NONETWORKBUTTON                   EQUATE(000020000h)
OFN_NOLONGNAMES                       EQUATE(000040000h)
OFN_EXPLORER                          EQUATE(000080000h)
OFN_NODEREFERENCELINKS                EQUATE(000100000h)
OFN_LONGNAMES                         EQUATE(000200000h)
OFN_ENABLEINCLUDENOTIFY               EQUATE(000400000h)
OFN_ENABLESIZING                      EQUATE(000800000h)
OFN_DONTADDTORECENT                   EQUATE(002000000h)
OFN_FORCESHOWHIDDEN                   EQUATE(010000000h)
!
! (3) My IO Handler:
!
IOTextFileStructure                   GROUP,PRE(IOTFS),TYPE
Mode                                     LONG             ! GENERIC_READ = Read, GENERIC_WRITE = Write
CollectAsUPPER                           LONG             ! True = Characters Buffered in all Uppercase on Reading
FileHandle                               hFile
FileSizeToProcessInBytes                 LONG
FileSizeProcessedInBytes                 LONG
PreviousCharactersz                      ASCIIsz(' ')
CurrentCharactersz                       ASCIIsz(' ')
CurrentRecordIndex                       LONG
MaximumTRecordIndex                      LONG             ! SIZE(IOTRecordsz) - 1
CoJoinedFileHandle                       hFile
LineNumber                               LONG
                                      END
!
! (4) OllyDebug 'specials':
!
DLL_PROCESS_ATTACH                    EQUATE(1)
PM_MAIN                               EQUATE(0)               ! Main window        (NULL)
NM_LABEL                              EQUATE(031h)            ! User-defined Label
WM_USER_CHALL                         EQUATE(WM_USER|
                                             + 116)           ! Redraw (almost) everything
STAT_NONE                             EQUATE(0)               ! Thread/process is empty
STAT_STOPPED                          EQUATE(1)               ! Thread/process suspended
STAT_EVENT                            EQUATE(2)               ! Processing debug event, process paused
STAT_RUNNING                          EQUATE(3)               ! Thread/process running
STAT_FINISHED                         EQUATE(4)               ! Process finished
STAT_CLOSING                          EQUATE(5)               ! Process is requested to terminate
!
! (4) My own Functions:
!
    INCLUDE('Veronica.clw','GlobalInclude')
!
! Security Attributes Structure:
!
SecurityAttributesStructure                GROUP,TYPE
StructureLength                               LONG                   ! Structure Size in Bytes
Descriptor                                    LONG                   ! FARPointer
InheritHandle                                 BOOL
                                           END
!
! File Time Structure
!
FileTimeStructure                          GROUP,TYPE
LowDateTime                                   DWORD
HighDateTime                                  DWORD
                                           END
!
! System Time Structure:
!
SystemTimeStructure                        GROUP,TYPE
Year                                          SHORT
Month                                         SHORT
DayOfWeek                                     SHORT
Day                                           SHORT
Hour                                          SHORT
Minute                                        SHORT
Second                                        SHORT
Milliseconds                                  SHORT
                                           END
!
! This structure describes a file found by the FindFirstFile, FindFirstFileEx or FindNextFile function.
!
Win32FindDataStructure                     GROUP,TYPE
FileAttributes                                DWORD
CreationTime                                  LIKE(FileTimeStructure)
LastAccessTime                                LIKE(FileTimeStructure)
LastWriteTime                                 LIKE(FileTimeStructure)
FileSizeHigh                                  DWORD
FileSizeLow                                   DWORD
Reserved0                                     DWORD
Reserved1                                     DWORD
FileName                                      ASCIIsz(MAX_PATH)
AlternateFileName                             ASCIIsz(14)
                                           END
!
! (Shell) Browse Information Structure:
!
BrowseInfoStructure                       GROUP,TYPE
hWndOwner                                    LONG
PIDLRoot                                     LONG(0)     ! = ADDRESS(PCIDLIST_ABSOLUTE)
PathNameBuffersz                             LONG(0)     ! = ADDRESS(ASCIIsz(...))
Titlesz                                      LONG(0)     ! = ADDRESS(ASCIIsz(...))
Flags                                        LONG
CallBackFunction                             LONG(0)     ! = ADDRESS(BFFCALLBACK)
Param                                        LONG
Image                                        LONG
                                          END
!
! Open/Save File Name Structure:
!
OpenFileNameStructure                     GROUP,TYPE
StructSize                                   DWORD
hwndOwner                                    hWnd
hInstance                                    hInstance
Filter                                       LONG(0)     ! = ADDRESS(ASCIIsz(...))
CustomFilter                                 LONG(0)     ! = ADDRESS(ASCIIsz(...))
MaxCustFilter                                DWORD
FilterIndex                                  DWORD
File                                         LONG(0)     ! = ADDRESS(ASCIIsz(...))
MaxFile                                      DWORD
FileTitle                                    LONG(0)     ! = ADDRESS(ASCIIsz(...))
MaxFileTitle                                 DWORD
InitialDir                                   LONG(0)     ! = ADDRESS(ASCIIsz(...))
Title                                        LONG(0)     ! = ADDRESS(ASCIIsz(...))
Flags                                        DWORD
FileOffset                                   SHORT
FileExtension                                SHORT
DefExt                                       LONG(0)     ! = ADDRESS(ASCIIsz(...))
CustData                                     DWORD
fnHook                                       LONG(0)     ! = ADDRESS(OFNHOOKPROC(...))
TemplateName                                 LONG(0)     ! = ADDRESS(ASCIIsz(...))
                                          END
!
! Global Map:
!
 MAP
!
! (1) Internals (called BY OllyDebug's PlugIn Mechanism):
!
    DllEntryPoint(hInstance,|
                  DWORD Reason,|
                  LONG AddressOfReserved),|
                     LONG,|
                        PASCAL
    _ODBG_Plugindata(LONG AddressOfShortNamesz),|
                        LONG,|
                           C
    _ODBG_Plugininit(LONG OllyDebugVersion,|
                     hWnd OllyDebugMainWindowHandle,|
                     LONG AddressOfFeatures),|
                        LONG,|
                           C
    _ODBG_Pluginmenu(LONG ThisOrigin,|
                     LONG AddressOfMenuStringsz,|
                     LONG ThisItemAddress),|
                        LONG,|
                           C
    _ODBG_Pluginaction(LONG ThisOrigin,|
                       LONG ThisAction,|
                       LONG AddressOfThisItem),|
                           C
    _ODBG_Pluginmainloop(LONG AddressOfDebugEvent),|
                            C
    _ODBG_Pluginsaveudd(LONG AddressOfGroupTModule,|
                        LONG IsMainModule),|
                           C
    _ODBG_Pluginuddrecord(LONG AddressOfGroupTModule,|
                          LONG IsMainModule,|
                          LONG ThisTag,|
                          LONG ThisSize,|
                          LONG AddressOfThisData),|
                             LONG,|
                                C
    _ODBG_Pluginshortcut(LONG ThisOrigin,|
                         LONG ThisControl,|
                         LONG ThisAlt,|
                         LONG ThisShift,|
                         LONG ThisKey,|
                         LONG AddressOfThisItem),|
                            LONG,|
                               C
    _ODBG_Pluginreset(),|
                      C
    _ODBG_Pluginclose(),|
                      LONG,|
                         C
    _ODBG_Plugindestroy(),|
                        C
    _ODBG_Plugincmd(LONG ThisReason,|
                    LONG AddressOfGroupTReg,|
                    LONG AddressOfCommandsz),|
                       LONG,|
                          C
!
! Pure Internals:
!
     OpenFileForReadOrWrite(*IOTextFileStructure,|
                            *ASCIIsz PathAndFileNamesz,|
                            *ASCIIsz IORecordsz),|
                               LONG
     LoadFile(*IOTextFileStructure,|
              *ASCIIsz,|
              *PerRecordFunction,|
              *PerCharacterFunction)
     MAPFilePerCharacterReadingProcessor(*IOTextFileStructure,|
                                         *ASCIIsz IORecordsz),|
                                            LONG
     MAPFilePerRecordReadingProcessor(*IOTextFileStructure,|
                                      *ASCIIsz IORecordsz),|
                                         LONG
     LongFileDialogue(*ASCIIsz DialogueCaptionsz,|
                      *ASCIIsz PathAndFileNamesz,|
                      *ASCIIsz FileFiltersz,|
                      LONG Flags = 0,|
                      LONG hWndOwner = 0),|
                         BOOL,|
                            PROC
     LongPathName(*ASCIIsz LongPathsz,|
                  <*ASCIIsz ShortPathsz>,|
                  LONG = True,|
                  LONG = False)
     LongPathDialogue(*ASCIIsz DialogueCaptionsz,|
                      *ASCIIsz FileNameBuffersz,|
                      LONG EnsureTrailingSlash = True,|
                      LONG Flags = 0,|
                      LONG hWndOwner = 0),|
                         BOOL,|
                            PROC
     HelpAbout()
     RunHelp(hWnd CallersWindowHandle),|
             hWnd
!
! (2) Veronica's IO File Handler (TYPE specifications):
!
    MODULE('IOHANDLER')
       PerRecordFunction(*IOTextFileStructure,|
                         *ASCIIsz IORecordsz),|
                            LONG,|
                               TYPE
       PerCharacterFunction(*IOTextFileStructure,|
                            *ASCIIsz IORecordsz),|
                               LONG,|
                                  TYPE
    END
!
! (3) Externals in Veronica's Function Library:
!
    INCLUDE('Veronica.clw','GlobalMap')
!
! (4) Externals within the Sub Labelling PlugIn:
!
!     These have to be defined this way in order to cater for Clarion & any other Programming Language.
!
    MODULE('SubPlugIn')
       MAPFilePerCharacterHandler(LONG LengthOfPreviousCharactersz,|
                                  *ASCIIsz PreviousCharactersz,|
                                  LONG LengthOfThisCharactersz,|
                                  *ASCIIsz ThisCharactersz),|
                                     LONG,|
                                        PASCAL,RAW
       MAPFilePerRecordHandler(LONG LengthOfRecordsz,|
                               *ASCIIsz Recordsz),|
                                  LONG,|
                                     PASCAL,RAW
    END
!
! (4) Externals within OllyDbg.exe:
!
    MODULE('OllyDebug')
       GetStatus(),|
                 LONG,|
                    C,RAW,NAME('_Getstatus')
       AddToList(LONG AddressValue,|
                 LONG Highlight,|
                 *ASCIIsz Textsz),|
                    C,RAW,NAME('_Addtolist')
       DeleteEntireNameRange(ULONG = 0,|
                             ULONG = 0FFFFFFFFFFFFFFFFh,|
                             LONG = NM_LABEL),|
                                C,NAME('_Deletenamerange')
       Broadcast(LONG MessageCode, wParam, lParam),|
                    LONG,|
                       C,RAW,PROC,NAME('_Broadcast')
    END
!
! (5) External Windows APIs:
!
    MODULE('WINDOWS')
       IsWindow(hWnd),|
                   BOOL,|
                      PASCAL,RAW
       HtmlHelp(hWnd CallersWindowHandle,|
                *ASCIIsz CHMCompiledHelpFileNamesz,|
                DWORD HelpCommand,|
                DWORD CommandData),|
                   hWnd,|
                      PASCAL,RAW,PROC,NAME('HtmlHelpA')
       GetOpenFileName(*OpenFileNameStructure),|
                           BOOL,|
                              PASCAL,RAW,PROC,NAME('GetOpenFileNameA')
       GetSaveFileName(*OpenFileNameStructure),|     
                           BOOL,|
                              PASCAL,RAW,PROC,NAME('GetSaveFileNameA')
       GetLongPathName(*ASCIIsz ShortPathsz,|
                       *ASCIIsz LongPathBuffersz,|
                       DWORD LongPathBufferBufferSize),|
                          DWORD,|
                             PASCAL,RAW,NAME('GetLongPathNameA'),PROC
       MoveWindow(hWnd,|
                  LONG X,|
                  LONG Y,|
                  LONG Width,|
                  LONG Height,|
                  BOOL Repaint),BOOL,RAW,PASCAL,PROC
       CreateDiscFile(*ASCIIsz FileName,|                               ! Path and File Name
                      LONG DesiredAccess,|                              ! Access (Read-Write) Mode
                      LONG ShareMode,|                                  ! Share Mode
                      *SecurityAttributesStructure,|                    ! Security Attributes
                      LONG CreationDistribution,|                       ! How to create
                      LONG FlagsAndAttributes,|                         ! File Attributes
                      hTemplateFile),|                                  ! Handle to file with Attributes to copy
                      hFile,|
                         PASCAL,RAW,NAME('CreateFileA')
       ReadFileNonOverlapped(hFile,|
                             *ASCIIsz DataBuffersz,|
                             DWORD NumberOfBytesToRead,|
                             *DWORD NumberOfBytesRead,|
                              LONG=0),|                                       ! Null Overlapped Structure Pointer
                                BOOL,|
                                    PASCAL,RAW,PROC,NAME('ReadFile')
       FindFirstFile(*ASCIIsz FileNamesz,|    ! May contain wildcards * and ?
                     *Win32FindDataStructure),hFindFile,PASCAL,RAW,NAME('FindFirstFileA')
       FindNextFile(hFindFile,|               ! Search Handle
                    *Win32FindDataStructure),BOOL,PASCAL,RAW,NAME('FindNextFileA')
       FindClose(hFindFile),BOOL,PASCAL,RAW,PROC
       CloseHandle(HANDLE),BOOL,PASCAL,RAW,PROC
       SHBrowseForFolder(*BrowseInfoStructure),|
                              LONG,|
                                 PASCAL,RAW,NAME('SHBrowseForFolderA')
       SHGetPathFromIDList(LONG AbsolutePIDL,|
                           *ASCIIsz AbsolutePathsz),|
                              BOOL,|
                                 PASCAL,RAW,PROC,NAME('SHGetPathFromIDListA')
    END
 END
!
! Global Data:
!
PluginVersion              LONG(110),STATIC
PluginNamesz               ASCIIsz('Bulk Labelling'),STATIC
PlugInFullIdentitysz       ASCIIsz('Bulk Labels Loading Plugin v1.0'),STATIC
PlugInAuthorsz             ASCIIsz('Written by: Veronica Chapman'),STATIC
MenuStringsz               ASCIIsz('0 &Run|1 &Help,2 &About'),STATIC
MAPSelectionCaptionsz      ASCIIsz('Select MAP File')
MAPPathAndFileNamesz       ASCIIsz(WindowsMaximumPathNameLength|
                                   + WindowsMaximumFileNameLength|
                                   + 1),STATIC
MAPFileFilterssz           ASCIIsz('MAP Files *.map|*.map|AllFiles *.*|*.*')
HelpFileNamesz             ASCIIsz('BlkLabel.chm'),STATIC
IOBuffersz                 ASCIIsz(8192),STATIC           ! Create a large (8K?) IO Buffer (could be bigger for "faster")
IORecordsz                 ASCIIsz(2048),STATIC           ! 2K (maximum) Record
!
LocalIOReadFileStructure   LIKE(IOTextFileStructure),STATIC
!
OllyDbgMainWindowHandle    hWnd,STATIC
OurhInstance               hInstance,STATIC
OllyDbgStatus              LONG,STATIC
HTMLHelpWindowHandle       hWnd(0),STATIC
!
! Main Entry Point:
!
 CODE
 RETURN
! ======================================================================================================================
DllEntryPoint              FUNCTION(FnhInstance,|
                                    FnReason,|
                                    FnAddressOfReserved)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
 IF FnReason = DLL_PROCESS_ATTACH
    OurhInstance = FnhInstance
 END
!
 RestoreRegisters()
 RETURN(True)
! ======================================================================================================================
_ODBG_Plugindata           FUNCTION(FnAddressOfShortNamesz)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
 StringCopy(PluginNamesz,|
            FnAddressOfShortNamesz)
!
 RestoreRegisters()
 RETURN(PluginVersion)
! ======================================================================================================================
_ODBG_Plugininit           FUNCTION(FnOllyDebugVersion,|
                                    FnMainhWnd,|
                                    FnAddressOfFeatures)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
 IF (FnOllyDebugVersion < PluginVersion)
    RestoreRegisters()
    RETURN(S_False)
 END
!
! Keep handle of main OllyDbg Window. This handle is necessary, for example,
!    to display a Message Box:
!
 OllyDbgMainWindowHandle = FnMainhWnd
!
 AddToList(0,|
           0,|
           PlugInFullIdentitysz)
 AddToList(0,|
           -1,|
           PlugInAuthorsz)
!
 RestoreRegisters()
 RETURN(S_OK)
! ======================================================================================================================
_ODBG_Pluginmenu           FUNCTION(FnThisOrigin,|
                                    FnAddressOfMenuStringsz,|
                                    FnAddressOfThisItem)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
 IF FnThisOrigin NOT = PM_MAIN
    RestoreRegisters()
    RETURN(False)
 END
!
 StringCopy(MenuStringsz,|
            FnAddressOfMenuStringsz)
!
 RestoreRegisters()
 RETURN(True)
! ======================================================================================================================
_ODBG_Pluginaction         PROCEDURE (FnThisOrigin,|
                                      FnThisAction,|
                                      FnAddressOfThisItem)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
 IF FnThisOrigin = PM_MAIN
    CASE FnThisAction
    OF 0 ! Load the Labelling in Bulk
       OllyDbgStatus = GetStatus()
       IF ((OllyDbgStatus = STAT_STOPPED)|
        OR (OllyDbgStatus = STAT_EVENT))
          LongPathName(MAPPathAndFileNamesz)
          MAPPathAndFileNamesz = MAPPathAndFileNamesz|
                               & '\*.map'
          IF LongFileDialogue(MAPSelectionCaptionsz,|
                              MAPPathAndFileNamesz,|
                              MAPFileFilterssz,|
                              (FILE_DIALOGUE_Open|
                               + FILE_DIALOGUE_HoldCurrentPath|
                               + FILE_DIALOGUE_LongFileNames)) > 0
             LocalIOReadFileStructure.Mode = GENERIC_READ
             LocalIOReadFileStructure.CollectAsUPPER = True
             IF OpenFileForReadOrWrite(LocalIOReadFileStructure,|
                                       MAPPathAndFileNamesz,|
                                       IORecordsz) = 0
!
                DeleteEntireNameRange()
!
                LoadFile(LocalIOReadFileStructure,|
                         IORecordsz,|
                         MapFilePerRecordReadingProcessor,|
                         MapFilePerCharacterReadingProcessor)
!
! Update all Displays:
!
                Broadcast(WM_USER_CHALL,|
                          0,|
                          0)
             END
          END
       ELSE
          MESSAGE('There is nothing to Label',|
                  'Bulk Labels Loader',|
                  ICON:Question)
       END
!
    OF 1 ! Help File
       HTMLHelpWindowHandle = RunHelp(0)
!
    OF 2 ! Help About
       HelpAbout()
    END
 END
!
 RestoreRegisters()
 RETURN
! ======================================================================================================================
_ODBG_Pluginsaveudd        PROCEDURE(FnAddressOfGroupTModule,|
                                     FnIsMainModule)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN
! ======================================================================================================================
_ODBG_Pluginuddrecord      FUNCTION(FnAddressOfGroupTModule,|
                                    FnIsMainModule,|
                                    FnThisTag,|
                                    FnThisSize,|
                                    FnAddressOfThisData)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN(False)
! ======================================================================================================================
_ODBG_Pluginmainloop       PROCEDURE(FnAddressOfDebugEvent)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN
! ======================================================================================================================
_ODBG_Pluginshortcut       FUNCTION(FnThisOrigin,|
                                    FnThisControl,|
                                    FnThisAlt,|
                                    FnThisShift,|
                                    FnThisKey,|
                                    FnAddressOfThisItem)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN(False)
! ======================================================================================================================
_ODBG_Pluginreset          PROCEDURE()
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN
! ======================================================================================================================
_ODBG_Pluginclose          FUNCTION()
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN(False)                          ! Don't hold up on Closure
! ======================================================================================================================
_ODBG_Plugindestroy        PROCEDURE()
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN
! ======================================================================================================================
_ODBG_Plugincmd            FUNCTION(FnReason,|
                                    FnAddressOfGroupTReg,|
                                    FnAddressOfCommandsz)
!
! No Local Data can be declared because this directly called Externally.
!
 CODE
 SaveRegisters()
!
! No Logic required in the context of this current Plugin, therefore:
!
 RestoreRegisters()
 RETURN(False)
! ======================================================================================================================
OpenFileForReadOrWrite     FUNCTION (*FnIOTextFileStructure,|
                                     FnPathAndFileNamesz,|
                                     *FnIORecordsz)
!
! Local (Stack Frame) Data:
!
! (Local Data can be declared because this only called Internally)
!
LocalSecurityAttributesStructure        LIKE(SecurityAttributesStructure)
LocalWin32FindDataStructure             LIKE(Win32FindDataStructure)
 CODE
 IF FnIOTextFileStructure.FileHandle NOT = 0
    RETURN(1)                                                    ! Invalid IO Control Structure
 ELSIF ((FnIOTextFileStructure.Mode NOT = GENERIC_READ)|
  AND (FnIOTextFileStructure.Mode NOT = GENERIC_WRITE))
    RETURN(1)                                                    ! Invalid IO Control Structure
 END
!
 LocalWin32FindDataStructure.FileSizeHigh = 0
 LocalWin32FindDataStructure.FileSizeLow = 0
!
 FnIOTextFileStructure.FileHandle = FindFirstFile(FnPathAndFileNamesz,|
                                                  LocalWin32FindDataStructure)
 IF FnIOTextFileStructure.FileHandle = INVALID_HANDLE_VALUE
    IF (FnIOTextFileStructure.Mode = GENERIC_READ)
       RETURN(2)                                                    ! Read File Not Found
    END
 ELSE
    CloseHandle(FnIOTextFileStructure.FileHandle)
 END
!
 FnIOTextFileStructure.FileHandle = 0
!
! Prepare Null Security Attributes:
!
 LocalSecurityAttributesStructure.StructureLength = SIZE(LocalSecurityAttributesStructure)
 LocalSecurityAttributesStructure.Descriptor = 0
 LocalSecurityAttributesStructure.InheritHandle = False
!
 IF (FnIOTextFileStructure.Mode = GENERIC_WRITE)
!
! +++++++++++
! + Writing +
! +++++++++++
!
    IF ((LocalWin32FindDataStructure.FileSizeHigh NOT = 0)|
     OR (LocalWin32FindDataStructure.FileSizeLow NOT = 0))
       CASE MESSAGE('"'|
                  & FnPathAndFileNamesz|
                  & '" exists.'||
                  & '||'|
                  & 'Is it OK to OVERWRITE it?',|
                    'Opening File for Output',|
                    ICON:Question,|
                    BUTTON:Yes+BUTTON:No,|
                    BUTTON:No)
       OF BUTTON:No
          RETURN(3)                                              ! Write File WAS Found and NOT overwritten
       END
    END
!
    FnIOTextFileStructure.FileHandle = CreateDiscFile(FnPathAndFileNamesz,|
                                                      GENERIC_WRITE,|                     ! Access (Read-Write) Mode
                                                      0,|                                 ! Not Shared
                                                      LocalSecurityAttributesStructure,|  ! Security Attributes required
                                                      CREATE_ALWAYS,|                     ! Create as a New File (even if it exists already)
                                                      FILE_ATTRIBUTE_ARCHIVE,|            ! Give it Archived Status
                                                      0)                                  ! No 'Template' File'
    IF FnIOTextFileStructure.FileHandle = INVALID_HANDLE_VALUE
       FnIOTextFileStructure.FileHandle = 0
       RETURN(4)                                                 ! File Not Created
    END
!
    FnIOTextFileStructure.FileSizeToProcessInBytes = 0
!
 ELSE
!
! +++++++++++
! + Reading +
! +++++++++++
!
    IF LocalWin32FindDataStructure.FileSizeHigh > 0
       RETURN(5)                                                 ! Read File Too Large
    ELSIF LocalWin32FindDataStructure.FileSizeLow = 0
       RETURN(6)                                                 ! Read File Is Empty
    END
!
    FnIOTextFileStructure.FileSizeToProcessInBytes = LocalWin32FindDataStructure.FileSizeLow
!
    FnIOTextFileStructure.FileHandle = CreateDiscFile(FnPathAndFileNamesz,|
                                                      GENERIC_READ,|                      ! Access (Read-Write) Mode
                                                      FILE_SHARE_READ,|                   ! Shared for Reading
                                                      LocalSecurityAttributesStructure,|  ! Security Attributes required
                                                      OPEN_EXISTING,|                     ! Only Open is existing
                                                      FILE_ATTRIBUTE_NORMAL,|             ! Normal Status
                                                      0)                                  ! No 'Template' File'
    IF FnIOTextFileStructure.FileHandle = INVALID_HANDLE_VALUE
       FnIOTextFileStructure.FileHandle = 0
       RETURN(2)                                                   ! Read File Not Found
    END
 END
!
 FnIOTextFileStructure.CoJoinedFileHandle = 0
 FnIOTextFileStructure.PreviousCharactersz = ''
 FnIOTextFileStructure.CurrentCharactersz = ''
 FnIOTextFileStructure.CurrentRecordIndex = 0     ! Arrange to start at the base of the Record
 FnIOTextFileStructure.MaximumTRecordIndex = (SIZE(FnIORecordsz)|
                                              - 1)
 FnIOTextFileStructure.FileSizeProcessedInBytes = 0
 FnIOTextFileStructure.LineNumber = 0
!
 RETURN(0)
! ======================================================================================================================
LoadFile             PROCEDURE (*FnIOTextFileStructure,|
                                *FnIORecordsz,|
                                *FnPerRecordProcessor,|
                                *FnPerCharacterProcessor)
!
! Local (Stack Frame) Data:
!
! (Local Data can be declared because this only called Internally)
!
EndOfFile                              LONG
NumberOfBytesLoaded                    LONG
ProgressPercentage                     LONG
IOBufferMaximumIndex                   LONG
IOBufferCharacterIndex                 LONG
TimerInterval                          EQUATE(1)
Window                                 WINDOW,AT(,,145,16)
                                          PROGRESS,USE(ProgressPercentage),AT(4,4,138,8),RANGE(0,100)
                                       END
 CODE
 IOBufferMaximumIndex = (SIZE(IOBuffersz)|
                         - 1)
!
 EndOfFile = False
!
 OPEN(Window)
 Window{PROP:Timer} = TimerInterval
 ACCEPT
    CASE EVENT()
    OF EVENT:Timer
       Window{PROP:Timer} = 0
       DO TimerFunction
       IF EndOfFile = False
          Window{PROP:Timer} = TimerInterval
       ELSE
          ProgressPercentage = 100            ! ) Show completion
          DISPLAY(?ProgressPercentage)        ! ) even on premature abort
          POST(EVENT:CloseWindow)
       END
    END
 END
!
! The LOOP terminates on End of File (which might be premature, but still the file has been finished with):
!
 IF FnIOTextFileStructure.CurrentRecordIndex > 0
    DO EndOfRecordProcessing                        ! Make sure everything is flushed through
 END
!
 CloseHandle(FnIOTextFileStructure.FileHandle)
 FnIOTextFileStructure.FileHandle = 0
!
 RETURN
! ------------------------------------------------------------------------------------------------------------------------
TimerFunction ROUTINE
!
! Get Next Buffer-full:
!
 IF ((ReadFileNonOverlapped(FnIOTextFileStructure.FileHandle,|         ! ) This will
                            IOBuffersz,|                               ! ) only load
                            IOBufferMaximumIndex,|                     ! ) as much as it can,
                            NumberOfBytesLoaded) = False)|             ! ) up to IOBufferMaximumIndex
  OR (NumberOfBytesLoaded = 0))
    EndOfFile = True
 ELSE
!
! Process the Buffer just loaded:
!
    LOOP IOBufferCharacterIndex = 1 TO NumberOfBytesLoaded BY 1        ! LOOP by the actual number loaded
!
! Note the Previous Character (might be useful):
!
       FnIOTextFileStructure.PreviousCharactersz = FnIOTextFileStructure.CurrentCharactersz
!
! Prepare the Character to work on:
!
       FnIOTextFileStructure.CurrentCharactersz = IOBuffersz[IOBufferCharacterIndex]
!
! Run the "Per Character" CallBack:
!
       EndOfFile = FnPerCharacterProcessor(FnIOTextFileStructure,|
                                           FnIORecordsz)
       IF EndOfFile = False
          IF FnIOTextFileStructure.CurrentCharactersz = CHR(13) ! 0Dh Carriage Return
             CYCLE
!
! Check for "End of Record" and, if so, run the "Per Record" CallBack:
!
          ELSIF FnIOTextFileStructure.CurrentCharactersz = CHR(10) ! 0Ah Line Feed/New Line
             DO EndOfRecordProcessing
!
! Else, if not a special charactreer, just add it in to the Record:
!
          ELSIF FnIOTextFileStructure.CollectAsUPPER = True
             FnIOTextFileStructure.CurrentCharactersz = UPPER(FnIOTextFileStructure.CurrentCharactersz)
             DO AddCurrentCharacterToRecord
          ELSE
             DO AddCurrentCharacterToRecord
          END
       END
!
! Abort the Sub-LOOP if necessary (premature End of File):
!
       IF EndOfFile = True
          BREAK
       END
    END
 END
!
 EXIT
EndOfRecordProcessing ROUTINE
 FnIOTextFileStructure.CurrentCharactersz[1] = CHR(0)  ! Arrange to terminate the CSTRING
 DO AddCurrentCharacterToRecord
!
 FnIOTextFileStructure.CurrentRecordIndex -= 1         ! Is the Number of Characters in the Record
!
 FnIOTextFileStructure.LineNumber += 1                 ! Allocate a Line Number
 EndOfFile = FnPerRecordProcessor(FnIOTextFileStructure,|
                                  FnIORecordsz)
!
! Maintain the Progress %:
!
 FnIOTextFileStructure.FileSizeProcessedInBytes += FnIOTextFileStructure.CurrentRecordIndex
 ProgressPercentage = ((FnIOTextFileStructure.FileSizeProcessedInBytes * 100)|
                       / FnIOTextFileStructure.FileSizeToProcessInBytes)
 DISPLAY(?ProgressPercentage)
!
! Reset the Index for the next Record:
!
 FnIOTextFileStructure.CurrentRecordIndex = 0
!
 EXIT
AddCurrentCharacterToRecord ROUTINE
 IF FnIOTextFileStructure.CurrentRecordIndex < FnIOTextFileStructure.MaximumTRecordIndex
    FnIOTextFileStructure.CurrentRecordIndex += 1
!
! [1] is needed to defeat Clarion considering FnIOTextFileStructure.CurrentCharactersz being an empty CSTRING
!  ... when CHR(0) is the Current Character:
!
    FnIORecordsz[FnIOTextFileStructure.CurrentRecordIndex] = FnIOTextFileStructure.CurrentCharactersz[1]
 ELSE
    EndOfFile = True   ! If FnIORecordsz is too small
 END
!
 EXIT
! ======================================================================================================================
MAPFilePerCharacterReadingProcessor FUNCTION (*FnIOTextFileStructure,|
                                              *FnIORecordsz)
!
! (Local Data *could* be declared because this only called Internally)
!
 CODE
 RETURN(MAPFilePerCharacterHandler(1,|
                                   FnIOTextFileStructure.PreviousCharactersz,|
                                   1,|
                                   FnIOTextFileStructure.CurrentCharactersz))
! ======================================================================================================================
MAPFilePerRecordReadingProcessor FUNCTION (*FnIOTextFileStructure,|
                                           *FnIORecordsz)
!
! (Local Data *could* be declared because this only called Internally)
!
 CODE
 RETURN(MAPFilePerRecordHandler(FnIOTextFileStructure.CurrentRecordIndex,|
                                FnIORecordsz))
! ======================================================================================================================
LongFileDialogue                FUNCTION(*FnDialogueCaptionsz,|
                                         *FnFileNameBuffersz,|
                                         *FnFileFilterssz,|
                                         FnFlags,|
                                         FnhWndOwner)
!
! Local (Stack Frame) Data:
!
! (Local Data can be declared because this only called Internally)
!
FolderSeparatorsz                  ASCIIsz('\')
LocalOpenFileNameStructure         LIKE(OpenFileNameStructure)
LocalPathAndFileNamesz             ASCIIsz(WindowsMaximumPathNameLength|
                                           + WindowsMaximumFileNameLength|
                                           + 1)
LocalPathNamesz                    ASCIIsz(WindowsMaximumPathNameLength|
                                           + 1)
LocalFileNamesz                    ASCIIsz(WindowsMaximumFileNameLength|
                                           + 1)
LocalFileNameFilterssz             ASCIIsz(513)
ReturnCode                         BOOL
 CODE
 IF BAND(FnFlags,|
         FILE_DIALOGUE_DirectorySelection) = FILE_DIALOGUE_DirectorySelection
    ReturnCode = LongPathDialogue(FnDialogueCaptionsz,|
                                  FnFileNameBuffersz,|
                                  ,|
                                  FnFlags,|
                                  FnhWndOwner)
    RETURN(ReturnCode)
 END
!
! Reset The Open File Structure:
!
 CLEAR(LocalOpenFileNameStructure)
 LocalOpenFileNameStructure.StructSize = SIZE(LocalOpenFileNameStructure)
!
 LocalOpenFileNameStructure.hWndOwner = FnhWndOwner
!
! Dialogue Caption:
!
 LocalOpenFileNameStructure.Title = ADDRESS(FnDialogueCaptionsz)
!
! (Path and) File Name Buffer:
!
 IF SliceByTestString(FolderSeparatorsz,|
                      FnFileNameBuffersz,|
                      LocalPathNamesz,|
                      LocalFileNamesz,|
                      (SLICE_RemoveLeading|
                       + SLICE_REmoveTrailing|
                       + SLICE_Reversed)) = S_False
    LocalFileNamesz = '*.*'
    LocalPathNamesz = ''
 END
!
 LocalOpenFileNameStructure.FileTitle = ADDRESS(LocalFileNamesz)
 LocalOpenFileNameStructure.MaxFileTitle = SIZE(LocalFileNamesz)
!
! Initial Directory:
!
 LocalOpenFileNameStructure.InitialDir = ADDRESS(LocalPathNamesz)
!
! Initial File Name:
!
 LocalPathAndFileNamesz = LocalPathNamesz|
                        & LocalFileNamesz
 LocalOpenFileNameStructure.File = ADDRESS(LocalPathAndFileNamesz)
 LocalOpenFileNameStructure.MaxFile = SIZE(LocalPathAndFileNamesz)
!
! File Name Filters:
!
 LocalFileNameFilterssz = FnFileFilterssz
!
 LocalOpenFileNameStructure.FilterIndex = 1
 LOOP Index# = 1 TO LEN(LocalFileNameFilterssz) BY 1
    IF LocalFileNameFilterssz[Index#] = '|'
       LocalFileNameFilterssz[Index#] = CHR(0)
       LocalOpenFileNameStructure.FilterIndex += 1
    END
 END
 LocalFileNameFilterssz[Index#] = CHR(0)
 Index# += 1
 LocalFileNameFilterssz[Index#] = CHR(0)
 LocalOpenFileNameStructure.FilterIndex = (LocalOpenFileNameStructure.FilterIndex|
                                           / 2)
 LocalOpenFileNameStructure.Filter = ADDRESS(LocalFileNameFilterssz)
!
! Flags:
!
 LocalOpenFileNameStructure.Flags = (OFN_LONGNAMES|
                                    + OFN_HIDEREADONLY|
                                    + OFN_DONTADDTORECENT|
                                    + OFN_SHAREAWARE)
 IF FnFlags = 0
    LocalOpenFileNameStructure.Flags = BOR(LocalOpenFileNameStructure.Flags,|
                                           (OFN_FILEMUSTEXIST|
                                            + OFN_PATHMUSTEXIST))
 ELSE
    IF BAND(FnFlags,|
            FILE_DIALOGUE_HoldCurrentPath) = FILE_DIALOGUE_HoldCurrentPath
       LocalOpenFileNameStructure.Flags = BOR(LocalOpenFileNameStructure.Flags,|
                                              OFN_NOCHANGEDIR)
    END
    IF BAND(FnFlags,|
            FILE_DIALOGUE_AllowMultipleSelections) = FILE_DIALOGUE_AllowMultipleSelections
       LocalOpenFileNameStructure.Flags = BOR(LocalOpenFileNameStructure.Flags,|
                                              OFN_ALLOWMULTISELECT)
    END
    IF BAND(FnFlags,|
            FILE_DIALOGUE_DontReportErrors) = FILE_DIALOGUE_DontReportErrors
       LocalOpenFileNameStructure.Flags = BOR(LocalOpenFileNameStructure.Flags,|
                                              (OFN_FILEMUSTEXIST|
                                               + OFN_PATHMUSTEXIST))
    END
    IF BAND(FnFlags,|
            FILE_DIALOGUE_LongFileNames) = FILE_DIALOGUE_LongFileNames
       LocalOpenFileNameStructure.Flags = BOR(LocalOpenFileNameStructure.Flags,|
                                              OFN_LONGNAMES)
    END
 END
!
! Required Operation:
!
 IF BAND(FnFlags,|
         FILE_DIALOGUE_Save) = FILE_DIALOGUE_Save
    ReturnCode = GetSaveFileName(LocalOpenFileNameStructure)
 ELSE
    ReturnCode = GetOpenFileName(LocalOpenFileNameStructure)
 END
!
 IF ReturnCode = True
    FnFileNameBuffersz = LocalPathAndFileNamesz
 END
!
 RETURN(ReturnCode)
! ======================================================================================================================
LongPathName                    FUNCTION(*FnLongPathsz,|
                                         <*FnShortPathsz>,|
                                         FnEnsureTrailingSlash,|
                                         FnReportNoLongConversion)
!
! Local (Stack Frame) Data:
!
! (Local Data can be declared because this only called Internally)
!
ShortPathsz                        ASCIIsz(WindowsMaximumPathNameLength|
                                           + 1)
 CODE
 IF OMITTED(2)
    ShortPathsz = PATH()
 ELSE
    ShortPathsz = FnShortPathsz
 END
!
 IF GetLongPathName(ShortPathsz,|
                    FnLongPathsz,|
                    (SIZE(FnLongPathsz)|
                     - 1)) = 0
    IF FnReportNoLongConversion = True
       MESSAGE('The Long Path of "'|
             & ShortPathsz|
             & '" could not be determined.',|
               'Long Path Conversion failure',|
               ICON:Exclamation)
    END
 END
!
! Provide default for this anyway:
!
 FnLongPathsz = ShortPathsz
!
 Len# = LEN(FnLongPathsz)
 IF ((FnEnsureTrailingSlash = True)|
  AND (Len# > 0)|
  AND (FnLongPathsz[Len#] NOT = '\'))
    FnLongPathsz = FnLongPathsz|
                 & '\'
 END
!
 RETURN
! ======================================================================================================================
LongPathDialogue                FUNCTION(*FnDialogueCaptionsz,|
                                         *FnSelectedPathsz,|
                                         FnEnsureTrailingSlash,|
                                         FnFlags,|
                                         FnhWndOwner)
!
! Local (Stack Frame) Data:
!
! (Local Data can be declared because this only called Internally)
!
LocalBrowseInfoStructure           LIKE(BrowseInfoStructure),PRE(LCLBIF)
AbsolutePIDL                       LONG
 CODE
! LongPathDialogue
!
! Template = LongPathDialogue
!
 LocalBrowseInfoStructure.PIDLRoot = 0                     ! Browses from DeskTop
 LocalBrowseInfoStructure.PathNameBuffersz = ADDRESS(FnSelectedPathsz) ! Only comprises lowest Folder from SHBrowseForFolder
 LocalBrowseInfoStructure.Titlesz = ADDRESS(FnDialogueCaptionsz)  ! Sub-Title for the Dialogue Box
 LocalBrowseInfoStructure.CallBackFunction = 0             ! CallBack Function not used here
!
 LocalBrowseInfoStructure.hWndOwner = FnhWndOwner          ! A handle to the owner window for the dialog box (can be Null/Zero)
 LocalBrowseInfoStructure.Flags = FnFlags                  ! Caller's BIF_ Flags
 LocalBrowseInfoStructure.Param = 0                        ! CallBack Function not used, so no parameter for it
 LocalBrowseInfoStructure.Image = 0                        ! Clear down the (returned) Image Pointer (not interpreted here)
!
 AbsolutePIDL = SHBrowseForFolder(LocalBrowseInfoStructure)
 IF AbsolutePIDL = False
    RETURN(False)
 END
!
! LocalBrowseInfoStructure.PathNameBuffersz only contains the name of lowest level Folder selected at this point.
!
! The Item List needs to be expanded to obtain the Full (Absolute) Path. AbsolutePIDL contains a Memory Address
!   (LONG Pointer) to the base of this Item List, and can be handed over to SHGetPathFromIDList to decode the Item List
!   into an Absolute Path:
!
 IF SHGetPathFromIDList(AbsolutePIDL,|
                        FnSelectedPathsz) = False
    RETURN(False)
 END
!
! Here when SHGetPathFromIDList has expanded the Full (Absolute) Path Name into FnSelectedPathsz:
!
 Len# = LEN(FnSelectedPathsz)
 IF ((FnEnsureTrailingSlash = True)|
  AND (Len# > 0)|
  AND (FnSelectedPathsz[Len#] NOT = '\'))
    FnSelectedPathsz = FnSelectedPathsz|
                     & '\'
 END
!
 RETURN(True)
! ======================================================================================================================
HelpAbout                           PROCEDURE()
!
! Local (Stack Frame) Data:
!
! (Local Data can be declared because this only called Internally)
!
Nullsz                                 ASCIIsz('')
Window WINDOW,AT(,,427,233),FONT('MS Sans Serif',8,,FONT:regular),CENTER,GRAY,NOFRAME
       PANEL,AT(0,0,427,232),BEVEL(6)
       PANEL,AT(8,7,412,112),FILL(COLOR:White),BEVEL(-2,1)
       STRING(@s40),AT(10,11,403,17),USE(PlugInFullIdentitysz),TRN,CENTER,FONT('MS Sans Serif',14,COLOR:Navy,FONT:bold)
       PANEL,AT(25,35,376,9),BEVEL(-1,1,09H)
       STRING(@s40),AT(55,54,283,10),USE(PlugInAuthorsz),TRN,FONT('MS Sans Serif',8,,FONT:bold)
       STRING('For:'),AT(82,64),TRN,FONT('MS Sans Serif',8,,FONT:bold)
       STRING('Anyone who wants Labels instead of numbers, in OllyDbg'),AT(100,64,285,10),TRN,FONT('MS Sans Serif',8,,FONT:bold)
       PANEL,AT(7,117,413,109),USE(?Panel4),FILL(COLOR:White),BEVEL(3,0,01H)
       STRING('Written under Clarion for Windows from'),AT(101,121),TRN,FONT('MS Serif',14,COLOR:Purple,FONT:bold+FONT:italic,CHARSET:ANSI)
       IMAGE('Soft Velocity Angled.jpg'),AT(137,135,167,90),CENTERED
       BUTTON('&Back'),AT(11,49,38,30),USE(?BackButton),TIP('Close this Window')
       BUTTON('&Help'),AT(11,85,38,30),USE(?HelpButton),TIP('What this PlugIn does')
     END
 CODE
 OPEN(Window)
!
 Window{PROP:Text} = ''
!
 ?BackButton{PROP:Icon} = '~ShftLeft.ico'
!
 IF HTMLHelpWindowHandle NOT = 0
    DISABLE(?HelpButton)
    Window{PROP:Timer} = 10
 ELSE
    ENABLE(?HelpButton)
 END
!
!
 ACCEPT
    CASE EVENT()
    OF EVENT:Timer
       IF IsWindow(HTMLHelpWindowHandle) = 0
          HTMLHelpWindowHandle = 0
          ENABLE(?HelpButton)
          Window{PROP:Timer} = 0
       END
!
    OF EVENT:CloseWindow
       IF ((HTMLHelpWindowHandle NOT = 0)|
        AND (IsWindow(HTMLHelpWindowHandle) NOT = 0))
          Nullsz = ''
          HtmlHelp(0,|
                   Nullsz,|
                   HH_CLOSE_ALL,|
                   0)
       END
       HTMLHelpWindowHandle = 0
    END
!
    CASE ACCEPTED()
    OF ?HelpButton
       HTMLHelpWindowHandle = RunHelp(Window{PROP:Handle})
!
    OF ?BackButton
       POST(EVENT:CloseWindow)
    END
 END
!
 CLOSE(Window)
!
!
 RETURN
! ======================================================================================================================
RunHelp                            FUNCTION(FnhWnd)
!
! Local (Stack Frame) Data:
!
! (Local Data *could* be declared because this only called Internally)
!
 CODE
!
 RETURN(HtmlHelp(FnhWnd,|
                 HelpFileNamesz,|
                 HH_DISPLAY_TOPIC,|
                 0))
