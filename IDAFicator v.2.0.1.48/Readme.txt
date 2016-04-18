2/15/2011

Here's the new version (2) of idaficator, totally redesigned and rewrote
from scratch with lots of new features and ideas ... HAVE FUN.

1. What's new:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    1.1. ROTE : (aka Rainbow Olly Text Editor)
        The old mucasm (MUlti Commands Assembler) is now ROTE (think of
        it as WROTE). Not just a new name, but it is a new concept. You
        can now master the code injection at its deepest levels. With 
        this version, you write your code in asm or in C (yes, you 've 
        read it the right way, C, then with one (1) click,
        you will be able to inject it directly to reversed target.
        
    1.2. GOTO dialog box:
        A sophisticated one, with codecompletion, filtration by Imports
        Exports or Labels.
        
    1.3. HWBP dialog box:
        The new tiny hardware breakpoint management dialog bow is ready.
        
    1.4. Lots of new keyboard, mouse shortcuts will be discussed later.
        

2. installation:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    2.1. installation of the plugin:
        first, copy idaficator.dll + bytes.oep to you plugin derictory
        then, copy ico directory to ollydbg directory (same place with
        ollydbg.exe)
    
    2.2. installation of ROTE requirements:
        idaficator->options->settings->rote tab then set correctly dirs
        .i.e radasm dir and help file root
        To work correctly, here's the hierarchy that idaficator expects:
        RADASM
          |
          |___ compiler.ini             <--------- (* see below)
          |___ RACodeComplete.dll
          |___ RAEdit.dll
          
          
          * (masm.ini, cpp.ini, gcc.ini  [goasm.ini ... next version]
          where paths to masm/bin, cpp/bin, gcc/bin [goasm/bin ...] and 
          masm/include, cpp/include, gcc/include are set).
 
 
3. Features details:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    3.1. ROTE:
    This version supports:
        - MASM (others assembelrs will come later)
        - Visual C++   ( V6.0 tested, other versions may also work)
        - MingW  ( V4.4.1 tested, but other versions may also work)
        
    With code completion feature and info tooltip, ROTE will drastically
    decrease the time spent in RCE, and especially in code injection and
    inline patching.
    Thus, you have to follow some rules:
        3.1.1. Features:
            * The Code completion : Triggered automatically under asm with 
                classic invoke, call, fn, func(. or manually by CTRL+SPACE
            * Info ToolTips : Triggered automatically after API insertion
                or manually by SHIFT+SPACE.
            * HELP : If you've set help file path, you can get help on API
                by putting cursor then hit F1.
            * Menus : There are 2 menus in addition to menu bar ones. The
                First is triggered by RIGHT CLICK in ROTE  and the second
                by MIDDLE CLICK.
            * Saving : Each piece of text is saved separately to UDD of
                main debuggee and is restitute when correspondant debugee
                is loaded to ollydbg.
            * Other options : classic options of a text editor are there.

        3.1.2. Knowen issues:
        -> Under MASM:
            * Code mixing:
                You can mix masm code with olly's one (.i.e you can inline
                some primary unauthorized instructions that  normally give
                errors under MASM like this one: 'mov [401000H], eax' that
                will give as error: 'the immediate operand not allowed'.
                BUT TO DO SO, you HAVE TO PREFIX YOUR INSTRUCTIONS WITH AN
                ASTERISK (*). Examples:
                *  mov [401000H], eax
                *fstp [401000]
                (Note the H is not needed in this case)

                Exception: 
                To make your life easier, there's no need to prefix calls
                and direct c/jumps. So, use them directly like this:
                jmp 401000
                call 402000H
                jng 401020
                (Here again, the H is not mandatory)

            * NB:
                Be careful with some instructions, they may compile under 
                MASM in an unwanted way like;
                mov eax, DWORD PTR [401000h] that will be compiled to
                mov eax, 401000 if not 'asterisked'

            * NB:
                Your code must be masm compatible (an ommitted h after 11
                for example, will be compiled as 11  (decimal format) but 
                not 17 (decimal of 11h) so BE CAREFUL.


        -> Under MSVC or MingW:
            * EVERY GLOBAL variable MUST be initialized :  HWND hwnd = 0;
            * Inline asm works only with MSVC. Won't work with MingW
            * Send me other problems you may encounter.


        -> Under Ollydbg-scripter:
            * Have to be maintained ... stay tuned for future update.
            
        -> Under ALL:
            * If you keep scrolling by mouse, RAEdit crashes, can't figure
              out the source of the problem. Please, if you can get it, send
              me a mail, or a message on forum.

        3.1.3. Examples :
    
You can now assemble a piece of code like this with MASM:

    ; -------------code---------------

    .data

    k32 db "kernel32.dll", 0
    dd1 dd 0

    .code

    DummyProc1 proc iVar1:dword, iVar2:dword
        LOCAL dummyVar:dword
        
        invoke GetModuleHandle, offset k32
        fn GetModuleHandle, "kernel32.dll"
        call 00997A3C ; no need to prefix with *
        mov dd1, eax
        call 00997A3C ; no need to prefix with *
        imul eax, dd1
        *mov [009B7030], eax
        xor eax, iVar2
        add eax, iVar1
        ret
    DummyProc1 endp

    DummyProc2 proc
        LOCAL iVar1:DWORD
        LOCAL iVar2:DWORD
        
        invoke DummyProc1, iVar1, iVar2
        ret
    DummyProc2 endp

    ;-------------end of code---------------



Or this piece under VC++ or MingW

    //-----------------code------------------

    // Unicode :)
    void part(HWND hwnd) {
        DWORD dw;
        dw = MessageBoxW(hwnd, L"Text", L"Caption", MB_OK);
        // ...
        return;
    }

    //-------------end of code---------------

The compiled binary code follows these rules :
    - code goes to address highllighted in ACPUASM win
    - data goes to address highllighted in ACPUDUMP win


    3.2. Shortcuts:
    /** DISASM WINDOW FEATURES
     * 
     * MIDDLE CLICK:
     * 
     *         In first column:
     *         - MClick = Copy address to clipboard. (address of cursor).
     *         - CTRL + MClick = copy RVA to clip (address of cursor)..
     *        
     *         In second column:
     *         - MClick = copy data to clip.
     *        
     *         In Third column:
     *         - MClick = Copy constants to clipboard
     *         - CTRL + MClick = Copy command to clipboard
     *         - CTRL + SHIFT + MClick = Copy MASM compatible commands (all 
     *        constants are prefixed with 0 and suffixed with H)
     *         
     *         In Fourth columns:
     *         - MClick = Copy user comment to clipboard
     * 
     * LEFT DOUBLE CLICK:
     * 
     *         In Third column:
     *         - If command is a jump or a call, you'll follow it in disasm
     *        window.
     *         - If there's a constant bellonging to memory, you'll dump it
     *        to dump window.
     * 
     *     EVERY WHERE:
     *         - CTRL + V = Pasted binary code from clipboard
     * 
     */


    /** DUMP WINDOW FEATURES:
     *    MIDDLE CLICK:
     *         In first column:
     *         - MClick = Copy activated address to clipboard. (address of
     *        cursor).
     *         - CTRL + MClick = copy RVA to clip (address of cursor)..
     *        
     *         In second column:
     *         - MClick = copy data to clip.
     *         - CTRL + MClick = copy DWORD to clip.
     *        
     *         In Third column:
     *         - MClick = copy ascii text (NULL terminators will be deleted
     *        and text will be
     *         concatenated.
     *
     * LEFT DOUBLE CLICK:
     *
     *         In second column:
     *         - DClick will set a software breakpoint if disasm view in ON.
     *         - DClick = Edit value depending on the view  (ex: Edit float
     *        if float view is on)
     *         - CTRL + DClick = Follow DWORD if it belongs to valid memory.
     *
     *     EVERY WHERE:
     *         - CTRL + V = Pasted binary code from clipboard
     *         - F2 sets a software breakpoint.
     */

    /** STACK WINDOW :
     * 
     *     LEFT DOUBLE CLICK:
     *         - If data column is double clicked, then address in stack is 
     *           followed in dump window.
     *         - If data column is double clicked while CTRL is held,  then
     *           the 'modify dialog' box will show up.
     *         - If comment column is simply double clicked, then the modify
     *           dialogbox will show up.
     *         
     *     MIDDLE CLICK:
     *         - If address column is clicked, then the address is copied to
     *        clipboard.
     *         - If data column is clicked, then the data is binary copied (
     *        max = 400 bytes).
     *         - If data column is clicked while CTRL is held, data is copied
     *        as a DWORD (little endian).
     */

    3.3. HWBP window:
        - It's more sophisticated now, very stable and very handy, try it.
        - It will save your HWBP and there sates even between 2 debugging
          sessions.
        
    3.4. Goto#Supremacy Dialogbox: 
        - Can be enabeled from Options->Setting->General Tab
        
    3.5. All previous features are of course present: other shortcuts, the
        custom buttons functions, extra menus ...




4. History and news:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
18/2/2011 # idaficator-2.0.1.48
    [+]: Added mouse scrolling ability to Goto listbox.
    [+]: Added a workaround StrongOD problem hooking NtCreateProcess: You have to
         go to settings>rote tab, then set the number of milliseconds to sleep
         after compilation (500 ms works fine form me on a core 2 duo 1.60GHz).
         Thus, I noticed that it succeed only if THERE ARE NO ERRORS in the code.
    [+]: Ability to supersede StrongOD and AdvancedOlly Goto dialogbox even if
         not disabled ('Choosable').
    [+]: Ability to supersede StrongOD status bar info (idaficator must be loaded
         last, and to do so, just execute ollydbg without idaficator, then add it
         and re-run (after checking 'Supremacy mode' from settings)).
    [!]: Fix various tiny bugs.

17/2/2011 # idaficator-2.0.1.9 # first public release of version 2.0

16/02/2011 # idaficator-2.0.0.33 private build (testing):
    [+]: Goto#Supremacy is now choosable.
    [!]: Olly crashes if ROTE not closed before closing olly.
    [!]: Olly crashes if ROTE closed from close button or menu.
    [!]: Tooltips not working under C++.
    [!]: API addresses were not updated between 2 debugging sessions.
    [!]: Syncronisation problem in assmbling process
    [!]: 5th button right click to customize didn't work.

15/02/2011 # idaficator-2.0.0.12 private build (testing):
    [+]: Checks to prevent crashes if no RAEdit or RACodecomplete found.
    [!]: bug idaficator disables stack pointer on dclick address column.
    [!]: various little bugs and memory leaks.

14/02/2011 # idaficator-2.0 private build (testing):
    [!]: bug in CTRL+SHIFT+MClick disasm (compatible masm code).
    [!]: bug in saving restoring user APIs of breakpoints API menu
    [!]: bug no hwbp restoring between sessions.

06/05/2008 # V 1.2.12 
    [+]: Adding support for asm like command in 'multicommand assembler'.
    [+]: Position saving for most important and most used dialog boxes.
    [+]: MuCAsm remembers last entered text between 2 debugging sessions.

27/04/2008 # (private version) 
    Testing

21/04/2008 #c(public version) 
    [+]: Advanced GUI for shortcuts MENU manipulator.
    [+]: Added tooltips for WHBP custom window.
    [+]: Now, code can be restored after assembling commands.

17/04/2008 # (private version) 
    [+]: Now you can ENABLE/DISABLE hwbp form hwbp dialog 

15/04/2008 # (private version) 
    [+]: The 5th button is now activated. 
    [+]: U can now add custom APIs to APIs menu.
    [+]: 'Search for all strings' fixed. 
    [+]: Now the hardware bp dialog box is all costumised.
    [+]: I think the problem of painting olly is now off. 

12/12/2007 # v1.2.0
    [+]: Added a wide support for mouse.
    [+]: Added 3 new buttons with nice functions.
    [+]: Added 5 customizable buttons.
    [!]: Position of buttons is now fixed.

12/05/2007 # v1.1 
    (Private release).

12/02/2007 # v1.1.0
    [+]: Added THE IDA-LIKE MOUSE EFFECTS.
    [!]: bug path are not valid and bytes.oep was not loaded.
    [!]: Always dealling with spelling mistakes.

28/11/2007 # v1.0.8
    [!]: Corrction of 'no patterns in combo box' bug -> thanks LFC-AT
    [+]: Tested ok with CIM version -> thanks to Ahmed18
    [+]: Arabic/English help are now available in the package
    [+]: Correction of some spelling errors.

26/11/2007 # v1.0.7
    First public release




5. Greetings:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Thanks to ALL AT4RE MEMBERS FOR SUPPORT, TESTING, BUG REPORT.
Thanks to LCF-AT, Sunbeam for bug testing.
Thanks to Roobenhoodarab, Alzri2 for the nice graphics.
Thanks tou you for using my useless plugin.













Regards
Your best friend Zool@nder|AT4RE

