; Translated for MASM32 by Eviloid, 2oo3
; eviloid@mail.ru
; Modified by AvAtAr, 2005

; version: 1.08.03
; Wed 17 Aug 23:07:34 2005

; not tested!

comment ^
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             OLLYDBG PLUGIN API                             //
//                                                                            //
//                                Version 1.08                                //
//                                                                            //
//               Written by Oleh Yuschuk (ollydbg@t-online.de)                //
//                                                                            //
//              Internet:  http://home.t-online.de/home/Ollydbg               //
//                        Email:  ollydbg@t-online.de                         //
//                                                                            //
// This code is distributed "as is", without warranty of any kind, expressed  //
// or implied, including, but not limited to warranty of fitness for any      //
// particular purpose. In no event will Oleh Yuschuk be liable to you for any //
// special, incidental, indirect, consequential or any other damages caused   //
// by the use, misuse, or the inability to use of this code, including any    //
// lost profits or lost savings, even if Oleh Yuschuk has been advised of the //
// possibility of such damages.                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//////////////////////////// IMPORTANT INFORMATION /////////////////////////////

// 1. Export all callback functions by name, NOT by ordinal!
// 2. Force byte alignment of OllyDbg structures!
// 3. Set default char type to unsigned!
// 4. Read documentation!
^

; And here I check that settings are correct:

t_aligntest struc 1
        a       byte        ?
        b       dword       ?
t_aligntest ends

if sizeof t_aligntest NE (sizeof byte + sizeof dword)
.err <Please assure byte alignment of OllyDbg structures>
endif

;////////////////////////////////////////////////////////////////////////////////
;///////////////////////////// GENERAL DECLARATIONS /////////////////////////////

PLUGIN_VERSION      equ     108             ; Version of plugin interface

TEXTLEN             equ     256             ; Maximal length of text string
ARGLEN              equ     1024            ; Maximal length of argument string
USERLEN             equ     4096            ; Maximal length of record in .udd file
SHORTLEN            equ     8               ; Maximal length of short name

BLACK               equ     0               ; Indices of colours used by OllyDbg. In
BLUE                equ     1               ; syntax highlighting, use only colours 
GREEN               equ     2               ; 0 to 15 in the least significant bits 
CYAN                equ     3               ; of the corresponding mask byte.       
RED                 equ     4
MAGENTA             equ     5
BROWN               equ     6
LIGHTGRAY           equ     7
DARKGRAY            equ     8
LIGHTBLUE           equ     9
LIGHTGREEN          equ     10
LIGHTCYAN           equ     11
LIGHTRED            equ     12
LIGHTMAGENTA        equ     13
YELLOW              equ     14
WHITE               equ     15
MINT                equ     16
SKYBLUE             equ     17
IVORY               equ     18
GRAY                equ     19

NCOLORS             equ     20              ; Total number of defined colours

BKTRANSP            equ     00h             ; Background colours in syntax hiliting
BKBLACK             equ     10h
BKGRAY              equ     20h
BKWHITE             equ     30h
BKCYAN              equ     40h
BKGREEN             equ     50h
BKRED               equ     60h
BKYELLOW            equ     70h


BLACKWHITE          equ     0               ; Colour schemes used by OllyDbg
BLUEGOLD            equ     1
SKYWIND             equ     2
NIGHTSTARS          equ     3
SCHEME4             equ     4
SCHEME5             equ     5
SCHEME6             equ     6
SCHEME7             equ     7

FIXEDFONT           equ     0               ; Fonts used by OllyDbg. Variable-pitch
TERMINAL6           equ     1               ; fonts are placed at the end of this
FIXEDSYS            equ     2               ; table.
COURIERFONT         equ     3
LUCIDACONS          equ     4
FONT5               equ     5
FONT6               equ     6
FONT7               equ     7
MAINFONT            equ     8
SYSFONT             equ     9
INFOFONT            equ     10

;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////// INFORMATION FUNCTIONS /////////////////////////////

Addtolist           proto C addr_:dword, highlight:dword, format: ptr byte, :VARARG
Updatelist          proto C
Createlistwindow    proto C
Error               proto C format:ptr byte, :VARARG
Message             proto C addr_:dword, format:ptr byte, :VARARG
Infoline            proto C format:ptr byte, :VARARG
Progress            proto C promille:dword, format:ptr byte, :VARARG
Flash               proto C format:ptr byte, :VARARG


;////////////////////////////////////////////////////////////////////////////////
;////////////////////////// DATA FORMATTING FUNCTIONS ///////////////////////////

;// Bits used in Decodeaddress(), Decoderelativeoffset() and  Decodethreadname()
;// to specify decoding mode:
ADC_DEFAULT         equ     0000h       ; Default decoding mode
ADC_DIFFMOD         equ     0001h       ; Show module only if different
ADC_NOMODNAME       equ     0002h       ; Never show module name
ADC_VALID           equ     0004h       ; Only decode if allocated memory
ADC_INMODULE        equ     0008h       ; Only decode if in some module
ADC_SAMEMOD         equ     0010h       ; Decode only address in same module
ADC_SYMBOL          equ     0020h       ; Only decode if symbolic name
ADC_JUMP            equ     0040h       ; Check if points to JMP/CALL command
ADC_OFFSET          equ     0080h       ; Check if symbol for data
ADC_STRING          equ     0100h       ; Check if pointer to ASCII or UNICODE
ADC_ENTRY           equ     0200h       ; Check if entry to subroutine
ADC_UPPERCASE       equ     0400h       ; First letter in uppercase if possible
ADC_WIDEFORM        equ     0800h       ; Extended form of decoded name
ADC_NONTRIVIAL      equ     1000h       ; Name + non-zero offset
ADC_DYNAMIC         equ     2000h       ; JMP/CALL to dynamically loaded name

PLAINASCII          equ     01h         ; Plain ASCII character
DIACRITICAL         equ     02h         ; Diacritical character
RAREASCII           equ     10h         ; Rare ASCII character

Decodeaddress proto C addr_:dword, base:dword, addrmode:dword, symb:ptr byte, nsymb:dword, comment_:ptr byte;
Decoderelativeoffset proto C addr_:dword, addrmode:dword, symb:ptr byte, nsymb:dword
Decodecharacter proto C s:ptr byte, c_:dword
Printfloat4  proto C s:ptr byte, f:real4

Printfloat8 proto C s:ptr byte, double_1:dword, double_2:dword
Printfloat10 proto C s:ptr byte, double_1:dword, double_2:dword
Print3dnow proto s:ptr byte, f:ptr byte
Printsse proto s:ptr byte, f:ptr byte
Followcall proto addr_:dword
IstextA proto C c_:byte
IstextW proto C w:word
Stringtotext proto C data:ptr byte, ndata:dword, text:ptr byte, ntext:dword

;////////////////////////////////////////////////////////////////////////////////
;///////////////////////////// DATA INPUT FUNCTIONS /////////////////////////////

MAXCMDSIZE          equ     16          ; Maximal length of 80x86 command
NSEQ                equ     8           ; Max length of command sequence
NMODELS             equ     8           ; Number of assembler search models

;// Note that each of dialog functions decodes only subset of listed flags.
DIA_ASKGLOBAL       equ     0001h       ; Display checkbox "Global search"
DIA_HEXONLY         equ     0002h       ; Hexadecimal format only
DIA_ALIGNED         equ     0004h       ; Display checkbox "Aligned search"
DIA_DEFHEX          equ     0000h       ; On startup, cursor in hex control
DIA_DEFASCII        equ     0010h       ; On startup, cursor in ASCII control
DIA_DEFUNICODE      equ     0020h       ; On startup, cursor in UNICODE control
DIA_SEARCH          equ     0040h       ; Is a search dialog
DIA_HISTORY         equ     0100h       ; Allows previous hex strings

;// Status of animation or trace.
ANIMATE_OFF         equ     0           ; No animation
ANIMATE_IN          equ     1           ; Animate into
ANIMATE_OVER        equ     2           ; Animate over
ANIMATE_RET         equ     3           ; Execute till RET
ANIMATE_SKPRET      equ     4           ; Skip RET instruction
ANIMATE_USER        equ     5           ; Execute till user code
ANIMATE_TRIN        equ     6           ; Run trace in
ANIMATE_TROVER      equ     7           ; Run trace over
ANIMATE_STOP        equ     8           ; Gracefully stop animation

t_hexstr struc 1    ; String used for hex/text search
        n           dword       ?                   ; String length
        data        byte        TEXTLEN dup (?)     ; Data
        mask_       byte        TEXTLEN dup (?)     ; Mask, 0 bits are masked
t_hexstr ends

t_asmmodel struc 1  ; Model to search for assembler command
        code_       byte        MAXCMDSIZE dup (?)  ; Binary code                          
        mask_       byte        MAXCMDSIZE dup (?)  ; Mask for binary code (0: bit ignored)t_asmmodel ENDS
        length_     dword       ?                   ; Length of code, bytes (0: empty)     
        jmpsize     dword       ?                   ; Offset size if relative jump         
        jmpoffset   dword       ?                   ; Offset relative to IP                
        jmppos      dword       ?                   ; Position of jump offset in command   
t_asmmodel ends

t_extmodel struc 1  ; Model for extended command search
        code_       byte        MAXCMDSIZE dup (?)  ; Binary code                          
        mask_       byte        MAXCMDSIZE dup (?)  ; Mask for binary code (0: bit ignored)
        length_     dword       ?                   ; Length of code, bytes (0: empty)     
        jmpsize     dword       ?                   ; Offset size if relative jump         
        jmpoffset   dword       ?                   ; Offset relative to IP                
        jmppos      dword       ?                   ; Position of jump offset in command   
        isany       dword       ?                   ; Count for ANY's argument             
        cmdoffset   dword       ?                   ; Offset of command in the source      
        ramask      byte        MAXCMDSIZE dup (?)  ; Mask for pseudoregister RA           
        rbmask      byte        MAXCMDSIZE dup (?)  ; Mask for pseudoregister RB           
t_extmodel ends


Getlong             proto C title_:ptr byte, data:dword, datasize:dword, letter:byte, mode:dword
Getlongxy           proto C title_:ptr byte, data:dword, datasize:dword, letter:byte, mode:dword, x:dword, y:dword
Getregxy            proto C title_:ptr byte, data:dword, letter:byte, x:dword, y:dword
Getline             proto C title_:ptr byte, data:dword
Getlinexy           proto C title_:ptr byte, data:dword, x:dword, y:dword
Getfloat10          proto C title_:ptr byte, fdata:ptr real10, tag:ptr byte, letter:byte, mode:dword
Getfloat10xy        proto C title_:ptr byte, fdata:ptr real10, tag:ptr byte, letter:byte, mode:dword, x:dword, y:dword
Getfloat            proto C title_:ptr byte, fdata:dword, size_:dword, letter:byte, mode:dword
Getfloatxy          proto C title_:ptr byte, fdata:dword, size_:dword, letter:byte, mode:dword, x:dword, y:dword
Getasmfindmodel     proto C model_:ptr t_asmmodel, letter:byte, searchall:dword
Getasmfindmodelxy   proto C model_:ptr t_asmmodel, letter:byte, searchall:dword, x:dword, y:dword
Gettext             proto C title_:ptr byte, text:ptr byte, letter:byte, type_:dword, fontindex:dword
Gettextxy           proto C title_:ptr byte, text:ptr byte, letter:byte, type_:dword, fontindex:dword, x:dword, y:dword
Gethexstring        proto C title_:ptr byte, hs:ptr t_hexstr, mode:dword, fontindex:dword, letter:byte
Gethexstringxy      proto C title_:ptr byte, hs:ptr t_hexstr, mode:dword, fontindex:dword, letter:byte, x:dword, y:dword
Getmmx              proto C title_:ptr byte, data:byte, mode:dword
Getmmxxy            proto C title_:ptr byte, data:byte, mode:dword, x:dword, y:dword
Get3dnow            proto C title_:ptr byte, data:byte, mode:dword
Get3dnowxy          proto C title_:ptr byte, data:byte, mode:dword, x:dword, y:dword
Browsefilename      proto C title_:ptr byte, name_:ptr byte, defext:ptr byte, getarguments:dword
OpenEXEfile         proto C path:ptr byte, dropped:dword
Animate             proto C animation:dword

;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////// SORTED DATA FUNCTIONS /////////////////////////////

NBAR                equ     17          ; Max allowed number of segments in bar

BAR_PRESSED         equ     0001h       ; Bar segment pressed, used internally
BAR_DISABLED        equ     0002h       ; Bar segment disabled
BAR_NOSORT          equ     0004h       ; Flat bar column, supports no sorting
BAR_NORESIZE        equ     0008h       ; Bar column cannot be resized
BAR_BUTTON          equ     0010h       ; Segment sends WM_USER_BAR
BAR_SHIFTSEL        equ     0020h       ; Selection shifted 1/2 char to left

CAPT_FREE           equ     0           ; Bar and data are not captured

TABLE_DIR           equ     00001h      ; Bottom-to-top table
TABLE_COPYMENU      equ     00002h      ; Attach copy item
TABLE_SORTMENU      equ     00004h      ; Attach sort menu
TABLE_APPMENU       equ     00010h      ; Attach appearance menu
TABLE_WIDECOL       equ     00020h      ; Attach wide columns menu item
TABLE_USERAPP       equ     00040h      ; Attach user-processed appearance menu
TABLE_USERDEF       equ     00080h      ; User-drawn table
TABLE_NOHSCR        equ     00100h      ; Table contains no horizontal scroll
TABLE_SAVEPOS       equ     00200h      ; Save position & appearance to .ini
TABLE_CPU           equ     00400h      ; Table belongs to CPU window
TABLE_FASTSEL       equ     00800h      ; Update when selection changes
TABLE_COLSEL        equ     01000h      ; Column-wide selection
TABLE_SAVEAPP       equ     02000h      ; Save multiinstance appearance to .ini
TABLE_HILMENU       equ     04000h      ; Attach Syntax highlighting menu

DRAW_NORMAL         equ     00000h      ; Normal plain text
DRAW_GRAY           equ     00001h      ; Grayed text
DRAW_HILITE         equ     00002h      ; Highlighted text
DRAW_UL             equ     00004h      ; Underlined text
DRAW_SELECT         equ     00008h      ; Selected background
DRAW_EIP            equ     00010h      ; Inverted normal text/background
DRAW_BREAK          equ     00020h      ; Breakpoint background
DRAW_GRAPH          equ     00040h      ; Graphical element
DRAW_MASK           equ     00080h      ; Use mask to set individual colors
DRAW_DIRECT         equ     00080h      ; Direct text colour index (mask only)
DRAW_EXTSEL         equ     00100h      ; Extend mask till end of column
DRAW_UNICODE        equ     00200h      ; Text in UNICODE
DRAW_TOP            equ     00400h      ; Draw upper half of text
DRAW_BOTTOM         equ     00800h      ; Draw lower half of text

; Symbolic names for graphical characters. Any other character is displayed
; as space. Use only characters in range [1..0x7F]!
D_SPACE             equ     'N'         ;  Space
D_SEP               equ     ' '         ;  Thin separating line
D_BEGIN             equ     'B'         ;  Begin of procedure or scope
D_BODY              equ     'I'         ;  Body of procedure or scope
D_ENTRY             equ     'J'         ;  Loop entry point
D_LEAF              equ     'K'         ;  Intermediate leaf on a tree
D_END               equ     'E'         ;  End of procedure or scope
D_SINGLE            equ     'S'         ;  Single-line scope
D_ENDBEG            equ     'T'         ;  End and begin of stack scope
D_POINT             equ     '.'         ;  Point
D_JMPUP             equ     'U'         ;  Jump upstairs
D_JMPOUT            equ     '<'         ;  Jump to same location or out of module
D_JMPDN             equ     'D'         ;  Jump downstairs
D_PATHUP            equ     'u'         ;  Jump path upstairs (highlighted)
D_GRAYUP            equ     'v'         ;  Jump path upstairs (grayed)
D_PATH              equ     'i'         ;  Jump path through text (highlighted)
D_GRAYPATH          equ     'j'         ;  Jump path through text (grayed)
D_PATHDN            equ     'd'         ;  Jump path downstairs (highlighted)
D_GRAYDN            equ     'e'         ;  Jump path downstairs (grayed)
D_PATHUPEND         equ     'r'         ;  End of path upstairs (highlighted)
D_GRAYUPEND         equ     's'         ;  End of path upstairs (grayed)
D_PATHDNEND         equ     'f'         ;  End of path downstairs (highlighted)
D_GRAYDNEND         equ     'g'         ;  End of path downstairs (grayed)
D_SWTOP             equ     't'         ;  Start of switch
D_SWBODY            equ     'b'         ;  Switch body
D_CASE              equ     'c'         ;  Intermediate switch case
D_LASTCASE          equ     'l'         ;  Last switch case


;// Please note: Although types here contain mostly unique bit assignments, it's
;// not really necessary. Same bits, except for reserved general types, can be
;// freely shared between different types of sorted data.
;// General item types:
TY_NEW              equ     000000001h  ; Item is new
TY_CONFIRMED        equ     000000002h  ; Item still exists
TY_MAIN             equ     000000004h  ; Main item (thread or module)
TY_INVALID          equ     000000008h  ; Invalid type (item does not exist)
TY_SELECTED         equ     080000000h  ; Reserved for multiple selection
;// Module-specific types:
TY_REPORTED         equ     000000010h  ; Stop on module was reported
;// Reference-specific types:
TY_REFERENCE        equ     000000020h  ; Item is a real reference
TY_ORIGIN           equ     000000040h  ; Item is a search origin
;// Breakpoint-specific types:
TY_STOPAN           equ     000000080h  ; Stop animation if TY_ONESHOT
TY_SET              equ     000000100h  ; Code INT3 is in memory
TY_ACTIVE           equ     000000200h  ; Permanent breakpoint
TY_DISABLED         equ     000000400h  ; Permanent disabled breakpoint
TY_ONESHOT          equ     000000800h  ; Temporary stop
TY_TEMP             equ     000001000h  ; Temporary breakpoint
TY_KEEPCODE         equ     000002000h  ; Set and keep command code
TY_KEEPCOND         equ     000004000h  ; Keep condition unchanged (0: remove)
TY_NOUPDATE         equ     000008000h  ; Don't redraw breakpoint window
TY_RTRACE           equ     000010000h  ; Pseudotype of run trace breakpoint
;// Namelist-specific types:
TY_EXPORT           equ     000010000h  ; Exported name
TY_IMPORT           equ     000020000h  ; Imported name
TY_LIBRARY          equ     000040000h  ; Name extracted from object file
TY_LABEL            equ     000080000h  ; User-defined name
TY_ANYNAME          equ     0000F0000h  ; Any of the namelist flags above
TY_KNOWN            equ     000100000h  ; Name of known function

;// Memory-specific types:
TY_DEFHEAP          equ     000020000h  ; Contains default heap
TY_HEAP             equ     000040000h  ; Contains non-default heap
TY_SFX              equ     000080000h  ; Contains self-extractor
TY_CODE             equ     000100000h  ; Contains image of code section
TY_DATA             equ     000200000h  ; Contains image of data section
TY_IMPDATA          equ     000400000h  ; Memory block includes import data
TY_EXPDATA          equ     000800000h  ; Memory block includes export data
TY_RSRC             equ     001000000h  ; Memory block includes resources
TY_RELOC            equ     002000000h  ; Memory block includes relocation data
TY_STACK            equ     004000000h  ; Contains stack of some thread
TY_THREAD           equ     008000000h  ; Contains data block of some thread
TY_HEADER           equ     010000000h  ; COFF header
;TY_ANYMEM           equ     01FF80000h  ; Any of the memory flags above
TY_ANYMEM           equ     01FFE0000h  ; Any of the memory flags above
TY_GUARDED          equ     020000000h  ; NT only: guarded memory block
;// Procedure data-specific types:
TY_PURE             equ     000004000h  ; No side effects except in stack
TY_PASCAL           equ     000010000h  ; Procedure ends with RET nnn
TY_C                equ     000020000h  ; ADD ESP,nnn after call to procedure
TY_NOTENTRY         equ     000100000h  ; Not necessarily entry point

; Switch data-specific types.
TY_CHARSW           equ     000100000h  ; ASCII switch
TY_WMSW             equ     000200000h  ; Window message switch
TY_EXCEPTSW         equ     000400000h  ; Exception switch
; Stack walk data-specific types.
TY_RELIABLE         equ     001000000h  ; Reliable call
TY_GUESSED          equ     002000000h  ; Not a real entry, just guessed
TY_BELONGS          equ     004000000h  ; Not a real entry, just belongs to proc
; Call tree-specific types.
TY_RECURSIVE        equ     000000100h  ; Routine calls self
TY_TERMINAL         equ     000000200h  ; Leaf function, doesn't call others
TY_SYSTEM           equ     000000400h  ; Function resides in system DLL
TY_DIRECT           equ     000000800h  ; Called directly
TY_NODATA           equ     000001000h  ; Not analyzed or outside procedure
TY_DUMMY            equ     000002000h  ; Consists of single RET command
TY_NOSIDE           equ     000004000h  ; No side effects except in stack



t_scheme struc 1 ; Color scheme
        name_       dword   ?   ; Name of the scheme
        textcolor   dword   ?   ; Colour used to draw text
        hitextcolor dword   ?   ; Colour used to draw highlited text
        lowcolor    dword   ?   ; Colour used to draw auxiliary text
        bkcolor     dword   ?   ; Colour used to draw backgrounds
        selbkcolor  dword   ?   ; Colour used for selecting background
        linecolor   dword   ?   ; Colour used for separating lines
        auxcolor    dword   ?   ; Colour used for auxiliary objects
        condbkcolor dword   ?   ; Colour used for background of cond brk
t_scheme ends

t_schemeopt struc 1 ; Color scheme, alternative form
        name_       dword   ?               ; Name of the scheme
        color       dd      8 dup (?)       ; Colours used as in t_scheme
t_schemeopt ends

t_bar struc 1
        nbar        dword   ?               ; Number of active columns         
        font        dword   ?               ; Font used for bar segments       
        dx_         dd      NBAR dup (?)    ; Actual widths of columns, pixels 
        defdx       dd      NBAR dup (?)    ; Default widths of columns, chars 
        name_       dd      NBAR dup (?)    ; Column names (may be NULL)       
        mode        db      NBAR dup (?)    ; Combination of BAR_xxx bits      
        captured    dword   ?               ; One of CAPT_xxx, set to CAPT_FREE
        active      dword   ?               ; Info about how mouse is captured 
        prevx       dword   ?               ; Previous mouse coordinate        
t_bar ends
; sizeof t_bar = 241

t_sortheader struc 1    ; Header of sorted data field
        addr_       dword   ?               ; Base address of the entry
        size_       dword   ?               ; Size address of the entry
        type_       dword   ?               ; Entry type, TY_xxx       
t_sortheader ends

SORTFUNC typedef proto :ptr t_sortheader, :ptr t_sortheader, :dword
PSORTFUNC typedef ptr SORTFUNC
DRAWFUNC typedef proto :ptr byte, :ptr byte, :ptr dword, :ptr t_sortheader, :dword
PDRAWFUNC typedef ptr DRAWFUNC
DESTFUNC typedef proto :ptr t_sortheader
PDESTFUNC typedef ptr DESTFUNC

AUTOARRANGE     equ     1   ; Autoarrangeable sorted data

t_sorted struc 1    ; Descriptor of sorted table
        name_       db      MAX_PATH dup (?) ; Name of table, as appears in error
        n           dword   ?       ; Actual number of entries
        nmax        dword   ?       ; Maximal number of entries
        selected    dword   ?       ; Index of selected entry or -1
        seladdr     dword   ?       ; Base address of selected entry
        itemsize    dword   ?       ; Size of single entry
        version_    dword   ?       ; Unique version of table
        data        dword   ?       ; Entries, sorted by address
        sortfunc    PSORTFUNC   ?   ; Function which sorts data or NULL
        destfunc    PDESTFUNC   ?   ; Destructor function or NULL
        sort        dword   ?       ; Sorting criterium (column)
        sorted      dword   ?       ; Whether indexes are sorted
        index       dword   ?       ; Indexes, sorted by criterium
        suppresserr dword   ?       ; Suppress multiple overflow errors

t_sorted ends
; sizeof t_sorted = 312

t_table struc 1 ; Window with sorted data and bar
        hw          HWND    ?       ; Handle of window or NULL
        data        t_sorted    <>  ; Sorted data
        bar         t_bar       <>  ; Description of bar                   
        showbar     dword       ?   ; Bar: 1-displayed, 0-hidden, -1-absent
        hscroll     word        ?   ; Horiz. scroll: 1-displayed, 0-hidden 
        colsel      word        ?   ; Column in TABLE_COLSEL window
        mode        dword       ?   ; Combination of bits TABLE_xxx        
        font        dword       ?   ; Font used by window                  
        scheme      word        ?   ; Colour scheme used by window         
        hilite      word        ?   ; Syntax highlighting used by window

        offset_     dword       ?   ; First displayed row                  
        xshift      dword       ?   ; Shift in X direction, pixels         
        drawfunc    PDRAWFUNC   ?   ; Function which decodes table fields  
t_table ends
;sizeof t_table = 589

;!!!  DRAWFUNC or ptr DRAWFUNC !!!


Createsorteddata proto C sd:ptr t_sorted, name_:ptr byte, itemsize:dword, nmax:dword, sortfunc:PSORTFUNC, destfunc:PDESTFUNC
Destroysorteddata proto C sd:ptr t_sorted
Addsorteddata proto C sd:ptr t_sorted, item:dword
Deletesorteddata proto C sd:ptr t_sorted, addr_:dword
Deletesorteddatarange proto C sd:ptr t_sorted, addr0:dword, addr1:dword
Deletenonconfirmedsorteddata proto C sd:ptr t_sorted
Findsorteddata proto C sd:ptr t_sorted, addr_:dword
Findsorteddatarange proto C sd:ptr t_sorted, addr0:dword, addr1:dword
Findsorteddataindex proto C sd:ptr t_sorted, addr0:dword, addr1:dword
Sortsorteddata proto C sd:ptr t_sorted, sort:dword
Getsortedbyselection proto C sd:ptr t_sorted, index:dword
Defaultbar proto C pb:ptr t_bar
Tablefunction proto C pt:ptr t_table, hw:HWND, msg:UINT, wp:WPARAM, lp:LPARAM
Painttable proto C hw:HWND, pt:ptr t_table, getline:DRAWFUNC
Gettableselectionxy proto C pt:ptr t_table, column:dword, px:ptr dword, py:ptr dword
Selectandscroll proto C pt:ptr t_table, index:dword, mode:dword


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////// NAME FUNCTIONS ////////////////////////////////

;// Types of names used in name functions. Note that higher-priority types have
;// smaller identifiers!

NM_NONAME           equ     000h        ; Undefined name
NM_ANYNAME          equ     0FFh        ; Name of any type
;// Names saved in the data file of module they appear.
NM_LABEL            equ     031h        ; User-defined label
NM_EXPORT           equ     032h        ; Exported (global) name
NM_IMPORT           equ     033h        ; Imported name
NM_LIBRARY          equ     034h        ; Name from library or object file
NM_CONST            equ     035h        ; User-defined constant
NM_COMMENT          equ     036h        ; User-defined comment
NM_LIBCOMM          equ     037h        ; Comment from library or object file
NM_BREAK            equ     038h        ; Condition related with breakpoint
NM_ARG              equ     039h        ; Arguments decoded by analyzer
NM_ANALYSE          equ     03Ah        ; Comment added by analyzer
NM_BREAKEXPR        equ     03Bh        ; Expression related with breakpoint
NM_BREAKEXPL        equ     03Ch        ; Explanation related with breakpoint
NM_ASSUME           equ     03Dh        ; Assume function with known arguments
NM_STRUCT           equ     03Eh        ; Code structure decoded by analyzer
NM_CASE             equ     03Fh        ; Case description decoded by analyzer
;// Names saved in the data file of main module.
NM_INSPECT          equ     040h        ; Several last inspect expressions
NM_WATCH            equ     041h        ; Watch expressions
NM_ASM              equ     042h        ; Several last assembled strings
NM_FINDASM          equ     043h        ; Several last find assembler strings
NM_LASTWATCH        equ     048h        ; Several last watch expressions
NM_SOURCE           equ     049h        ; Several last source search strings
NM_REFTXT           equ     04Ah        ; Several last ref text search strings
NM_GOTO             equ     04Bh        ; Several last expressions to follow
NM_GOTODUMP         equ     04Ch        ; Several expressions to follow in Dump
NM_TRPAUSE          equ     04Dh        ; Several expressions to pause trace
; Pseudonames.
NM_IMCALL           equ     0FEh        ; Intermodular call

NMHISTORY           equ     040h        ; Converts NM_xxx to type of init list


Insertname proto C addr_:dword, type_:dword, name_:ptr byte
Quickinsertname proto C addr_:dword, type_:dword, name_:ptr byte
Mergequicknames proto C
Discardquicknames proto C
Findname proto C addr_:dword, type_:dword, name_:ptr byte
Decodename proto C addr_:dword, type_:dword, name_:ptr byte
Findnextname proto C name_:ptr byte
Findlabel proto C addr_:dword, name_:ptr byte
Deletenamerange proto C addr0:dword, addr1:dword, type_:dword
Findlabelbyname proto C name_:ptr byte, addr_:ptr dword, addr0:dword, addr1:dword
Findimportbyname proto C name_:ptr byte, addr0:dword, addr1:dword
Demanglename proto C name_:ptr byte, type_:dword, undecorated:ptr byte
Findsymbolicname proto C addr_:dword, fname:ptr byte

;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////// DISASSEMBLY FUNCTIONS /////////////////////////////
                    
REG_EAX             equ     0       ; Indexes of general-purpose registers
REG_ECX             equ     4       ; in t_reg.
REG_EDX             equ     8
REG_EBX             equ     12
REG_ESP             equ     16
REG_EBP             equ     20
REG_ESI             equ     24
REG_EDI             equ     28

SEG_UNDEF           equ     -1
SEG_ES              equ     0       ; Indexes of segment/selector registers
SEG_CS              equ     1       ; in t_reg.
SEG_SS              equ     2
SEG_DS              equ     3
SEG_FS              equ     4
SEG_GS              equ     5

;// Selected items in register window.
RS_NONE             equ     00000h      ; No selection
RS_INT              equ     00010h      ; General-purpose 32-bit registers
RS_EIP              equ     00020h      ; EIP (instruction pointer)
RS_FLG              equ     00030h      ; 1-bit decoded flags
RS_SEG              equ     00040h      ; Segment (selector) registers
RS_EFL              equ     00050h      ; 32-bit flag register
RS_TAG              equ     00060h      ; FPU register tag
RS_FPU              equ     00070h      ; 80-bit FPU registers
RS_FST              equ     00080h      ; FPU status
RS_FCO              equ     00090h      ; FPU condition bits
RS_FER              equ     000A0h      ; FPU error bits
RS_FCW              equ     000B0h      ; FPU control word
RS_FPR              equ     000C0h      ; FPU precision fields
RS_FEM              equ     000D0h      ; FPU error mask bits
RS_MMX              equ     000E0h      ; MMX registers
RS_3DN              equ     000F0h      ; 3DNow! registers
RS_SSE              equ     00100h      ; SSE registers
RS_CSR              equ     00110h      ; SSE MXCSR register
RS_CSB              equ     00120h      ; SSE MXCSR bits
RS_CPR              equ     00130h      ; SSE rounding control
RS_ERR              equ     00140h      ; Last thread error

RS_GROUP            equ     001F0h      ; Mask to extract group of registers
RS_INDEX            equ     0000Fh      ; Mask to extract index of register

NREGSTACK           equ     32          ; Length of stack trace buffer
MAXCALSIZE          equ     8           ; Max length of CALL without prefixes

INT3                equ     0CCh        ; Code of 1-byte breakpoint
NOP_                equ     090h        ; Code of 1-byte NOP command
TRAPFLAG            equ     000000100h  ; Trap flag in CPU flag register

C_TYPEMASK          equ     0F0h        ; Mask for command type
  C_CMD             equ     000h        ; Ordinary instruction
  C_PSH             equ     010h        ; PUSH instruction
  C_POP             equ     020h        ; POP instruction
  C_MMX             equ     030h        ; MMX instruction
  C_FLT             equ     040h        ; FPU instruction
  C_JMP             equ     050h        ; JUMP instruction
  C_JMC             equ     060h        ; Conditional JUMP instruction
  C_CAL             equ     070h        ; CALL instruction
  C_RET             equ     080h        ; RET instruction
  C_FLG             equ     090h        ; Changes system flags
  C_RTF             equ     0A0h        ; C_JMP and C_FLG simultaneously
  C_REP             equ     0B0h        ; Instruction with REPxx prefix
  C_PRI             equ     0C0h        ; Privileged instruction
  C_SSE             equ     0D0h        ; SSE instruction
  C_NOW             equ     0E0h        ; 3DNow! instruction
  C_BAD             equ     0F0h        ; Unrecognized command
C_RARE              equ     008h        ; Rare command, seldom used in programs
C_SIZEMASK          equ     007h        ; MMX data size or special flag
  C_EXPL            equ     001h        ; (non-MMX) Specify explicit memory size

C_DANGER95          equ     001h        ; Command is dangerous under Win95/98
C_DANGER            equ     003h        ; Command is dangerous everywhere
C_DANGERLOCK        equ     007h        ; Dangerous with LOCK prefix

DEC_TYPEMASK        equ     01Fh        ; Type of memory byte
  DEC_UNKNOWN       equ     000h        ; Unknown type
  DEC_BYTE          equ     001h        ; Accessed as byte
  DEC_WORD          equ     002h        ; Accessed as short
  DEC_NEXTDATA      equ     003h        ; Subsequent byte of data
  DEC_DWORD         equ     004h        ; Accessed as long
  DEC_FLOAT4        equ     005h        ; Accessed as float
  DEC_FWORD         equ     006h        ; Accessed as descriptor/long pointer
  DEC_FLOAT8        equ     007h        ; Accessed as double
  DEC_QWORD         equ     008h        ; Accessed as 8-byte integer
  DEC_FLOAT10       equ     009h        ; Accessed as long double
  DEC_TBYTE         equ     00Ah        ; Accessed as 10-byte integer
  DEC_STRING        equ     00Bh        ; Zero-terminated ASCII string
  DEC_UNICODE       equ     00Ch        ; Zero-terminated UNICODE string
  DEC_3DNOW         equ     00Dh        ; Accessed as 3Dnow operand
  DEC_SSE           equ     00Eh        ; Accessed as SSE operand
  DEC_TEXT          equ     010h        ; For use in t_result only
  DEC_BYTESW        equ     011h        ; Accessed as byte index to switch
  DEC_NEXTCODE      equ     013h        ; Subsequent byte of command
  DEC_COMMAND       equ     01Dh        ; First byte of command
  DEC_JMPDEST       equ     01Eh        ; Jump destination
  DEC_CALLDEST      equ     01Fh        ; Call (and maybe jump) destination
DEC_PROCMASK        equ     060h        ; Procedure analysis
  DEC_PROC          equ     020h        ; Start of procedure
  DEC_PBODY         equ     040h        ; Body of procedure
  DEC_PEND          equ     060h        ; End of procedure
DEC_CHECKED         equ     080h        ; Byte was analysed
DEC_SIGNED          equ     0100h       ; For use in t_result only

DECR_TYPEMASK       equ     03Fh        ; Type of register or memory
  DECR_BYTE         equ     021h        ; Byte register
  DECR_WORD         equ     022h        ; Short integer register
  DECR_DWORD        equ     024h        ; Long integer register
  DECR_QWORD        equ     028h        ; MMX register
  DECR_FLOAT10      equ     029h        ; Floating-point register
  DECR_SEG          equ     02Ah        ; Segment register
  DECR_3DNOW        equ     02Dh        ; 3Dnow! register
  DECR_SSE          equ     02Eh        ; SSE register
DECR_ISREG          equ     020h        ; Mask to check that operand is register

DISASM_SIZE         equ     0           ; Determine command size only
DISASM_DATA         equ     1           ; Determine size and analysis data
DISASM_TRACE        equ     2           ; Trace integer registers
DISASM_FILE         equ     3           ; Disassembly, no symbols/registers
DISASM_CODE         equ     4           ; Disassembly, registers undefined
DISASM_ALL          equ     5           ; Complete disassembly
DISASM_RTRACE       equ     6           ; Disassemble with run-trace registers

DISASM_MODE         equ     00000000Fh  ; Mask to extract disassembling mode
DISASM_HILITE       equ     0000F0000h  ; Mask to extract highlighting mode
DISASM_HLSHIFT      equ     16          ; Shift to extract highlighting mode


;// Warnings issued by Disasm():
DAW_FARADDR         equ     00001h      ; Command is a far jump, call or return
DAW_SEGMENT         equ     00002h      ; Command loads segment register
DAW_PRIV            equ     00004h      ; Privileged command
DAW_IO              equ     00008h      ; I/O command
DAW_SHIFT           equ     00010h      ; Shift constant out of range 1..31
DAW_PREFIX          equ     00020h      ; Superfluous prefix
DAW_LOCK            equ     00040h      ; Command has LOCK prefix
DAW_STACK           equ     00080h      ; Unaligned stack operation
DAW_DANGER95        equ     01000h      ; May mess up Win95/98 if executed
DAW_DANGEROUS       equ     03000h      ; May mess up any OS if executed

RST_INVALID         equ     0           ; Register undefined
RST_VALUE           equ     1           ; Register contains regdata
RST_VFIXUP          equ     2           ; Reg contains regdata that is fixup
RST_INDIRECT        equ     3           ; Register contains [regdata]

NREGSTACK           equ     32          ; Length of stack trace buffer


t_reg struc 1   ; Excerpt from context
        modified        dword   ?           ; Some regs modified, update context    
        modifiedbyuser  dword   ?           ; Among modified, some modified by user 
        singlestep      dword   ?           ; Type of single step, SS_xxx           
        r               dd      8 dup (?)   ; EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI       
        ip              dword   ?           ; Instruction pointer (EIP)             
        flags           dword   ?           ; Flags                                 
        top             dword   ?           ; Index of top-of-stack                 
        f               real10  8 dup (?)   ; Float registers, f[top] - top of stack
        tag             db 8 dup (?)        ; Float tags (0x3 - empty register)     
        fst_            dword   ?           ; FPU status word                       
        fcw             dword   ?           ; FPU control word                      
        s               dd 6 dup (?)        ; Segment registers ES,CS,SS,DS,FS,GS   
        base            dd 6 dup (?)        ; Segment bases                         
        limit           dd 6 dup (?)        ; Segment limits                        
        big             db 6 dup (?)        ; Default size (0-16, 1-32 bit)         
        dr6_            dword   ?           ; Debug register DR6                    
        threadid        dword   ?           ; ID of thread that owns registers      
        lasterror       dword   ?           ; Last thread error or 0xFFFFFFFF
        ssevalid        dword   ?           ; Whether SSE registers valid
        ssemodified     dword   ?           ; Whether SSE registers modified
        ssereg          db 8 dup (16 dup (?))     ; SSE registers                         
        mxcsr           dword   ?           ; SSE control and status register       
        selected        dword   ?           ; Reports selected register to plugin   
        dummy           dd 4 dup (?)        ; Reserved for future compatibility     
t_reg ends
; sizeof t_reg = 402

t_operand struc 1   ; Full decription of command's operand
        optype          byte    ?       ; DEC_xxx (mem) or DECR_xxx (reg,const)
        opsize          byte    ?       ; Size of operand                      
        regscale        db 8 dup (?)    ; Scales of registers                  
        seg_            byte    ?       ; Segment register                     
        opconst         dword   ?       ; Constant                             
t_operand ends
; sizeof t_operand = 15


t_disasm struc 1
        ip              dword   ?               ; Instrucion pointer                         
        dump            db TEXTLEN dup (?)      ; Hexadecimal dump of the command            
        result          db TEXTLEN dup (?)      ; Disassembled command                       
        comment_        db TEXTLEN dup (?)      ; Brief comment                              
        opinfo          db 3 dup (TEXTLEN dup (?))  ; Comments to command's operands             
        cmdtype         dword   ?                   ; One of C_xxx                               
        memtype         dword   ?                   ; Type of addressed variable in memory
        nprefix         dword   ?                   ; Number of prefixes
        indexed         dword   ?                   ; Address contains register(s)
        jmpconst        dword   ?                   ; Constant jump address
        jmptable        dword   ?                   ; Possible address of switch table
        adrconst        dword   ?                   ; Constant part of address
        immconst        dword   ?                   ; Immediate constant
        zeroconst       dword   ?                   ; Whether contains zero constant
        fixupoffset     dword   ?                   ; Possible offset of 32-bit fixups
        fixupsize       dword   ?                   ; Possible total size of fixups or 0
        jmpaddr         dword   ?                   ; Destination of jump/call/return
        condition       dword   ?                   ; 0xFF:unconditional, 0:false, 1:true
        error           dword   ?                   ; Error while disassembling command
        warnings        dword   ?                   ; Combination of DAW_xxx
        optype          dd 3 dup (?)                ; Type of operand (extended set DEC_xxx)
        opsize          dd 3 dup (?)                ; Size of operand, bytes                     
        opgood          dd 3 dup (?)                ; Whether address and data valid
        opaddr          dd 3 dup (?)                ; Address if memory, index if register
        opdata          dd 3 dup (?)                ; Actual value (only integer operands)
        op              t_operand 3 dup (<>)        ; Full description of operand
        regdata         dd 8 dup (?)                ; Registers after command is executed
        regstatus       dd 8 dup (?)                ; Status of registers, one of RST_xxx
        addrdata        dword   ?                   ; Traced memory address
        addrstatus      dword   ?                   ; Status of addrdata, one of RST_xxx
        regstack        dd NREGSTACK dup (?)        ; Stack tracing buffer
        rststatus       dd NREGSTACK dup (?)        ; Status of stack items
        nregstack       dword   ?                   ; Number of items in stack trace buffer
        reserved        dd 29 dup (?)               ; Reserved for plugin compatibility
t_disasm ends
; sizeof t_operand = 2153

Disasm proto C src:ptr byte, srcsize:dword, srcip:dword, srcdec:ptr byte, disasm:ptr t_disasm, disasmmode:dword, threadid:dword
Disassembleback proto C block:ptr byte, base:dword, size_:dword, ip:dword, n:dword, usedec:dword
Disassembleforward proto C block:ptr byte, base:dword, size_:dword, ip:dword, n:dword, usedec:dword
Issuspicious proto C cmd:ptr byte, size_:dword, ip:dword, threadid:dword, preg:ptr t_reg, s:ptr byte
Isfilling proto C offset_:dword, data:ptr byte, size_:dword, align_:dword


;////////////////////////////////////////////////////////////////////////////////
;////////////////////////////// ASSEMBLY FUNCTIONS //////////////////////////////

Assemble proto C cmd:ptr byte, ip:dword, model_:ptr t_asmmodel, attempt:dword, constsize:dword, errtext:ptr byte
Checkcondition proto C code_:dword, flags:dword

;////////////////////////////////////////////////////////////////////////////////
;///////////////////////////////// EXPRESSIONS //////////////////////////////////
t_result struc 1    ; Result of expression's evaluation
        type_       dword       ?           ; Type of expression, DEC(R)_xxx
        dtype       dword       ?           ; Type of data, DEC_xxx         
        union
            data        db 10 dup (?)       ; Binary form of expression's value
            u           dword       ?       ; Value as unsigned integer        
            l           dword       ?       ; Value as signed integer          
            f           real10      ?       ; Value as 80-bit float            
        ends
        union
            value       db TEXTLEN dup (?)      ; ASCII form of expression's value        
            wvalue      dw TEXTLEN/2 dup (?)    ; UNICODE form of expression's value
        ends
        lvaddr      dword   ?               ; Address of lvalue or NULL
t_result ends
; sizeof t_result = 278

Expression proto C result:ptr t_result, expression:ptr byte, a:dword, b:dword, data:ptr byte, database:dword, datasize:dword, threadid:dword


;////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////// THREAD FUNCTIONS ///////////////////////////////

t_thread struc 1    ; Information about active threads
        threadid        dword   ?	  ; Thread identifier               
        dummy           dword   ?     ; Always 1                        
        type_           dword   ?     ; Service information, TY_xxx     
        thread          HANDLE  ?     ; Thread handle                   
        datablock       dword   ?     ; Per-thread data block           
        entry           dword   ?     ; Thread entry point              
        stacktop        dword   ?     ; Working variable of Listmemory()
        stackbottom     dword   ?     ; Working variable of Listmemory()
        context         CONTEXT <>     ; Actual context of the thread    
        reg             t_reg   <>     ; Actual contents of registers    
        regvalid        dword   ?     ; Whether reg is valid            
        oldreg          t_reg   <>      ; Previous contents of registers
        oldregvalid     dword   ?     ; Whether oldreg is valid
        suspendcount    dword   ?     ; Suspension count (may be negative)
        usertime        dword   ?     ; Time in user mode, 1/10th ms, or -1
        systime         dword   ?     ; Time in system mode, 1/10th ms, or -1
        reserved        dd 16 dup (?)     ; Reserved for future compatibility
t_thread ends

Createthreadwindow proto C
Findthread proto C threadid:dword
Decodethreadname proto C s:dword, threadid:dword, mode_:dword
Getcputhreadid proto C
Runsinglethread proto C threadid:dword
Restoreallthreads proto C


;////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////// MEMORY FUNCTIONS ///////////////////////////////

;// Mode bits recognized by Readmemory() and Writememory().
MM_RESTORE          equ     001h        ; Restore or remove INT3 breakpoints
MM_SILENT           equ     002h        ; Don't display error message
MM_DELANAL          equ     004h        ; Delete analysis from the memory
MM_RESILENT         equ     MM_RESTORE or MM_SILENT

t_memory struc 1    ; Memory block descriptor
        base        dword   ?           ; Base address of memory block       
        size_       dword   ?           ; Size of block                      
        type_       dword   ?           ; Service information, TY_xxx        
        owner       dword   ?           ; Address of owner of the memory     
        initaccess  dword   ?           ; Initial read/write access          
        access      dword   ?           ; Actual status and read/write access
        threadid    dword   ?           ; Block belongs to this thread or 0  
        sect        db SHORTLEN dup (?) ; Name of module section             
        copy        dword   ?           ; Copy used in CPU window or NULL    
        reserved    dd 8 dup (?)        ; Reserved for plugin compatibility  
t_memory ends

t_heap struc 1 ; Heap block descriptor
        base        dword   ?           ; Base address of heap block
        size_       dword   ?           ; Size of heap block
        type_       dword   ?           ; Service information, TY_xxx
        parent      dword   ?           ; Handle of heap descriptor block
t_heap ends

Findmemory proto C
Guardmemory proto C base:dword, size_:dword, guard:dword
Havecopyofmemory proto C copy:ptr byte, base:dword, size_:dword
Readmemory proto C buf:dword, addr_:dword, size_:dword, mode:dword
Writememory proto C buf:dword, addr_:dword, size_:dword, mode:dword
Readcommand proto C ip:dword, cmd:ptr byte


;////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////// MODULE FUNCTIONS ///////////////////////////////

NVERS               equ     32          ; Max allowed length of file version

; Types of recognized jumps.
JT_JUMP             equ     0           ; Unconditional jump
JT_COND             equ     1           ; Conditional jump
JT_SWITCH           equ     2           ; Jump via switch table

t_stringtable struc 1   ; Pointers to string resources
        name_       dword   ?   ; Name of block of strings
        language    dword   ?   ; Language identifier
        addr_       dword   ?   ; Address of block in memory       
        size_       dword   ?   ; Size of block in memory          
t_stringtable ends

t_fixup struc 1
        base        dword   ?   ; Address of fixup                    
        size_       dword   ?   ; Size of fixup (usually 2 or 4 bytes)
t_fixup ends

t_symvar struc 1    ; Symbolic variable from debug data
        next        dword   ?       ; Index of next variable in chain or -1
        kind        word    ?       ; Kind of variable
        union
            type_       dword   ?   ; Type of variable
            regs        dword   ?   ; Registers in optvar
        ends
        union
            addr_       dword   ?   ; Address or description of registers
            offset_     dword   ?   ; Offset for EBP-relative data       
        ends
        size_       dword   ?       ; Size of variable or optvar data
        optvar      dword   ?       ; Index of optvar chain or -1   
        nameaddr    dword   ?       ; NM_DEBUG address of var's name
t_symvar ends


PIMAGE_SECTION_HEADER typedef ptr IMAGE_SECTION_HEADER

t_jdest struc 1 ; Element of jump data
        type_           byte    ?       ; Type of jump, one of JT_xxx
        from            dword   ?       ; Jump source
        to              dword   ?       ; Jump destination
t_jdest ends

t_module struc 1    ; Executable module descriptor
        base            dword   ?       ; Base address of module           
        size_           dword   ?       ; Size occupied by module
        type_           dword   ?       ; Service information, TY_xxx
        codebase        dword   ?       ; Base address of module code block
        codesize        dword   ?       ; Size of module code block
        resbase         dword   ?       ; Base address of resources
        ressize         dword   ?       ; Size of resources
        ; t_stringtable  *stringtable
        stringtable     dword   ?       ; Pointers to string resources or NULL
        nstringtable    dword   ?       ; Actual number of used stringtable
        maxstringtable  dword   ?       ; Actual number of allocated string
        entry           dword   ?       ; Address of <ModuleEntryPoint> or 
        database        dword   ?       ; Base address of module data block
        idatatable      dword   ?       ; Base address of import data table
        idatabase       dword   ?       ; Base address of import data block
        edatatable      dword   ?       ; Base address of export data table
        edatasize       dword   ?       ; Size of export data table        
        reloctable      dword   ?       ; Base address of relocation table 
        relocsize       dword   ?       ; Size of relocation table         
        name_           db SHORTLEN dup (?) ; Short name of the module
        path            db MAX_PATH dup (?) ; Full name of the module
        nsect           dword   ?           ; Number of sections in the module
        sect            PIMAGE_SECTION_HEADER ? ; Copy of section headers from file
        headersize      dword   ?               ; Total size of headers in executable
        fixupbase       dword   ?       ; Base of image in executable file   
        nfixup          dword   ?       ; Number of fixups in executable     
        fixup           dword   ?       ; Extracted fixups or NULL           
        codedec         dword   ?       ; Decoded code features or NULL      
        codecrc         dword   ?       ; Code CRC for actual decoding       
        hittrace        dword   ?       ; Hit tracing data or NULL     
        hittracecopy    dword   ?       ; Copy of INT3-substituted code
        datadec         dword   ?       ; Decoded data features or NULL
        namelist        t_table <>      ; List of module names
        ; t_symvar *symvar
        symvar          dword   ?       ; Descriptions of symbolic variables
        nsymvar         dword   ?       ; Actual number of elements in symvar 
        maxsymvar       dword   ?       ; Maximal number of elements in symvar
        globaltypes     dword   ?       ; Global types from debug info
        mainentry       dword   ?       ; Address of WinMain() etc. in dbg data
        realsfxentry    dword   ?       ; Entry of packed code or NULL         
        updatenamelist  dword   ?       ; Request to update namelist        
        origcodesize    dword   ?       ; Original size of module code block
        sfxbase         dword   ?       ; Base of memory block with SFX     
        sfxsize         dword   ?       ; Size of memory block with SFX     
;        reserved        dd 28 dup (?)   ; Reserved for plugin compatibility 
        issystemdll     dword   ?       ; Whether system DLL
        processed       dword   ?       ; 0: not processed, 1: good, -1: bad
        dbghelpsym      dword   ?       ; 1: symbols loaded by dbghelp.dll
        version_        db      NVERS dup (?)   ; Version of executable file
        jddata          dword   ? ;ptr t_jdest  ; Recognized jumps within the module
        njddata         dword   ?       ; Number of recognized jumps
        reserved        dd 15 dup (?)   ; Reserved for plugin compatibility
t_module ends

Findmodule proto C addr_:dword
Findfixup proto C pmod:ptr t_module, addr_:dword
Finddecode proto C addr_:dword, psize:ptr dword
Findfileoffset proto C pmod:ptr t_module, addr_:dword
Decoderange proto C addr_:dword, size_:dword, s:ptr byte
Analysecode proto C pm:ptr t_module


;////////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////// DUMP /////////////////////////////////////

;// Standard dump types.
DU_ESCAPABLE        equ     020000h     ; Create escapable dump window
DU_BACKUP           equ     010000h     ; Bit indicates that backup is displayed
DU_TYPE             equ     00F000h     ; Mask for dump type
DU_COUNT            equ     000FF0h     ; Mask for number of items/line
DU_SIZE             equ     00000Fh     ; Mask for size of single item

DU_UNDEF            equ     000000h     ; Undefined dump type
DU_HEXTEXT          equ     001000h     ; Hexadecimal dump with ASCII text
DU_TEXT             equ     002000h     ; Character dump
DU_UNICODE          equ     003000h     ; Unicode dump
DU_INT              equ     004000h     ; Integer signed dump
DU_UINT             equ     005000h     ; Integer unsigned dump
DU_IHEX             equ     006000h     ; Integer hexadecimal dump
DU_FLOAT            equ     007000h     ; Floating-point dump
DU_ADDR             equ     008000h     ; Address dump
DU_DISASM           equ     009000h     ; Disassembly
DU_HEXUNI           equ     00A000h     ; Hexadecimal dump with UNICODE text
DU_ADRASC           equ     00B000h     ; Address dump with ASCII text
DU_ADRUNI           equ     00C000h     ; Address dump with UNICODE text
DU_SPEC             equ     00D000h     ; Special decoding
;DU_PLUGIN           equ     00E000h     ; Menu items reserved by plugins

;// Standard menu types.
MT_BACKUP           equ     00001h      ; Backup, Undo
MT_COPY             equ     00002h      ; Copy to clipboard
MT_EDIT             equ     00004h      ; Edit, Modify, Assemble
MT_SEARCH           equ     00008h      ; Search, Next
MT_DHISTORY         equ     00010h      ; Previous, Next in history

SPECFUNC typedef proto :ptr byte, :dword, :dword, :dword, :ptr t_disasm, :dword
PSPECFUNC typedef ptr SPECFUNC

t_dump struc 1  ; Current status of dump window
        table               t_table <>          ; Treat dump window as custom table
        dimmed              dword   ?           ; Draw in lowcolor if nonzero
        threadid            dword   ?           ; Use decoding and registers if not 0
        dumptype            dword   ?           ; Current dump type, DU_xxx+count+size
        specdump            PSPECFUNC   ?       ; Decoder of DU_SPEC dump types                           
        menutype            dword   ?           ; Standard menues, MT_xxx                                 
        itemwidth           dword   ?           ; Length of displayed item, characters
        showstackframes     dword   ?           ; Show stack frames in address dump
        showstacklocals     dword   ?           ; Show names of locals in stack
        commentmode         dword   ?           ; 0: comment, 1: source, 2: profile
        filename            db MAX_PATH dup (?) ; Name of displayed or backup file                        
        base                dword   ?           ; Start of memory block or file
        size_               dword   ?           ; Size of memory block or file
        addr_               dword   ?           ; Address of first displayed byte
        lastaddr            dword   ?           ; Address of last displayed byte + 1
        sel0                dword   ?           ; Address of first selected byte
        sel1                dword   ?           ; Last selected byte (not included!)
        startsel            dword   ?           ; Start of last selection                                 
        captured            dword   ?           ; Mouse is captured by dump                               
        reladdr             dword   ?           ; Addresses relative to this                              
        relname             db SHORTLEN dup (?) ; Symbol for relative zero address base                   
        filecopy            dword   ?           ; Copy of the file or NULL                                
        backup              dword   ?           ; Old backup of memory/file or NULL
        runtraceoffset      dword   ?           ; Offset back in run trace
        reserved            dd 8 dup (?)        ; Reserved for the future extentions                      
t_dump ends


;////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////// WINDOW FUNCTIONS ///////////////////////////////
                    
WM_USER_MENU        equ     WM_USER + 101   ; Activate context-sensitive menu
WM_USER_SCR         equ     WM_USER + 102   ; Redraw scroll(s)
WM_USER_SAVE        equ     WM_USER + 103   ; Save data to disk
WM_USER_VABS        equ     WM_USER + 104   ; Scroll contents of window by lines
WM_USER_VREL        equ     WM_USER + 105   ; Scroll contents of window by percent
WM_USER_VBYTE       equ     WM_USER + 106   ; Scroll contents of window by bytes
WM_USER_STS         equ     WM_USER + 107   ; Start selection in window
WM_USER_CNTS        equ     WM_USER + 108   ; Continue selection in window
WM_USER_CHGS        equ     WM_USER + 109   ; Move single-line selection
WM_USER_BAR         equ     WM_USER + 110   ; Message from bar segment as button
WM_USER_DBLCLK      equ     WM_USER + 111   ; Doubleclick in column
WM_USER_SIZE        equ     WM_USER + 112   ; Resize children in CPU window
WM_USER_FOCUS       equ     WM_USER + 113   ; Set focus to child of CPU window
WM_USER_FILE        equ     WM_USER + 114   ; Change state of file dump
WM_USER_HERE        equ     WM_USER + 115   ; Query presence list
WM_USER_CHALL       equ     WM_USER + 116   ; Redraw (almost) everything
WM_USER_CHMEM       equ     WM_USER + 117   ; Range of debuggee's memory changed
WM_USER_CHREG       equ     WM_USER + 118   ; Debuggee's register(s) changed
WM_USER_CHNAM       equ     WM_USER + 119   ; Redraw name tables
WM_USER_MOUSE       equ     WM_USER + 120   ; Check mouse coordinates
WM_USER_KEY         equ     WM_USER + 121   ; Emulate WM_KEYDOWN
WM_USER_SYSKEY      equ     WM_USER + 122   ; Emulate WM_SYSKEYDOWN

;// Constants used for scrolling and selection.
MAXTRACK            equ     16384           ; Maximal scroll of user-drawn table
MOVETOP             equ     07FFFFFFFh      ; Move selection to top of table
MOVEBOTTOM          equ     07FFFFFFEh      ; Move selection to bottom of table
                                            
CONT_BROADCAST      equ     00000h          ; Continue sending msg to other windows
STOP_BROADCAST      equ     01234h          ; Stop sending message to other windows

;// Dumpbackup() actions.
BKUP_CREATE         equ     1               ; Create or update backup copy
BKUP_VIEWDATA       equ     2               ; View original data
BKUP_VIEWCOPY       equ     3               ; View backup copy
BKUP_LOADCOPY       equ     4               ; Read backup copy from file
BKUP_SAVEDATA       equ     5               ; Save original data to file
BKUP_SAVECOPY       equ     6               ; Save backup copy to file
BKUP_DELETE         equ     7               ; Delete backup copy

Registerotclass proto C classname:ptr byte, iconname:ptr byte, classproc:dword
Newtablewindow proto C pt:ptr t_table, nlines:dword, maxcolumns:dword, winclass:ptr byte, wintitle:ptr byte
Quicktablewindow proto C pt:ptr t_table, nlines:dword, maxcolumns:dword, winclass:ptr byte, wintitle:ptr byte
Createdumpwindow proto C name_:ptr byte, base:dword, size_:dword, addr_:dword, type_:dword, specdump:PSPECFUNC
Setdumptype proto C pd:ptr t_dump, dumptype:dword
Dumpbackup proto pd:ptr t_dump, action:dword
Broadcast proto C msg:UINT, wp:WPARAM, lp:LPARAM


;////////////////////////////////////////////////////////////////////////////////
;////////////////////////// DATA CONVERSION FUNCTIONS ///////////////////////////

Compress proto C bufin:ptr byte, nbufin:dword, bufout:ptr byte, nbufout:dword
Getoriginaldatasize proto C bufin:ptr byte, nbufin:dword
Decompress proto C bufin:ptr byte, nbufin:dword, bufout:ptr byte, nbufout:dword
Calculatecrc proto C copy:ptr byte, base:dword, size_:dword, pmod:ptr t_module, fixupoffset:dword


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////// REFERENCES AND SEARCH /////////////////////////////

t_ref struc 1   ; Description of reference
        addr_       dword   ?   ; Address of reference             
        size_       dword   ?   ; 1: single command, otherwise size
        type_       dword   ?   ; Type of reference, TY_xxx        
        dest        dword   ?   ; Destination of call
t_ref ends

Findreferences proto C base:dword, size_:dword, addr0:dword, addr1:dword, origin:dword, recurseonjump:dword, title_:ptr byte
Findstrings proto C base:dword, size_:dword, origin:dword, title_:ptr byte
Findalldllcalls proto C pd:ptr t_dump, origin:dword, title_:ptr byte
Findallcommands proto C pd:ptr t_dump, model_:ptr t_asmmodel, origin:dword, title_:ptr byte
Findallsequences proto C pd:ptr t_dump, model_:ptr t_extmodel, origin:dword, title_:ptr byte
Walkreference proto C dir:dword
Walkreferenceex proto C dir:dword, size_:dword


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////// BREAKPOINT AND TRACE FUNCTIONS ////////////////////////

MEMBP_READ          equ     00001h          ; Memory breakpoint on read
MEMBP_WRITE         equ     00002h          ; Memory breakpoint on write
MEMBP_SFX           equ     01000h          ; Skip self-extraction

;// Bits of module hit trace buffer.
TR_HIT              equ     001h            ; Command was executed during trace
TR_SET              equ     002h            ; Code INT3 is in memory, copy valid
TR_RTSKIP           equ     020h            ; Disable run trace on command
TR_RTRACE           equ     040h            ; Force run trace on command
TR_TRACE            equ     080h            ; Command is included in trace

HB_FREE             equ     0               ; Breakpoint is not used
HB_CODE             equ     1               ; Active on command execution
HB_ACCESS           equ     2               ; Active on read/write access
HB_WRITE            equ     3               ; Active on write access
HB_IO               equ     4               ; Active on port I/O
HB_ONESHOT          equ     5               ; One-shot on command execution
HB_STOPAN           equ     6               ; One-shot on command and stop
HB_TEMP             equ     7               ; Temporary on command execution

;// Parameter mode of function Modifyhittrace().
ATR_ADD             equ     1               ; Add trace to buffer
ATR_ADDPROC         equ     2               ; Add only recognized procedures
ATR_RESET           equ     3               ; Mark range as not traced
ATR_REMOVE          equ     4               ; Remove range and breakpoints
ATR_REMOVEALL       equ     5               ; Destroy range and breakpoints
ATR_RESTORE         equ     6               ; Restore breakpoints
ATR_RTRADD          equ     7               ; Add trace and force run trace
ATR_RTRJUMPS        equ     8               ; Add trace and run trace jumps only
ATR_RTRENTRY        equ     9               ; Add trace and run trace entries only
ATR_RTREMOVE        equ     10              ; Remove trace from range
ATR_RTSKIP          equ     11              ; Skip when run tracing

t_bpoint struc 1    ; Description of INT3 breakpoint
        addr_       dword   ?   ; Address of breakpoint                     
        dummy       dword   ?   ; Always 1                                  
        type_       dword   ?   ; Type of breakpoint, TY_xxx                
        cmd         byte    ?   ; Old value of command                      
t_bpoint ends

t_hardbpoint struc 1
        addr_       dword   ?       ; Base address of hardware breakpoint
        size_       dword   ?       ; Size of hardware breakpoint        
        type_       dword   ?       ; Type of breakpoint, one of HB_xxx  
        reserved    dd 4 dup (?)    ; Reserved for the future
t_hardbpoint ends

Setbreakpoint proto C addr_:dword, type_:dword, cmd:byte
Manualbreakpoint proto C addr_:dword, key:dword, shiftkey:dword, nametype:dword, font:dword
Deletebreakpoints proto C addr0:dword, addr1:dword, silent:dword
Getbreakpointtype proto addr_:dword
Getnextbreakpoint proto C addr_:dword, type_:ptr dword, cmd:ptr dword
Tempbreakpoint proto C addr_:dword, mode:dword
Hardbreakpoints proto C closeondelete:dword
Sethardwarebreakpoint proto C addr_:dword, size_:dword, type_:dword
Deletehardwarebreakpoint proto C index:dword
Deletehardwarebreakbyaddr proto C addr_:dword
Setmembreakpoint proto C type_:dword, addr_:dword, size_:dword
Findhittrace proto C addr_:dword, ptracecopy:ptr LPBYTE, psize:ptr byte
Modifyhittrace proto C addr0:dword, addr1:dword, mode:dword
Isretaddr proto C retaddr:dword, procaddr:LPDWORD
Creatertracewindow proto C
Settracecondition proto C cond:ptr byte, onsuspicious:dword, in0:dword, in1:dword, out0:dword, out1:dword
Startruntrace proto C preg:ptr t_reg
Deleteruntrace proto C
Runtracesize proto C
Findprevruntraceip proto C ip:dword, startback:dword
Findnextruntraceip proto C ip:dword, startback:dword
Getruntraceregisters proto C nback:dword, preg:ptr t_reg, pold:ptr t_reg, cmd:ptr byte, comment_:ptr byte
Getruntraceprofile proto C addr_:dword, size_:dword, profile:LPDWORD
Scrollruntracewindow proto C back:dword
Createprofilewindow proto C base:dword, size_:dword


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////// CODE INJECTION ////////////////////////////////

INJECTANSWER typedef proto data:ptr byte, datasize:dword, parm2:dword
PINJECTANSWER typedef ptr INJECTANSWER

t_inject struc 1 ; Description of injected code
        codesize        dword   ?   ; Size of code, including INT3
        code_           LPBYTE  ?   ; Pointer to code             
        stacksize       dword   ?   ; Stack size to save          
        datatype        dword   ?   ; 0: in/out, 1: in, 2: out    
t_inject ends


Injectcode proto C threadid:dword, inject:ptr t_inject, data:ptr byte, datasize:dword, parm1:dword, parm2:dword, answerfunc: PINJECTANSWER


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////// CPU-SPECIFIC FUNCTIONS ////////////////////////////

;// Setcpu modes.
CPU_ASMHIST         equ     000001h     ; Add change to Disassembler history
CPU_ASMCENTER       equ     000004h     ; Make address in the middle of window
CPU_ASMFOCUS        equ     000008h     ; Move focus to Disassembler
CPU_DUMPHIST        equ     000010h     ; Add change to Dump history
CPU_DUMPFIRST       equ     000020h     ; Make address the first byte in Dump
CPU_DUMPFOCUS       equ     000080h     ; Move focus to Dump
CPU_REGAUTO         equ     000100h     ; Automatically switch to FPU/MMX/3DNow!
CPU_RUNTRACE        equ     000200h     ; Show run trace data at offset asmaddr
CPU_STACKFOCUS      equ     000800h     ; Move focus to Stack
CPU_NOCREATE        equ     004000h     ; Don't create CPU window if absent
CPU_REDRAW          equ     008000h     ; Redraw CPU window immediately
CPU_NOFOCUS         equ     010000h     ; Don't assign focus to main window

Setcpu proto C threadid:dword, asmaddr:dword, dumpaddr:dword, stackaddr:dword, mode:dword
Setdisasm proto C asmaddr:dword, selsize:dword, mode:dword
Redrawdisassembler proto C
Getdisassemblerrange proto C pbase:LPDWORD, psize:LPDWORD
Findprocbegin proto C addr_:dword
Findprocend proto C addr_:dword 
Findprevproc proto C addr_:dword
Findnextproc proto C addr_:dword
Getproclimits proto C addr_:dword, start:LPDWORD, end_:LPDWORD
Sendshortcut proto C where:dword, addr_:dword, msg_:dword, ctrl_:dword, shift_:dword, vkcode_:dword


;////////////////////////////////////////////////////////////////////////////////
;/////////////////////////////// PROCESS CONTROL ////////////////////////////////

;// Parameter stepmode in call to Go().
STEP_SAME           equ     0               ; Same action as on previous call
STEP_RUN            equ     1               ; Run program
STEP_OVER           equ     2               ; Step over
STEP_IN             equ     3               ; Step in
STEP_SKIP           equ     4               ; Skip sequence

; enum t_status ; Thread/process status
STAT_NONE           equ     0               ; Thread/process is empty
STAT_STOPPED        equ     1               ; Thread/process suspended
STAT_EVENT          equ     2               ; Processing debug event, process paused
STAT_RUNNING        equ     3               ; Thread/process running
STAT_FINISHED       equ     4               ; Process finished
STAT_CLOSING        equ     5               ; Process is requested to terminate


Getstatus proto C
Go proto C threadid:dword, tilladdr:dword, stepmode:dword, givechance:dword, backupregs:dword
Suspendprocess proto C processevents:dword


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////// DECODING OF ARGUMENTS /////////////////////////////

DASC_TEST           equ     0               ; Test whether a string
DASC_NOHEX          equ     1               ; Test, print nothing if not a string
DASC_ASCII          equ     2               ; Force ASCII
DASC_PASCAL         equ     3               ; Force Pascal

Findknownfunction proto C addr_:dword, direct:dword, level:dword, fname:ptr byte
Decodeknownargument proto C addr_:dword, arg_:ptr byte, value:dword, valid:dword, s:ptr byte, mask_:ptr byte, pset:ptr byte
Findunknownfunction proto C ip:dword, code_:ptr byte, dec_:ptr byte, size_:dword, fname:ptr byte
Decodeascii proto C value:dword, s:ptr byte, len:dword, mode:dword
Decodeunicode proto C value:dword, s:ptr byte, len:dword


;////////////////////////////////////////////////////////////////////////////////
;///////////////////////////// SOURCE CODE SUPPORT //////////////////////////////

FIRST_CODE_LINE     equ     0FFFFFFFFh     ; First available source line

t_sourceline struc 1    ; Source line descriptor
        addr_       dword   ?              ; Start address of corresponding code  
        size_       dword   ?              ; Code size                            
        srcname     dword   ?              ; Index of source name (as NM_DEBUG)   
        line        dword   ?              ; 0-based line number                  
t_sourceline ends

Showsourcefromaddress proto C addr_:dword, show:dword
Getresourcestring proto C pm:ptr t_module, id:dword, s:ptr byte
Getlinefromaddress proto C addr_:dword
Getaddressfromline proto C addr0:dword, addr1:dword, path:ptr byte, line:dword
Getsourcefilelimits proto C nameaddr:dword, addr0:LPDWORD, addr1:LPDWORD
Decodefullvarname proto C pmod:ptr t_module, psym:ptr t_symvar, offset_:dword, name_:ptr byte
Getbprelname proto C pmod:ptr t_module, addr_:dword, offset_:dword, s:ptr byte, nsymb:dword


;////////////////////////////////////////////////////////////////////////////////
;//////////////////////////////// WATCH SUPPORT /////////////////////////////////

Createwatchwindow proto C
Deletewatch proto C indexone:dword
Insertwatch proto C indexone:dword, text:ptr byte
Getwatch proto C indexone:dword, text:ptr byte

;////////////////////////////////////////////////////////////////////////////////
;////////////////////////// WINDOWS-SPECIFIC FUNCTIONS //////////////////////////

MAXNEST             equ     32          ; Max allowed code structure nesting

t_window struc 1    ; Description of window
            hwnd        dword   ?   ; Window's handle
            dummy       dword   ?   ; Must be 1
            type_       dword   ?   ; Type of window, TY_xxx
            parenthw    dword   ?   ; Handle of parent or 0
            winproc     dword   ?   ; Address of WinProc or 0
            threadid    dword   ?   ; ID of the owning thread
            exstyle     dword   ?   ; Extended style
            style       dword   ?   ; Style
            id          dword   ?   ; Identifier
            classproc   dword   ?   ; Address of default (class) WinProc
            child       dword   ?   ; Index of next child
            level       dword   ?   ; Level in genealogy (0: topmost)
            sibling     dword   ?   ; Index of next sibling
            byparent    dword   ?   ; Index when sorted by parent
            title_      db      TEXTLEN dup (?)  ; Window's title
            classname   db      TEXTLEN dup (?)  ; Class name
            tree        db      MAXNEST dup (?)  ; Tree display
t_window ends

Createwinwindow proto C


;////////////////////////////////////////////////////////////////////////////////
;////////////////////////// PLUGIN-SPECIFIC FUNCTIONS ///////////////////////////

;// Parameters of Plugingetvalue().
VAL_HINST           equ     1       ; Current program instance
VAL_HWMAIN          equ     2       ; Handle of the main window
VAL_HWCLIENT        equ     3       ; Handle of the MDI client window
VAL_NCOLORS         equ     4       ; Number of common colors
VAL_COLORS          equ     5       ; RGB values of common colors
VAL_BRUSHES         equ     6       ; Handles of common color brushes
VAL_PENS            equ     7       ; Handles of common color pens
VAL_NFONTS          equ     8       ; Number of common fonts
VAL_FONTS           equ     9       ; Handles of common fonts
VAL_FONTNAMES       equ     10      ; Internal font names
VAL_FONTWIDTHS      equ     11      ; Average widths of common fonts
VAL_FONTHEIGHTS     equ     12      ; Average heigths of common fonts
VAL_NFIXFONTS       equ     13      ; Actual number of fixed-pitch fonts
VAL_DEFFONT         equ     14      ; Index of default font
VAL_NSCHEMES        equ     15      ; Number of color schemes
VAL_SCHEMES         equ     16      ; Color schemes
VAL_DEFSCHEME       equ     17      ; Index of default colour scheme
VAL_DEFHSCROLL      equ     18      ; Default horizontal scroll
VAL_RESTOREWINDOWPOS    equ     19      ; Restore window positions from .ini
VAL_HPROCESS        equ     20      ; Handle of Debuggee
VAL_PROCESSID       equ     21      ; Process ID of Debuggee
VAL_HMAINTHREAD     equ     22      ; Handle of main thread
VAL_MAINTHREADID    equ     23      ; Thread ID of main thread
VAL_MAINBASE        equ     24      ; Base of main module in the process
VAL_PROCESSNAME     equ     25      ; Name of the active process
VAL_EXEFILENAME     equ     26      ; Name of the main debugged file
VAL_CURRENTDIR      equ     27      ; Current directory for debugged process
VAL_SYSTEMDIR       equ     28      ; Windows system directory
VAL_DECODEANYIP     equ     29      ; Decode registers dependless on EIP
VAL_PASCALSTRINGS   equ     30      ; Decode Pascal-style string constants
VAL_ONLYASCII       equ     31      ; Only printable ASCII chars in dump
VAL_DIACRITICALS    equ     32      ; Allow diacritical symbols in strings
VAL_GLOBALSEARCH    equ     33      ; Search from the beginning of block
VAL_ALIGNEDSEARCH   equ     34      ; Search aligned to item's size
VAL_IGNORECASE      equ     35      ; Ignore case in string search
VAL_SEARCHMARGIN    equ     36      ; Floating search allows error margin
VAL_KEEPSELSIZE     equ     37      ; Keep size of hex edit selection
VAL_MMXDISPLAY      equ     38      ; MMX display mode in dialog
VAL_WINDOWFONT      equ     39      ; Use calling window's font in dialog
VAL_TABSTOPS        equ     40      ; Distance between tab stops
VAL_MODULES         equ     41      ; Table of modules (.EXE and .DLL)
VAL_MEMORY          equ     42      ; Table of allocated memory blocks
VAL_THREADS         equ     43      ; Table of active threads
VAL_BREAKPOINTS     equ     44      ; Table of active breakpoints
VAL_REFERENCES      equ     45      ; Table with found references
VAL_SOURCELIST      equ     46      ; Table of source files
VAL_WATCHES         equ     47      ; Table of watches
VAL_CPUFEATURES     equ     50      ; CPU feature bits
VAL_TRACEFILE       equ     51      ; Handle of run trace log file
VAL_ALIGNDIALOGS    equ     52      ; Whether to align dialogs
VAL_CPUDASM         equ     53      ; Dump descriptor of CPU Disassembler
VAL_CPUDDUMP        equ     54      ; Dump descriptor of CPU Dump
VAL_CPUDSTACK       equ     55      ; Dump descriptor of CPU Stack
VAL_APIHELP         equ     56      ; Name of selected API help file
VAL_HARDBP          equ     57      ; Whether hardware breakpoints enabled

Registerpluginclass proto C classname:ptr byte, iconname:ptr byte, dllinst:HINSTANCE, classproc:dword
Unregisterpluginclass proto C classname:ptr byte
Pluginwriteinttoini proto C dllinst:HINSTANCE, key:ptr byte, value:dword
Pluginwritestringtoini proto dllinst:HINSTANCE, key:ptr byte, s:ptr byte
Pluginreadintfromini proto C dllinst:HINSTANCE, key:ptr byte, def:dword
Pluginreadstringfromini proto C dllinst:HINSTANCE, key:ptr byte, s:ptr byte, def:ptr byte
Pluginsaverecord proto C tag:dword, size_:dword, data:dword
Plugingetvalue proto C type_:dword

;////////////////////////////////////////////////////////////////////////////////
;////////////////////// EXPORTED PLUGIN CALLBACK FUNCTIONS //////////////////////

;// Origins of standard OllyDbg windows as passed to plugin. In parenthesis is
;// the type of item you get in ODBG_Pluginmenu(), ODBG_Pluginaction() and
;// ODBG_Pluginshortcut(). Note that this item can be NULL!

PM_MAIN             equ     0       ; Main window        (NULL)
PM_DUMP             equ     10      ; Any Dump window    (t_dump*)
PM_MODULES          equ     11      ; Modules window     (t_module*)
PM_MEMORY           equ     12      ; Memory window      (t_memory*)
PM_THREADS          equ     13      ; Threads window     (t_thread*)
PM_BREAKPOINTS      equ     14      ; Breakpoints window (t_bpoint*)
PM_REFERENCES       equ     15      ; References window  (t_ref*)
PM_RTRACE           equ     16      ; Run trace window   (int*)
PM_WATCHES          equ     17      ; Watches window     (1-based index)
PM_WINDOWS          equ     18      ; Windows window     (t_window*)
PM_DISASM           equ     31      ; CPU Disassembler   (t_dump*)
PM_CPUDUMP          equ     32      ; CPU Dump           (t_dump*)
PM_CPUSTACK         equ     33      ; CPU Stack          (t_dump*)
PM_CPUREGS          equ     34      ; CPU Registers      (t_reg*)

PP_EVENT	    equ     0       ; Paused on debugging event
PP_PAUSE	    equ     1       ; Paused on user's request
PP_TERMINATED	    equ     2       ; Application terminated
PP_BYPROGRAM	    equ     8       ; Debugging event caused by program
PP_INT3BREAK	    equ     16      ; INT3 breakpoint
PP_MEMBREAK	    equ     32      ; Memory breakpoint
PP_HWBREAK	    equ     64      ; Hardware breakpoint
PP_SINGLESTEP	    equ     128     ; Single-step trap
PP_EXCEPTION	    equ     256     ; Exception, like division by 0
PP_ACCESS	    equ     512     ; Access violation, like writing to NULL pointer
PP_GUARDED	    equ     1024    ; Guarded page

_ODBG_Plugindata proto C shortname:ptr byte
_ODBG_Plugininit proto C ollydbgversion:dword, hw:HWND, features:ptr dword
_ODBG_Pluginmainloop proto C debugevent:ptr DEBUG_EVENT
_ODBG_Pluginsaveudd proto C pmod:ptr t_module, ismainmodule:dword
_ODBG_Pluginuddrecord proto C pmod:ptr t_module, ismainmodule:dword, tag:dword, size_:dword, data:dword
_ODBG_Pluginmenu proto C origin:dword, data:ptr byte, item:dword
_ODBG_Pluginaction proto C origin:dword, action:dword, item:dword
_ODBG_Pluginshortcut proto C origin:dword, ctrl:dword, alt:dword, shift:dword, key:dword, item:dword
_ODBG_Pluginreset proto C
_ODBG_Pluginclose proto C
_ODBG_Plugindestroy proto C
_ODBG_Paused proto C reason:dword, preg:ptr t_reg
_ODBG_Pausedex proto C reason:dword, extdata:dword, preg:ptr t_reg, debugevent:ptr DEBUG_EVENT
_ODBG_Plugincmd proto C reason:dword, preg:ptr t_reg, cmd:dword

ODBG_Plugindata         textequ     <_ODBG_Plugindata>
ODBG_Plugininit         textequ     <_ODBG_Plugininit>    
ODBG_Pluginmainloop     textequ     <_ODBG_Pluginmainloop>
ODBG_Pluginsaveudd      textequ     <_ODBG_Pluginsaveudd>
ODBG_Pluginuddrecord    textequ     <_ODBG_Pluginuddrecord>
ODBG_Pluginmenu         textequ     <_ODBG_Pluginmenu>
ODBG_Pluginaction       textequ     <_ODBG_Pluginaction>
ODBG_Pluginshortcut     textequ     <_ODBG_Pluginshortcut>
ODBG_Pluginreset        textequ     <_ODBG_Pluginreset>
ODBG_Plugindestroy      textequ     <_ODBG_Plugindestroy>
ODBG_Pluginclose        textequ     <_ODBG_Pluginclose>
ODBG_Paused             textequ     <_ODBG_Paused>
ODBG_Pausedex           textequ     <_ODBG_Pausedex>
ODBG_Plugincmd          textequ     <_ODBG_Plugincmd>