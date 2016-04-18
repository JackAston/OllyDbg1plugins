{ KOL MCK } // Do not remove this line!
{$DEFINE KOL_MCK}
unit uMain;

// n1kto, 10/08/2006
// contact: look at forum on www.cracklab.ru

interface

{$IFDEF KOL_MCK}
uses Windows, Messages, ShellAPI, KOL {$IFNDEF KOL_MCK}, mirror, Classes, Controls, mckControls, mckObjs, Graphics,
  StdCtrls, mckCtrls {$ENDIF (place your units here->)};
{$ELSE}
{$I uses.inc}
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, mirror;
{$ENDIF}

type
  {$IFDEF KOL_MCK}
  {$I MCKfakeClasses.inc}
  {$IFDEF KOLCLASSES} TfrmMain = class; PfrmMain = TfrmMain; {$ELSE OBJECTS} PfrmMain = ^TfrmMain; {$ENDIF CLASSES/OBJECTS}
  {$IFDEF KOLCLASSES}{$I TfrmMain.inc}{$ELSE} TfrmMain = object(TObj) {$ENDIF}
    Form: PControl;
  {$ELSE not_KOL_MCK}
  TfrmMain = class(TForm)
  {$ENDIF KOL_MCK}
    KOLProject1: TKOLProject;
    frmMain: TKOLForm;
    lbxPaths: TKOLListBox;
    Button1: TKOLButton;
    Button2: TKOLButton;
    Button3: TKOLButton;
    procedure frmMainFormCreate(Sender: PObj);
    procedure frmMainClose(Sender: PObj; var Accept: Boolean);
    procedure Button1Click(Sender: PObj);
    procedure Button2Click(Sender: PObj);
    procedure Button3Click(Sender: PObj);
  private
    { Private declarations }
  public
    { Public declarations }
    ischg: Boolean;
    ininame: string;
    lastpath: String;
    procedure SaveData;
  end;

var
  frmMain {$IFDEF KOL_MCK} : PfrmMain {$ELSE} : TfrmMain {$ENDIF} ;

{$IFDEF KOL_MCK}
procedure NewfrmMain( var Result: PfrmMain; AParent: PControl );
{$ENDIF}

implementation

{$IFNDEF KOL_MCK} {$R *.DFM} {$ENDIF}

{$IFDEF KOL_MCK}
{$I uMain_1.inc}
{$ENDIF}

procedure TfrmMain.frmMainFormCreate(Sender: PObj);
var
  cnt, idx: Integer;
  ws: string;
  inifile: PIniFile;
begin
  ischg := false;
// טלÿ פאיכא ini
  lastpath := ExtractFilePath(ParamStr(0));
  ininame := IncludeTrailingPathDelimiter(lastpath) + 'loadsome.ini';
//
  inifile := OpenIniFile(ininame);
  inifile.Mode := ifmRead;
  inifile.Section := 'paths';
  cnt := inifile.ValueInteger('counter', 0);
  for idx := 1 to cnt do
    begin
      ws := 'Path' + int2str(idx);
      ws := inifile.ValueString(ws, '');
      lbxPaths.Add(ws);
    end;
  Free_And_Nil(inifile);  
end;

procedure TfrmMain.frmMainClose(Sender: PObj; var Accept: Boolean);
begin
  if ischg then
    begin
      SaveData;
      MessageBoxA(0,
        'Changes will be applied at next start of program',
        'Changes saved', MB_OK);
    end;
end;

procedure TfrmMain.Button1Click(Sender: PObj);
var
  idx: Integer;
  ws: string;
  opndir:POpenDirDialog;
begin
  opndir := NewOpenDirDialog('',[odOnlyFileSystemAncestors,odNewDialogStyle,odEditBox]);
  opndir.InitialPath := lastpath;
  if opndir.Execute then
    begin
      ws := Trim(opndir.Path);
      for idx := 0 to lbxPaths.Count - 1 do
        if lbxPaths.Items[idx] = ws then
          Exit;
      ischg := true;
      lastpath := ws;
      lbxPaths.Add(ws);
    end;
  opndir.Free;
end;

procedure TfrmMain.Button2Click(Sender: PObj);
var
  idx: Integer;
begin
  idx := 0;
  while idx < lbxPaths.Count do
    begin
      if lbxPaths.ItemSelected[idx] then
        begin
          lbxPaths.Delete(idx);
          ischg := true;
        end
      else
        begin
          inc(idx);
        end;
    end;
end;

procedure TfrmMain.Button3Click(Sender: PObj);
begin
  SaveData;
  form.ModalResult := 1;
  form.Close;
end;

procedure TfrmMain.SaveData;
var
  cnt, idx: Integer;
  ws: string;
  inifile:PIniFile;
begin
  inifile := OpenIniFile(ininame);
  inifile.Mode := ifmWrite;
  inifile.Section := 'paths';
  inifile.ClearSection;
  cnt := lbxPaths.Count;
  inifile.ValueInteger('counter', cnt);
  for idx := 1 to cnt do
    begin
      ws := 'Path' + int2str(idx);
      inifile.ValueString(ws, lbxPaths.Items[idx - 1]);
    end;
  Free_And_Nil(inifile);      
end;

end.



