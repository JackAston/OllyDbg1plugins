// n1kto, 10/08/2006
// contact: look at forum on www.cracklab.ru
//
// based on: A simple plugin for OllyDbg, written by TQN

unit uEngine;

interface

uses windows,kol,plugin;

implementation

uses uMain;

var
  g_hwndOlly: HWND;
  g_hmodOlly: HMODULE;
  SaveDLLProc: TDLLProc;

resourcestring
  VERSION = '0.1b';
  AUTHOR  = 'n1kto';
  PLUGIN_NAME = 'LoadSome Plugin';
  ABOUT   = 'A simple plugin for OllyDbg'#13+
    'loading plugins from different paths'#13+
    'by n1kto';

var
  plst:PStrList;
  llst:PStrList;
  nlst:PStrList;
  curpath: Integer = 0;
  lastlibr: String;

  oldfnext:cardinal;
  oldllibr:cardinal;

procedure z(s: String);
begin
//  OutputDebugString(pchar(s));
end;

// extended ;) function findnextfile
// stack frames MUST BE ENABLED!
function ext_findnextfile(hnd:cardinal;var fnd:TWIN32FindData): cardinal; stdcall;
var
  sr:TWIN32FindData;
begin
//  asm db $cc; end;
  z('ext_findnextfile');
  // first check, for return address
  if curpath<llst.Count then
    begin
      FindClose(FindFirstFile(pchar(llst.Items[curpath]),sr));
      lastlibr := pchar(@sr.cFileName);
      fnd := sr;
      result := 1;
    end
  else
    begin
      lastlibr := '';
      Result := cardinal(FindNextFile(hnd,fnd));
    end;
end;

exports ext_findnextfile;

// extended ;) function loadlibrary
// stack frames MUST BE ENABLED!
function ext_loadlibrary(pstr:pchar):cardinal; stdcall;
var
  ws: String;
begin
//  asm db $cc; end;
  z('ext_loadlibrary');
  // first check, for return address
  ws := ExtractFileName(pstr);
  if lastlibr = ws then
    begin
      result := LoadLibrary(pchar(llst.Items[curpath]));
      inc(curpath);
    end
  else
    begin
      result := LoadLibrary(pstr);
    end;
end;

exports ext_loadlibrary;

procedure LoadIniFile;
var
  cnt, idx: Integer;
  ws: String;
  inifile:PIniFile;
begin
  ws := ExtractFilePath(ParamStr(0));
  ws := IncludeTrailingPathDelimiter(ws) + 'loadsome.ini';
//
  inifile := OpenIniFile(ws);
  inifile.Mode := ifmRead;
  inifile.Section := 'paths';
  cnt := inifile.ValueInteger('counter', 0);
  plst.Clear;
  for idx := 1 to cnt do
    begin
      ws := 'Path' + int2str(idx);
      ws := inifile.ValueString(ws, '');
      plst.Add(ws);
    end;
  inifile.Free;
end;

procedure GetLibrNames;
var
  i: Integer;
  sr:TWIN32FindData;
  srh:cardinal;
  ws, ts: String;
begin
  llst.Clear;
  for i := 0 to plst.Count-1 do
    begin
      ws := IncludeTrailingPathDelimiter(plst.Items[i])+'*.dll';
      srh := FindFirstFile(pchar(ws),sr);
      while srh>0 do
        begin
          ws := IncludeTrailingPathDelimiter(plst.Items[i])+sr.cFileName;
          ts := lowercase(sr.cFileName);
          if nlst.IndexOf(ts) = -1 then
            begin
              llst.Add(ws);
              nlst.Add(ts);
            end;
          if not FindNextFile(srh,sr) then
            begin
              FindClose(srh);
              srh := 0;
            end;
        end;
    end;
end;

procedure InstallExtended;
var
  i: cardinal;
  h: cardinal;
begin
//  asm db $cc; end;
  h := OpenProcess(PROCESS_VM_READ+PROCESS_VM_WRITE+PROCESS_VM_OPERATION,false,GetCurrentProcessId);
  VirtualProtectEx(h,pointer($496000),$1000,PAGE_EXECUTE_READWRITE,nil);

  ReadProcessMemory(h,pointer($50d2e0),@oldfnext,4,i);
  i := cardinal(@ext_findnextfile)-($496aef);

//  WriteProcessMemory(h,pointer($50d2e0),@i,4,i);
  WriteProcessMemory(h,pointer($496aeb),@i,4,i);
  GetLastError;

  ReadProcessMemory(h,pointer($50d3b4),@oldllibr,4,i);
  i := cardinal(@ext_loadlibrary)-($496817);
  WriteProcessMemory(h,pointer($496813),@i,4,i);

  CloseHandle(h);
end;

procedure UnInstallExtended;
var
  i: cardinal;
  h: cardinal;
begin
  h := OpenProcess(PROCESS_VM_READ+PROCESS_VM_WRITE+PROCESS_VM_OPERATION,false,GetCurrentProcessId);
  VirtualProtectEx(h,pointer($496000),$1000,PAGE_EXECUTE_READWRITE,nil);

  i := oldfnext-($496aef);
  WriteProcessMemory(GetCurrentProcess,pointer($496aeb),@i,4,i);
  i := oldllibr-($496817);
  WriteProcessMemory(GetCurrentProcess,pointer($496813),@oldllibr,4,i);

  CloseHandle(h);
end;

function ODBG_Plugindata(name: PChar): Integer; cdecl;
begin
  z('ODBG_Plugindata');
  StrLCopy(name, PChar(PLUGIN_NAME), 32); // Name of plugin
  Result := PLUGIN_VERSION;
end;

function ODBG_Pluginclose: Integer; cdecl;
begin
  UnInstallExtended;
  result := 0;
end;

function ODBG_Plugininit(ollydbgversion: Integer; hWndOlly: HWND; features: PULONG): Integer; cdecl;
begin
  z('ODBG_Plugininit');
  if (ollydbgversion < PLUGIN_VERSION) then
  begin
    Result := -1;
    Exit;
  end;

  g_hwndOlly := hWndOlly;

  Addtolist(0, 0, 'LoadSome Plugin - Ver %s by %s', VERSION, AUTHOR);
  InstallExtended;

  Result := 0;
end;

function ODBG_Pluginmenu(origin: Integer; pData: PChar; pItem: Pointer): Integer; cdecl;
begin
  z('ODBG_Pluginmenu');
  case origin of
    PM_MAIN:
      begin
        // Plugin menu in main window
        StrCopy(pData, '0 &Config plugin paths|1 &About...');
        Result := 1;
      end;
  else
    Result := 0; // Any other window
  end;
end;

procedure ODBG_Pluginaction(origin: Integer; action: Integer; pItem: Pointer); cdecl;
var
  frm:PfrmMain;
begin
  z('ODBG_Pluginaction');
  if (origin = PM_MAIN) then
  begin
    case action of
      0: begin
           Applet := NewApplet( 'fake applet' );
           Applet.Hide;
           NewfrmMain(frm,Applet);
           frm.Form.ShowModalEx;
           frm.Form.Free;
           frm := nil;
           Free_And_Nil(Applet);
           AppletTerminated := false;
         end;
      1: MessageBox(g_hwndOlly, PChar(ABOUT), PChar(PLUGIN_NAME), MB_OK);
    end;
  end;
end;

exports
  ODBG_Plugindata   name '_ODBG_Plugindata',
  ODBG_Plugininit   name '_ODBG_Plugininit',
  ODBG_Pluginmenu   name '_ODBG_Pluginmenu',
  ODBG_Pluginaction name '_ODBG_Pluginaction',
  ODBG_Pluginclose  name '_ODBG_Pluginclose';

procedure DLLExit(dwReason: DWORD);
begin
  if (dwReason = DLL_PROCESS_DETACH) then
  begin
    // Uninitialize code here
    OutputDebugString('Plugin unloaded by DLL_PROCESS_DETACH');
  end;

  // Call saved entry point procedure
  if Assigned(SaveDLLProc) then
    SaveDLLProc(dwReason);
end;

initialization
  SaveDLLProc := @DLLProc;
  DLLProc := @DLLExit;
  g_hmodOlly := GetModuleHandle(nil);

  plst := NewStrList;
  llst := NewStrList;
  nlst := NewStrList;
  LoadIniFile;
  GetLibrNames;

finalization
  plst.Free;
  llst.Free;
  nlst.Free;

end.
