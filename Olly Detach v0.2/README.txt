This is a *very* simple plug-in that does nothing more then call the
following routine:

    void detach_from_debuggee (void)
    {
        DebugSetProcessKillOnExit(FALSE);
        exit(1);
    }
    
To detach OllyDbg from the debuggee.