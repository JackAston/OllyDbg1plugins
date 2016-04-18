*******************************
 PluginLoader v0.1    by NEG
*******************************
original language is japanese

■outline■
========
必要なときにプラグインをロードできるようにするOllyDbg1.10用プラグイン
It can load any plugin, when you need  
for OllyDbg1.10

■function■
========
1.最大100個のプラグインをサポート
support max 100 plugin
2.最大100個のショートカットキーをサポート
support max 100 shortcutkey 

■notes■
========
デバッグイベントを受け取るプラグインの場合、プロセス作成後にロードするとプロセス作成イベントなどを受け取れない
if you load plugin after process was made, plugin can't receive dbg event

OllyDbg.exeを書き換えるプラグインの場合、プラグインをアンロードするとアクセス違反が発生する場合がある
unload patching OllyDbg.exe plugin, cause access violation

uddファイルを読み書きするプラグインの場合、プロセス作成後にロードするとuddを読めず、プロセス終了前にアンロードするとuddにデータを保存できない
if you load plugin after process was made, plugin can't read udd
if you unload plugin before process was finish, plugin can't save udd


■ini form■
=============
[OllyDump]					;display name of plugin。you can use &
FileName=.\Plugin2\OllyDump301.dll		;File name
AutoLoad=0					;Auto = 1  not = 0
Shortcut=0x44,CS,0,PM_DISASM; 0x41,A,63,PM_MAIN	;shortcutkey ( you need ; )

;shortcutkey syntax: [virtual key code],[C(Ctrl)S(Shift)A(Alt)],[menu ID],[window class]
;use ; to separate next shortcutkey

;;window class definition
;PM_MAIN        = 0;   // Main window (NULL)
;PM_DUMP        = 10;  // Any Dump window (t_dump* )
;PM_MODULES     = 11;  // Modules window (t_module* )
;PM_MEMORY      = 12;  // Memory window (t_memory* )
;PM_THREADS     = 13;  // Threads window (t_thread* )
;PM_BREAKPOINTS = 14;  // Breakpoints window (t_bpoint* )
;PM_REFERENCES  = 15;  // References window (t_ref* )
;PM_RTRACE      = 16;  // Run trace window (int* )
;PM_WATCHES     = 17;  // Watches window (1-based index)
;PM_WINDOWS     = 18;  // Windows window (t_window* )
;PM_DISASM      = 31;  // CPU Disassembler (t_dump* )
;PM_CPUDUMP     = 32;  // CPU Dump (t_dump* )
;PM_CPUSTACK    = 33;  // CPU Stack (t_dump* )
;PM_CPUREGS     = 34;  // CPU Registers (t_reg* )

;;example
;0x44,CS,0,PM_DISASM	Ctrl+Shift+D = click plugin menu on disassemble window "0 &Dump debugged process" (only disassemble window)
;0x41,A,63,PM_MAIN	Alt+A = click plugin menu "63 &About" (only window class PM_MAIN)


Excuse me for my bad english    

translated by Ryohma