////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             OLLYDBG PLUGIN API                             //
//                                                                            //
//                                Version 1.10                                //
//                                                                            //
//               Written by Oleh Yuschuk (ollydbg@t-online.de)                //
//                                                                            //
//              Internet:  http://home.t-online.de/home/Ollydbg               //
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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//            Porting from C header file to Delphi unit file by TQN           //
//                Will compile well with Delphi 6 and Delphi 7                //
//                                                                            //
// Note:                                                                      //
//    -The _Findknownfunction function not existed in OllyDbg.exe ver 1.10    //
//    - The Isprefix function not existed in Plugin.h                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

unit Plugin;

interface

uses
  Windows;

{$A1}                                   // Struct byte alignment

{$IFDEF VER150}
  {$WARN UNSAFE_CODE OFF}
  {$WARN UNSAFE_TYPE OFF}
  {$WARN UNSAFE_CAST OFF}
{$ENDIF}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// GENERAL DECLARATIONS ////////////////////////////

const
  PLUGIN_VERSION    = 110;              // Version of plugin interface
  WM_USER           = $0400;

  TEXTLEN           = 256;              // Maximal length of text string
  ARGLEN            = 1024;             // Maximal length of argument string
  USERLEN           = 4096;             // Maximal length of record in .udd file
  SHORTLEN          = 8;                // Maximal length of short name

  BLACK             = 0;                // Indices of colours used by OllyDbg. In
  BLUE              = 1;                // syntax highlighting, use only colours
  GREEN             = 2;                // 0 to 15 in the least significant bits
  CYAN              = 3;                // of the corresponding mask byte.
  RED               = 4;
  MAGENTA           = 5;
  BROWN             = 6;
  LIGHTGRAY         = 7;
  DARKGRAY          = 8;
  LIGHTBLUE         = 9;
  LIGHTGREEN        = 10;
  LIGHTCYAN         = 11;
  LIGHTRED          = 12;
  LIGHTMAGENTA      = 13;
  YELLOW            = 14;
  WHITE             = 15;
  MINT              = 16;
  SKYBLUE           = 17;
  IVORY             = 18;
  GRAY              = 19;

  NCOLORS           = 20;               // Total number of defined colours

  BKTRANSP          = $00;              // Background colours in syntax hiliting
  BKBLACK           = $10;
  BKGRAY            = $20;
  BKWHITE           = $30;
  BKCYAN            = $40;
  BKGREEN           = $50;
  BKRED             = $60;
  BKYELLOW          = $70;

  BLACKWHITE        = 0;                // Colour schemes used by OllyDbg
  BLUEGOLD          = 1;
  SKYWIND           = 2;
  NIGHTSTARS        = 3;
  SCHEME4           = 4;
  SCHEME5           = 5;
  SCHEME6           = 6;
  SCHEME7           = 7;

  FIXEDFONT         = 0;                // Fonts used by OllyDbg. Variable-pitch
  TERMINAL6         = 1;                // fonts are placed at the end of this
  FIXEDSYS          = 2;                // table.
  COURIERFONT       = 3;
  LUCIDACONS        = 4;
  FONT5             = 5;
  FONT6             = 6;
  FONT7             = 7;
  MAINFONT          = 8;
  SYSFONT           = 9;
  INFOFONT          = 10;

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// INFORMATION FUNCTIONS ////////////////////////////

procedure Addtolist(addr: LongInt; highlight: Integer; format: PChar); cdecl; varargs;
procedure Updatelist; cdecl;
function  Createlistwindow: HWND; cdecl;
procedure Error(format: PChar); cdecl; varargs;
procedure Message(addr: ULONG; format: PChar); cdecl; varargs;
procedure Infoline(format: PChar); cdecl; varargs;
procedure Progress(promille: Integer; format: PChar); cdecl; varargs;
procedure Flash(format: PChar); cdecl; varargs;

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DATA FORMATTING FUNCTIONS //////////////////////////

// Bits used in Decodeaddress(), Decoderelativeoffset() and Decodethreadname()
// to specify decoding mode:

const
  ADC_DEFAULT       = $0000;          // Default decoding mode
  ADC_DIFFMOD       = $0001;          // Show module only if different
  ADC_NOMODNAME     = $0002;          // Never show module name
  ADC_VALID         = $0004;          // Only decode if allocated memory
  ADC_INMODULE      = $0008;          // Only decode if in some module
  ADC_SAMEMOD       = $0010;          // Decode only address in same module
  ADC_SYMBOL        = $0020;          // Only decode if symbolic name
  ADC_JUMP          = $0040;          // Check if points to JMP/CALL command
  ADC_OFFSET        = $0080;          // Check if symbol for data
  ADC_STRING        = $0100;          // Check if pointer to ASCII or UNICODE
  ADC_ENTRY         = $0200;          // Check if entry to subroutine
  ADC_UPPERCASE     = $0400;          // First letter in uppercase if possible
  ADC_WIDEFORM      = $0800;          // Extended form of decoded name
  ADC_NONTRIVIAL    = $1000;          // Name + non-zero offset
  ADC_DYNAMIC       = $2000;          // JMP/CALL to dynamically loaded name

  PLAINASCII        = $01;            // Plain ASCII character
  DIACRITICAL       = $02;            // Diacritical character
  RAREASCII         = $10;            // Rare ASCII character

function Decodeaddress(addr: ULONG; base: ULONG; addrmode: Integer;
                       symb: PChar; nsymb: Integer; comment: PChar): Integer; cdecl;

function Decoderelativeoffset(addr: ULONG; addrmode: Integer;
                              symb: PChar; nsymb: Integer): Integer; cdecl;

function Decodecharacter(s: PChar; c: UINT): Integer; cdecl;

function Printfloat4(s: PChar; f: Single): Integer; cdecl;

function Printfloat8(s: PChar; d: Double): Integer; cdecl;

function Printfloat10(s: PChar; ext: Extended): Integer; cdecl;

function Print3dnow(s: PChar; f: PChar): Integer; cdecl;

function Printsse(s: PChar; f: PChar): Integer; cdecl;

function Followcall(addr: ULONG): ULONG; cdecl;

function IstextA(c: Char): Integer; cdecl;

function IstextW(w: WCHAR): Integer; cdecl;

function Stringtotext(data: PChar; ndata: Integer;
                      text: PChar; ntext: Integer): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////// DATA INPUT FUNCTIONS ////////////////////////////

const
  MAXCMDSIZE        = 16;             // Maximal length of 80x86 command
  NSEQ              = 8;              // Max length of command sequence
  NMODELS           = 8;              // Number of assembler search models

  // Note that each of dialog functions decodes only subset of listed flags.
  DIA_ASKGLOBAL     = $0001;          // Display checkbox 'Global search'
  DIA_HEXONLY       = $0002;          // Hexadecimal format only
  DIA_ALIGNED       = $0004;          // Display checkbox 'Aligned search'
  DIA_DEFHEX        = $0000;          // On startup, cursor in hex control
  DIA_DEFASCII      = $0010;          // On startup, cursor in ASCII control
  DIA_DEFUNICODE    = $0020;          // On startup, cursor in UNICODE control
  DIA_SEARCH        = $0040;          // Is a search dialog
  DIA_HISTORY       = $0100;          // Allows previous hex strings

  // Status of animation or trace.
  ANIMATE_OFF       = 0;              // No animation
  ANIMATE_IN        = 1;              // Animate into
  ANIMATE_OVER      = 2;              // Animate over
  ANIMATE_RET       = 3;              // Execute till RET
  ANIMATE_SKPRET    = 4;              // Skip RET instruction
  ANIMATE_USER      = 5;              // Execute till user code
  ANIMATE_TRIN      = 6;              // Run trace in
  ANIMATE_TROVER    = 7;              // Run trace over
  ANIMATE_STOP      = 8;              // Gracefully stop animation

type
  p_hexstr = ^t_hexstr;
  t_hexstr = packed record                  // String used for hex/text search
    n: Integer;                             // String length
    data: array[0..TEXTLEN-1] of Char;      // Data
    mask: array[0..TEXTLEN-1] of Char;      // Mask, 0 bits are masked
  end;

  p_asmmodel = ^t_asmmodel;
  t_asmmodel = packed record                // Model to search for assembler command
    code: array[0..MAXCMDSIZE-1] of Char;   // Binary code
    mask: array[0..MAXCMDSIZE-1] of Char;   // Mask for binary code (0: bit ignored)
    length: Integer;                        // Length of code, bytes (0: empty)
    jmpsize: Integer;                       // Offset size if relative jump
    jmpoffset: Integer;                     // Offset relative to IP
    jmppos: Integer;                        // Position of jump offset in command
  end;

  p_asmmodels = ^t_asmmodels;
  t_asmmodels = array[0..NMODELS - 1] of t_asmmodel;

  t_extmodel = packed record                // Model for extended command search
    code: array[0..MAXCMDSIZE-1] of Char;   // Binary code
    mask: array[0..MAXCMDSIZE-1] of Char;   // Mask for binary code (0: bit ignored)
    length: Integer;                        // Length of code, bytes (0: empty)
    jmpsize: Integer;                       // Offset size if relative jump
    jmpoffset: Integer;                     // Offset relative to IP
    jmppos: Integer;                        // Position of jump offset in command
    isany: Integer;                         // Count for ANY's argument
    cmdoffset: Integer;                     // Offset of command in the source
    ramask: array[0..MAXCMDSIZE-1] of Char; // Mask for pseudoregister RA
    rbmask: array[0..MAXCMDSIZE-1] of Char; // Mask for pseudoregister RB
  end;

  p_extmodels = ^t_extmodels;
  t_extmodels = array[0..NSEQ - 1, 0..NMODELS - 1] of t_extmodel;

function Getlong(title: PChar; data: PULONG; datasize: Integer;
                 letter: Char; mode: Integer): Integer; cdecl;

function Getlongxy(title: PChar; data: PULONG; datasize: Integer;
                   letter: Char; mode: Integer;
                   x: Integer; y: Integer): Integer; cdecl;

function Getregxy(title: PChar; data: PULONG; letter: Char;
                  x: Integer; y: Integer): Integer; cdecl;

function Getline(title: PChar; data: PULONG): Integer; cdecl;

function Getlinexy(title: PChar; data: PULONG; x: Integer; y: Integer): Integer; cdecl;

function Getfloat10(title: PChar; fdata: PExtended; tag: PChar;
                    letter: Char; mode: Integer): Integer; cdecl;

function Getfloat10xy(title: PChar; fdata: PExtended; tag: PChar; letter: Char;
                      mode: Integer; x: Integer; y: Integer): Integer; cdecl;

function Getfloat(title: PChar; fdata: Pointer; size: Integer;
                  letter: Char; mode: Integer): Integer; cdecl;

function Getfloatxy(title: PChar; fdata: Pointer; size: Integer;
                    letter: Char; mode: Integer;
                    x: Integer; y: Integer): Integer; cdecl;

procedure Getasmfindmodel(models: p_asmmodels; letter: Char;
                          searchall: Integer); cdecl;

procedure Getasmfindmodelxy(models: p_asmmodels; letter: Char; searchall: Integer;
                           x: Integer; y: Integer); cdecl;

function Gettext(title: PChar; text: PChar; letter: Char;
                 type_i: Integer; fontindex: Integer): Integer; cdecl;

function Gettextxy(title: PChar; text: PChar; letter: Char;
                   type_i: Integer; fontindex: Integer;
                   x: Integer; y: Integer): Integer; cdecl;

function Gethexstring(title: PChar; phs: p_hexstr; mode: Integer;
                      fontindex: Integer; letter: Char): Integer; cdecl;

function Gethexstringxy(title: PChar; phs: p_hexstr; mode: Integer;
                        fontindex: Integer; letter: Char;
                        x: Integer; y: Integer): Integer; cdecl;

function Getmmx(title: PChar; data: PChar; mode: Integer): Integer; cdecl;

function Getmmxxy(title: PChar; data: PChar; mode: Integer;
                  x: Integer; y: Integer): Integer; cdecl;

function Get3dnow(title: PChar; data: PChar; mode: Integer): Integer; cdecl;

function Get3dnowxy(title: PChar; data: PChar; mode: Integer;
                    x: Integer; y: Integer): Integer; cdecl;

function Browsefilename(title: PChar; name: PChar;
                        defext: PChar; getarguments: Integer): Integer; cdecl;

function OpenEXEfile(path: PChar; dropped: Integer): Integer; cdecl;

function Attachtoactiveprocess(newprocessid: Integer): Integer; cdecl;

procedure Animate(animation: Integer); cdecl;

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////// SORTED DATA FUNCTIONS /////////////////////////////

const
  NBAR              = 17;             // Max allowed number of segments in bar

  BAR_PRESSED       = $01;            // Bar segment pressed, used internally
  BAR_DISABLED      = $02;            // Bar segment disabled
  BAR_NOSORT        = $04;            // Flat bar column, supports no sorting
  BAR_NORESIZE      = $08;            // Bar column cannot be resized
  BAR_BUTTON        = $10;            // Segment sends WM_USER_BAR
  BAR_SHIFTSEL      = $20;            // Selection shifted 1/2 char to left

  CAPT_FREE         = 0;              // Bar and data are not captured

  TABLE_DIR         = $0001;          // Bottom-to-top table
  TABLE_COPYMENU    = $0002;          // Attach copy item
  TABLE_SORTMENU    = $0004;          // Attach sort menu
  TABLE_APPMENU     = $0010;          // Attach appearance menu
  TABLE_WIDECOL     = $0020;          // Attach wide columns menu item
  TABLE_USERAPP     = $0040;          // Attach user-processed appearance menu
  TABLE_USERDEF     = $0080;          // User-drawn table
  TABLE_NOHSCR      = $0100;          // Table contains no horizontal scroll
  TABLE_SAVEPOS     = $0200;          // Save position & appearance to .ini
  TABLE_CPU         = $0400;          // Table belongs to CPU window
  TABLE_FASTSEL     = $0800;          // Update when selection changes
  TABLE_COLSEL      = $1000;          // Column-wide selection
  TABLE_SAVEAPP     = $2000;          // Save multiinstance appearance to .ini
  TABLE_HILMENU     = $4000;          // Attach Syntax highlighting menu

  DRAW_NORMAL       = $0000;          // Normal plain text
  DRAW_GRAY         = $0001;          // Grayed text
  DRAW_HILITE       = $0002;          // Highlighted text
  DRAW_UL           = $0004;          // Underlined text
  DRAW_SELECT       = $0008;          // Selected background
  DRAW_EIP          = $0010;          // Inverted normal text/background
  DRAW_BREAK        = $0020;          // Breakpoint background
  DRAW_GRAPH        = $0040;          // Graphical element
  DRAW_DIRECT       = $0080;          // Direct text colour index (mask only)
  DRAW_MASK         = $0080;          // Use mask to set individual colors
  DRAW_EXTSEL       = $0100;          // Extend mask till end of column
  DRAW_UNICODE      = $0200;          // Text in UNICODE
  DRAW_TOP          = $0400;          // Draw upper half of text
  DRAW_BOTTOM       = $0800;          // Draw lower half of text

  // Symbolic names for graphical characters. Any other character is displayed
  // as space. Use only characters in range [1..0x7F]!
  D_SPACE           = 'N';            // Space
  D_SEP             = ' ';            // Thin separating line
  D_BEGIN           = 'B';            // Begin of procedure or scope
  D_BODY            = 'I';            // Body of procedure or scope
  D_ENTRY           = 'J';            // Loop entry point
  D_LEAF            = 'K';            // Intermediate leaf on a tree
  D_END             = 'E';            // End of procedure or scope
  D_SINGLE          = 'S';            // Single-line scope
  D_ENDBEG          = 'T';            // End and begin of stack scope
  D_POINT           = '.';            // Point
  D_JMPUP           = 'U';            // Jump upstairs
  D_JMPOUT          = '<';            // Jump to same location or out of module
  D_JMPDN           = 'D';            // Jump downstairs
  D_PATHUP          = 'u';            // Jump path upstairs (highlighted)
  D_GRAYUP          = 'v';            // Jump path upstairs (grayed)
  D_PATH            = 'i';            // Jump path through text (highlighted)
  D_GRAYPATH        = 'j';            // Jump path through text (grayed)
  D_PATHDN          = 'd';            // Jump path downstairs (highlighted)
  D_GRAYDN          = 'e';            // Jump path downstairs (grayed)
  D_PATHUPEND       = 'r';            // End of path upstairs (highlighted)
  D_GRAYUPEND       = 's';            // End of path upstairs (grayed)
  D_PATHDNEND       = 'f';            // End of path downstairs (highlighted)
  D_GRAYDNEND       = 'g';            // End of path downstairs (grayed)
  D_SWTOP           = 't';            // Start of switch
  D_SWBODY          = 'b';            // Switch body
  D_CASE            = 'c';            // Intermediate switch case
  D_LASTCASE        = 'l';            // Last switch case

  // Please note: Although types here contain mostly unique bit assignments, it's
  // not really necessary. Same bits, except for reserved general types, can be
  // freely shared between different types of sorted data.
  // General item types:
  TY_NEW            = $00000001;      // Item is new
  TY_CONFIRMED      = $00000002;      // Item still exists
  TY_MAIN           = $00000004;      // Main item (thread or module)
  TY_INVALID        = $00000008;      // Invalid type (item does not exist)
  TY_SELECTED       = $80000000;      // Reserved for multiple selection

  // Module-specific types:
  TY_REPORTED       = $00000010;      // Stop on module was reported

  // Reference-specific types:
  TY_REFERENCE      = $00000020;      // Item is a real reference
  TY_ORIGIN         = $00000040;      // Item is a search origin

  // Breakpoint-specific types:
  TY_STOPAN         = $00000080;      // Stop animation if TY_ONESHOT
  TY_SET            = $00000100;      // Code INT3 is in memory
  TY_ACTIVE         = $00000200;      // Permanent breakpoint
  TY_DISABLED       = $00000400;      // Permanent disabled breakpoint
  TY_ONESHOT        = $00000800;      // Temporary stop
  TY_TEMP           = $00001000;      // Temporary breakpoint
  TY_KEEPCODE       = $00002000;      // Set and keep command code
  TY_KEEPCOND       = $00004000;      // Keep condition unchanged (0: remove)
  TY_NOUPDATE       = $00008000;      // Don't redraw breakpoint window
  TY_RTRACE         = $00010000;      // Pseudotype of run trace breakpoint

  // Namelist-specific types:
  TY_EXPORT         = $00010000;      // Exported name
  TY_IMPORT         = $00020000;      // Imported name
  TY_LIBRARY        = $00040000;      // Name extracted from object file
  TY_LABEL          = $00080000;      // User-defined name
  TY_ANYNAME        = $000F0000;      // Any of the namelist flags above
  TY_KNOWN          = $00100000;      // Name of known function

  // Memory-specific types:
  TY_DEFHEAP        = $00020000;      // Contains default heap
  TY_HEAP           = $00040000;      // Contains non-default heap
  TY_SFX            = $00080000;      // Contains self-extractor
  TY_CODE           = $00100000;      // Contains image of code section
  TY_DATA           = $00200000;      // Contains image of data section
  TY_IMPDATA        = $00400000;      // Memory block includes import data
  TY_EXPDATA        = $00800000;      // Memory block includes export data
  TY_RSRC           = $01000000;      // Memory block includes resources
  TY_RELOC          = $02000000;      // Memory block includes relocation data
  TY_STACK          = $04000000;      // Contains stack of some thread
  TY_THREAD         = $08000000;      // Contains data block of some thread
  TY_HEADER         = $10000000;      // COFF header
  TY_ANYMEM         = $1FFE0000;      // Any of the memory flags above
  TY_GUARDED        = $20000000;      // NT only: guarded memory block

  // Procedure data-specific types:
  TY_PURE           = $00004000;      // No side effects except in stack
  TY_PASCAL         = $00010000;      // Procedure ends with RET nnn
  TY_C              = $00020000;      // ADD ESP,nnn after call to procedure
  TY_NOTENTRY       = $00100000;      // Not necessarily entry point

  // Switch data-specific types.
  TY_CHARSW         = $00100000;      // ASCII switch
  TY_WMSW           = $00200000;      // Window message switch
  TY_EXCEPTSW       = $00400000;      // Exception switch

  // Stack walk data-specific types.
  TY_RELIABLE       = $01000000;      // Reliable call
  TY_GUESSED        = $02000000;      // Not a real entry, just guessed
  TY_BELONGS        = $04000000;      // Not a real entry, just belongs to proc

  // Call tree-specific types.
  TY_RECURSIVE      = $00000100;      // Routine calls self
  TY_TERMINAL       = $00000200;      // Leaf function, doesn't call others
  TY_SYSTEM         = $00000400;      // Function resides in system DLL
  TY_DIRECT         = $00000800;      // Called directly
  TY_NODATA         = $00001000;      // Not analyzed or outside procedure
  TY_DUMMY          = $00002000;      // Consists of single RET command
  TY_NOSIDE         = $00004000;      // No side effects except in stack

type
  t_scheme = packed record            // Color scheme
    name: PChar;                      // Name of the scheme
    textcolor: Integer;               // Colour used to draw text
    hitextcolor: Integer;             // Colour used to draw highlited text
    lowcolor: Integer;                // Colour used to draw auxiliary text
    bkcolor: Integer;                 // Colour used to draw backgrounds
    selbkcolor: Integer;              // Colour used for selecting background
    linecolor: Integer;               // Colour used for separating lines
    auxcolor: Integer;                // Colour used for auxiliary objects
    condbkcolor: Integer;             // Colour used for background of cond brk
  end;

  t_schemeopt = packed record         // Color scheme, alternative form
    name: PChar;                      // Name of the scheme
    color: array[0..7] of Integer;    // Colours used as in t_scheme
  end;

  p_bar = ^t_bar;
  t_bar = packed record               // Number of active columns
    nbar: Integer;                    // Font used for bar segments
    font: Integer;                    // Actual widths of columns, pixels
    dx: array[0..NBAR-1] of Integer;  // Default widths of columns, chars
    defdx:array[0..NBAR-1] of Integer;// Column names (may be NULL)
    name: array[0..NBAR-1] of PChar;  // Combination of BAR_xxx bits
    mode: array[0..NBAR-1] of Char;   // One of CAPT_xxx, set to CAPT_FREE
    captured: Integer;                // Info about how mouse is captured
    active: Integer;                  // Previous mouse coordinate
    prevx: Integer;
  end;

  p_sortheader = ^t_sortheader;
  t_sortheader = packed record        // Header of sorted data field
    addr: ULONG;                      // Base address of the entry
    size: ULONG;                      // Size address of the entry
    type_ul: ULONG;                   // Entry type, TY_xxx
  end;

  SORTFUNC = function(const p1, p2: p_sortheader; sort: Integer): Integer; cdecl;
  DRAWFUNC = function(s: PChar; mask: PChar; select: PInteger;
                      ps: p_sortheader; column: Integer): Integer; cdecl;
  DESTFUNC = procedure(pe: p_sortheader); cdecl;

const
  AUTOARRANGE: SORTFUNC = SORTFUNC(1);  // Autoarrangeable sorted data

type
  p_sorted = ^t_sorted;                 // Descriptor of sorted table
  t_sorted = packed record              // Name of table, as appears in error
    name: array[0..MAX_PATH-1] of Char; // Actual number of entries
    n: Integer;                         // Maximal number of entries
    nmax: Integer;                      // Index of selected entry or -1
    selected: Integer;                  // Base address of selected entry
    seladdr: ULONG;                     // Size of single entry
    itemsize: Integer;                  // Unique version of table
    version: ULONG;                     // Entries, sorted by address
    data: Pointer;                      // Function which sorts data or NULL
    sortfunc: SORTFUNC;                 // Destructor function or NULL
    destfunc: DESTFUNC;                 // Sorting criterium (column)
    sort: Integer;                      // Whether indexes are sorted
    sorted: Integer;                    // Indexes, sorted by criterium
    index: PInteger;                    // Suppress multiple overflow errors
    suppresserr: Integer;
  end;

  p_table = ^t_table;
  t_table = packed record               // Window with sorted data and bar
    hw: HWND;                           // Handle of window or NULL
    data: T_SORTED;                     // Sorted data
    bar: T_BAR;                         // Description of bar
    showbar: Integer;                   // Bar: 1-displayed, 0-hidden, -1-absent
    hscroll: SmallInt;                  // Horiz. scroll: 1-displayed, 0-hidden
    colsel: SmallInt;                   // Column in TABLE_COLSEL window
    mode: Integer;                      // Combination of bits TABLE_xxx
    font: Integer;                      // Font used by window
    scheme: SmallInt;                   // Colour scheme used by window
    hilite: SmallInt;                   // Syntax highlighting used by window
    offset: Integer;                    // First displayed row
    xshift: Integer;                    // Shift in X direction, pixels
    drawfunc: DRAWFUNC;                 // Function which decodes table fields
  end;

function Createsorteddata(sd: p_sorted; name: PChar;
                          itemsize: Integer; nmax: Integer;
                          sortfn: SORTFUNC; destfn: DESTFUNC): Integer; cdecl;

procedure Destroysorteddata(sd: p_sorted); cdecl;

function Addsorteddata(sd: p_sorted; item: Pointer): Pointer; cdecl;

procedure Deletesorteddata(sd: p_sorted; addr: ULONG); cdecl;

procedure Deletesorteddatarange(sd: p_sorted; addr0: ULONG; addr1: ULONG); cdecl;

function Deletenonconfirmedsorteddata(sd: p_sorted): Integer; cdecl;

function Findsorteddata(sd: p_sorted; addr: ULONG): Pointer; cdecl;

function Findsorteddatarange(sd: p_sorted; addr0: ULONG; addr1: ULONG): Pointer; cdecl;

function Findsorteddataindex(sd: p_sorted; addr0: ULONG; addr1: ULONG): Integer; cdecl;

function Sortsorteddata(sd: p_sorted; sort: Integer): Integer; cdecl;

function Getsortedbyselection(sd: p_sorted; index: Integer): Pointer; cdecl;

procedure Defaultbar(pb: p_bar) cdecl;

function Tablefunction(pt: p_table; hw: HWND; msg: Word;
                       wp: WPARAM; lp: LPARAM): Integer; cdecl;

procedure Painttable(hw: HWND; pt: p_table; getline: DRAWFUNC) cdecl;

function Gettableselectionxy(pt: p_table; column: Integer;
                             px: PInteger; py: PInteger): Integer; cdecl;

procedure Selectandscroll(pt: p_table; index: Integer; mode: Integer); cdecl;


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// NAME FUNCTIONS ///////////////////////////////

// Types of names used in name functions. Note that higher-priority types have
// smaller identifiers!

const
  NM_NONAME         = $00;              // Undefined name
  NM_ANYNAME        = $FF;              // Name of any type

  // Names saved in the data file of module they appear.
  NM_PLUGCMD        = $30;              // Plugin commands to execute at break
  NM_LABEL          = $31;              // User-defined label
  NM_EXPORT         = $32;              // Exported (global) name
  NM_IMPORT         = $33;              // Imported name
  NM_LIBRARY        = $34;              // Name from library or object file
  NM_CONST          = $35;              // User-defined constant
  NM_COMMENT        = $36;              // User-defined comment
  NM_LIBCOMM        = $37;              // Comment from library or object file
  NM_BREAK          = $38;              // Condition related with breakpoint
  NM_ARG            = $39;              // Arguments decoded by analyzer
  NM_ANALYSE        = $3A;              // Comment added by analyzer
  NM_BREAKEXPR      = $3B;              // Expression related with breakpoint
  NM_BREAKEXPL      = $3C;              // Explanation related with breakpoint
  NM_ASSUME         = $3D;              // Assume function with known arguments
  NM_STRUCT         = $3E;              // Code structure decoded by analyzer
  NM_CASE           = $3F;              // Case description decoded by analyzer

  // Names saved in the data file of main module.
  NM_INSPECT        = $40;              // Several last inspect expressions
  NM_WATCH          = $41;              // Watch expressions
  NM_ASM            = $42;              // Several last assembled strings
  NM_FINDASM        = $43;              // Several last find assembler strings
  NM_LASTWATCH      = $48;              // Several last watch expressions
  NM_SOURCE         = $49;              // Several last source search strings
  NM_REFTXT         = $4A;              // Several last ref text search strings
  NM_GOTO           = $4B;              // Several last expressions to follow
  NM_GOTODUMP       = $4C;              // Several expressions to follow in Dump
  NM_TRPAUSE        = $4D;              // Several expressions to pause trace

  // Pseudonames.
  NM_IMCALL         = $FE;              // Intermodular call

  NMHISTORY         = $40;              // Converts NM_xxx to type of init list

function Insertname(addr: ULONG; itype: Integer; name: PChar): Integer; cdecl;

function Quickinsertname(addr: ULONG; itype: Integer; name: PChar): Integer; cdecl;

procedure Mergequicknames; cdecl;

procedure Discardquicknames; cdecl;

function Findname(addr: ULONG; itype: Integer; name: PChar): Integer; cdecl;

function Decodename(addr: ULONG; itype: Integer; name: PChar): Integer; cdecl;

function Findnextname(name: PChar): ULONG; cdecl;

function Findlabel(addr: ULONG; name: PChar): Integer; cdecl;

procedure Deletenamerange(addr0: ULONG; addr1: ULONG; itype: Integer); cdecl;

function Findlabelbyname(name: PChar; addr: PULONG;
                         addr0: ULONG; addr1: ULONG): Integer; cdecl;

function Findimportbyname(name: PChar; addr0: ULONG; addr1: ULONG): ULONG; cdecl;

function Demanglename(name: PChar; itype: Integer; undecorated: PChar): Integer; cdecl;

function Findsymbolicname(addr: ULONG; fname: PChar): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// DISASSEMBLY FUNCTIONS ////////////////////////////

const
  REG_EAX           = 0;                // Indexes of general-purpose registers
  REG_ECX           = 1;                // in t_reg.
  REG_EDX           = 2;
  REG_EBX           = 3;
  REG_ESP           = 4;
  REG_EBP           = 5;
  REG_ESI           = 6;
  REG_EDI           = 7;

  SEG_UNDEF         = -1;
  SEG_ES            = 0;                // Indexes of segment/selector registers
  SEG_CS            = 1;                // in t_reg.
  SEG_SS            = 2;
  SEG_DS            = 3;
  SEG_FS            = 4;
  SEG_GS            = 5;

  // Selected items in register window.
  RS_NONE           = $0000;            // No selection
  RS_INT            = $0010;            // General-purpose 32-bit registers
  RS_EIP            = $0020;            // EIP (instruction pointer)
  RS_FLG            = $0030;            // 1-bit decoded flags
  RS_SEG            = $0040;            // Segment (selector) registers
  RS_EFL            = $0050;            // 32-bit flag register
  RS_TAG            = $0060;            // FPU register tag
  RS_FPU            = $0070;            // 80-bit FPU registers
  RS_FST            = $0080;            // FPU status
  RS_FCO            = $0090;            // FPU condition bits
  RS_FER            = $00A0;            // FPU error bits
  RS_FCW            = $00B0;            // FPU control word
  RS_FPR            = $00C0;            // FPU precision fields
  RS_FEM            = $00D0;            // FPU error mask bits
  RS_MMX            = $00E0;            // MMX registers
  RS_3DN            = $00F0;            // 3DNow! registers
  RS_SSE            = $0100;            // SSE registers
  RS_CSR            = $0110;            // SSE MXCSR register
  RS_CSB            = $0120;            // SSE MXCSR bits
  RS_CPR            = $0130;            // SSE rounding control
  RS_ERR            = $0140;            // Last thread error

  RS_GROUP          = $01F0;            // Mask to extract group of registers
  RS_INDEX          = $000F;            // Mask to extract index of register

  NREGSTACK         = 32;               // Length of stack trace buffer
  MAXCALSIZE        = 8;                // Max length of CALL without prefixes

  INT3              = $CC;              // Code of 1-byte breakpoint
  NOP               = $90;              // Code of 1-byte NOP command
  TRAPFLAG          = $00000100;        // Trap flag in CPU flag register

  C_TYPEMASK        = $F0;              // Mask for command type
    C_CMD           = $00;              // Ordinary instruction
    C_PSH           = $10;              // PUSH instruction
    C_POP           = $20;              // POP instruction
    C_MMX           = $30;              // MMX instruction
    C_FLT           = $40;              // FPU instruction
    C_JMP           = $50;              // JUMP instruction
    C_JMC           = $60;              // Conditional JUMP instruction
    C_CAL           = $70;              // CALL instruction
    C_RET           = $80;              // RET instruction
    C_FLG           = $90;              // Changes system flags
    C_RTF           = $A0;              // C_JMP and C_FLG simultaneously
    C_REP           = $B0;              // Instruction with REPxx prefix
    C_PRI           = $C0;              // Privileged instruction
    C_SSE           = $D0;              // SSE instruction
    C_NOW           = $E0;              // 3DNow! instruction
    C_BAD           = $F0;              // Unrecognized command
  C_RARE            = $08;              // Rare command, seldom used in programs
  C_SIZEMASK        = $07;              // MMX data size or special flag
    C_EXPL          = $01;              // (non-MMX) Specify explicit memory size

  C_DANGER95        = $01;              // Command is dangerous under Win95/98
  C_DANGER          = $03;              // Command is dangerous everywhere
  C_DANGERLOCK      = $07;              // Dangerous with LOCK prefix

  DEC_TYPEMASK      = $1F;              // Type of memory byte
    DEC_UNKNOWN     = $00;              // Unknown type
    DEC_BYTE        = $01;              // Accessed as byte
    DEC_WORD        = $02;              // Accessed as short
    DEC_NEXTDATA    = $03;              // Subsequent byte of data
    DEC_DWORD       = $04;              // Accessed as long
    DEC_FLOAT4      = $05;              // Accessed as float
    DEC_FWORD       = $06;              // Accessed as descriptor/long pointer
    DEC_FLOAT8      = $07;              // Accessed as double
    DEC_QWORD       = $08;              // Accessed as 8-byte integer
    DEC_FLOAT10     = $09;              // Accessed as long double
    DEC_TBYTE       = $0A;              // Accessed as 10-byte integer
    DEC_STRING      = $0B;              // Zero-terminated ASCII string
    DEC_UNICODE     = $0C;              // Zero-terminated UNICODE string
    DEC_3DNOW       = $0D;              // Accessed as 3Dnow operand
    DEC_SSE         = $0E;              // Accessed as SSE operand
    DEC_TEXT        = $10;              // For use in t_result only
    DEC_BYTESW      = $11;              // Accessed as byte index to switch
    DEC_NEXTCODE    = $13;              // Subsequent byte of command
    DEC_COMMAND     = $1D;              // First byte of command
    DEC_JMPDEST     = $1E;              // Jump destination
    DEC_CALLDEST    = $1F;              // Call (and maybe jump) destination
  DEC_PROCMASK      = $60;              // Procedure analysis
    DEC_PROC        = $20;              // Start of procedure
    DEC_PBODY       = $40;              // Body of procedure
    DEC_PEND        = $60;              // End of procedure
  DEC_CHECKED       = $80;              // Byte was analysed
  DEC_SIGNED        = $100;             // For use in t_result only

  DISASM_SIZE       = 0;                // Determine command size only
  DISASM_DATA       = 1;                // Determine size and analysis data
  DISASM_TRACE      = 2;                // Trace integer registers
  DISASM_FILE       = 3;                // Disassembly, no symbols/registers
  DISASM_CODE       = 4;                // Disassembly, registers undefined
  DISASM_ALL        = 5;                // Complete disassembly
  DISASM_RTRACE     = 6;                // Disassemble with run-trace registers

  DISASM_MODE       = $0000000F;        // Mask to extract disassembling mode
  DISASM_HILITE     = $000F0000;        // Mask to extract highlighting mode
  DISASM_HLSHIFT    = 16;               // Shift to extract highlighting mode

  // Warnings issued by Disasm():
  DAW_FARADDR       = $0001;            // Command is a far jump, call or return
  DAW_SEGMENT       = $0002;            // Command loads segment register
  DAW_PRIV          = $0004;            // Privileged command
  DAW_IO            = $0008;            // I/O command
  DAW_SHIFT         = $0010;            // Shift constant out of range 1..31
  DAW_PREFIX        = $0020;            // Superfluous prefix
  DAW_LOCK          = $0040;            // Command has LOCK prefix
  DAW_STACK         = $0080;            // Unaligned stack operation
  DAW_DANGER95      = $1000;            // May mess up Win95/98 if executed
  DAW_DANGEROUS     = $3000;            // May mess up any OS if executed

  RST_INVALID       = 0;                // Register undefined
  RST_VALUE         = 1;                // Register contains regdata
  RST_VFIXUP        = 2;                // Reg contains regdata that is fixup
  RST_INDIRECT      = 3;                // Register contains [regdata]

type
  p_reg = ^t_reg;
  t_reg = packed record                 // Excerpt from context
    modified: Integer;                  // Some regs modified, update context
    modifiedbyuser: Integer;            // Among modified, some modified by user
    singlestep: Integer;                // Type of single step, SS_xxx
    r: array[0..7] of ULONG;            // EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI
    ip: ULONG;                          // Instruction pointer (EIP)
    flags: ULONG;                       // Flags
    top: Integer;                       // Index of top-of-stack
    f: array[0..7] of Extended;         // Float registers, f[top] - top of stack
    tag: array[0..7] of Char;           // Float tags (0x3 - empty register)
    fst: ULONG;                         // FPU status word
    fcw: ULONG;                         // FPU control word
    s: array[0..5] of ULONG;            // Segment registers ES,CS,SS,DS,FS,GS
    base: array[0..5] of ULONG;         // Segment bases
    limit: array[0..5] of ULONG;        // Segment limits
    big: array[0..5] of Char;           // Default size (0-16, 1-32 bit)
    dr6: ULONG;                         // Debug register DR6
    threadid: ULONG;                    // ID of thread that owns registers
    lasterror: ULONG;                   // Last thread error or 0xFFFFFFFF
    ssevalid: Integer;                  // Whether SSE registers valid
    ssemodified: Integer;               // Whether SSE registers modified
    ssereg: array[0..7, 0..15] of Char; // SSE registers
    mxcsr: ULONG;                       // SSE control and status register
    selected: Integer;                  // Reports selected register to plugin
    dummy: array[0..3] of ULONG;        // Reserved for future compatibility
  end;

  p_operand = ^t_operand;
  t_operand = packed record             // Full decription of command's operand
    optype: Char;                       // DEC_xxx (mem) or DECR_xxx (reg,const)
    opsize: Char;                       // Size of operand
    regscale: array[0..7] of Char;      // Scales of registers
    seg: Char;                          // Segment register
    opconst: ULONG;                     // Constant
  end;

  p_disasm = ^t_disasm;
  t_disasm = packed record                      // Results of disassembling
    ip: ULONG;                                  // Instrucion pointer
    dump: array[0..TEXTLEN-1] of Char;          // Hexadecimal dump of the command
    result: array[0..TEXTLEN-1] of Char;        // Disassembled command
    comment: array[0..TEXTLEN-1] of Char;       // Brief comment
    opinfo: array[0..2, 0..TEXTLEN-1] of Char;  // Comments to command's operands
    cmdtype: Integer;                           // One of C_xxx
    memtype: Integer;                           // Type of addressed variable in memory
    nprefix: Integer;                           // Number of prefixes
    indexed: Integer;                           // Address contains register(s)
    jmpconst: ULONG;                            // Constant jump address
    jmptable: ULONG;                            // Possible address of switch table
    adrconst: ULONG;                            // Constant part of address
    immconst: ULONG;                            // Immediate constant
    zeroconst: Integer;                         // Whether contains zero constant
    fixupoffset: Integer;                       // Possible offset of 32-bit fixups
    fixupsize: Integer;                         // Possible total size of fixups or 0
    jmpaddr: ULONG;                             // Destination of jump/call/return
    condition: Integer;                         // 0xFF:unconditional, 0:false, 1:true
    error: Integer;                             // Error while disassembling command
    warnings: Integer;                          // Combination of DAW_xxx
    optype: array[0..2] of Integer;             // Type of operand (extended set DEC_xxx)
    opsize: array[0..2] of Integer;             // Size of operand, bytes
    opgood: array[0..2] of Integer;             // Whether address and data valid
    opaddr: array[0..2] of ULONG;               // Address if memory, index if register
    opdata: array[0..2] of ULONG;               // Actual value (only integer operands)
    op: array[0..2] of t_operand;               // Full description of operand
    regdata: array[0..7] of ULONG;              // Registers after command is executed
    regstatus: array[0..7] of Integer;          // Status of registers, one of RST_xxx
    addrdata: ULONG;                            // Traced memory address
    addrstatus: Integer;                        // Status of addrdata, one of RST_xxx
    regstack: array[0..NREGSTACK-1] of ULONG;   // Stack tracing buffer
    rststatus: array[0..NREGSTACK-1] of Integer;// Status of stack items
    nregstack: Integer;                         // Number of items in stack trace buffer
    reserved: array[0..28] of ULONG;            // Reserved for plugin compatibility
  end;

function Disasm(src: PChar; srcsize: ULONG; srcip: ULONG; srcdec: PChar;
                disasm: p_disasm; disasmmode: Integer; threadid: ULONG): ULONG; cdecl;

function Disassembleback(block: PChar; base: ULONG; size: ULONG;
                         ip: ULONG; n: Integer; usedec: Integer): ULONG; cdecl;

function Disassembleforward(block: PChar; base: ULONG; size: ULONG;
                            ip: ULONG; n: Integer; usedec: Integer): ULONG; cdecl;

function Issuspicious(cmd: PChar; size: ULONG; ip: ULONG;
                      threadid: ULONG; preg: p_reg; s: PChar): Integer; cdecl;

function Isfilling(offset: ULONG; data: PChar; size: ULONG; align: ULONG): Integer; cdecl;

function Isprefix(c: Integer): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// ASSEMBLY FUNCTIONS /////////////////////////////

function Assemble(cmd: PChar; ip: ULONG; model: p_asmmodel; attempt: Integer;
                  constsize: Integer; errtext: PChar): Integer; cdecl;

function Checkcondition(code: Integer; flags: ULONG): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// EXPRESSIONS /////////////////////////////////

type
  t_result_union1 = packed record
    case Byte of
      0: (data: array[0..9] of Char);   // Binary form of expression's value
      1: (u: ULONG);                    // Value as unsigned integer
      2: (l: LongInt);                  // Value as signed integer
      3: (f: Extended);                 // Value as 80-bit float
  end;

  t_result_union2 = packed record
    case Byte of
      0: (value: array[0..TEXTLEN - 1] of Char);            // ASCII form of expression's value
      1: (wvalue: array[0..(TEXTLEN div 2) - 1] of WChar);  // UNICODE form of expression's value
  end;

  p_result = ^t_result;
  t_result = packed record              // Result of expression's evaluation
    etype: Integer;                     // Type of expression, DEC(R)_xxx
    dtype: Integer;                     // Type of data, DEC_xxx
    u1: t_result_union1;
    u2: t_result_union2;
    lvaddr: ULONG;                      // Address of lvalue or NULL
  end;

function Expression(result: p_result; expression: PChar; a, b: Integer;
                    data: PChar; database, datasize, threadid: ULONG): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// THREAD FUNCTIONS //////////////////////////////

type
  p_thread = ^t_thread;
  t_thread = packed record              // Information about active threads
    threadid: ULONG;                    // Thread identifier
    dummy: ULONG;                       // Always 1
    ltype: ULONG;                       // Service information, TY_xxx
    thread: THandle;                    // Thread handle
    datablock: ULONG;                   // Per-thread data block
    entry: ULONG;                       // Thread entry point
    stacktop: ULONG;                    // Working variable of Listmemory()
    stackbottom: ULONG;                 // Working variable of Listmemory()
    context: CONTEXT;                   // Actual context of the thread
    reg: t_reg;                         // Actual contents of registers
    regvalid: Integer;                  // Whether reg is valid
    oldreg: t_reg;                      // Previous contents of registers
    oldregvalid: Integer;               // Whether oldreg is valid
    suspendcount: Integer;              // Suspension count (may be negative)
    usertime: LongInt;                  // Time in user mode, 1/10th ms, or -1
    systime: LongInt;                   // Time in system mode, 1/10th ms, or -1
    reserved: array[0..15] of ULONG;    // Reserved for future compatibility
  end;

function Createthreadwindow: HWND; cdecl;

function Findthread(threadid: ULONG): p_thread; cdecl;

function Decodethreadname(s: PChar; threadid: ULONG; mode: Integer): Integer; cdecl;

function Getcputhreadid: ULONG; cdecl;

function Runsinglethread(threadid: ULONG): ULONG; cdecl;

procedure Restoreallthreads; cdecl;

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// MEMORY FUNCTIONS //////////////////////////////

// Mode bits recognized by Readmemory() and Writememory().
const
  MM_RESTORE        = $01;              // Restore or remove INT3 breakpoints
  MM_SILENT         = $02;              // Don't display error message
  MM_DELANAL        = $04;              // Delete analysis from the memory

  MM_RESILENT       = MM_RESTORE or MM_SILENT;

type
  p_memory = ^t_memory;
  t_memory = packed record              // Memory block descriptor
    base: ULONG;                        // Base address of memory block
    size: ULONG;                        // Size of block
    ltype: ULONG;                       // Service information, TY_xxx
    owner: ULONG;                       // Address of owner of the memory
    initaccess: ULONG;                  // Initial read/write access
    access: ULONG;                      // Actual status and read/write access
    threadid: ULONG;                    // Block belongs to this thread or 0
    sect: array[0..SHORTLEN-1] of Char; // Name of module section
    copy: PChar;                       // Copy used in CPU window or NULL
    reserved: array[0..7] of ULONG;     // Reserved for plugin compatibility
  end;

  p_heap = ^t_heap;
  t_heap = packed record                // Heap block descriptor
    base: ULONG;                        // Base address of heap block
    size: ULONG;                        // Size of heap block
    ltype: ULONG;                       // Service information, TY_xxx
    parent: ULONG;                      // Handle of heap descriptor block
  end;

function Listmemory: Integer; cdecl;

function Findmemory(addr: ULONG): p_memory; cdecl;

function Guardmemory(base: ULONG; size: ULONG; guard: Integer): Integer; cdecl;

procedure Havecopyofmemory(copy: PChar; base: ULONG; size: ULONG); cdecl;

function Readmemory(buf: Pointer; addr: ULONG; size: ULONG; mode: Integer): ULONG; cdecl;

function Writememory(buf: Pointer; addr: ULONG; size: ULONG; mode: Integer): ULONG; cdecl;

function Readcommand(ip: ULONG; cmd: PChar): ULONG; cdecl;

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// MODULE FUNCTIONS //////////////////////////////

const
  NVERS             = 32;               // Max allowed length of file version

  // Types of recognized jumps.
  JT_JUMP           = 0;                // Unconditional jump
  JT_COND           = 1;                // Conditional jump
  JT_SWITCH         = 2;                // Jump via switch table

type
  p_ahint = ^t_ahint;
  t_ahint = packed record               // User-supplied hint for analysis
    addr: ULONG;                        // Hint address
    size: ULONG;                        // Hint size
    ltype: ULONG;                       // Hint type, bits from DEC_TYPEMASK
  end;

  p_stringtable = ^t_stringtable;
  t_stringtable = packed record         // Pointers to string resources
    name: ULONG;                        // Name of block of strings
    language: ULONG;                    // Language identifier
    addr: ULONG;                        // Address of block in memory
    size: ULONG;                        // Size of block in memory
  end;

  p_fixup = ^t_fixup;
  t_fixup = packed record
    base: ULONG;                        // Address of fixup
    size: ULONG;                        // Size of fixup (usually 2 or 4 bytes)
  end;

  t_symvar_union1 = packed record
    case Byte of
      0: (ltype: ULONG);                // Type of variable
      1: (regs: ULONG);                 // Registers in optvar
  end;

  t_symvar_union2 = packed record
    case Byte of
     1: (addr: ULONG);                  // Address or description of registers
     2: (offset: LongInt);              // Offset for EBP-relative data
  end;

  p_symvar = ^t_symvar;
  t_symvar = packed record              // Symbolic variable from debug data
    next: Integer;                      // Index of next variable in chain or -1
    kind: Word;                         // Kind of variable
    u1: t_symvar_union1;
    u2: t_symvar_union2;
    size: ULONG;                        // Size of variable or optvar data
    optvar: Integer;                    // Index of optvar chain or -1
    nameaddr: ULONG;                    // NM_DEBUG address of var's name
  end;

  p_jdest = ^t_jdest;
  t_jdest = packed record               // Element of jump data
    ctype: Char;                        // Type of jump, one of JT_xxx
    lfrom: ULONG;                       // Jump source
    lto: ULONG;                         // Jump destination
  end;

  p_module = ^t_module;
  t_module = packed record              // Executable module descriptor
    base: ULONG;                        // Base address of module
    size: ULONG;                        // Size occupied by module
    ltype: ULONG;                       // Service information, TY_xxx
    codebase: ULONG;                    // Base address of module code block
    codesize: ULONG;                    // Size of module code block
    resbase: ULONG;                     // Base address of resources
    ressize: ULONG;                     // Size of resources
    stringtable: p_stringtable;         // Pointers to string resources or NULL
    nstringtable: Integer;              // Actual number of used stringtable
    maxstringtable: Integer;            // Actual number of allocated stringtable
    entry: ULONG;                       // Address of <ModuleEntryPoint> or NULL
    database: ULONG;                    // Base address of module data block
    idatatable: ULONG;                  // Base address of import data table
    idatabase: ULONG;                   // Base address of import data block
    edatatable: ULONG;                  // Base address of export data table
    edatasize: ULONG;                   // Size of export data table
    reloctable: ULONG;                  // Base address of relocation table
    relocsize: ULONG;                   // Size of relocation table
    name: array[0..SHORTLEN-1] of Char; // Short name of the module
    path: array[0..MAX_PATH-1] of Char; // Full name of the module
    nsect: Integer;                     // Number of sections in the module
    sect: PImageSectionHeader;          // Copy of section headers from file
    headersize: ULONG;                  // Total size of headers in executable
    fixupbase: ULONG;                   // Base of image in executable file
    nfixup: Integer;                    // Number of fixups in executable
    fixup: p_fixup;                     // Extracted fixups or NULL
    codedec: PChar;                     // Decoded code features or NULL
    codecrc: ULONG;                     // Code CRC for actual decoding
    hittrace: PChar;                    // Hit tracing data or NULL
    hittracecopy: PChar;                // Copy of INT3-substituted code
    datadec: PChar;                     // Decoded data features or NULL
    namelist: t_table;                  // List of module names
    symvar: p_symvar;                   // Descriptions of symbolic variables
    nsymvar: Integer;                   // Actual number of elements in symvar
    maxsymvar: Integer;                 // Maximal number of elements in symvar
    globaltypes: PChar;                 // Global types from debug info
    mainentry: ULONG;                   // Address of WinMain() etc. in dbg data
    realsfxentry: ULONG;                // Entry of packed code or NULL
    updatenamelist: Integer;            // Request to update namelist
    origcodesize: ULONG;                // Original size of module code block
    sfxbase: ULONG;                     // Base of memory block with SFX
    sfxsize: ULONG;                     // Size of memory block with SFX
    issystemdll: Integer;               // Whether system DLL
    processed: Integer;                 // 0: not processed, 1: good, -1: bad
    dbghelpsym: Integer;                // 1: symbols loaded by dbghelp.dll
    version: array[0..NVERS-1] of Char; // Version of executable file
    jddata: p_jdest;                    // Recognized jumps within the module
    njddata: Integer;                   // Number of recognized jumps
    reserved: array[0..14] of ULONG;    // Reserved for plugin compatibility
  end;

function Findmodule(addr: ULONG): p_module; cdecl;

function Findfixup(pmod: p_module; addr: ULONG): p_fixup; cdecl;

function Finddecode(addr: ULONG; psize: PULONG): PChar; cdecl;

function Findfileoffset(pmod: p_module; addr: ULONG): ULONG; cdecl;

function Decoderange(addr: ULONG; size: ULONG; s: PChar): Integer; cdecl;

function Analysecode(pm: p_module): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// DUMP ////////////////////////////////////

const
  // Standard dump types.
  DU_ESCAPABLE      = $20000;           // Create escapable dump window
  DU_BACKUP         = $10000;           // Bit indicates that backup is displayed
  DU_TYPE           = $0F000;           // Mask for dump type
  DU_COUNT          = $00FF0;           // Mask for number of items/line
  DU_SIZE           = $0000F;           // Mask for size of single item

  DU_UNDEF          = $00000;           // Undefined dump type
  DU_HEXTEXT        = $01000;           // Hexadecimal dump with ASCII text
  DU_TEXT           = $02000;           // Character dump
  DU_UNICODE        = $03000;           // Unicode dump
  DU_INT            = $04000;           // Integer signed dump
  DU_UINT           = $05000;           // Integer unsigned dump
  DU_IHEX           = $06000;           // Integer hexadecimal dump
  DU_FLOAT          = $07000;           // Floating-point dump
  DU_ADDR           = $08000;           // Address dump
  DU_DISASM         = $09000;           // Disassembly
  DU_HEXUNI         = $0A000;           // Hexadecimal dump with UNICODE text
  DU_ADRASC         = $0B000;           // Address dump with ASCII text
  DU_ADRUNI         = $0C000;           // Address dump with UNICODE text
  DU_SPEC           = $0D000;           // Special decoding

  // Standard menu types.
  MT_BACKUP         = $0001;            // Backup, Undo
  MT_COPY           = $0002;            // Copy to clipboard
  MT_EDIT           = $0004;            // Edit, Modify, Assemble
  MT_SEARCH         = $0008;            // Search, Next
  MT_DHISTORY       = $0010;            // Previous, Next in history

type
  SPECFUNC = function(pc: PChar; a, b, c: ULONG; pdisam: p_disasm; i: Integer): ULONG; cdecl;

  p_dump = ^t_dump;
  t_dump = packed record                    // Current status of dump window
    table: t_table;                         // Treat dump window as custom table
    dimmed: Integer;                        // Draw in lowcolor if nonzero
    threadid: ULONG;                        // Use decoding and registers if not 0
    dumptype: Integer;                      // Current dump type, DU_xxx+count+size
    specdump: SPECFUNC;                     // Decoder of DU_SPEC dump types
    menutype: Integer;                      // Standard menues, MT_xxx
    itemwidth: Integer;                     // Length of displayed item, characters
    showstackframes: Integer;               // Show stack frames in address dump
    showstacklocals: Integer;               // Show names of locals in stack
    commentmode: Integer;                   // 0: comment, 1: source, 2: profile
    filename: array[0..MAX_PATH-1] of Char; // Name of displayed or backup file
    base: ULONG;                            // Start of memory block or file
    size: ULONG;                            // Size of memory block or file
    addr: ULONG;                            // Address of first displayed byte
    lastaddr: ULONG;                        // Address of last displayed byte + 1
    sel0: ULONG;                            // Address of first selected byte
    sel1: ULONG;                            // Last selected byte (not included!)
    startsel: ULONG;                        // Start of last selection
    captured: Integer;                      // Mouse is captured by dump
    reladdr: ULONG;                         // Addresses relative to this
    relname: array[0..SHORTLEN-1] of Char;  // Symbol for relative zero address base
    filecopy: PChar;                        // Copy of the file or NULL
    backup: PChar;                          // Old backup of memory/file or NULL
    runtraceoffset: Integer;                // Offset back in run trace
    reserved: array[0..7] of ULONG;         // Reserved for the future extentions
  end;

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// WINDOW FUNCTIONS //////////////////////////////

const
  WM_USER_MENU      = WM_USER + 101;        // Activate context-sensitive menu
  WM_USER_SCR       = WM_USER + 102;        // Redraw scroll(s)
  WM_USER_SAVE      = WM_USER + 103;        // Save data to disk
  WM_USER_VABS      = WM_USER + 104;        // Scroll contents of window by lines
  WM_USER_VREL      = WM_USER + 105;        // Scroll contents of window by percent
  WM_USER_VBYTE     = WM_USER + 106;        // Scroll contents of window by bytes
  WM_USER_STS       = WM_USER + 107;        // Start selection in window
  WM_USER_CNTS      = WM_USER + 108;        // Continue selection in window
  WM_USER_CHGS      = WM_USER + 109;        // Move single-line selection
  WM_USER_BAR       = WM_USER + 110;        // Message from bar segment as button
  WM_USER_DBLCLK    = WM_USER + 111;        // Doubleclick in column
  WM_USER_SIZE      = WM_USER + 112;        // Resize children in CPU window
  WM_USER_FOCUS     = WM_USER + 113;        // Set focus to child of CPU window
  WM_USER_FILE      = WM_USER + 114;        // Change state of file dump
  WM_USER_HERE      = WM_USER + 115;        // Query presence list
  WM_USER_CHALL     = WM_USER + 116;        // Redraw (almost) everything
  WM_USER_CHMEM     = WM_USER + 117;        // Range of debuggee's memory changed
  WM_USER_CHREG     = WM_USER + 118;        // Debuggee's register(s) changed
  WM_USER_CHNAM     = WM_USER + 119;        // Redraw name tables
  WM_USER_MOUSE     = WM_USER + 120;        // Check mouse coordinates
  WM_USER_KEY       = WM_USER + 121;        // Emulate WM_KEYDOWN
  WM_USER_SYSKEY    = WM_USER + 122;        // Emulate WM_SYSKEYDOWN

  // Constants used for scrolling and selection.
  MAXTRACK          = 16384;                // Maximal scroll of user-drawn table
  MOVETOP           = $7FFFFFFF;            // Move selection to top of table
  MOVEBOTTOM        = $7FFFFFFE;            // Move selection to bottom of table

  CONT_BROADCAST    = $0000;                // Continue sending msg to other windows
  STOP_BROADCAST    = $1234;                // Stop sending message to other windows

  // Dumpbackup() actions.
  BKUP_CREATE       = 1;                    // Create or update backup copy
  BKUP_VIEWDATA     = 2;                    // View original data
  BKUP_VIEWCOPY     = 3;                    // View backup copy
  BKUP_LOADCOPY     = 4;                    // Read backup copy from file
  BKUP_SAVEDATA     = 5;                    // Save original data to file
  BKUP_SAVECOPY     = 6;                    // Save backup copy to file
  BKUP_DELETE       = 7;                    // Delete backup copy

function Registerotclass(classname: PChar; iconname: PChar; classproc: Pointer): Integer; cdecl;

function Newtablewindow(pt: p_table; nlines: Integer; maxcolumns: Integer;
                        winclass: PChar; wintitle: PChar): HWND; cdecl;

function Quicktablewindow(pt: p_table; nlines: Integer; maxcolumns: Integer;
                          winclass: PChar; wintitle: PChar): HWND; cdecl;

function Createdumpwindow(name: PChar; base: ULONG; size: ULONG; addr: ULONG;
                          itype: Integer; specdump: SPECFUNC): HWND; cdecl;

procedure Setdumptype(pd: p_dump; dumptype: Integer); cdecl;

procedure Dumpbackup(pd: p_dump; action: Integer); cdecl;

function Broadcast(msg: Word; wp: WPARAM; lp: LPARAM): Integer; cdecl;

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DATA CONVERSION FUNCTIONS //////////////////////////

function Compress(bufin: PChar; nbufin: ULONG; bufout: PChar; nbufout: ULONG): ULONG; cdecl;

function Getoriginaldatasize(bufin: PChar; nbufin: ULONG): ULONG; cdecl;

function Decompress(bufin: PChar; nbufin: ULONG; bufout: PChar; nbufout: ULONG): ULONG; cdecl;

function Calculatecrc(copy: PChar; base: ULONG; size: ULONG;
                      pmod: p_module; fixupoffset: ULONG): ULONG; cdecl;

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// REFERENCES AND SEARCH ////////////////////////////

type
  p_ref = ^t_ref;
  t_ref = packed record                 // Description of reference
    addr: ULONG;                        // Address of reference
    size: ULONG;                        // 1: single command, otherwise size
    ltype: ULONG;                       // Type of reference, TY_xxx
    dest: ULONG;                        // Destination of call
  end;

function Findreferences(base: ULONG; size: ULONG; addr0: ULONG; addr1: ULONG;
  origin: ULONG; recurseonjump: Integer; title: PChar): Integer; cdecl;

function Findstrings(base: ULONG; size: ULONG; origin: ULONG; title: PChar): Integer; cdecl;

function Findalldllcalls(pd: p_dump; origin: ULONG; title: PChar): Integer; cdecl;

function Findallcommands(pd: p_dump; model: p_asmmodel; origin: ULONG; title: PChar): Integer; cdecl;

function Findallsequences(pd: p_dump; model: p_extmodels; origin: ULONG; title: PChar): Integer; cdecl;

function Walkreference(dir: Integer): ULONG; cdecl;

function Walkreferenceex(dir: Integer; size: PULONG): ULONG; cdecl;

////////////////////////////////////////////////////////////////////////////////
///////////////////////// BREAKPOINT AND TRACE FUNCTIONS ///////////////////////

const
  MEMBP_READ        = $0001;            // Memory breakpoint on read
  MEMBP_WRITE       = $0002;            // Memory breakpoint on write
  MEMBP_SFX         = $1000;            // Skip self-extraction

  // Bits of module hit trace buffer.
  TR_HIT            = $01;              // Command was executed during trace
  TR_SET            = $02;              // Code INT3 is in memory, copy valid
  TR_RTSKIP         = $20;              // Disable run trace on command
  TR_RTRACE         = $40;              // Force run trace on command
  TR_TRACE          = $80;              // Command is included in trace

  HB_FREE           = 0;                // Breakpoint is not used
  HB_CODE           = 1;                // Active on command execution
  HB_ACCESS         = 2;                // Active on read/write access
  HB_WRITE          = 3;                // Active on write access
  HB_IO             = 4;                // Active on port I/O
  HB_ONESHOT        = 5;                // One-shot on command execution
  HB_STOPAN         = 6;                // One-shot on command and stop
  HB_TEMP           = 7;                // Temporary on command execution

  // Parameter mode of function Modifyhittrace().
  ATR_ADD           = 1;                // Add trace to buffer
  ATR_ADDPROC       = 2;                // Add only recognized procedures
  ATR_RESET         = 3;                // Mark range as not traced
  ATR_REMOVE        = 4;                // Remove range and breakpoints
  ATR_REMOVEALL     = 5;                // Destroy range and breakpoints
  ATR_RESTORE       = 6;                // Restore breakpoints
  ATR_RTRADD        = 7;                // Add trace and force run trace
  ATR_RTRJUMPS      = 8;                // Add trace and run trace jumps only
  ATR_RTRENTRY      = 9;                // Add trace and run trace entries only
  ATR_RTREMOVE      = 10;               // Remove trace from range
  ATR_RTSKIP        = 11;               // Skip when run tracing

type
  p_bpoint = ^t_bpoint;
  t_bpoint = packed record              // Description of INT3 breakpoint
    addr: ULONG;                        // Address of breakpoint
    dummy: ULONG;                       // Always 1
    ltype: ULONG;                       // Type of breakpoint, TY_xxx
    cmd: Char;                          // Old value of command
    passcount: ULONG;                   // Actual pass count
  end;

  p_hardbpoint = ^t_hardbpoint;
  t_hardbpoint = packed record          // Description of hardware breakpoint
    addr: ULONG;                        // Base address of hardware breakpoint
    size: Integer;                      // Size of hardware breakpoint
    itype: Integer;                     // Type of breakpoint, one of HB_xxx
    reserved: array[0..3] of ULONG;     // Reserved for the future
  end;

function Setbreakpoint(addr: ULONG; ltype: ULONG; cmd: Char): Integer; cdecl;

function Setbreakpointext(addr: ULONG; ltype: ULONG; cmd: Char; passcount: ULONG): Integer; cdecl;

function Manualbreakpoint(addr: ULONG; key: Integer; shiftkey: Integer;
                          nametype: ULONG; font: Integer): Integer; cdecl;

procedure Deletebreakpoints(addr0: ULONG; addr1: ULONG; silent: Integer); cdecl;

function Getbreakpointtype(addr: ULONG): ULONG; cdecl;

function Getbreakpointtypecount(addr: ULONG; passcount: PULONG): ULONG; cdecl;

function Getnextbreakpoint(addr: ULONG; ptype: PULONG; cmd: PInteger): ULONG; cdecl;

procedure Tempbreakpoint(addr: ULONG; mode: Integer); cdecl;

function Hardbreakpoints(closeondelete: Integer): Integer; cdecl;

function Sethardwarebreakpoint(addr: ULONG; size: Integer; itype: Integer): Integer; cdecl;

function Deletehardwarebreakpoint(index: Integer): Integer; cdecl;

function Deletehardwarebreakbyaddr(addr: ULONG): Integer; cdecl;

function Setmembreakpoint(itype: Integer; addr: ULONG; size: ULONG): Integer; cdecl;

function Findhittrace(addr: ULONG; ptracecopy: PPChar; psize: PULONG): PChar; cdecl;

function Modifyhittrace(addr0: ULONG; addr1: ULONG; mode: Integer): Integer; cdecl;

function Isretaddr(retaddr: ULONG; procaddr: PULONG): ULONG; cdecl;

function Creatertracewindow: HWND; cdecl;

procedure Settracecondition(cond: PChar; onsuspicious: Integer;
                            in0: ULONG; in1: ULONG;
                            out0: ULONG; out1: ULONG); cdecl;

procedure Settracecount(count: ULONG); cdecl;

procedure Settracepauseoncommands(cmdset: PChar); cdecl;

function Startruntrace(preg: p_reg): Integer; cdecl;

procedure Deleteruntrace; cdecl;

function Runtracesize: Integer; cdecl;

function Findprevruntraceip(ip: ULONG; startback: Integer): Integer; cdecl;

function Findnextruntraceip(ip: ULONG; startback: Integer): Integer; cdecl;

function Getruntraceregisters(nback: Integer; preg: p_reg; pold: p_reg;
                              cmd: PChar; comment: PChar): Integer; cdecl;

function Getruntraceprofile(addr: ULONG; size: ULONG; profile: PULONG): Integer; cdecl;

procedure Scrollruntracewindow(back: Integer); cdecl;

function Createprofilewindow(base: ULONG; size: ULONG): HWND; cdecl;


////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// CODE INJECTION ///////////////////////////////

type
  INJECTANSWER = function(data: PChar; datasize: ULONG; parm2: ULONG): Integer; cdecl;

  p_inject = ^t_inject;
  t_inject = packed record
    codesize: ULONG;
    code: PChar;
    stacksize: Integer;
    datatype: Integer;
  end;

function Injectcode(threadid: ULONG; inject: p_inject; data: PChar;
                    datasize: ULONG; parm1: ULONG; parm2: ULONG;
                    answerfunc: INJECTANSWER): Integer; cdecl;


////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CPU-SPECIFIC FUNCTIONS ///////////////////////////

// Setcpu modes.
const
  CPU_ASMHIST       = $00001;           // Add change to Disassembler history
  CPU_ASMCENTER     = $00004;           // Make address in the middle of window
  CPU_ASMFOCUS      = $00008;           // Move focus to Disassembler
  CPU_DUMPHIST      = $00010;           // Add change to Dump history
  CPU_DUMPFIRST     = $00020;           // Make address the first byte in Dump
  CPU_DUMPFOCUS     = $00080;           // Move focus to Dump
  CPU_REGAUTO       = $00100;           // Automatically switch to FPU/MMX/3DNow!
  CPU_RUNTRACE      = $00200;           // Show run trace data at offset asmaddr
  CPU_STACKFOCUS    = $00800;           // Move focus to Stack
  CPU_NOCREATE      = $04000;           // Don't create CPU window if absent
  CPU_REDRAW        = $08000;           // Redraw CPU window immediately
  CPU_NOFOCUS       = $10000;           // Don't assign focus to main window


procedure Setcpu(threadid: ULONG; asmaddr: ULONG; dumpaddr: ULONG;
                 stackaddr: ULONG; mode: Integer); cdecl;

procedure Setdisasm(asmaddr: ULONG; selsize: ULONG; mode: Integer); cdecl;

procedure Redrawdisassembler; cdecl;

procedure Getdisassemblerrange(pbase: PULONG; psize: PULONG); cdecl;

function Findprocbegin(addr: ULONG): ULONG; cdecl;

function Findprocend(addr: ULONG): ULONG; cdecl;

function Findprevproc(addr: ULONG): ULONG; cdecl;

function Findnextproc(addr: ULONG): ULONG; cdecl;

function Getproclimits(addr: ULONG; pstart: PULONG; pend: PULONG): Integer; cdecl;

procedure Sendshortcut(where: Integer; addr: ULONG; msg: Integer;
                       ctrl: Integer; shift: Integer; vkcode: Integer); cdecl;

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// PROCESS CONTROL ////////////////////////////////

// Parameter stepmode in call to Go().
const
  STEP_SAME         = 0;                // Same action as on previous call
  STEP_RUN          = 1;                // Run program
  STEP_OVER         = 2;                // Step over
  STEP_IN           = 3;                // Step in
  STEP_SKIP         = 4;                // Skip sequence

type
  t_status = (                          // Thread/process status
    STAT_NONE,                          // Thread/process is empty
    STAT_STOPPED,                       // Thread/process suspended
    STAT_EVENT,                         // Processing debug event, process paused
    STAT_RUNNING,                       // Thread/process running
    STAT_FINISHED,                      // Process finished
    STAT_CLOSING                        // Process is requested to terminate
  );

function Getstatus:  t_status; cdecl;

function Go(threadid: ULONG; tilladdr: ULONG; stepmode: Integer;
            givechance: Integer; backupregs: Integer): Integer; cdecl;

function Suspendprocess(processevents: Integer): Integer; cdecl;

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////// DECODING OF ARGUMENTS /////////////////////////////

const
  DASC_TEST         = 0;                // Test whether a string
  DASC_NOHEX        = 1;                // Test, print nothing if not a string
  DASC_ASCII        = 2;                // Force ASCII
  DASC_PASCAL       = 3;                // Force Pascal

function Findknownfunction(addr: ULONG; direct: Integer; level: Integer;
                           fname: PChar): PChar; cdecl;

function Decodeknownargument(addr: ULONG; arg: PChar; value: ULONG; valid: Integer;
                             s: PChar; mask: PChar; pset: PPChar): Integer; cdecl;

function Findunknownfunction(ip: ULONG; code: PChar; dec: PChar;
                             size: ULONG; fname: PChar): PChar; cdecl;

function Decodeascii(value: ULONG; s: PChar; len: Integer; mode: Integer): Integer; cdecl;

function Decodeunicode(value: ULONG; s: PChar; len: Integer): Integer; cdecl;

/////////////////////////////////////////////////////////////////////////////////
////////////////////////////// SOURCE CODE SUPPORT //////////////////////////////

const
  FIRST_CODE_LINE = $FFFFFFFF;          // First available source line

type
  p_sourceline = ^t_sourceline;
  t_sourceline = packed record          // Source line descriptor
    addr: ULONG;                        // Start address of corresponding code
    size: ULONG;                        // Code size
    srcname: ULONG;                     // Index of source name (as NM_DEBUG)
    line: ULONG;                        // 0-based line number
  end;


function Showsourcefromaddress(addr: ULONG; show: Integer): HWND; cdecl;

function Getresourcestring(pm: p_module; id: ULONG; s: PChar): Integer; cdecl;

function Getlinefromaddress(addr: ULONG): p_sourceline; cdecl;

function Getaddressfromline(addr0: ULONG; addr1: ULONG; path: PChar;
                            line: ULONG): ULONG; cdecl;

function Getsourcefilelimits(nameaddr: ULONG; addr0: PULONG; addr1: PULONG): Integer; cdecl;

function Decodefullvarname(pmod: p_module; psym: p_symvar; offset: Integer;
                           name: PChar): Integer; cdecl;

function Getbprelname(pmod: p_module; addr: ULONG; offset: LongInt;
                      s: PChar; nsymb: Integer): Integer; cdecl;

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// WATCH SUPPORT /////////////////////////////////

function Createwatchwindow: HWND; cdecl;

function Deletewatch(indexone: Integer): Integer; cdecl;

function Insertwatch(indexone: Integer; text: PChar): Integer; cdecl;

function Getwatch(indexone: Integer; text: PChar): Integer; cdecl;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// WINDOWS-SPECIFIC FUNCTIONS //////////////////////////

const
  MAXNEST           = 32;               // Max allowed code structure nesting

type
  p_window = ^t_window;
  t_window = packed record                    // Description of window
    hwnd: ULONG;                              // Window's handle
    dummy: ULONG;                             // Must be 1
    ltype: ULONG;                             // Type of window, TY_xxx
    parenthw: ULONG;                          // Handle of parent or 0
    winproc: ULONG;                           // Address of WinProc or 0
    threadid: ULONG;                          // ID of the owning thread
    exstyle: ULONG;                           // Extended style
    style: ULONG;                             // Style
    id: ULONG;                                // Identifier
    classproc: ULONG;                         // Address of default (class) WinProc
    child: Integer;                           // Index of next child
    level: Integer;                           // Level in genealogy (0: topmost)
    sibling: Integer;                         // Index of next sibling
    byparent: Integer;                        // Index when sorted by parent
    title: array[0..TEXTLEN-1] of Char;       // Window's title
    classname: array[0..TEXTLEN-1] of Char;   // Class name
    tree: array[0..MAXNEST-1] of Char;        // Tree display
  end;

function Createwinwindow: HWND; cdecl;

/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// PATCHES ////////////////////////////////////

type
  p_patch = ^t_patch;
  t_patch = packed record
    addr: ULONG;                        // Base address of patch in memory
    size: ULONG;                        // Size of patch, bytes
    ltype: ULONG;                       // Type of patch, set of TY_xxx
    orig: array[0..TEXTLEN-1] of Char;  // Original code
    modify: array[0..TEXTLEN-1] of Char;// Patched code
  end;

function Createpatchwindow: HWND; cdecl;


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PLUGIN-SPECIFIC FUNCTIONS ///////////////////////////

// Parameters of Plugingetvalue().
const
  VAL_HINST         = 1;                // Current program instance
  VAL_HWMAIN        = 2;                // Handle of the main window
  VAL_HWCLIENT      = 3;                // Handle of the MDI client window
  VAL_NCOLORS       = 4;                // Number of common colors
  VAL_COLORS        = 5;                // RGB values of common colors
  VAL_BRUSHES       = 6;                // Handles of common color brushes
  VAL_PENS          = 7;                // Handles of common color pens
  VAL_NFONTS        = 8;                // Number of common fonts
  VAL_FONTS         = 9;                // Handles of common fonts
  VAL_FONTNAMES     = 10;               // Internal font names
  VAL_FONTWIDTHS    = 11;               // Average widths of common fonts
  VAL_FONTHEIGHTS   = 12;               // Average heigths of common fonts
  VAL_NFIXFONTS     = 13;               // Actual number of fixed-pitch fonts
  VAL_DEFFONT       = 14;               // Index of default font
  VAL_NSCHEMES      = 15;               // Number of color schemes
  VAL_SCHEMES       = 16;               // Color schemes
  VAL_DEFSCHEME     = 17;               // Index of default colour scheme
  VAL_DEFHSCROLL    = 18;               // Default horizontal scroll
  VAL_RESTOREWINDOWPOS = 19;            // Restore window positions from .ini
  VAL_HPROCESS      = 20;               // Handle of Debuggee
  VAL_PROCESSID     = 21;               // Process ID of Debuggee
  VAL_HMAINTHREAD   = 22;               // Handle of main thread
  VAL_MAINTHREADID  = 23;               // Thread ID of main thread
  VAL_MAINBASE      = 24;               // Base of main module in the process
  VAL_PROCESSNAME   = 25;               // Name of the active process
  VAL_EXEFILENAME   = 26;               // Name of the main debugged file
  VAL_CURRENTDIR    = 27;               // Current directory for debugged process
  VAL_SYSTEMDIR     = 28;               // Windows system directory
  VAL_DECODEANYIP   = 29;               // Decode registers dependless on EIP
  VAL_PASCALSTRINGS = 30;               // Decode Pascal-style string constants
  VAL_ONLYASCII     = 31;               // Only printable ASCII chars in dump
  VAL_DIACRITICALS  = 32;               // Allow diacritical symbols in strings
  VAL_GLOBALSEARCH  = 33;               // Search from the beginning of block
  VAL_ALIGNEDSEARCH = 34;               // Search aligned to item's size
  VAL_IGNORECASE    = 35;               // Ignore case in string search
  VAL_SEARCHMARGIN  = 36;               // Floating search allows error margin
  VAL_KEEPSELSIZE   = 37;               // Keep size of hex edit selection
  VAL_MMXDISPLAY    = 38;               // MMX display mode in dialog
  VAL_WINDOWFONT    = 39;               // Use calling window's font in dialog
  VAL_TABSTOPS      = 40;               // Distance between tab stops
  VAL_MODULES       = 41;               // Table of modules (.EXE and .DLL)
  VAL_MEMORY        = 42;               // Table of allocated memory blocks
  VAL_THREADS       = 43;               // Table of active threads
  VAL_BREAKPOINTS   = 44;               // Table of active breakpoints
  VAL_REFERENCES    = 45;               // Table with found references
  VAL_SOURCELIST    = 46;               // Table of source files
  VAL_WATCHES       = 47;               // Table of watches
  VAL_CPUFEATURES   = 50;               // CPU feature bits
  VAL_TRACEFILE     = 51;               // Handle of run trace log file
  VAL_ALIGNDIALOGS  = 52;               // Whether to align dialogs
  VAL_CPUDASM       = 53;               // Dump descriptor of CPU Disassembler
  VAL_CPUDDUMP      = 54;               // Dump descriptor of CPU Dump
  VAL_CPUDSTACK     = 55;               // Dump descriptor of CPU Stack
  VAL_APIHELP       = 56;               // Name of selected API help file
  VAL_HARDBP        = 57;               // Whether hardware breakpoints enabled
  VAL_PATCHES       = 58;               // Table of patches
  VAL_HINTS         = 59;               // Sorted data with analysis hints

function Registerpluginclass(classname: PChar; iconname: PChar;
                             dllinst: THandle; classproc: TFarProc): Integer; cdecl;

procedure Unregisterpluginclass(classname: PChar); cdecl;

function Pluginwriteinttoini(dllinst: THandle; key: PChar; value: Integer): Integer; cdecl;

function Pluginwritestringtoini(dllinst: THandle; key: PChar; s: PChar): Integer; cdecl;

function Pluginreadintfromini(dllinst: THandle; key: PChar; def: Integer): Integer; cdecl;

function Pluginreadstringfromini(dllinst: THandle; key: PChar;
                                 s: PChar; def: PChar): Integer; cdecl;

function Pluginsaverecord(tag: ULONG; size: ULONG; data: Pointer): Integer; cdecl;

function Plugingetvalue(itype: Integer): Integer; cdecl;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////// EXPORTED PLUGIN CALLBACK FUNCTIONS //////////////////////

// Origins of standard OllyDbg windows as passed to plugin. In parenthesis is
// the type of item you get in ODBG_Pluginmenu(), ODBG_Pluginaction() and
// ODBG_Pluginshortcut(). Note that this item can be NULL!
const
  PM_MAIN           = 0;                // Main window (NULL)
  PM_DUMP           = 10;               // Any Dump window (t_dump* )
  PM_MODULES        = 11;               // Modules window (t_module* )
  PM_MEMORY         = 12;               // Memory window (t_memory* )
  PM_THREADS        = 13;               // Threads window (t_thread* )
  PM_BREAKPOINTS    = 14;               // Breakpoints window (t_bpoint* )
  PM_REFERENCES     = 15;               // References window (t_ref* )
  PM_RTRACE         = 16;               // Run trace window (int* )
  PM_WATCHES        = 17;               // Watches window (1-based index)
  PM_WINDOWS        = 18;               // Windows window (t_window* )
  PM_DISASM         = 31;               // CPU Disassembler (t_dump* )
  PM_CPUDUMP        = 32;               // CPU Dump (t_dump* )
  PM_CPUSTACK       = 33;               // CPU Stack (t_dump* )
  PM_CPUREGS        = 34;               // CPU Registers (t_reg* )

  // Reasons why debugged application was paused, as a first argument in call to
  // ODBG_Paused(), ODBG_Pausedex() and ODBG_Plugincmd().
  PP_MAIN           = $0003;            // Mask to extract main reason
    PP_EVENT        = $0000;            // Paused on debugging event
    PP_PAUSE        = $0001;            // Paused on user's request
    PP_TERMINATED   = $0002;            // Application terminated

  // Extended reasons in ODBG_Pausedex().
  PP_BYPROGRAM      = $0004;            // Debugging event caused by program
  PP_INT3BREAK      = $0010;            // INT3 breakpoint
  PP_MEMBREAK       = $0020;            // Memory breakpoint
  PP_HWBREAK        = $0040;            // Hardware breakpoint
  PP_SINGLESTEP     = $0080;            // Single-step trap
  PP_EXCEPTION      = $0100;            // Exception, like division by 0
  PP_ACCESS         = $0200;            // Access violation
  PP_GUARDED        = $0400;            // Guarded page

  // Record tags in .udd files.
  MI_SIGNATURE      = $00646F4D;        // Module info signature
  MI_VERSION        = $7265560A;        // OllyDbg version
  MI_FILENAME       = $6C69460A;        // Record with full name of executable
  MI_FILESIZE       = $7A69530A;        // Record with file size
  MI_TIMESTAMP      = $7473540A;        // Record with timestamp file data
  MI_SFXENTRY       = $6566530A;        // Real entry of SFX-able module
  MI_PATCH          = $7461500A;        // Record with patch data
  MI_USER           = $0073550A;        // Record with user-defined label/comment
  MI_PROCDATA       = $6372500A;        // Record with procedure data
  MI_SWDATA         = $6977530A;        // Record with switch data
  MI_CALLFINT       = $6966430A;        // Record with internal call
  MI_CALLFMOD       = $6D66430A;        // Record with intermodular call
  MI_CALLFABS       = $6166430A;        // Record with absolute call
  MI_INT3BREAK      = $7470420A;        // Record with breakpoint data
  MI_INT3BRKC       = $6370420A;        // Record with checked breakpoint data
  MI_HDWRBREAK      = $7262480A;        // Record with hardware breakpoint data
  MI_JDDATA         = $74644A0A;        // Record with all module jump data
  MI_ANALYSIS       = $616E410A;        // Record with analysis data
  MI_ANALPACK       = $636E410A;        // Record with compressed analysis data
  MI_AHINT          = $7468410A;        // Record with analysis hint data
  MI_TRACE          = $6172540A;        // Record with trace data
  MI_TRACEPACK      = $6372540A;        // Record with compressed trace data
  MI_CODECRC        = $7263430A;        // Record with CRC of code for analysis
  MI_SAVEAREA       = $6176530A;        // Record with general-purpose save area
  MI_END            = $646E450A;        // End of module info data

  // Tags reserved for 3rd-party plugins.
  MI_WINJUG         = $67754A0A;        // WindowJuggler by EsseEmme
  MI_WINJU1         = $31754A0A;
  MI_WINJU2         = $32754A0A;
  MI_WINJU3         = $33754A0A;
  MI_APPST          = $73614F0A;        // OllyAppStarter by Homunculus

// Prototypes for plugin callback functions.
type
  TODBG_Plugindata = function(shortname: PChar): Integer; cdecl;

  TODBG_Plugininit = function(ollydbgversion: Integer; hw: HWND;
                              features: PULONG): Integer; cdecl;

  TODBG_Pluginmainloop = procedure(debugevent: PDebugEvent); cdecl;

  TODBG_Pluginsaveudd = procedure(pmod: p_module; ismainmodule: Integer); cdecl;

  TODBG_Pluginuddrecord = function(pmod: p_module; ismainmodule: Integer; tag: ULONG;
                                   size: ULONG; data: Pointer): Integer; cdecl;

  TODBG_Pluginmenu = function(origin: Integer; data: PChar;
                              item: Pointer): Integer; cdecl;

  TODBG_Pluginaction = procedure(origin: Integer; action: Integer;
                                item: Pointer); cdecl;

  TODBG_Pluginshortcut =  function(origin: Integer; ctrl: Integer; alt: Integer;
                                   shift: Integer; key: Integer;
                                   item: Pointer): Integer; cdecl;

  TODBG_Pluginreset = procedure; cdecl;

  TODBG_Pluginclose = function: Integer; cdecl;

  TODBG_Plugindestroy = procedure; cdecl;

  TODBG_Paused = function(reason: Integer; reg: p_reg): Integer; cdecl;

  TODBG_Pausedex = function(reasonex: Integer; dummy: Integer; reg: p_reg;
                            debugevent: PDebugEvent): Integer; cdecl;

  TODBG_Plugincmd = function(reason: Integer; reg: p_reg; cmd: PChar): Integer; cdecl;

implementation

const
  OLLYDBG = 'OllyDbg.exe';

function  Addsorteddata; external OLLYDBG name '_Addsorteddata';
procedure Addtolist; external OLLYDBG name '_Addtolist';
function  Analysecode; external OLLYDBG name '_Analysecode';
procedure Animate; external OLLYDBG name '_Animate';
function  Assemble; external OLLYDBG name '_Assemble';
function  Attachtoactiveprocess; external OLLYDBG name '_Attachtoactiveprocess';
function  Broadcast; external OLLYDBG name '_Broadcast';
function  Browsefilename; external OLLYDBG name '_Browsefilename';
function  Calculatecrc; external OLLYDBG name '_Calculatecrc';
function  Checkcondition; external OLLYDBG name '_Checkcondition';
function  Compress; external OLLYDBG name '_Compress';
function  Createdumpwindow; external OLLYDBG name '_Createdumpwindow';
function  Createlistwindow; external OLLYDBG name '_Createlistwindow';
function  Createpatchwindow; external OLLYDBG name '_Createpatchwindow';
function  Createprofilewindow; external OLLYDBG name '_Createprofilewindow';
function  Creatertracewindow; external OLLYDBG name '_Creatertracewindow';
function  Createsorteddata; external OLLYDBG name '_Createsorteddata';
function  Createthreadwindow; external OLLYDBG name '_Createthreadwindow';
function  Createwatchwindow; external OLLYDBG name '_Createwatchwindow';
function  Createwinwindow; external OLLYDBG name '_Createwinwindow';
function  Decodeaddress; external OLLYDBG name '_Decodeaddress';
function  Decodeascii; external OLLYDBG name '_Decodeascii';
function  Decodecharacter; external OLLYDBG name '_Decodecharacter';
function  Decodefullvarname; external OLLYDBG name '_Decodefullvarname';
function  Decodeknownargument; external OLLYDBG name '_Decodeknownargument';
function  Decodename; external OLLYDBG name '_Decodename';
function  Decoderange; external OLLYDBG name '_Decoderange';
function  Decoderelativeoffset; external OLLYDBG name '_Decoderelativeoffset';
function  Decodethreadname; external OLLYDBG name '_Decodethreadname';
function  Decodeunicode; external OLLYDBG name '_Decodeunicode';
function  Decompress; external OLLYDBG name '_Decompress';
procedure Defaultbar; external OLLYDBG name '_Defaultbar';
procedure Deletebreakpoints; external OLLYDBG name '_Deletebreakpoints';
function  Deletehardwarebreakbyaddr; external OLLYDBG name '_Deletehardwarebreakbyaddr';
function  Deletehardwarebreakpoint; external OLLYDBG name '_Deletehardwarebreakpoint';
procedure Deletenamerange; external OLLYDBG name '_Deletenamerange';
function  Deletenonconfirmedsorteddata; external OLLYDBG name '_Deletenonconfirmedsorteddata';
procedure Deleteruntrace; external OLLYDBG name '_Deleteruntrace';
procedure Deletesorteddata; external OLLYDBG name '_Deletesorteddata';
procedure Deletesorteddatarange; external OLLYDBG name '_Deletesorteddatarange';
function  Deletewatch; external OLLYDBG name '_Deletewatch';
function  Demanglename; external OLLYDBG name '_Demanglename';
procedure Destroysorteddata; external OLLYDBG name '_Destroysorteddata';
function  Disasm; external OLLYDBG name '_Disasm';
function  Disassembleback; external OLLYDBG name '_Disassembleback';
function  Disassembleforward; external OLLYDBG name '_Disassembleforward';
procedure Discardquicknames; external OLLYDBG name '_Discardquicknames';
procedure Dumpbackup; external OLLYDBG name '_Dumpbackup';
procedure Error; external OLLYDBG name '_Error';
function  Expression; external OLLYDBG name '_Expression';
function  Findallcommands; external OLLYDBG name '_Findallcommands';
function  Findalldllcalls; external OLLYDBG name '_Findalldllcalls';
function  Findallsequences; external OLLYDBG name '_Findallsequences';
function  Finddecode; external OLLYDBG name '_Finddecode';
function  Findfileoffset; external OLLYDBG name '_Findfileoffset';
function  Findfixup; external OLLYDBG name '_Findfixup';
function  Findhittrace; external OLLYDBG name '_Findhittrace';
function  Findimportbyname; external OLLYDBG name '_Findimportbyname';
function  Findknownfunction; external OLLYDBG name '_Findknownfunction';
function  Findlabel; external OLLYDBG name '_Findlabel';
function  Findlabelbyname; external OLLYDBG name '_Findlabelbyname';
function  Findmemory; external OLLYDBG name '_Findmemory';
function  Findmodule; external OLLYDBG name '_Findmodule';
function  Findname; external OLLYDBG name '_Findname';
function  Findnextname; external OLLYDBG name '_Findnextname';
function  Findnextproc; external OLLYDBG name '_Findnextproc';
function  Findnextruntraceip; external OLLYDBG name '_Findnextruntraceip';
function  Findprevproc; external OLLYDBG name '_Findprevproc';
function  Findprevruntraceip; external OLLYDBG name '_Findprevruntraceip';
function  Findprocbegin; external OLLYDBG name '_Findprocbegin';
function  Findprocend; external OLLYDBG name '_Findprocend';
function  Findreferences; external OLLYDBG name '_Findreferences';
function  Findsorteddata; external OLLYDBG name '_Findsorteddata';
function  Findsorteddataindex; external OLLYDBG name '_Findsorteddataindex';
function  Findsorteddatarange; external OLLYDBG name '_Findsorteddatarange';
function  Findstrings; external OLLYDBG name '_Findstrings';
function  Findsymbolicname; external OLLYDBG name '_Findsymbolicname';
function  Findthread; external OLLYDBG name '_Findthread';
function  Findunknownfunction; external OLLYDBG name '_Findunknownfunction';
procedure Flash; external OLLYDBG name '_Flash';
function  Followcall; external OLLYDBG name '_Followcall';
function  Get3dnow; external OLLYDBG name '_Get3dnow';
function  Get3dnowxy; external OLLYDBG name '_Get3dnowxy';
function  Getaddressfromline; external OLLYDBG name '_Getaddressfromline';
procedure Getasmfindmodel; external OLLYDBG name '_Getasmfindmodel';
procedure Getasmfindmodelxy; external OLLYDBG name '_Getasmfindmodelxy';
function  Getbprelname; external OLLYDBG name '_Getbprelname';
function  Getbreakpointtype; external OLLYDBG name '_Getbreakpointtype';
function  Getbreakpointtypecount; external OLLYDBG name '_Getbreakpointtypecount';
function  Getcputhreadid; external OLLYDBG name '_Getcputhreadid';
procedure Getdisassemblerrange; external OLLYDBG name '_Getdisassemblerrange';
function  Getfloat10; external OLLYDBG name '_Getfloat10';
function  Getfloat10xy; external OLLYDBG name '_Getfloat10xy';
function  Getfloat; external OLLYDBG name '_Getfloat';
function  Getfloatxy; external OLLYDBG name '_Getfloatxy';
function  Gethexstring; external OLLYDBG name '_Gethexstring';
function  Gethexstringxy; external OLLYDBG name '_Gethexstringxy';
function  Getline; external OLLYDBG name '_Getline';
function  Getlinefromaddress; external OLLYDBG name '_Getlinefromaddress';
function  Getlinexy; external OLLYDBG name '_Getlinexy';
function  Getlong; external OLLYDBG name '_Getlong';
function  Getlongxy; external OLLYDBG name '_Getlongxy';
function  Getmmx; external OLLYDBG name '_Getmmx';
function  Getmmxxy; external OLLYDBG name '_Getmmxxy';
function  Getnextbreakpoint; external OLLYDBG name '_Getnextbreakpoint';
function  Getoriginaldatasize; external OLLYDBG name '_Getoriginaldatasize';
function  Getproclimits; external OLLYDBG name '_Getproclimits';
function  Getregxy; external OLLYDBG name '_Getregxy';
function  Getresourcestring; external OLLYDBG name '_Getresourcestring';
function  Getruntraceprofile; external OLLYDBG name '_Getruntraceprofile';
function  Getruntraceregisters; external OLLYDBG name '_Getruntraceregisters';
function  Getsortedbyselection; external OLLYDBG name '_Getsortedbyselection';
function  Getsourcefilelimits; external OLLYDBG name '_Getsourcefilelimits';
function  Getstatus; external OLLYDBG name '_Getstatus';
function  Gettableselectionxy; external OLLYDBG name '_Gettableselectionxy';
function  Gettext; external OLLYDBG name '_Gettext';
function  Gettextxy; external OLLYDBG name '_Gettextxy';
function  Getwatch; external OLLYDBG name '_Getwatch';
function  Go; external OLLYDBG name '_Go';
function  Guardmemory; external OLLYDBG name '_Guardmemory';
function  Hardbreakpoints; external OLLYDBG name '_Hardbreakpoints';
procedure Havecopyofmemory; external OLLYDBG name '_Havecopyofmemory';
procedure Infoline; external OLLYDBG name '_Infoline';
function  Injectcode; external OLLYDBG name '_Injectcode';
function  Insertname; external OLLYDBG name '_Insertname';
function  Insertwatch; external OLLYDBG name '_Insertwatch';
function  Isfilling; external OLLYDBG name '_Isfilling';
function  Isprefix; external OLLYDBG name '_Isprefix';
function  Isretaddr; external OLLYDBG name '_Isretaddr';
function  Issuspicious; external OLLYDBG name '_Issuspicious';
function  IstextA; external OLLYDBG name '_IstextA';
function  IstextW; external OLLYDBG name '_IstextW';
function  Listmemory; external OLLYDBG name '_Listmemory';
function  Manualbreakpoint; external OLLYDBG name '_Manualbreakpoint';
procedure Mergequicknames; external OLLYDBG name '_Mergequicknames';
procedure Message; external OLLYDBG name '_Message';
function  Modifyhittrace; external OLLYDBG name '_Modifyhittrace';
function  Newtablewindow; external OLLYDBG name '_Newtablewindow';
function  OpenEXEfile; external OLLYDBG name '_OpenEXEfile';
procedure Painttable; external OLLYDBG name '_Painttable';
function  Plugingetvalue; external OLLYDBG name '_Plugingetvalue';
function  Pluginreadintfromini; external OLLYDBG name '_Pluginreadintfromini';
function  Pluginreadstringfromini; external OLLYDBG name '_Pluginreadstringfromini';
function  Pluginsaverecord; external OLLYDBG name '_Pluginsaverecord';
function  Pluginwriteinttoini; external OLLYDBG name '_Pluginwriteinttoini';
function  Pluginwritestringtoini; external OLLYDBG name '_Pluginwritestringtoini';
function  Print3dnow; external OLLYDBG name '_Print3dnow';
function  Printfloat10; external OLLYDBG name '_Printfloat10';
function  Printfloat4; external OLLYDBG name '_Printfloat4';
function  Printfloat8; external OLLYDBG name '_Printfloat8';
function  Printsse; external OLLYDBG name '_Printsse';
procedure Progress; external OLLYDBG name '_Progress';
function  Quickinsertname; external OLLYDBG name '_Quickinsertname';
function  Quicktablewindow; external OLLYDBG name '_Quicktablewindow';
function  Readcommand; external OLLYDBG name '_Readcommand';
function  Readmemory; external OLLYDBG name '_Readmemory';
procedure Redrawdisassembler; external OLLYDBG name '_Redrawdisassembler';
function  Registerotclass; external OLLYDBG name '_Registerotclass';
function  Registerpluginclass; external OLLYDBG name '_Registerpluginclass';
procedure Restoreallthreads; external OLLYDBG name '_Restoreallthreads';
function  Runsinglethread; external OLLYDBG name '_Runsinglethread';
function  Runtracesize; external OLLYDBG name '_Runtracesize';
procedure Scrollruntracewindow; external OLLYDBG name '_Scrollruntracewindow';
procedure Selectandscroll; external OLLYDBG name '_Selectandscroll';
procedure Sendshortcut; external OLLYDBG name '_Sendshortcut';
function  Setbreakpoint; external OLLYDBG name '_Setbreakpoint';
function  Setbreakpointext; external OLLYDBG name '_Setbreakpointext';
procedure Setcpu; external OLLYDBG name '_Setcpu';
procedure Setdisasm; external OLLYDBG name '_Setdisasm';
procedure Setdumptype; external OLLYDBG name '_Setdumptype';
function  Sethardwarebreakpoint; external OLLYDBG name '_Sethardwarebreakpoint';
function  Setmembreakpoint; external OLLYDBG name '_Setmembreakpoint';
procedure Settracecondition; external OLLYDBG name '_Settracecondition';
procedure Settracecount; external OLLYDBG name '_Settracecount';
procedure Settracepauseoncommands; external OLLYDBG name '_Settracepauseoncommands';
function  Showsourcefromaddress; external OLLYDBG name '_Showsourcefromaddress';
function  Sortsorteddata; external OLLYDBG name '_Sortsorteddata';
function  Startruntrace; external OLLYDBG name '_Startruntrace';
function  Stringtotext; external OLLYDBG name '_Stringtotext';
function  Suspendprocess; external OLLYDBG name '_Suspendprocess';
function  Tablefunction; external OLLYDBG name '_Tablefunction';
procedure Tempbreakpoint; external OLLYDBG name '_Tempbreakpoint';
procedure Unregisterpluginclass; external OLLYDBG name '_Unregisterpluginclass';
procedure Updatelist; external OLLYDBG name '_Updatelist';
function  Walkreference; external OLLYDBG name '_Walkreference';
function  Walkreferenceex; external OLLYDBG name '_Walkreferenceex';
function  Writememory; external OLLYDBG name '_Writememory';

end.
