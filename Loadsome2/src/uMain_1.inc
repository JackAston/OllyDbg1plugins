{ KOL MCK } // Do not remove this line!
const Sizeof_TfrmMain = Sizeof(TfrmMain);
type TControl_ = object( TControl ) end;
type TObj_ = object( TObj ) end;
type _TObj_ = object( _TObj ) end;

{$IFDEF Pcode}
procedure NewfrmMain( var Result: PfrmMain; AParent: PControl );
{$IFDEF Psource}
 PROC(2)
 Load4 ####@@formvmt L(0)
 TObj.Create<2> RESULT
 SetSELF
 DUP C2 Store
 LoadAnsiStr '123' #0
 C4 
 NewForm<2> RESULT
 DUP C3 AddByte_Store #TfrmMain.Form
 L(126) L(192) C2 TControl.SetPosition<3> L(361) L(430) C2 TControl.SetClientSize<3>
 DUP StoreVar ####Applet
 LoadSELF C1 
 TControl.Add2AutoFree<2>
 IFDEF(USE_NAMES)
 LoadAnsiStr 'frmMain' #0
 C2 TObj.SetName<2>
 DelAnsiStr
 ENDIF
 L(3) L(16) <<
 ~ C1 AddWord_LoadRef ##TControl_.fStyle & 
 C1 TControl_.SetStyle<2>
 L(0) LoadWord ##SC_CLOSE 
 L(0) C3 TControl.GetWindowHandle<1> RESULT
 GetSystemMenu RESULT
 DeleteMenu
 LoadSELF Load4 ####TfrmMain.frmMainClose
 C2 TControl_.SetOnClose<1>
 L(16)
 C1
 NewListBox<2>
 RESULT
L(2) C1 TControl_.SetAlign<2> L(327) L(0) C2 TControl.SetSize<3>
 DUP LoadSELF AddWord_Store ##TfrmMain.lbxPaths
 IFDEF(USE_NAMES)
 LoadAnsiStr 'lbxPaths' #0
 C2 TObj_.SetName<2>
 DelAnsiStr
 ENDIF
 L($FF000005)
 C1 TControl_.SetCtlColor<2>
 L(0) C1 TControl_.SetCurIndex<2>
 DEL //lbxPaths
 LoadAnsiStr 'Add path' #0
 C2
 NewButton<2>
 RESULT
L(336) L(2) C2 TControl.SetPosition<3> L(0) L(95) C2 TControl.SetSize<3> xySwap DelAnsiStr
 DUP LoadSELF AddWord_Store ##TfrmMain.Button1
 IFDEF(USE_NAMES)
 LoadAnsiStr 'Button1' #0
 C2 TObj_.SetName<2>
 DelAnsiStr
 ENDIF
 DEL //Button1
 LoadAnsiStr 'Remove path' #0
 C2
 NewButton<2>
 RESULT
L(336) L(104) C2 TControl.SetPosition<3> L(0) L(97) C2 TControl.SetSize<3> xySwap DelAnsiStr
 DUP LoadSELF AddWord_Store ##TfrmMain.Button2
 IFDEF(USE_NAMES)
 LoadAnsiStr 'Button2' #0
 C2 TObj_.SetName<2>
 DelAnsiStr
 ENDIF
 DEL //Button2
 LoadAnsiStr 'Close' #0
 C2
 NewButton<2>
 RESULT
L(336) L(332) C2 TControl.SetPosition<3> L(0) L(96) C2 TControl.SetSize<3> xySwap DelAnsiStr
 DUP LoadSELF AddWord_Store ##TfrmMain.Button3
 IFDEF(USE_NAMES)
 LoadAnsiStr 'Button3' #0
 C2 TObj_.SetName<2>
 DelAnsiStr
 ENDIF
 DEL //Button3
 LoadSELF AddWord_LoadRef ##TfrmMain.Button1
 Load4 ####TfrmMain.Button1Click
 C1 AddWord_Store ##TControl_.fOnClick
 LoadSELF C1 AddWord_Store ##(4+TControl_.fOnClick)
 DEL //Button1
 LoadSELF AddWord_LoadRef ##TfrmMain.Button2
 Load4 ####TfrmMain.Button2Click
 C1 AddWord_Store ##TControl_.fOnClick
 LoadSELF C1 AddWord_Store ##(4+TControl_.fOnClick)
 DEL //Button2
 LoadSELF AddWord_LoadRef ##TfrmMain.Button3
 Load4 ####TfrmMain.Button3Click
 C1 AddWord_Store ##TControl_.fOnClick
 LoadSELF C1 AddWord_Store ##(4+TControl_.fOnClick)
 DEL //Button3
 DUP TControl.CenterOnParent<1> L(0) C1 TControl_.SetCanResize<2>
 LoadSELF DUP TfrmMain.frmMainFormCreate<2>
 DEL DelAnsiStr DEL(3) EXIT
           ####Sizeof_TfrmMain
           ####0
@@formvmt: ####_TObj_.Init
           ####TObj.Destroy
 ENDP
{$ENDIF Psource}
{$ELSE OldCode}
procedure NewfrmMain( var Result: PfrmMain; AParent: PControl );
begin

  {$IFDEF KOLCLASSES}
  Result := PfrmMain.Create;
  {$ELSE OBJECTS}
  New( Result, Create );
  {$ENDIF KOL CLASSES/OBJECTS}
  Result.Form := NewForm( AParent, '123' ).SetPosition( 192, 126 ){No MainMenu!}.SetClientSize( 430, 361 );
  Applet :=  Result.Form;
  Result.Form.Add2AutoFree( Result );
   {$IFDEF USE_NAMES}
    Result.Form.Name := 'frmMain';
   {$ENDIF}
    Result.Form.Style := Result.Form.Style and not (WS_MINIMIZEBOX or WS_MAXIMIZEBOX);
    DeleteMenu( GetSystemMenu( Result.Form.GetWindowHandle, False ), SC_CLOSE, MF_BYCOMMAND );
      Result.Form.OnClose := Result.frmMainClose;
    // Result.lbxPaths.TabOrder = 0
    Result.lbxPaths := NewListBox( Result.Form, [ loNoIntegralHeight ] ).SetAlign ( caTop ).SetSize( 0, 327 );
   {$IFDEF USE_NAMES}
    Result.lbxPaths.Name := 'lbxPaths';
   {$ENDIF}
    Result.lbxPaths.Color := clWindow;
    Result.lbxPaths.CurIndex := 0;
    // Result.Button1.TabOrder = 1
    Result.Button1 := NewButton( Result.Form, 'Add path' ).SetPosition( 2, 336 ).SetSize( 95, 0 );
   {$IFDEF USE_NAMES}
    Result.Button1.Name := 'Button1';
   {$ENDIF}
    // Result.Button2.TabOrder = 2
    Result.Button2 := NewButton( Result.Form, 'Remove path' ).SetPosition( 104, 336 ).SetSize( 97, 0 );
   {$IFDEF USE_NAMES}
    Result.Button2.Name := 'Button2';
   {$ENDIF}
    // Result.Button3.TabOrder = 3
    Result.Button3 := NewButton( Result.Form, 'Close' ).SetPosition( 332, 336 ).SetSize( 96, 0 );
   {$IFDEF USE_NAMES}
    Result.Button3.Name := 'Button3';
   {$ENDIF}
      Result.Button1.OnClick := Result.Button1Click;
      Result.Button2.OnClick := Result.Button2Click;
      Result.Button3.OnClick := Result.Button3Click;
    Result.Form.CenterOnParent.CanResize := False;
    Result.frmMainFormCreate( Result );

end;
{$ENDIF OldCode}

