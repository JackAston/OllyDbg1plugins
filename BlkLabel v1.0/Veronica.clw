 SECTION('GlobalInclude')
!
! Identifier Definitions for my own Functions:
!
SLICE_CaseSensitive               EQUATE(01h)
SLICE_RemoveLeading               EQUATE(02h)
SLICE_RemoveTrailing              EQUATE(04h)
SLICE_Reversed                    EQUATE(08h)

 SECTION('GlobalMap')
!
! Map Definitions for my own Functions:
!
 MODULE('Veronica')
    SaveRegisters(),|
                  PASCAL,RAW
    RestoreRegisters(),|
                     PASCAL,RAW
    SliceByTestString(*ASCIIsz FnCTestStringsz,|
                      *ASCIIsz FnInspectedStringsz,|
                      *ASCIIsz FnLeadingSliceStringsz,|
                      *ASCIIsz FnTrailingSliceStringsz,|
                      LONG SliceControl = (SLICE_RemoveLeading|
                                           + SLICE_RemoveTrailing)),|
                         LONG,|
                            PASCAL,RAW,PROC
    RemoveLeadingSpaces(*ASCIIsz FnInputStringsz),|
                           LONG,|
                              PASCAL,RAW,PROC
    RemoveTrailingSpaces(*ASCIIsz FnInputStringsz),|
                            LONG,|
                               PASCAL,RAW,PROC
    StringCopy(LONG AddressOfInputStringsz,|
               LONG AddressOfOutputStringsz),|
                  LONG,|
                     PASCAL,RAW,PROC
    StringCopy(LONG AddressOfInputStringsz,|
               *ASCIIsz OutputStringsz),|
                  LONG,|
                     PASCAL,RAW,PROC
    StringCopy(*ASCIIsz InputStringsz,|
               LONG AddressOfOutputStringsz),|
                  LONG,|
                     PASCAL,RAW,PROC
    StringCopy(*ASCIIsz InputStringsz,|
               *ASCIIsz OutputStringsz),|
                  LONG,|
                     PASCAL,RAW,PROC
    StandardisePath(*ASCIIsz PathStringsz),|
                       LONG,|
                          PASCAL,RAW,PROC
    ValueForByte(*ASCIIsz StringCharactersz),|
                    LONG,|
                       PASCAL,RAW
    ValueForShort(*ASCIIsz StringCharacterMSBsz,|
                  *ASCIIsz StringCharacterLSBsz),|
                     LONG,|
                        PASCAL,RAW
    ValueForLong(*ASCIIsz StringCharacterMSBsz,|
                 *ASCIIsz StringCharacterNSB1sz,|
                 *ASCIIsz StringCharacterNSB2sz,|
                 *ASCIIsz StringCharacterLSBsz),|
                    LONG,|
                       PASCAL,RAW
 END
