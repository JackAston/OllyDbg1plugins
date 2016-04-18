Catcha! v1.0
Coded by mikado @ vnsecurity, 4vn
Website: http://www.vnsecurity.net - http://www.4vn.org
Email: mikado[at]4vn[dot]org

[ About ]
Sometimes you don't know how to start a program correctly
from OllyDgb. Catcha! plugin will help you to attach to your
program automatically as soon as possible each time your
program runs (outside OllyDbg).

It works like Olly De-Attach Helper plugin:
http://www.openrce.org/downloads/details/185/Olly%20De-Attach%20Helper

Catcha! has more advantages than Olly De-Attach Helper.
It helps reversers not to miss many opcodes before attaching
target program.

Check it out! Have fun and feel free to contact me.

[ Instructions ]
- Copy Catcha!.dll and Catcha!.sys to OllyDbg plugin directory.
- First, select target program by chosing menu:
  Plugins -> Catcha! -> Select Catcha! target.
- Run target program outside OllyDbg.
  It will be attached in OllyDbg automatically as soon as possible.
- Press F9 to continue running program or,
  right click on Disassembler window and chose Thread -> Main
  on Popup menu to switch to program's main thread and continue
  your debug session.

[ History ]
2007.07.06:
- Version 1.0 released.

[ Known bugs ]
1. Target program can only be attached automatically one time.
   You have to restart OllyDbg in order for Catcha! to work correctly.
2. Only tested on Windows XP SP2. The kernel driver was built
   on WinDDK with Windows XP Build Environment.

[ TODO ]
- Fix bug (1).
- Implement de-attach function without closing target program.

mikado.