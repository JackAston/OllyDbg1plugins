object fAdd2Off: TfAdd2Off
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Address to File Offset'
  ClientHeight = 43
  ClientWidth = 136
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 4
    Top = 4
    Width = 43
    Height = 13
    Caption = 'Address:'
  end
  object Label2: TLabel
    Left = 71
    Top = 4
    Width = 54
    Height = 13
    Caption = 'File Offset:'
  end
  object eAddr: TEdit
    Left = 4
    Top = 23
    Width = 61
    Height = 16
    TabStop = False
    Color = clInfoBk
    Font.Charset = OEM_CHARSET
    Font.Color = clWindowText
    Font.Height = -8
    Font.Name = 'Terminal'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
    OnClick = CopyAddresstoClipboard1Click
  end
  object eFOff: TEdit
    Left = 71
    Top = 23
    Width = 61
    Height = 16
    TabStop = False
    Color = clInfoBk
    Font.Charset = OEM_CHARSET
    Font.Color = clWindowText
    Font.Height = -8
    Font.Name = 'Terminal'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
    OnClick = CopyFileOffsettoClipboard1Click
  end
end
