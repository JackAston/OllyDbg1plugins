# Sample project file for BCC 5.5, makes attachanyway.dll

BCC = $(MAKEDIR)\..
PROJECT = attachanyway.dll
OBJFILES = attachanyway.obj
RESFILES =
RESDEPEN = $(RESFILES)
LIBFILES = ollydbg.lib
PATHCPP = .;
PATHASM = .;
PATHRC = .;
RELEASELIBPATH = $(BCC)\lib\release

CFLAG1 = -I$(BCC)\include -WD -Oc -O2 -w -Ve -C -AT -x- -RT- \
  -r -a1 -d -k- -K -y -v -vi -c -b- -w-par -w-inl -Vx -tWD 
RFLAGS = -i$(BCC)\include
AFLAGS = /i$(BCC)\include /mx /w2 /zd
LFLAGS = -L$(BCC)\lib\obj;$(BCC)\lib;$(RELEASELIBPATH) \
  -aa -Tpd -x -Gn -Gi -w -v
ALLOBJ = c0d32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cw32.lib

.autodepend
BCC32 = bcc32
CPP32 = cpp32
TASM32 = tasm32
LINKER = ilink32
BRCC32 = brcc32

.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
.PATH.ASM = $(PATHASM)
.PATH.RC  = $(PATHRC)

$(PROJECT): $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCC)\BIN\$(LINKER) @&&!
    $(LFLAGS) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!

.c.obj:
    $(BCC)\BIN\$(BCC32) $(CFLAG1) -n$(@D) {$< }

.asm.obj:
    $(BCC)\BIN\$(TASM32) $(AFLAGS) $<, $@

.rc.res:
    $(BCC)\BIN\$(BRCC32) $(RFLAGS) -fo$@ $<
