;////////////////////////////////////////////////////////////////////////////////
;//                                                                            //
;//                         SAMPLE PLUGIN FOR OLLYDBG                          //
;//                                                                            //
;// This plugin allows to set up to 10 code bookmarks using keyboard shortcuts //
;
;// or popup menus in Disassembler and then quickly return to one of the       //
;// bookmarks using shortcuts, popup menu or Bookmark window. Bookmarks        //
;// are kept between sessions in .udd file.                                    //
;//                                                                            //
;////////////////////////////////////////////////////////////////////////////////

.386
.model flat, stdcall  ; 32 bit memory model
option scoped         ; local labels are enabled, global labels inside
                      ; PROC should be defined with double colons (LABEL::)
option casemap :none  ; case sensitive


include windows.inc
include kernel32.inc
include user32.inc
include ..\plugin.inc

includelib kernel32.lib
includelib user32.lib
includelib ..\ollydbg.lib

.data?

hinst               HINSTANCE        ?  ; DLL instance
hwmain              HWND             ?  ; Handle of main OllyDbg window
bookmarkwinclass    db 32 dup (?)       ; Name of bookmark window class

; OllyDbg supports and makes extensive use of special kind of data collections
; called sorted tables. A table consists of descriptor (t_table) and data. All

; data elements has same size and begin with a 3-dword header: address, size
; and type. Table automatically sorts items by address, overlapping is not
; allowed. Our bookmark table consists of elements of type t_bookmark.
t_bookmark struc 1
    index   dword   ?       ; Bookmark index (0..9)
    size_   dword   ?       ; Size of index, always 1 in our case
    type_   dword   ?       ; Type of entry, always 0
    addr_   dword   ?       ; Address of bookmark
t_bookmark ends

bookmark    t_table     <>  ; Bookmark table

; Functions in this file are placed in more or less "chronological" order,
; i.e. order in which they will be called by OllyDbg. This requires forward
; referencing.

Bookmarksortfunc proto C b1:ptr t_bookmark, b2:ptr t_bookmark, sort:dword

Bookmarkwinproc proto hw:HWND, msg:UINT, wp:WPARAM, lp:LPARAM

Createbookmarkwindow proto

;int Bookmarkgettext(char *s,char *mask,int *select,t_sortheader *ph,int column);
;void Createbookmarkwindow(void);


.code

; Entry point into a plugin DLL. Many system calls require DLL instance
; which is passed to DllEntryPoint() as one of parameters. Remember it.
; Preferrable way is to place initializations into ODBG_Plugininit() and
; cleanup in ODBG_Plugindestroy().

DllEntryPoint proc hi:HINSTANCE, reason:dword, res:dword
        .IF reason == DLL_PROCESS_ATTACH
            m2m     hinst, hi               ; Mark plugin instance
        .ENDIF
        return  1                           ; Report success
DllEntryPoint endp

; ODBG_Plugindata() is a "must" for valid OllyDbg plugin. It must fill in
; plugin name and return version of plugin interface. If function is absent,
; or version is not compatible, plugin will be not installed. Short name
; identifies it in the Plugins menu. This name is max. 31 alphanumerical

; characters or spaces + terminating '\0' long. To keep life easy for users,
; this name should be descriptive and correlate with the name of DLL.

ODBG_Plugindata proc C shortname:ptr byte
        invoke  lstrcpy, shortname, CTEXT("Bookmarks")      ; Name of plugin
        return  PLUGIN_VERSION;
ODBG_Plugindata endp

; OllyDbg calls this obligatory function once during startup. Place all
; one-time initializations here. If all resources are successfully allocated,

; function must return 0. On error, it must free partially allocated resources
; and return -1, in this case plugin will be removed. Parameter ollydbgversion
; is the version of OllyDbg, use it to assure that it is compatible with your
; plugin; hw is the handle of main OllyDbg window, keep it if necessary.
; Parameter features is reserved for future extentions, do not use it.
ODBG_Plugininit proc C ollydbgversion:dword, hw:HWND, features:ptr dword

        ; Check that version of OllyDbg is correct.
        .IF ollydbgversion < PLUGIN_VERSION
            jmp     @@bad_exit
        .ENDIF

        ; Keep handle of main OllyDbg window. This handle is necessary, for example,
        ; to display message box.
        m2m     hwmain, hw
        ; Initialize bookmark data. Data consists of elements of type t_bookmark,
        ; we reserve space for 10 elements. If necessary, table will allocate more
        ; space, but in our case maximal number of bookmarks is 10. Elements do not

        ; allocate memory or other resources, so destructor is not necessary.

        invoke  Createsorteddata, offset bookmark.data, CTEXT("Bookmarks"),
                                  sizeof t_bookmark, 10, offset Bookmarksortfunc, NULL

        .IF eax
            jmp     @@bad_exit
        .ENDIF


        ; Register window class for MDI window that will display plugins. Please
        ; note that formally this class belongs to instance of main OllyDbg program,
        ; not a plugin DLL. String bookmarkwinclass gets unique name of new class.

        ; Keep it to create window and unregister on shutdown.
        invoke  Registerpluginclass, offset bookmarkwinclass, NULL, hinst, offset Bookmarkwinproc
        .IF eax < 0
            ; Failure! Destroy sorted data and exit.
            invoke  Destroysorteddata, offset bookmark.data
            jmp     @@bad_exit
        .ENDIF

        ; Plugin successfully initialized. Now is the best time to report this fact
        ; to the log window. To conform OllyDbg look and feel, please use two lines.
        ; The first, in black, should describe plugin, the second, gray and indented

        ; by two characters, bears copyright notice.
        invoke  Addtolist, 0, 0, CTEXT("Bookmarks sample plugin v1.08 (plugin demo)")
        invoke  Addtolist, 0, -1, CTEXT("  Copyright (C) 2001, 2002 Oleh Yuschuk")
        ; OllyDbg saves positions of plugin windows with attribute TABLE_SAVEPOS to
        ; the .ini file but does not automatically restore them. Let us add this
        ; functionality here. I keep information whether window was open when
        ; OllyDbg terminated also in ollydbg.ini. This information is saved in

        ; ODBG_Pluginclose. To conform to OllyDbg norms, window is restored only
        ; if corresponding option is enabled.
        invoke  Plugingetvalue, VAL_RESTOREWINDOWPOS
        push    eax
        invoke  Pluginreadintfromini, hinst, CTEXT("Restore bookmarks window"), 0
        pop     ecx
        .IF eax && ecx
            invoke  Createbookmarkwindow
        .ENDIF

        return  0
@@bad_exit:
        return  -1
ODBG_Plugininit endp

; OllyDbg calls this optional function once on exit. At this moment, all MDI
; windows created by plugin are already destroyed (and received WM_DESTROY
; messages). Function must free all internally allocated resources, like
; window classes, files, memory and so on.
ODBG_Plugindestroy proc C
        invoke  Unregisterpluginclass, offset bookmarkwinclass
        invoke  Destroysorteddata, offset bookmark.data
        ret
ODBG_Plugindestroy endp

; Function is called when user opens new or restarts current application.
; Plugin should reset internal variables and data structures to initial state.
ODBG_Pluginreset proc C
        invoke  Deletesorteddatarange, offset bookmark.data, 0, 0FFFFFFFFh
        ret
ODBG_Pluginreset endp

; OllyDbg calls this optional function when user wants to terminate OllyDbg.
; All MDI windows created by plugins still exist. Function must return 0 if
; it is safe to terminate. Any non-zero return will stop closing sequence. Do
; not misuse this possibility! Always inform user about the reasons why
; termination is not good and ask for his decision!
ODBG_Pluginclose proc C
        ; For automatical restoring of open windows, mark in .ini file whether
        ; Bookmarks window is still open.
        mov     eax, bookmark.hw
        .IF eax
            mov     eax, 1
        .ENDIF
        invoke  Pluginwriteinttoini, hinst, CTEXT("Restore bookmarks window"), eax
        return  0
ODBG_Pluginclose endp


; To sort sorted data by some criterium, one must supply sort function that
; returns -1 if first element is less than second, 1 if first element is
; greater and 0 if elements are equal according to criterium sort. Usually

; this criterium is the zero-based index of the column in window.
Bookmarksortfunc proc C b1:ptr t_bookmark, b2:ptr t_bookmark, sort:dword
LOCAL i:dword

        mov     ecx, 1
        mov     esi, b1
        mov     edi, b2

        .IF sort == 1
            mov     eax, [esi].t_bookmark.addr_
            .IF eax < [edi].t_bookmark.addr_
                    mov     ecx, -1
            .ELSEIF eax > [edi].t_bookmark.addr_
                    mov     ecx, 1
            .ENDIF
        .ENDIF

        .IF ecx == 1
            mov     eax, [esi].t_bookmark.index
            .IF eax < [edi].t_bookmark.index
                    mov     ecx, -1
            .ELSEIF eax > [edi].t_bookmark.index
                    mov     ecx, 1
            .ENDIF
        .ENDIF
        return  ecx
Bookmarksortfunc endp

; Standard function Painttable() makes most of OllyDbg windows redrawing. You
; only need to supply another function that prepares text strings and
; optionally colours them. Case of custom windows is a bit more complicated,
; please read documentation.
Bookmarkgettext proc s:ptr byte, mask_:ptr byte, select:ptr dword, ph:ptr t_sortheader, column:dword

        mov     eax, column
        .IF !eax        ; Name of bookmark
            ; Column 0 contains name of bookmark in form "Alt+n", where n is the
            ; digit from 0 to 9. Mainly for demonstration purposes, I display prefix
            ; "Alt+" in grayed and digit in normal text. Standard table windows do
            ; not need to bother about selection.
            mov     eax, ph
            invoke  wsprintf, s, CTEXT("Alt+%i"), [eax].t_bookmark.index
            mov     eax, select
            mov     dword ptr [eax], DRAW_MASK
            invoke  RtlFillMemory, mask_, 4, DRAW_GRAY
            mov     eax, mask_
            mov     byte ptr [eax + 4], DRAW_NORMAL
            invoke  lstrlen, s

        .ELSEIF eax == 1    ; Address of bookmark
            mov     eax, ph
            invoke  wsprintf, s, CTEXT("%08X"), [eax].t_bookmark.addr_
            invoke  lstrlen, s

        .ELSEIF eax == 2    ; Disassembled command
            ; Function Disasm() requires that calling routine supplies code to be
            ; disassembled. Read this code from memory. First determine possible
            ; code size.
            invoke  wsprintf, s, CTEXT("column %d"), eax
            invoke  lstrlen, s

        .ELSEIF eax == 3
            mov     eax, ph
            invoke  Findname, [eax].t_bookmark.addr_, NM_COMMENT, s

        .ELSE
            xor     eax, eax
        .ENDIF
        ret
Bookmarkgettext endp

; Each window class needs its own window procedure. Both standard and custom
; OllyDbg windows must pass some system and OllyDbg-defined messages to
; Tablefunction(). See description of Tablefunction() for more details.
Bookmarkwinproc proc hw:HWND, msg:UINT, wp:WPARAM, lp:LPARAM
;LOCAL controlkey, shiftkey:dword
LOCAL menu:HMENU
;LOCAL pb:ptr t_bookmark

        mov     eax, msg

        ; Standard messages. You can process them, but - unless absolutely sure -

        ; always pass them to Tablefunction().
        .IF eax == WM_DESTROY || eax == WM_MOUSEMOVE || eax == WM_LBUTTONDOWN || \
            eax == WM_LBUTTONDBLCLK || eax == WM_LBUTTONUP || eax == WM_RBUTTONDOWN || \
            eax == WM_RBUTTONDBLCLK || eax == WM_HSCROLL || eax == WM_VSCROLL || eax == WM_TIMER || eax == WM_SYSKEYDOWN

            invoke  Tablefunction, offset bookmark, hw, msg, wp, lp
            ; Pass message to DefMDIChildProc

        ; Custom messages responsible for scrolling and selection. User-drawn
        ; windows must process them, standard OllyDbg windows without extra
        ; functionality pass them to Tablefunction().
        .ELSEIF eax == WM_USER_SCR || eax == WM_USER_VABS || eax == WM_USER_VREL || \
                eax == WM_USER_VBYTE || eax == WM_USER_STS || eax == WM_USER_CNTS || eax == WM_USER_CHGS
            invoke  Tablefunction, offset bookmark, hw, msg, wp, lp
            ret

        .ELSEIF eax == WM_USER_MENU
            invoke  CreatePopupMenu
            mov     menu, eax

            ; Find selected bookmark. Any operations with bookmarks make sense only
            ; if at least one bookmark exists and is selected. Note that sorted data
            ; has special sort index table which is updated only when necessary.
            ; Getsortedbyselection() does this; some other sorted data functions
            ; don't and you must call Sortsorteddata(). Read documentation!
            invoke  Getsortedbyselection, offset bookmark.data, bookmark.data.selected
            mov     esi, eax
            .IF menu && eax
                invoke  AppendMenu, menu, MF_STRING, 1, CTEXT("&Follow",9,"Enter")
                invoke  AppendMenu, menu, MF_STRING, 2, CTEXT("&Delete",9,"Del")
            .ENDIF
            ; Even when menu is NULL, call to Tablefunction is still meaningful.
            invoke  Tablefunction, offset bookmark, hw, WM_USER_MENU, 0, menu
            push    eax
            mov     eax, menu
            .IF eax
                invoke  DestroyMenu, eax
            .ENDIF
            pop     eax

            .IF eax == 1        ; Follow bookmark in Disassembler
                invoke  Setcpu, 0, [esi].t_bookmark.addr_, 0, 0, CPU_ASMHIST or CPU_ASMCENTER or CPU_ASMFOCUS

            .ELSEIF eax == 2    ; Delete bookmark
                invoke  Deletesorteddata, offset bookmark.data, [esi].t_bookmark.index
                ; There is no automatical window update, do it yourself.
                invoke  InvalidateRect, hw, NULL, FALSE
            .ENDIF
            return  0

        .ELSEIF eax == WM_USER_DBLCLK
            ; Doubleclicking row follows bookmark in Disassembler.
            invoke  Getsortedbyselection, offset bookmark.data, bookmark.data.selected
            .IF eax
                invoke  Setcpu, 0,[eax].t_bookmark.addr_, 0, 0, CPU_ASMHIST or CPU_ASMCENTER or CPU_ASMFOCUS
            .ENDIF
            return  1   ; Doubleclick processed

        .ELSEIF eax == WM_USER_CHALL || eax == WM_USER_CHMEM
            ; Something is changed, redraw window.
            invoke  InvalidateRect, hw, NULL, FALSE
            return  0

        .ELSEIF eax == WM_PAINT
            ; Painting of all OllyDbg windows is done by Painttable(). Make custom
            ; drawing only if you have important reasons to do this.
            invoke  Painttable, hw, offset bookmark, offset Bookmarkgettext
            return  0

        .ENDIF
        invoke  DefMDIChildProc, hw, msg, wp, lp
        ret
Bookmarkwinproc endp

; Record types must be unique among OllyDbg and all plugins. The best way to
; assure this is to register record type by OllDbg (Oleh Yuschuk). Registration
; is absolutely free of charge, except for email costs :)
TAG_BOOKMARK        equ     0236D420Ah      ; Bookmark record type in .udd file

; Time to save data to .udd file! This is done by calling Pluginsaverecord()
; for each data item that must be saved. Global, process-oriented data must
; be saved in main .udd file (named by .exe); module-relevant data must be
; saved in module files. Don't forget to save all addresses relative to
; module's base, so that data will be restored correctly even when module is
; relocated.
ODBG_Pluginsaveudd proc C pmod:ptr t_module, ismainmodule:dword
LOCAL data[2]:dword

        .IF !ismainmodule       ; Save bookmarks to main file only
            ret
        .ENDIF

         mov     esi, bookmark.data.data
         mov     ebx, bookmark.data.n

         .WHILE ebx > 0
             mov     eax, [esi].t_bookmark.index
             mov     data[0], eax
             mov     eax, [esi].t_bookmark.addr_
             mov     data[1 * sizeof dword], eax
             invoke  Pluginsaverecord, TAG_BOOKMARK, sizeof data, addr data
             dec     ebx
             add     esi, sizeof t_bookmark
        .ENDW
        ret
ODBG_Pluginsaveudd endp  


; OllyDbg restores data from .udd file. If record belongs to plugin, it must
; process record and return 1, otherwise it must return 0 to pass record to
; other plugins. Note that module descriptor pointed to by pmod can be
; incomplete, i.e. does not necessarily contain all informations, especially
; that from .udd file.
ODBG_Pluginuddrecord proc C pmod:ptr t_module, ismainmodule:dword, tag:dword, size_:dword, data:dword
LOCAL mark:t_bookmark

        mov     eax, ismainmodule
        .IF !eax
            return eax              ; Bookmarks saved in main file only
        .ENDIF


        .IF tag != TAG_BOOKMARK
            return  0               ; Tag is not recognized
        .ENDIF

        mov     ecx, data
        mov     eax, [ecx]
        mov     mark.index, eax
        mov     mark.size_, 1
        mov     mark.type_, 0
        mov     eax, [ecx + 4]
        mov     mark.addr_, eax
        invoke  Addsorteddata, offset bookmark.data, addr mark
        return  1                   ;  Record processed
ODBG_Pluginuddrecord endp



; OllyDbg makes most of work when creating standard MDI window. Plugin must
; only describe number of columns, their properties and properties of window
; as a whole.
Createbookmarkwindow proc
        ; Describe table columns. Note that column names are pointers, so strings
        ; must exist as long as table itself.
        .IF !bookmark.bar.nbar
            ; Bar still uninitialized.
            mov     bookmark.bar.name_, CTEXT("Bookmark")   ; Name of bookmark
            mov     bookmark.bar.defdx, 9
            mov     bookmark.bar.mode, 0
            mov     bookmark.bar.name_[1 * sizeof dword], CTEXT("Address") ; Bookmark address
            mov     bookmark.bar.defdx[1 * sizeof dword], 9
            mov     bookmark.bar.mode[1 * sizeof byte], 0
            mov     bookmark.bar.name_[2 * sizeof dword], CTEXT("Disassembly") ; Disassembled command
            mov     bookmark.bar.defdx[2 * sizeof dword], 32
            mov     bookmark.bar.mode[2 * sizeof byte], BAR_NOSORT
            mov     bookmark.bar.name_[3 * sizeof dword], CTEXT("Comment")   ; Comment
            mov     bookmark.bar.defdx[3 * sizeof dword], 256
            mov     bookmark.bar.mode[3 * sizeof byte], BAR_NOSORT
            mov     bookmark.bar.nbar, 4
            mov     bookmark.mode, TABLE_COPYMENU or TABLE_SORTMENU or TABLE_APPMENU or TABLE_SAVEPOS;
            mov     bookmark.drawfunc, offset Bookmarkgettext
        .ENDIF
        ; If window already exists, Quicktablewindow() does not create new window,
        ; but restores and brings to top existing. This is the simplest way,
        ; Newtablewindow() is more flexible but more complicated. I do not recommend
        ; custom (plugin-drawn) windows without very important reasons to do this.
        invoke  Quicktablewindow, offset bookmark, 15, 4, offset bookmarkwinclass, CTEXT("Bookmarks")
        ret
Createbookmarkwindow endp

; If you define ODBG_Pluginmainloop, this function will be called each time
; from the main Windows loop in OllyDbg. If there is some debug event from
; the debugged application, debugevent points to it, otherwise it is NULL. Do
; not declare this function unnecessarily, as this may negatively influence
; the overall speed!
;ODBG_Pluginmainloop proc C debugevent:ptr DEBUG_EVENT
;ODBG_Pluginmainloop endp

; Function adds items either to main OllyDbg menu (origin=PM_MAIN) or to popup
; menu in one of standard OllyDbg windows. When plugin wants to add own menu
; items, it gathers menu pattern in data and returns 1, otherwise it must
; return 0. Except for static main menu, plugin must not add inactive items.

; Item indices must range in 0..63. Duplicated indices are explicitly allowed.
ODBG_Pluginmenu proc C uses ebx origin:dword, data:ptr byte, item:dword
LOCAL pb:ptr t_bookmark
LOCAL pd:ptr t_dump

        mov     eax, origin
        ; Menu creation is very simple. You just fill in data with menu pattern.
        ; Some examples:
        ; 0 Aaa,2 Bbb|3 Ccc|,,  - linear menu with 3 items, relative IDs 0, 2 and
        ;                         3, separator between second and third item, last
        ;
        ;                         separator and commas are ignored;
        ; #A{0Aaa,B{1Bbb|2Ccc}} - unconditional separator, followed by popup menu
        ;                         A with two elements, second is popup with two
        ;                         elements and separator inbetween.
        .IF eax == PM_MAIN ; Plugin menu in main window
            invoke  lstrcpy, data, CTEXT("0 &Bookmarks|1 &About")
            ; If your plugin is more than trivial, I also recommend to include Help.
            return  1

        .ELSEIF eax == PM_DISASM ; Popup menu in Disassembler
            ; First check that menu applies.
            mov     eax, item
            .IF !eax || ![eax].t_dump.size_
                return  0                       ; Window empty, don't add
            .ENDIF
            ; Start second-level popup menu.
            invoke  wsprintf, data, CTEXT("Bookmark{")
            invoke  lstrlen, data
            add     eax, data
            mov     ebx, eax

            ; Add item "Insert bookmark n" if there are free bookmarks and some part
            ; of Disassembler is selected. Note that OllyDbg correctly interpretes
            ; superfluos commas, separators and, to some extent, missed braces.
            mov     esi, bookmark.data.data
            xor     ecx, ecx
            .WHILE ecx < bookmark.data.n
                .BREAK .IF [esi].t_bookmark.index != ecx
                add     esi, sizeof dword
                inc     ecx
            .ENDW

            mov     esi, item
            mov     eax, [esi].t_dump.sel1
            .IF ecx < 10 && (eax > [esi].t_dump.sel0)
                invoke  wsprintf, ebx, CTEXT("%i &Insert bookmark %i",9,"Alt+Shift+%i,"), ecx, ecx, ecx
                invoke  lstrlen, ebx
                add     ebx, eax
            .ENDIF
            ; Add item "Delete bookmark n" for each available bookmark. Menu
            ; identifiers are not necessarily consecutive.
            mov     esi, bookmark.data.data
            xor     ecx, ecx
            .WHILE ecx < bookmark.data.n
                mov     eax, [esi].t_bookmark.index
                add     eax, 10
                push    ecx
                invoke  wsprintf, ebx, CTEXT("%i Delete bookmark %i,"), eax, [esi].t_bookmark.index
                add     esi, sizeof dword
                invoke  lstrlen, ebx
                pop     ecx
                add     ebx, eax
                inc     ecx
            .ENDW
            ; Add separator to menu.
            mov     byte ptr [ebx], '|'
            inc     ebx
            ; Add item "Go to bookmark n" for each available bookmark. Bookmarks
            ; set at selected command are not shown.
            mov     esi, bookmark.data.data
            xor     ecx, ecx
            .WHILE ecx < bookmark.data.n
                mov     eax, item
                mov     eax, [eax].t_dump.sel0
                .IF eax != [esi].t_bookmark.addr_ 
                    mov     eax, [esi].t_bookmark.index
                    add     eax, 20
                    push    ecx
                    invoke  wsprintf, ebx, CTEXT("%i Go to bookmark %i",9,"Alt+%i,"), eax, [esi].t_bookmark.index, [esi].t_bookmark.index
                    add     esi, sizeof dword
                    invoke  lstrlen, ebx
                    pop     ecx
                    add     ebx, eax
                .ENDIF
                inc     ecx
            .ENDW
            ; Close popup. If you forget to do this, OllyDbg will try to correct
            ; your error.
            mov     word ptr [ebx], '}'
            return  1
        .ENDIF
        return  0
ODBG_Pluginmenu endp

; This optional function receives commands from plugin menu in window of type
; origin. Argument action is menu identifier from ODBG_Pluginmenu(). If user

; activates automatically created entry in main menu, action is 0.
ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
LOCAL mark:t_bookmark
;LOCAL pb:ptr t_bookmark
;LOCAL pd:ptr t_dump

        mov     eax, origin
        .IF eax == PM_MAIN
            mov     eax, action
            .IF !eax
                ; Menu item "Bookmarks", creates bookmark window.
                invoke  Createbookmarkwindow

            .ELSEIF eax == 1
                ; Menu item "About", displays plugin info.
                invoke  MessageBox, hwmain, CTEXT("Bookmark plugin v1.08",13,10,"(demonstration of plugin capabilities)",13,10,"Copyright (C) 2001, 2002 Oleh Yuschuk",13,10,"MASM32 version by Eviloid, 2oo3"),\
                                            CTEXT("Bookmark plugin"),MB_OK or MB_ICONINFORMATION
            .ENDIF
        .ELSEIF eax == PM_DISASM
            mov     eax, action
            .IF eax >= 0 && eax < 10 ; Insert bookmark
                mov     mark.index, eax
                mov     mark.size_, 1
                mov     mark.type_, 0
                mov     eax, item
                m2m     mark.addr_, [eax].t_dump.sel0
                invoke  Addsorteddata, offset bookmark.data, addr mark

                .IF bookmark.hw
                    invoke  InvalidateRect, bookmark.hw, NULL, FALSE
                .ENDIF

            .ELSEIF eax >= 10 && eax < 20 ; Delete bookmark
                sub     eax, 10
                mov     ebx, eax
                invoke  Findsorteddata, offset bookmark.data, eax
                .IF eax
                    invoke  Deletesorteddata, offset bookmark.data, ebx
                    .IF bookmark.hw
                        invoke  InvalidateRect, bookmark.hw, NULL, FALSE
                    .ENDIF
                .ENDIF

            .ELSEIF eax >= 20 && eax < 30 ; Go to bookmark
                sub     eax, 20
;                mov     ebx, eax
                invoke  Findsorteddata, offset bookmark.data, eax
                .IF eax
                    invoke  Setcpu, 0, [eax].t_bookmark.addr_, 0, 0, CPU_ASMHIST or CPU_ASMCENTER or CPU_ASMFOCUS
                .ENDIF
            .ENDIF
        .ENDIF
        ret
ODBG_Pluginaction endp

end DllEntryPoint
