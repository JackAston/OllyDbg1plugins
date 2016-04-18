! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
! + The Function of this Sub-PlugIn is to support the Bulk Labelling Insertion, into OllyDbg, of Labels and Addresses +
! +   from a Clarion MAP File.                                                                                        +
! +                                                                                                                   +
! + It has been written this way in order to provide Clarion MAP File support for the main PlugIn, BlkLabel.          +
! +                                                                                                                   +
! + Because this is a Sub-PlugIn, it would be very easy to substitute the logic for any other format of a MAP File.   +
! +                                                                                                                   +
! + All that is necessary is to write this Sub-Plugin for a specific Programming Language, and throw this "Clarion    +
! +   version" away. (Just use the same name, SubLabel.dll, with two Exports:                                         +
! +                                                                                                                   +
! +   MAPFilePerCharacterHandler (to the specification below), and                                                    +
! +   MAPFilePerRecordHandler (also to the specification below)                                                       +
! +                                                                                                                   +
! + SubLabel.dll needs to reside in the OllyDbg Folder but - since it does not Export anything the OllyDbg (itself)   +
! +   will recognise, it won't be picked up as a PlugIn ... in its own right.                                         +
! +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
DWORD                                 EQUATE(LONG)
MAX_PATH                              EQUATE(260)
!
! (2) 'Windows in general':
!
WindowsMaximumPathNameLength          EQUATE(MAX_PATH)
WindowsMaximumFileNameLength          EQUATE(256)
!
S_OK                                  EQUATE(0)
S_False                               EQUATE(-1)
DLL_PROCESS_ATTACH                    EQUATE(1)
!
! (3) OllyDebug 'specials':
!
NM_LABEL                              EQUATE(031h)            ! User-defined Label
!
! (4) My own Functions:
!
    INCLUDE('Veronica.clw','GlobalInclude')
!
! Global Map:
!
 MAP
!
! (1) Internals:
!
    DllEntryPoint(hInstance,|
                  DWORD Reason,|
                  LONG AddressOfReserved),|
                     LONG,|
                        PASCAL
    ConvertHexStringszToDecimal(*ASCIIsz HexStringsz),|
                                   LONG
!
! (2) Exports:
!
    MAPFilePerCharacterHandler(*ASCIIsz PreviousCharactersz,|
                               *ASCIIsz ThisCharactersz),|
                                  LONG,|
                                     PASCAL
    MAPFilePerRecordHandler(*ASCIIsz Recordsz),|
                               LONG,|
                                  PASCAL
!
! (3) Imports: Veronica's Function Library:
!
    INCLUDE('Veronica.clw','GlobalMap')
!
! (4) Imports: OllyDbg itself:
!
    MODULE('OllyDebug')
       InsertName(LONG AddressValue,|
                  LONG TypeCode,|
                  *ASCIIsz Labelsz),|
                     LONG,|
                        C,RAW,PROC,NAME('_Insertname')
    END
 END
!
! Global Data:
!
OurhInstance               hInstance,STATIC
!
! Main Entry Point:
!
 CODE
 RETURN
! ======================================================================================================================
DllEntryPoint              FUNCTION(FnhInstance,|
                                    FnReason,|
                                    FnAddressOfReserved)
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
MAPFilePerCharacterHandler          FUNCTION (FnPreviousCharactersz,|
                                              FnThisCharactersz)
 CODE
 SaveRegisters()
!
 IF FnThisCharactersz = CHR(9)     ! Tab Character
    FnThisCharactersz = ' '        ! Just change to White Space
 END
!
 RestoreRegisters()
 RETURN(False)                     ! Continue processing (NOT End of File)
! ======================================================================================================================
MAPFilePerRecordHandler             FUNCTION(FnRecordsz)
!
! Local (Stack Frame) Data:
!
MemoryAddress              LONG
Labelsz                    ASCIIsz(255)
MAPInfosz                  ASCIIsz(255)
 CODE
 SaveRegisters()
!
 IF FnRecordsz NOT = ''
    IF FnRecordsz[10] NOT = ' '
       Labelsz = UPPER(FnRecordsz[10 : LEN(FnRecordsz)])
    ELSE
       Labelsz = UPPER(FnRecordsz[39 : LEN(FnRecordsz)])
    END
!
    RemoveLeadingSpaces(Labelsz)
    RemoveTrailingSpaces(Labelsz)
!
    IF Labelsz NOT = ''
       MAPInfosz = FnRecordsz[1 : 8]
       RemoveLeadingSpaces(MAPInfosz)
       RemoveTrailingSpaces(MAPInfosz)
!
       IF MAPInfosz NOT = ''
          MemoryAddress = ConvertHexStringszToDecimal(MAPInfosz)
!
          IF MemoryAddress NOT = 0
             InsertName(MemoryAddress,|
                        NM_LABEL,|
                        Labelsz)
          END
       END
    END
 END
!
 RestoreRegisters()
 RETURN(False)                     ! Continue processing (NOT End of File)
! ======================================================================================================================
ConvertHexStringszToDecimal         FUNCTION (*FnHexStringsz)
!
! Local (Stack Frame) Data:
!
LONGAddress                LONG
CurrentDigit               LONG
 CODE
 LONGAddress = 0
!
 LOOP Index# = 1 TO LEN(FnHexStringsz) BY 1
    CurrentDigit = VAL(FnHexStringsz[Index#])
    CurrentDigit -= VAL('0')
    IF CurrentDigit > 9
       CurrentDigit -= 7                  ! The offset to "A" (good job this is all done in Uppercase!)
    END
    LONGAddress = (BSHIFT(LONGAddress,|
                         + 4)|
                   + CurrentDigit)
 END
!
 RETURN(LONGAddress)
! ======================================================================================================================
