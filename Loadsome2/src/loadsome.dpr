{ KOL MCK } // Do not remove this line!
library loadsome;

// n1kto, 10/08/2006
// contact: look at forum on www.cracklab.ru

uses
  KOL,
  uMain in 'uMain.pas' {frmMain},
  Plugin in 'Plugin.pas',
  uEngine in 'uEngine.pas';

//{$R *.res}

begin // PROGRAM START HERE -- Please do not remove this comment

{$IFDEF KOL_MCK} {$I loadsome_0.inc} {$ELSE}


{$ENDIF}

end.

