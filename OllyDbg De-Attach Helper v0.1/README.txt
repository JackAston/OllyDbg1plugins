OllyDbg De-Attach Helper
Pedram Amini <pedram.amini@gmail.com>

This is a *very* simple plug-in that exposes two useful features. One,
it allows you to detach from the debuggee on systems that support doing
so by doing nothing more then calling the following routine:

    void detach_from_debuggee (void)
    {
        DebugSetProcessKillOnExit(FALSE);
        exit(1);
    }

Two, it creates a plug-in menu option to attach to the last created
process. This is a feature that WinDbg users essentially have (and
constantly rub in the faces of Olly users) due to the way the process 
list is ordered. At some point in the future it would be nice to extend
this feature to display an OllyDbg table showing the entire process
list, allowing the user to manually select the target process to attach
to.

This plug-in was built against Alex Clarke's modified PDK:

    http://www.openrce.org/downloads/details/193/Modified%20PDK

Which correctly exports the Attachtoactiveprocess() routine. I guess
Oleh forgot to include that in the official PDK lib.

The hokey Alt+A is registered for attaching to the last process and the
hotkey Alt+Shift+D is registered for detaching from the current process.
Note however, that hotkeys are not functional from all views. For
example, if the CPU or log views are in focus the hotkey fails. I'm not
quite sure why this is or if it's a problem I can address on my end.