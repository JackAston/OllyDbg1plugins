library FileOffset;

uses
  Windows,
  SysUtils,
  Plugin,
  uAdd2Off in 'uAdd2Off.pas' {fAdd2Off};

resourcestring
  VERSION = '1.0';
  AUTHOR  = 'BlurCode';
  PLUGIN_NAME = 'Address to File Offset';
  ABOUT   = 'Used:' + #13#10 +
            'OllyDbg Plugin SDK translation by TQN' + #13#10 +
            'Delphi 10';

var
  g_hwndOlly: HWND;
  g_hmodOlly: HMODULE;
  SaveDLLProc: TDLLProc;

function ODBG_Plugindata(name: PChar): Integer; cdecl;
begin
  StrLCopy(name, PChar(PLUGIN_NAME), 32); // Name of plugin
  Result := PLUGIN_VERSION;
end;

function ODBG_Plugininit(ollydbgversion: Integer; hWndOlly: HWND; features: PULONG): Integer; cdecl;
begin
  if (ollydbgversion < PLUGIN_VERSION) then begin
    Result := -1;
    Exit;
  end;

  Enabled:= False;

  g_hwndOlly := hWndOlly;
  Addtolist(0,  0, 'Address to File Offset');
  Addtolist(0, -1, ' version %s by %s', VERSION, AUTHOR);  

  Result := 0;
end;

function ODBG_Pluginmenu(origin: Integer; pData: PChar; pItem: Pointer): Integer; cdecl;
begin
  case origin of
    PM_MAIN:
      begin
        // Plugin menu in main window
        StrCopy(pData, '0 &Enable, 1 &Disable|3 &About...');
        Result := 1;
      end;
  else
    Result := 0; // Any other window
  end;
end;

procedure ODBG_Pluginaction(origin: Integer; action: Integer; pItem: Pointer); cdecl;
begin
  case origin of
   PM_MAIN:
    begin
      case action of
        0: PluginEnableWindow;//(g_hwndOlly);
        1: PluginDisableWindow;
        3: MessageBox(g_hwndOlly, PChar(ABOUT), PChar(PLUGIN_NAME), MB_OK);
      end;
    end;
  end; 
end;

function ODBG_Paused(reason: Integer; reg: p_reg): Integer; cdecl;
begin
  case reason of
    PP_EVENT:
     begin
       if Enabled then begin
         if not (reg=nil) then         
           PluginUpdateAddress(reg^.ip);
       end;
     end;
  end;

  ODBG_Paused:= 0;
end;

procedure DLLExit(dwReason: DWORD);
begin
  if (dwReason = DLL_PROCESS_DETACH) then
  begin
    // Uninitialize code here
    //OutputDebugString('Plugin unloaded by DLL_PROCESS_DETACH');

    if Enabled then
      PluginDisableWindow;
  end;

  // Call saved entry point procedure
  if Assigned(SaveDLLProc) then
    SaveDLLProc(dwReason);
end;

exports
  ODBG_Plugindata   name '_ODBG_Plugindata',
  ODBG_Plugininit   name '_ODBG_Plugininit',
  ODBG_Pluginmenu   name '_ODBG_Pluginmenu',
  ODBG_Pluginaction name '_ODBG_Pluginaction',
  ODBG_Paused       name '_ODBG_Paused';

begin
  // Initialize code here
  SaveDLLProc := @DLLProc;
  DLLProc := @DLLExit;

  g_hmodOlly := GetModuleHandle(nil);
end.
