unit uAdd2Off;

interface

uses
  Windows, SysUtils, Classes, Graphics, Controls, Forms,
  StdCtrls;

type
  TfAdd2Off = class(TForm)
    Label1: TLabel;
    Label2: TLabel;
    eAddr: TEdit;
    eFOff: TEdit;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure CopyAddresstoClipboard1Click(Sender: TObject);
    procedure CopyFileOffsettoClipboard1Click(Sender: TObject);
  private
    { Private declarations }
  public
    procedure UpdateAddress(const Address: Cardinal);
  end;

var
  fAdd2Off: TfAdd2Off;

  Enabled: Boolean;  

procedure PluginEnableWindow(OwnerWindow: HWND = 0);
procedure PluginDisableWindow;
procedure PluginUpdateAddress(const Address: Cardinal);

implementation

uses Plugin;

{$R *.dfm}

procedure PluginEnableWindow(OwnerWindow: HWND = 0);
begin
  PluginDisableWindow;

  fAdd2Off:= TfAdd2Off.Create(nil);
  fAdd2Off.ParentWindow:= OwnerWindow;
  
  fAdd2Off.Show;

  Enabled:= True;
end;

procedure PluginDisableWindow;
begin
  if not (fAdd2Off=nil) then begin
    try
      fAdd2Off.Close;
    except
      ;
    end;
  end;

  Enabled:= False;   
end;

procedure PluginUpdateAddress(const Address: Cardinal);
begin
  if not (fAdd2Off=nil) then begin
    try
      fAdd2Off.UpdateAddress(Address);
    except
      ;
    end;
  end;
end;

procedure TfAdd2Off.CopyAddresstoClipboard1Click(Sender: TObject);
begin
  eAddr.SelectAll;
  eAddr.CopyToClipboard;
end;

procedure TfAdd2Off.CopyFileOffsettoClipboard1Click(Sender: TObject);
begin
  eFOff.SelectAll;
  eFOff.CopyToClipboard;
end;

procedure TfAdd2Off.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Enabled:= False;
  
  Action:= caFree;
end;

procedure TfAdd2Off.UpdateAddress(const Address: Cardinal);
var
  FileOffset: Cardinal;
begin
  FileOffset:= Findfileoffset(nil, Address);
  if FileOffset = 0 then
    eFOff.Color:= clRed
  else
    eFOff.Color:= clInfoBk;
      
  eAddr.Text:= IntToHex(Address, 8);
  eFOff.Text:= IntToHex(FileOffset, 8);
end;

end.
