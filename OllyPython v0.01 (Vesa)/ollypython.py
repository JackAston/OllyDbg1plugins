# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.

import _ollypython
import new
new_instancemethod = new.instancemethod
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types



def Browsefilename(*args):
  """Browsefilename(char title, char name, char defext, int getarguments) -> int"""
  return _ollypython.Browsefilename(*args)

def Decodeaddress(*args):
  """
    Decodeaddress(ulong addr, ulong base, int addrmode, char symb, int nsymb, 
        char textlen_buffer) -> int
    """
  return _ollypython.Decodeaddress(*args)

def Decoderelativeoffset(*args):
  """Decoderelativeoffset(ulong addr, int addrmode, char symb, int nsymb) -> int"""
  return _ollypython.Decoderelativeoffset(*args)

def Decodecharacter(*args):
  """Decodecharacter(char textlen_buffer, uint c) -> int"""
  return _ollypython.Decodecharacter(*args)

def Printfloat4(*args):
  """Printfloat4(char textlen_buffer, float f) -> int"""
  return _ollypython.Printfloat4(*args)

def Printfloat8(*args):
  """Printfloat8(char textlen_buffer, double d) -> int"""
  return _ollypython.Printfloat8(*args)

def Printfloat10(*args):
  """Printfloat10(char textlen_buffer, long double ext) -> int"""
  return _ollypython.Printfloat10(*args)

def Print3dnow(*args):
  """Print3dnow(char textlen_buffer, uchar f) -> int"""
  return _ollypython.Print3dnow(*args)

def Printsse(*args):
  """Printsse(char textlen_buffer, char f) -> int"""
  return _ollypython.Printsse(*args)

def Findname(*args):
  """Findname(ulong addr, int type, char textlen_buffer) -> int"""
  return _ollypython.Findname(*args)

def Decodename(*args):
  """Decodename(ulong addr, int type, char textlen_buffer) -> int"""
  return _ollypython.Decodename(*args)

def Findnextname(*args):
  """Findnextname(char textlen_buffer) -> ulong"""
  return _ollypython.Findnextname(*args)

def Findlabelbyname(*args):
  """Findlabelbyname(char textlen_buffer, ulong OUTPUT, ulong addr0, ulong addr1) -> int"""
  return _ollypython.Findlabelbyname(*args)

def Findimportbyname(*args):
  """Findimportbyname(char textlen_buffer, ulong addr0, ulong addr1) -> ulong"""
  return _ollypython.Findimportbyname(*args)

def Findlabel(*args):
  """Findlabel(ulong addr, char textlen_buffer) -> int"""
  return _ollypython.Findlabel(*args)

def Demanglename(*args):
  """Demanglename(char name, int type, char textlen_buffer) -> int"""
  return _ollypython.Demanglename(*args)

def Findsymbolicname(*args):
  """Findsymbolicname(ulong addr, char textlen_buffer) -> int"""
  return _ollypython.Findsymbolicname(*args)

def Issuspicious(*args):
  """
    Issuspicious(char cmd, ulong size, ulong ip, ulong threadid, t_reg OUTPUT, 
        char textlen_buffer) -> int
    """
  return _ollypython.Issuspicious(*args)

def Assemble(*args):
  """
    Assemble(char cmd, ulong ip, t_asmmodel OUTPUT, int attempt, 
        int constsize, char textlen_buffer) -> int
    """
  return _ollypython.Assemble(*args)

def Decodethreadname(*args):
  """Decodethreadname(char textlen_buffer, ulong threadid, int mode) -> int"""
  return _ollypython.Decodethreadname(*args)

def Decoderange(*args):
  """Decoderange(ulong addr, ulong size, char textlen_buffer) -> int"""
  return _ollypython.Decoderange(*args)

def Readcommand(*args):
  """Readcommand(ulong ip, char cmdbuffer) -> ulong"""
  return _ollypython.Readcommand(*args)

def Getruntraceregisters(*args):
  """
    Getruntraceregisters(int nback, t_reg OUTPUT, t_reg OUTPUT, char cmdbuffer, 
        char textlen_buffer) -> int
    """
  return _ollypython.Getruntraceregisters(*args)

def Decodeascii(*args):
  """Decodeascii(ulong value, char textlen_buffer, int len, int mode) -> int"""
  return _ollypython.Decodeascii(*args)

def Decodeunicode(*args):
  """Decodeunicode(ulong value, char textlen_buffer, int len) -> int"""
  return _ollypython.Decodeunicode(*args)

def Getresourcestring(*args):
  """Getresourcestring(t_module pm, ulong id, char textlen_buffer) -> int"""
  return _ollypython.Getresourcestring(*args)

def Getwatch(*args):
  """Getwatch(int indexone, char textlen_buffer) -> int"""
  return _ollypython.Getwatch(*args)

def Pluginreadstringfromini(*args):
  """Pluginreadstringfromini(HINSTANCE dllinst, char key, char textlen_buffer, char def) -> int"""
  return _ollypython.Pluginreadstringfromini(*args)

def Registerpluginclass(*args):
  """Registerpluginclass(char name, char iconname, HINSTANCE dllinst, WNDPROC classproc) -> int"""
  return _ollypython.Registerpluginclass(*args)

def Disasm(*args):
  """
    Disasm(uchar src, ulong srcsize, ulong srcip, uchar srcdec, 
        t_disasm OUTPUT, int disasmmode, ulong threadid) -> ulong
    """
  return _ollypython.Disasm(*args)

def Expression(*args):
  """
    Expression(t_result OUTPUT, char expression, int a, int b, uchar data, 
        ulong database, ulong datasize, ulong threadid) -> int
    """
  return _ollypython.Expression(*args)

def Getbreakpointtypecount(*args):
  """Getbreakpointtypecount(ulong addr, ulong OUTPUT) -> ulong"""
  return _ollypython.Getbreakpointtypecount(*args)

def Findhittrace(*args):
  """Findhittrace(ulong addr, uchar OUTPUT, ulong OUTPUT) -> uchar"""
  return _ollypython.Findhittrace(*args)

def Isretaddr(*args):
  """Isretaddr(ulong retaddr, ulong OUTPUT) -> ulong"""
  return _ollypython.Isretaddr(*args)

def Getdisassemblerrange(*args):
  """Getdisassemblerrange(ulong OUTPUT, ulong OUTPUT)"""
  return _ollypython.Getdisassemblerrange(*args)

def Getproclimits(*args):
  """Getproclimits(ulong addr, ulong OUTPUT, ulong OUTPUT) -> int"""
  return _ollypython.Getproclimits(*args)

def Getsourcefilelimits(*args):
  """Getsourcefilelimits(ulong nameaddr, ulong OUTPUT, ulong OUTPUT) -> int"""
  return _ollypython.Getsourcefilelimits(*args)
PLUGIN_VERSION = _ollypython.PLUGIN_VERSION
TEXTLEN = _ollypython.TEXTLEN
ARGLEN = _ollypython.ARGLEN
USERLEN = _ollypython.USERLEN
SHORTLEN = _ollypython.SHORTLEN
BLACK = _ollypython.BLACK
BLUE = _ollypython.BLUE
GREEN = _ollypython.GREEN
CYAN = _ollypython.CYAN
RED = _ollypython.RED
MAGENTA = _ollypython.MAGENTA
BROWN = _ollypython.BROWN
LIGHTGRAY = _ollypython.LIGHTGRAY
DARKGRAY = _ollypython.DARKGRAY
LIGHTBLUE = _ollypython.LIGHTBLUE
LIGHTGREEN = _ollypython.LIGHTGREEN
LIGHTCYAN = _ollypython.LIGHTCYAN
LIGHTRED = _ollypython.LIGHTRED
LIGHTMAGENTA = _ollypython.LIGHTMAGENTA
YELLOW = _ollypython.YELLOW
WHITE = _ollypython.WHITE
MINT = _ollypython.MINT
SKYBLUE = _ollypython.SKYBLUE
IVORY = _ollypython.IVORY
GRAY = _ollypython.GRAY
NCOLORS = _ollypython.NCOLORS
BKTRANSP = _ollypython.BKTRANSP
BKBLACK = _ollypython.BKBLACK
BKGRAY = _ollypython.BKGRAY
BKWHITE = _ollypython.BKWHITE
BKCYAN = _ollypython.BKCYAN
BKGREEN = _ollypython.BKGREEN
BKRED = _ollypython.BKRED
BKYELLOW = _ollypython.BKYELLOW
BLACKWHITE = _ollypython.BLACKWHITE
BLUEGOLD = _ollypython.BLUEGOLD
SKYWIND = _ollypython.SKYWIND
NIGHTSTARS = _ollypython.NIGHTSTARS
SCHEME4 = _ollypython.SCHEME4
SCHEME5 = _ollypython.SCHEME5
SCHEME6 = _ollypython.SCHEME6
SCHEME7 = _ollypython.SCHEME7
FIXEDFONT = _ollypython.FIXEDFONT
TERMINAL6 = _ollypython.TERMINAL6
FIXEDSYS = _ollypython.FIXEDSYS
COURIERFONT = _ollypython.COURIERFONT
LUCIDACONS = _ollypython.LUCIDACONS
FONT5 = _ollypython.FONT5
FONT6 = _ollypython.FONT6
FONT7 = _ollypython.FONT7
MAINFONT = _ollypython.MAINFONT
SYSFONT = _ollypython.SYSFONT
INFOFONT = _ollypython.INFOFONT
ADC_DEFAULT = _ollypython.ADC_DEFAULT
ADC_DIFFMOD = _ollypython.ADC_DIFFMOD
ADC_NOMODNAME = _ollypython.ADC_NOMODNAME
ADC_VALID = _ollypython.ADC_VALID
ADC_INMODULE = _ollypython.ADC_INMODULE
ADC_SAMEMOD = _ollypython.ADC_SAMEMOD
ADC_SYMBOL = _ollypython.ADC_SYMBOL
ADC_JUMP = _ollypython.ADC_JUMP
ADC_OFFSET = _ollypython.ADC_OFFSET
ADC_STRING = _ollypython.ADC_STRING
ADC_ENTRY = _ollypython.ADC_ENTRY
ADC_UPPERCASE = _ollypython.ADC_UPPERCASE
ADC_WIDEFORM = _ollypython.ADC_WIDEFORM
ADC_NONTRIVIAL = _ollypython.ADC_NONTRIVIAL
ADC_DYNAMIC = _ollypython.ADC_DYNAMIC
PLAINASCII = _ollypython.PLAINASCII
DIACRITICAL = _ollypython.DIACRITICAL
RAREASCII = _ollypython.RAREASCII
MAXCMDSIZE = _ollypython.MAXCMDSIZE
NSEQ = _ollypython.NSEQ
NMODELS = _ollypython.NMODELS
DIA_ASKGLOBAL = _ollypython.DIA_ASKGLOBAL
DIA_HEXONLY = _ollypython.DIA_HEXONLY
DIA_ALIGNED = _ollypython.DIA_ALIGNED
DIA_DEFHEX = _ollypython.DIA_DEFHEX
DIA_DEFASCII = _ollypython.DIA_DEFASCII
DIA_DEFUNICODE = _ollypython.DIA_DEFUNICODE
DIA_SEARCH = _ollypython.DIA_SEARCH
DIA_HISTORY = _ollypython.DIA_HISTORY
ANIMATE_OFF = _ollypython.ANIMATE_OFF
ANIMATE_IN = _ollypython.ANIMATE_IN
ANIMATE_OVER = _ollypython.ANIMATE_OVER
ANIMATE_RET = _ollypython.ANIMATE_RET
ANIMATE_SKPRET = _ollypython.ANIMATE_SKPRET
ANIMATE_USER = _ollypython.ANIMATE_USER
ANIMATE_TRIN = _ollypython.ANIMATE_TRIN
ANIMATE_TROVER = _ollypython.ANIMATE_TROVER
ANIMATE_STOP = _ollypython.ANIMATE_STOP
class t_hexstr(_object):
    """Proxy of C t_hexstr struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_hexstr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_hexstr, name)
    __repr__ = _swig_repr
    __swig_setmethods__["n"] = _ollypython.t_hexstr_n_set
    __swig_getmethods__["n"] = _ollypython.t_hexstr_n_get
    if _newclass:n = property(_ollypython.t_hexstr_n_get, _ollypython.t_hexstr_n_set)
    __swig_setmethods__["data"] = _ollypython.t_hexstr_data_set
    __swig_getmethods__["data"] = _ollypython.t_hexstr_data_get
    if _newclass:data = property(_ollypython.t_hexstr_data_get, _ollypython.t_hexstr_data_set)
    __swig_setmethods__["mask"] = _ollypython.t_hexstr_mask_set
    __swig_getmethods__["mask"] = _ollypython.t_hexstr_mask_get
    if _newclass:mask = property(_ollypython.t_hexstr_mask_get, _ollypython.t_hexstr_mask_set)
    def __init__(self, *args): 
        """__init__(self) -> t_hexstr"""
        this = _ollypython.new_t_hexstr(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_hexstr
    __del__ = lambda self : None;
t_hexstr_swigregister = _ollypython.t_hexstr_swigregister
t_hexstr_swigregister(t_hexstr)

class t_asmmodel(_object):
    """Proxy of C t_asmmodel struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_asmmodel, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_asmmodel, name)
    __repr__ = _swig_repr
    __swig_setmethods__["code"] = _ollypython.t_asmmodel_code_set
    __swig_getmethods__["code"] = _ollypython.t_asmmodel_code_get
    if _newclass:code = property(_ollypython.t_asmmodel_code_get, _ollypython.t_asmmodel_code_set)
    __swig_setmethods__["mask"] = _ollypython.t_asmmodel_mask_set
    __swig_getmethods__["mask"] = _ollypython.t_asmmodel_mask_get
    if _newclass:mask = property(_ollypython.t_asmmodel_mask_get, _ollypython.t_asmmodel_mask_set)
    __swig_setmethods__["length"] = _ollypython.t_asmmodel_length_set
    __swig_getmethods__["length"] = _ollypython.t_asmmodel_length_get
    if _newclass:length = property(_ollypython.t_asmmodel_length_get, _ollypython.t_asmmodel_length_set)
    __swig_setmethods__["jmpsize"] = _ollypython.t_asmmodel_jmpsize_set
    __swig_getmethods__["jmpsize"] = _ollypython.t_asmmodel_jmpsize_get
    if _newclass:jmpsize = property(_ollypython.t_asmmodel_jmpsize_get, _ollypython.t_asmmodel_jmpsize_set)
    __swig_setmethods__["jmpoffset"] = _ollypython.t_asmmodel_jmpoffset_set
    __swig_getmethods__["jmpoffset"] = _ollypython.t_asmmodel_jmpoffset_get
    if _newclass:jmpoffset = property(_ollypython.t_asmmodel_jmpoffset_get, _ollypython.t_asmmodel_jmpoffset_set)
    __swig_setmethods__["jmppos"] = _ollypython.t_asmmodel_jmppos_set
    __swig_getmethods__["jmppos"] = _ollypython.t_asmmodel_jmppos_get
    if _newclass:jmppos = property(_ollypython.t_asmmodel_jmppos_get, _ollypython.t_asmmodel_jmppos_set)
    def __init__(self, *args): 
        """__init__(self) -> t_asmmodel"""
        this = _ollypython.new_t_asmmodel(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_asmmodel
    __del__ = lambda self : None;
t_asmmodel_swigregister = _ollypython.t_asmmodel_swigregister
t_asmmodel_swigregister(t_asmmodel)

class t_extmodel(_object):
    """Proxy of C t_extmodel struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_extmodel, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_extmodel, name)
    __repr__ = _swig_repr
    __swig_setmethods__["code"] = _ollypython.t_extmodel_code_set
    __swig_getmethods__["code"] = _ollypython.t_extmodel_code_get
    if _newclass:code = property(_ollypython.t_extmodel_code_get, _ollypython.t_extmodel_code_set)
    __swig_setmethods__["mask"] = _ollypython.t_extmodel_mask_set
    __swig_getmethods__["mask"] = _ollypython.t_extmodel_mask_get
    if _newclass:mask = property(_ollypython.t_extmodel_mask_get, _ollypython.t_extmodel_mask_set)
    __swig_setmethods__["length"] = _ollypython.t_extmodel_length_set
    __swig_getmethods__["length"] = _ollypython.t_extmodel_length_get
    if _newclass:length = property(_ollypython.t_extmodel_length_get, _ollypython.t_extmodel_length_set)
    __swig_setmethods__["jmpsize"] = _ollypython.t_extmodel_jmpsize_set
    __swig_getmethods__["jmpsize"] = _ollypython.t_extmodel_jmpsize_get
    if _newclass:jmpsize = property(_ollypython.t_extmodel_jmpsize_get, _ollypython.t_extmodel_jmpsize_set)
    __swig_setmethods__["jmpoffset"] = _ollypython.t_extmodel_jmpoffset_set
    __swig_getmethods__["jmpoffset"] = _ollypython.t_extmodel_jmpoffset_get
    if _newclass:jmpoffset = property(_ollypython.t_extmodel_jmpoffset_get, _ollypython.t_extmodel_jmpoffset_set)
    __swig_setmethods__["jmppos"] = _ollypython.t_extmodel_jmppos_set
    __swig_getmethods__["jmppos"] = _ollypython.t_extmodel_jmppos_get
    if _newclass:jmppos = property(_ollypython.t_extmodel_jmppos_get, _ollypython.t_extmodel_jmppos_set)
    __swig_setmethods__["isany"] = _ollypython.t_extmodel_isany_set
    __swig_getmethods__["isany"] = _ollypython.t_extmodel_isany_get
    if _newclass:isany = property(_ollypython.t_extmodel_isany_get, _ollypython.t_extmodel_isany_set)
    __swig_setmethods__["cmdoffset"] = _ollypython.t_extmodel_cmdoffset_set
    __swig_getmethods__["cmdoffset"] = _ollypython.t_extmodel_cmdoffset_get
    if _newclass:cmdoffset = property(_ollypython.t_extmodel_cmdoffset_get, _ollypython.t_extmodel_cmdoffset_set)
    __swig_setmethods__["ramask"] = _ollypython.t_extmodel_ramask_set
    __swig_getmethods__["ramask"] = _ollypython.t_extmodel_ramask_get
    if _newclass:ramask = property(_ollypython.t_extmodel_ramask_get, _ollypython.t_extmodel_ramask_set)
    __swig_setmethods__["rbmask"] = _ollypython.t_extmodel_rbmask_set
    __swig_getmethods__["rbmask"] = _ollypython.t_extmodel_rbmask_get
    if _newclass:rbmask = property(_ollypython.t_extmodel_rbmask_get, _ollypython.t_extmodel_rbmask_set)
    def __init__(self, *args): 
        """__init__(self) -> t_extmodel"""
        this = _ollypython.new_t_extmodel(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_extmodel
    __del__ = lambda self : None;
t_extmodel_swigregister = _ollypython.t_extmodel_swigregister
t_extmodel_swigregister(t_extmodel)

NBAR = _ollypython.NBAR
BAR_PRESSED = _ollypython.BAR_PRESSED
BAR_DISABLED = _ollypython.BAR_DISABLED
BAR_NOSORT = _ollypython.BAR_NOSORT
BAR_NORESIZE = _ollypython.BAR_NORESIZE
BAR_BUTTON = _ollypython.BAR_BUTTON
BAR_SHIFTSEL = _ollypython.BAR_SHIFTSEL
CAPT_FREE = _ollypython.CAPT_FREE
TABLE_DIR = _ollypython.TABLE_DIR
TABLE_COPYMENU = _ollypython.TABLE_COPYMENU
TABLE_SORTMENU = _ollypython.TABLE_SORTMENU
TABLE_APPMENU = _ollypython.TABLE_APPMENU
TABLE_WIDECOL = _ollypython.TABLE_WIDECOL
TABLE_USERAPP = _ollypython.TABLE_USERAPP
TABLE_USERDEF = _ollypython.TABLE_USERDEF
TABLE_NOHSCR = _ollypython.TABLE_NOHSCR
TABLE_SAVEPOS = _ollypython.TABLE_SAVEPOS
TABLE_CPU = _ollypython.TABLE_CPU
TABLE_FASTSEL = _ollypython.TABLE_FASTSEL
TABLE_COLSEL = _ollypython.TABLE_COLSEL
TABLE_SAVEAPP = _ollypython.TABLE_SAVEAPP
TABLE_HILMENU = _ollypython.TABLE_HILMENU
TABLE_ONTOP = _ollypython.TABLE_ONTOP
DRAW_NORMAL = _ollypython.DRAW_NORMAL
DRAW_GRAY = _ollypython.DRAW_GRAY
DRAW_HILITE = _ollypython.DRAW_HILITE
DRAW_UL = _ollypython.DRAW_UL
DRAW_SELECT = _ollypython.DRAW_SELECT
DRAW_EIP = _ollypython.DRAW_EIP
DRAW_BREAK = _ollypython.DRAW_BREAK
DRAW_GRAPH = _ollypython.DRAW_GRAPH
DRAW_DIRECT = _ollypython.DRAW_DIRECT
DRAW_MASK = _ollypython.DRAW_MASK
DRAW_EXTSEL = _ollypython.DRAW_EXTSEL
DRAW_UNICODE = _ollypython.DRAW_UNICODE
DRAW_TOP = _ollypython.DRAW_TOP
DRAW_BOTTOM = _ollypython.DRAW_BOTTOM
D_SPACE = _ollypython.D_SPACE
D_SEP = _ollypython.D_SEP
D_BEGIN = _ollypython.D_BEGIN
D_BODY = _ollypython.D_BODY
D_ENTRY = _ollypython.D_ENTRY
D_LEAF = _ollypython.D_LEAF
D_END = _ollypython.D_END
D_SINGLE = _ollypython.D_SINGLE
D_ENDBEG = _ollypython.D_ENDBEG
D_POINT = _ollypython.D_POINT
D_JMPUP = _ollypython.D_JMPUP
D_JMPOUT = _ollypython.D_JMPOUT
D_JMPDN = _ollypython.D_JMPDN
D_PATHUP = _ollypython.D_PATHUP
D_GRAYUP = _ollypython.D_GRAYUP
D_PATH = _ollypython.D_PATH
D_GRAYPATH = _ollypython.D_GRAYPATH
D_PATHDN = _ollypython.D_PATHDN
D_GRAYDN = _ollypython.D_GRAYDN
D_PATHUPEND = _ollypython.D_PATHUPEND
D_GRAYUPEND = _ollypython.D_GRAYUPEND
D_PATHDNEND = _ollypython.D_PATHDNEND
D_GRAYDNEND = _ollypython.D_GRAYDNEND
D_SWTOP = _ollypython.D_SWTOP
D_SWBODY = _ollypython.D_SWBODY
D_CASE = _ollypython.D_CASE
D_LASTCASE = _ollypython.D_LASTCASE
TY_NEW = _ollypython.TY_NEW
TY_CONFIRMED = _ollypython.TY_CONFIRMED
TY_MAIN = _ollypython.TY_MAIN
TY_INVALID = _ollypython.TY_INVALID
TY_SELECTED = _ollypython.TY_SELECTED
TY_REPORTED = _ollypython.TY_REPORTED
TY_REFERENCE = _ollypython.TY_REFERENCE
TY_ORIGIN = _ollypython.TY_ORIGIN
TY_STOPAN = _ollypython.TY_STOPAN
TY_SET = _ollypython.TY_SET
TY_ACTIVE = _ollypython.TY_ACTIVE
TY_DISABLED = _ollypython.TY_DISABLED
TY_ONESHOT = _ollypython.TY_ONESHOT
TY_TEMP = _ollypython.TY_TEMP
TY_KEEPCODE = _ollypython.TY_KEEPCODE
TY_KEEPCOND = _ollypython.TY_KEEPCOND
TY_NOUPDATE = _ollypython.TY_NOUPDATE
TY_RTRACE = _ollypython.TY_RTRACE
TY_EXPORT = _ollypython.TY_EXPORT
TY_IMPORT = _ollypython.TY_IMPORT
TY_LIBRARY = _ollypython.TY_LIBRARY
TY_LABEL = _ollypython.TY_LABEL
TY_ANYNAME = _ollypython.TY_ANYNAME
TY_KNOWN = _ollypython.TY_KNOWN
TY_DEFHEAP = _ollypython.TY_DEFHEAP
TY_HEAP = _ollypython.TY_HEAP
TY_SFX = _ollypython.TY_SFX
TY_CODE = _ollypython.TY_CODE
TY_DATA = _ollypython.TY_DATA
TY_IMPDATA = _ollypython.TY_IMPDATA
TY_EXPDATA = _ollypython.TY_EXPDATA
TY_RSRC = _ollypython.TY_RSRC
TY_RELOC = _ollypython.TY_RELOC
TY_STACK = _ollypython.TY_STACK
TY_THREAD = _ollypython.TY_THREAD
TY_HEADER = _ollypython.TY_HEADER
TY_ANYMEM = _ollypython.TY_ANYMEM
TY_GUARDED = _ollypython.TY_GUARDED
TY_PURE = _ollypython.TY_PURE
TY_PASCAL = _ollypython.TY_PASCAL
TY_C = _ollypython.TY_C
TY_NOTENTRY = _ollypython.TY_NOTENTRY
TY_CHARSW = _ollypython.TY_CHARSW
TY_WMSW = _ollypython.TY_WMSW
TY_EXCEPTSW = _ollypython.TY_EXCEPTSW
TY_RELIABLE = _ollypython.TY_RELIABLE
TY_GUESSED = _ollypython.TY_GUESSED
TY_BELONGS = _ollypython.TY_BELONGS
TY_RECURSIVE = _ollypython.TY_RECURSIVE
TY_TERMINAL = _ollypython.TY_TERMINAL
TY_SYSTEM = _ollypython.TY_SYSTEM
TY_DIRECT = _ollypython.TY_DIRECT
TY_NODATA = _ollypython.TY_NODATA
TY_DUMMY = _ollypython.TY_DUMMY
TY_NOSIDE = _ollypython.TY_NOSIDE
class t_scheme(_object):
    """Proxy of C t_scheme struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_scheme, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_scheme, name)
    __repr__ = _swig_repr
    __swig_setmethods__["name"] = _ollypython.t_scheme_name_set
    __swig_getmethods__["name"] = _ollypython.t_scheme_name_get
    if _newclass:name = property(_ollypython.t_scheme_name_get, _ollypython.t_scheme_name_set)
    __swig_setmethods__["textcolor"] = _ollypython.t_scheme_textcolor_set
    __swig_getmethods__["textcolor"] = _ollypython.t_scheme_textcolor_get
    if _newclass:textcolor = property(_ollypython.t_scheme_textcolor_get, _ollypython.t_scheme_textcolor_set)
    __swig_setmethods__["hitextcolor"] = _ollypython.t_scheme_hitextcolor_set
    __swig_getmethods__["hitextcolor"] = _ollypython.t_scheme_hitextcolor_get
    if _newclass:hitextcolor = property(_ollypython.t_scheme_hitextcolor_get, _ollypython.t_scheme_hitextcolor_set)
    __swig_setmethods__["lowcolor"] = _ollypython.t_scheme_lowcolor_set
    __swig_getmethods__["lowcolor"] = _ollypython.t_scheme_lowcolor_get
    if _newclass:lowcolor = property(_ollypython.t_scheme_lowcolor_get, _ollypython.t_scheme_lowcolor_set)
    __swig_setmethods__["bkcolor"] = _ollypython.t_scheme_bkcolor_set
    __swig_getmethods__["bkcolor"] = _ollypython.t_scheme_bkcolor_get
    if _newclass:bkcolor = property(_ollypython.t_scheme_bkcolor_get, _ollypython.t_scheme_bkcolor_set)
    __swig_setmethods__["selbkcolor"] = _ollypython.t_scheme_selbkcolor_set
    __swig_getmethods__["selbkcolor"] = _ollypython.t_scheme_selbkcolor_get
    if _newclass:selbkcolor = property(_ollypython.t_scheme_selbkcolor_get, _ollypython.t_scheme_selbkcolor_set)
    __swig_setmethods__["linecolor"] = _ollypython.t_scheme_linecolor_set
    __swig_getmethods__["linecolor"] = _ollypython.t_scheme_linecolor_get
    if _newclass:linecolor = property(_ollypython.t_scheme_linecolor_get, _ollypython.t_scheme_linecolor_set)
    __swig_setmethods__["auxcolor"] = _ollypython.t_scheme_auxcolor_set
    __swig_getmethods__["auxcolor"] = _ollypython.t_scheme_auxcolor_get
    if _newclass:auxcolor = property(_ollypython.t_scheme_auxcolor_get, _ollypython.t_scheme_auxcolor_set)
    __swig_setmethods__["condbkcolor"] = _ollypython.t_scheme_condbkcolor_set
    __swig_getmethods__["condbkcolor"] = _ollypython.t_scheme_condbkcolor_get
    if _newclass:condbkcolor = property(_ollypython.t_scheme_condbkcolor_get, _ollypython.t_scheme_condbkcolor_set)
    def __init__(self, *args): 
        """__init__(self) -> t_scheme"""
        this = _ollypython.new_t_scheme(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_scheme
    __del__ = lambda self : None;
t_scheme_swigregister = _ollypython.t_scheme_swigregister
t_scheme_swigregister(t_scheme)

class t_schemeopt(_object):
    """Proxy of C t_schemeopt struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_schemeopt, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_schemeopt, name)
    __repr__ = _swig_repr
    __swig_setmethods__["name"] = _ollypython.t_schemeopt_name_set
    __swig_getmethods__["name"] = _ollypython.t_schemeopt_name_get
    if _newclass:name = property(_ollypython.t_schemeopt_name_get, _ollypython.t_schemeopt_name_set)
    __swig_setmethods__["color"] = _ollypython.t_schemeopt_color_set
    __swig_getmethods__["color"] = _ollypython.t_schemeopt_color_get
    if _newclass:color = property(_ollypython.t_schemeopt_color_get, _ollypython.t_schemeopt_color_set)
    def __init__(self, *args): 
        """__init__(self) -> t_schemeopt"""
        this = _ollypython.new_t_schemeopt(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_schemeopt
    __del__ = lambda self : None;
t_schemeopt_swigregister = _ollypython.t_schemeopt_swigregister
t_schemeopt_swigregister(t_schemeopt)

class t_bar(_object):
    """Proxy of C t_bar struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_bar, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_bar, name)
    __repr__ = _swig_repr
    __swig_setmethods__["nbar"] = _ollypython.t_bar_nbar_set
    __swig_getmethods__["nbar"] = _ollypython.t_bar_nbar_get
    if _newclass:nbar = property(_ollypython.t_bar_nbar_get, _ollypython.t_bar_nbar_set)
    __swig_setmethods__["font"] = _ollypython.t_bar_font_set
    __swig_getmethods__["font"] = _ollypython.t_bar_font_get
    if _newclass:font = property(_ollypython.t_bar_font_get, _ollypython.t_bar_font_set)
    __swig_setmethods__["dx"] = _ollypython.t_bar_dx_set
    __swig_getmethods__["dx"] = _ollypython.t_bar_dx_get
    if _newclass:dx = property(_ollypython.t_bar_dx_get, _ollypython.t_bar_dx_set)
    __swig_setmethods__["defdx"] = _ollypython.t_bar_defdx_set
    __swig_getmethods__["defdx"] = _ollypython.t_bar_defdx_get
    if _newclass:defdx = property(_ollypython.t_bar_defdx_get, _ollypython.t_bar_defdx_set)
    __swig_setmethods__["name"] = _ollypython.t_bar_name_set
    __swig_getmethods__["name"] = _ollypython.t_bar_name_get
    if _newclass:name = property(_ollypython.t_bar_name_get, _ollypython.t_bar_name_set)
    __swig_setmethods__["mode"] = _ollypython.t_bar_mode_set
    __swig_getmethods__["mode"] = _ollypython.t_bar_mode_get
    if _newclass:mode = property(_ollypython.t_bar_mode_get, _ollypython.t_bar_mode_set)
    __swig_setmethods__["captured"] = _ollypython.t_bar_captured_set
    __swig_getmethods__["captured"] = _ollypython.t_bar_captured_get
    if _newclass:captured = property(_ollypython.t_bar_captured_get, _ollypython.t_bar_captured_set)
    __swig_setmethods__["active"] = _ollypython.t_bar_active_set
    __swig_getmethods__["active"] = _ollypython.t_bar_active_get
    if _newclass:active = property(_ollypython.t_bar_active_get, _ollypython.t_bar_active_set)
    __swig_setmethods__["prevx"] = _ollypython.t_bar_prevx_set
    __swig_getmethods__["prevx"] = _ollypython.t_bar_prevx_get
    if _newclass:prevx = property(_ollypython.t_bar_prevx_get, _ollypython.t_bar_prevx_set)
    def __init__(self, *args): 
        """__init__(self) -> t_bar"""
        this = _ollypython.new_t_bar(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_bar
    __del__ = lambda self : None;
t_bar_swigregister = _ollypython.t_bar_swigregister
t_bar_swigregister(t_bar)

class t_sortheader(_object):
    """Proxy of C t_sortheader struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_sortheader, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_sortheader, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_sortheader_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_sortheader_addr_get
    if _newclass:addr = property(_ollypython.t_sortheader_addr_get, _ollypython.t_sortheader_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_sortheader_size_set
    __swig_getmethods__["size"] = _ollypython.t_sortheader_size_get
    if _newclass:size = property(_ollypython.t_sortheader_size_get, _ollypython.t_sortheader_size_set)
    __swig_setmethods__["type"] = _ollypython.t_sortheader_type_set
    __swig_getmethods__["type"] = _ollypython.t_sortheader_type_get
    if _newclass:type = property(_ollypython.t_sortheader_type_get, _ollypython.t_sortheader_type_set)
    def __init__(self, *args): 
        """__init__(self) -> t_sortheader"""
        this = _ollypython.new_t_sortheader(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_sortheader
    __del__ = lambda self : None;
t_sortheader_swigregister = _ollypython.t_sortheader_swigregister
t_sortheader_swigregister(t_sortheader)

class t_sorted(_object):
    """Proxy of C t_sorted struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_sorted, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_sorted, name)
    __repr__ = _swig_repr
    __swig_setmethods__["name"] = _ollypython.t_sorted_name_set
    __swig_getmethods__["name"] = _ollypython.t_sorted_name_get
    if _newclass:name = property(_ollypython.t_sorted_name_get, _ollypython.t_sorted_name_set)
    __swig_setmethods__["n"] = _ollypython.t_sorted_n_set
    __swig_getmethods__["n"] = _ollypython.t_sorted_n_get
    if _newclass:n = property(_ollypython.t_sorted_n_get, _ollypython.t_sorted_n_set)
    __swig_setmethods__["nmax"] = _ollypython.t_sorted_nmax_set
    __swig_getmethods__["nmax"] = _ollypython.t_sorted_nmax_get
    if _newclass:nmax = property(_ollypython.t_sorted_nmax_get, _ollypython.t_sorted_nmax_set)
    __swig_setmethods__["selected"] = _ollypython.t_sorted_selected_set
    __swig_getmethods__["selected"] = _ollypython.t_sorted_selected_get
    if _newclass:selected = property(_ollypython.t_sorted_selected_get, _ollypython.t_sorted_selected_set)
    __swig_setmethods__["seladdr"] = _ollypython.t_sorted_seladdr_set
    __swig_getmethods__["seladdr"] = _ollypython.t_sorted_seladdr_get
    if _newclass:seladdr = property(_ollypython.t_sorted_seladdr_get, _ollypython.t_sorted_seladdr_set)
    __swig_setmethods__["itemsize"] = _ollypython.t_sorted_itemsize_set
    __swig_getmethods__["itemsize"] = _ollypython.t_sorted_itemsize_get
    if _newclass:itemsize = property(_ollypython.t_sorted_itemsize_get, _ollypython.t_sorted_itemsize_set)
    __swig_setmethods__["version"] = _ollypython.t_sorted_version_set
    __swig_getmethods__["version"] = _ollypython.t_sorted_version_get
    if _newclass:version = property(_ollypython.t_sorted_version_get, _ollypython.t_sorted_version_set)
    __swig_setmethods__["data"] = _ollypython.t_sorted_data_set
    __swig_getmethods__["data"] = _ollypython.t_sorted_data_get
    if _newclass:data = property(_ollypython.t_sorted_data_get, _ollypython.t_sorted_data_set)
    __swig_setmethods__["sortfunc"] = _ollypython.t_sorted_sortfunc_set
    __swig_getmethods__["sortfunc"] = _ollypython.t_sorted_sortfunc_get
    if _newclass:sortfunc = property(_ollypython.t_sorted_sortfunc_get, _ollypython.t_sorted_sortfunc_set)
    __swig_setmethods__["destfunc"] = _ollypython.t_sorted_destfunc_set
    __swig_getmethods__["destfunc"] = _ollypython.t_sorted_destfunc_get
    if _newclass:destfunc = property(_ollypython.t_sorted_destfunc_get, _ollypython.t_sorted_destfunc_set)
    __swig_setmethods__["sort"] = _ollypython.t_sorted_sort_set
    __swig_getmethods__["sort"] = _ollypython.t_sorted_sort_get
    if _newclass:sort = property(_ollypython.t_sorted_sort_get, _ollypython.t_sorted_sort_set)
    __swig_setmethods__["sorted"] = _ollypython.t_sorted_sorted_set
    __swig_getmethods__["sorted"] = _ollypython.t_sorted_sorted_get
    if _newclass:sorted = property(_ollypython.t_sorted_sorted_get, _ollypython.t_sorted_sorted_set)
    __swig_setmethods__["index"] = _ollypython.t_sorted_index_set
    __swig_getmethods__["index"] = _ollypython.t_sorted_index_get
    if _newclass:index = property(_ollypython.t_sorted_index_get, _ollypython.t_sorted_index_set)
    __swig_setmethods__["suppresserr"] = _ollypython.t_sorted_suppresserr_set
    __swig_getmethods__["suppresserr"] = _ollypython.t_sorted_suppresserr_get
    if _newclass:suppresserr = property(_ollypython.t_sorted_suppresserr_get, _ollypython.t_sorted_suppresserr_set)
    def __init__(self, *args): 
        """__init__(self) -> t_sorted"""
        this = _ollypython.new_t_sorted(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_sorted
    __del__ = lambda self : None;
t_sorted_swigregister = _ollypython.t_sorted_swigregister
t_sorted_swigregister(t_sorted)

class t_table(_object):
    """Proxy of C t_table struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_table, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_table, name)
    __repr__ = _swig_repr
    __swig_setmethods__["hw"] = _ollypython.t_table_hw_set
    __swig_getmethods__["hw"] = _ollypython.t_table_hw_get
    if _newclass:hw = property(_ollypython.t_table_hw_get, _ollypython.t_table_hw_set)
    __swig_setmethods__["data"] = _ollypython.t_table_data_set
    __swig_getmethods__["data"] = _ollypython.t_table_data_get
    if _newclass:data = property(_ollypython.t_table_data_get, _ollypython.t_table_data_set)
    __swig_setmethods__["bar"] = _ollypython.t_table_bar_set
    __swig_getmethods__["bar"] = _ollypython.t_table_bar_get
    if _newclass:bar = property(_ollypython.t_table_bar_get, _ollypython.t_table_bar_set)
    __swig_setmethods__["showbar"] = _ollypython.t_table_showbar_set
    __swig_getmethods__["showbar"] = _ollypython.t_table_showbar_get
    if _newclass:showbar = property(_ollypython.t_table_showbar_get, _ollypython.t_table_showbar_set)
    __swig_setmethods__["hscroll"] = _ollypython.t_table_hscroll_set
    __swig_getmethods__["hscroll"] = _ollypython.t_table_hscroll_get
    if _newclass:hscroll = property(_ollypython.t_table_hscroll_get, _ollypython.t_table_hscroll_set)
    __swig_setmethods__["colsel"] = _ollypython.t_table_colsel_set
    __swig_getmethods__["colsel"] = _ollypython.t_table_colsel_get
    if _newclass:colsel = property(_ollypython.t_table_colsel_get, _ollypython.t_table_colsel_set)
    __swig_setmethods__["mode"] = _ollypython.t_table_mode_set
    __swig_getmethods__["mode"] = _ollypython.t_table_mode_get
    if _newclass:mode = property(_ollypython.t_table_mode_get, _ollypython.t_table_mode_set)
    __swig_setmethods__["font"] = _ollypython.t_table_font_set
    __swig_getmethods__["font"] = _ollypython.t_table_font_get
    if _newclass:font = property(_ollypython.t_table_font_get, _ollypython.t_table_font_set)
    __swig_setmethods__["scheme"] = _ollypython.t_table_scheme_set
    __swig_getmethods__["scheme"] = _ollypython.t_table_scheme_get
    if _newclass:scheme = property(_ollypython.t_table_scheme_get, _ollypython.t_table_scheme_set)
    __swig_setmethods__["hilite"] = _ollypython.t_table_hilite_set
    __swig_getmethods__["hilite"] = _ollypython.t_table_hilite_get
    if _newclass:hilite = property(_ollypython.t_table_hilite_get, _ollypython.t_table_hilite_set)
    __swig_setmethods__["offset"] = _ollypython.t_table_offset_set
    __swig_getmethods__["offset"] = _ollypython.t_table_offset_get
    if _newclass:offset = property(_ollypython.t_table_offset_get, _ollypython.t_table_offset_set)
    __swig_setmethods__["xshift"] = _ollypython.t_table_xshift_set
    __swig_getmethods__["xshift"] = _ollypython.t_table_xshift_get
    if _newclass:xshift = property(_ollypython.t_table_xshift_get, _ollypython.t_table_xshift_set)
    __swig_setmethods__["drawfunc"] = _ollypython.t_table_drawfunc_set
    __swig_getmethods__["drawfunc"] = _ollypython.t_table_drawfunc_get
    if _newclass:drawfunc = property(_ollypython.t_table_drawfunc_get, _ollypython.t_table_drawfunc_set)
    def __init__(self, *args): 
        """__init__(self) -> t_table"""
        this = _ollypython.new_t_table(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_table
    __del__ = lambda self : None;
t_table_swigregister = _ollypython.t_table_swigregister
t_table_swigregister(t_table)

NM_NONAME = _ollypython.NM_NONAME
NM_ANYNAME = _ollypython.NM_ANYNAME
NM_PLUGCMD = _ollypython.NM_PLUGCMD
NM_LABEL = _ollypython.NM_LABEL
NM_EXPORT = _ollypython.NM_EXPORT
NM_IMPORT = _ollypython.NM_IMPORT
NM_LIBRARY = _ollypython.NM_LIBRARY
NM_CONST = _ollypython.NM_CONST
NM_COMMENT = _ollypython.NM_COMMENT
NM_LIBCOMM = _ollypython.NM_LIBCOMM
NM_BREAK = _ollypython.NM_BREAK
NM_ARG = _ollypython.NM_ARG
NM_ANALYSE = _ollypython.NM_ANALYSE
NM_BREAKEXPR = _ollypython.NM_BREAKEXPR
NM_BREAKEXPL = _ollypython.NM_BREAKEXPL
NM_ASSUME = _ollypython.NM_ASSUME
NM_STRUCT = _ollypython.NM_STRUCT
NM_CASE = _ollypython.NM_CASE
NM_INSPECT = _ollypython.NM_INSPECT
NM_WATCH = _ollypython.NM_WATCH
NM_ASM = _ollypython.NM_ASM
NM_FINDASM = _ollypython.NM_FINDASM
NM_LASTWATCH = _ollypython.NM_LASTWATCH
NM_SOURCE = _ollypython.NM_SOURCE
NM_REFTXT = _ollypython.NM_REFTXT
NM_GOTO = _ollypython.NM_GOTO
NM_GOTODUMP = _ollypython.NM_GOTODUMP
NM_TRPAUSE = _ollypython.NM_TRPAUSE
NM_IMCALL = _ollypython.NM_IMCALL
NMHISTORY = _ollypython.NMHISTORY
REG_EAX = _ollypython.REG_EAX
REG_ECX = _ollypython.REG_ECX
REG_EDX = _ollypython.REG_EDX
REG_EBX = _ollypython.REG_EBX
REG_ESP = _ollypython.REG_ESP
REG_EBP = _ollypython.REG_EBP
REG_ESI = _ollypython.REG_ESI
REG_EDI = _ollypython.REG_EDI
SEG_UNDEF = _ollypython.SEG_UNDEF
SEG_ES = _ollypython.SEG_ES
SEG_CS = _ollypython.SEG_CS
SEG_SS = _ollypython.SEG_SS
SEG_DS = _ollypython.SEG_DS
SEG_FS = _ollypython.SEG_FS
SEG_GS = _ollypython.SEG_GS
RS_NONE = _ollypython.RS_NONE
RS_INT = _ollypython.RS_INT
RS_EIP = _ollypython.RS_EIP
RS_FLG = _ollypython.RS_FLG
RS_SEG = _ollypython.RS_SEG
RS_EFL = _ollypython.RS_EFL
RS_TAG = _ollypython.RS_TAG
RS_FPU = _ollypython.RS_FPU
RS_FST = _ollypython.RS_FST
RS_FCO = _ollypython.RS_FCO
RS_FER = _ollypython.RS_FER
RS_FCW = _ollypython.RS_FCW
RS_FPR = _ollypython.RS_FPR
RS_FEM = _ollypython.RS_FEM
RS_MMX = _ollypython.RS_MMX
RS_3DN = _ollypython.RS_3DN
RS_SSE = _ollypython.RS_SSE
RS_CSR = _ollypython.RS_CSR
RS_CSB = _ollypython.RS_CSB
RS_CPR = _ollypython.RS_CPR
RS_ERR = _ollypython.RS_ERR
RS_GROUP = _ollypython.RS_GROUP
RS_INDEX = _ollypython.RS_INDEX
NREGSTACK = _ollypython.NREGSTACK
MAXCALSIZE = _ollypython.MAXCALSIZE
INT3 = _ollypython.INT3
NOP = _ollypython.NOP
TRAPFLAG = _ollypython.TRAPFLAG
C_TYPEMASK = _ollypython.C_TYPEMASK
C_CMD = _ollypython.C_CMD
C_PSH = _ollypython.C_PSH
C_POP = _ollypython.C_POP
C_MMX = _ollypython.C_MMX
C_FLT = _ollypython.C_FLT
C_JMP = _ollypython.C_JMP
C_JMC = _ollypython.C_JMC
C_CAL = _ollypython.C_CAL
C_RET = _ollypython.C_RET
C_FLG = _ollypython.C_FLG
C_RTF = _ollypython.C_RTF
C_REP = _ollypython.C_REP
C_PRI = _ollypython.C_PRI
C_SSE = _ollypython.C_SSE
C_NOW = _ollypython.C_NOW
C_BAD = _ollypython.C_BAD
C_RARE = _ollypython.C_RARE
C_SIZEMASK = _ollypython.C_SIZEMASK
C_EXPL = _ollypython.C_EXPL
C_DANGER95 = _ollypython.C_DANGER95
C_DANGER = _ollypython.C_DANGER
C_DANGERLOCK = _ollypython.C_DANGERLOCK
DEC_TYPEMASK = _ollypython.DEC_TYPEMASK
DEC_UNKNOWN = _ollypython.DEC_UNKNOWN
DEC_BYTE = _ollypython.DEC_BYTE
DEC_WORD = _ollypython.DEC_WORD
DEC_NEXTDATA = _ollypython.DEC_NEXTDATA
DEC_DWORD = _ollypython.DEC_DWORD
DEC_FLOAT4 = _ollypython.DEC_FLOAT4
DEC_FWORD = _ollypython.DEC_FWORD
DEC_FLOAT8 = _ollypython.DEC_FLOAT8
DEC_QWORD = _ollypython.DEC_QWORD
DEC_FLOAT10 = _ollypython.DEC_FLOAT10
DEC_TBYTE = _ollypython.DEC_TBYTE
DEC_STRING = _ollypython.DEC_STRING
DEC_UNICODE = _ollypython.DEC_UNICODE
DEC_3DNOW = _ollypython.DEC_3DNOW
DEC_SSE = _ollypython.DEC_SSE
DEC_TEXT = _ollypython.DEC_TEXT
DEC_BYTESW = _ollypython.DEC_BYTESW
DEC_NEXTCODE = _ollypython.DEC_NEXTCODE
DEC_COMMAND = _ollypython.DEC_COMMAND
DEC_JMPDEST = _ollypython.DEC_JMPDEST
DEC_CALLDEST = _ollypython.DEC_CALLDEST
DEC_PROCMASK = _ollypython.DEC_PROCMASK
DEC_PROC = _ollypython.DEC_PROC
DEC_PBODY = _ollypython.DEC_PBODY
DEC_PEND = _ollypython.DEC_PEND
DEC_CHECKED = _ollypython.DEC_CHECKED
DEC_SIGNED = _ollypython.DEC_SIGNED
DISASM_SIZE = _ollypython.DISASM_SIZE
DISASM_DATA = _ollypython.DISASM_DATA
DISASM_TRACE = _ollypython.DISASM_TRACE
DISASM_FILE = _ollypython.DISASM_FILE
DISASM_CODE = _ollypython.DISASM_CODE
DISASM_ALL = _ollypython.DISASM_ALL
DISASM_RTRACE = _ollypython.DISASM_RTRACE
DISASM_MODE = _ollypython.DISASM_MODE
DISASM_HILITE = _ollypython.DISASM_HILITE
DISASM_HLSHIFT = _ollypython.DISASM_HLSHIFT
DAW_FARADDR = _ollypython.DAW_FARADDR
DAW_SEGMENT = _ollypython.DAW_SEGMENT
DAW_PRIV = _ollypython.DAW_PRIV
DAW_IO = _ollypython.DAW_IO
DAW_SHIFT = _ollypython.DAW_SHIFT
DAW_PREFIX = _ollypython.DAW_PREFIX
DAW_LOCK = _ollypython.DAW_LOCK
DAW_STACK = _ollypython.DAW_STACK
DAW_DANGER95 = _ollypython.DAW_DANGER95
DAW_DANGEROUS = _ollypython.DAW_DANGEROUS
RST_INVALID = _ollypython.RST_INVALID
RST_VALUE = _ollypython.RST_VALUE
RST_VFIXUP = _ollypython.RST_VFIXUP
RST_INDIRECT = _ollypython.RST_INDIRECT
class t_reg(_object):
    """Proxy of C t_reg struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_reg, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_reg, name)
    __repr__ = _swig_repr
    __swig_setmethods__["modified"] = _ollypython.t_reg_modified_set
    __swig_getmethods__["modified"] = _ollypython.t_reg_modified_get
    if _newclass:modified = property(_ollypython.t_reg_modified_get, _ollypython.t_reg_modified_set)
    __swig_setmethods__["modifiedbyuser"] = _ollypython.t_reg_modifiedbyuser_set
    __swig_getmethods__["modifiedbyuser"] = _ollypython.t_reg_modifiedbyuser_get
    if _newclass:modifiedbyuser = property(_ollypython.t_reg_modifiedbyuser_get, _ollypython.t_reg_modifiedbyuser_set)
    __swig_setmethods__["singlestep"] = _ollypython.t_reg_singlestep_set
    __swig_getmethods__["singlestep"] = _ollypython.t_reg_singlestep_get
    if _newclass:singlestep = property(_ollypython.t_reg_singlestep_get, _ollypython.t_reg_singlestep_set)
    __swig_setmethods__["r"] = _ollypython.t_reg_r_set
    __swig_getmethods__["r"] = _ollypython.t_reg_r_get
    if _newclass:r = property(_ollypython.t_reg_r_get, _ollypython.t_reg_r_set)
    __swig_setmethods__["ip"] = _ollypython.t_reg_ip_set
    __swig_getmethods__["ip"] = _ollypython.t_reg_ip_get
    if _newclass:ip = property(_ollypython.t_reg_ip_get, _ollypython.t_reg_ip_set)
    __swig_setmethods__["flags"] = _ollypython.t_reg_flags_set
    __swig_getmethods__["flags"] = _ollypython.t_reg_flags_get
    if _newclass:flags = property(_ollypython.t_reg_flags_get, _ollypython.t_reg_flags_set)
    __swig_setmethods__["top"] = _ollypython.t_reg_top_set
    __swig_getmethods__["top"] = _ollypython.t_reg_top_get
    if _newclass:top = property(_ollypython.t_reg_top_get, _ollypython.t_reg_top_set)
    __swig_setmethods__["f"] = _ollypython.t_reg_f_set
    __swig_getmethods__["f"] = _ollypython.t_reg_f_get
    if _newclass:f = property(_ollypython.t_reg_f_get, _ollypython.t_reg_f_set)
    __swig_setmethods__["tag"] = _ollypython.t_reg_tag_set
    __swig_getmethods__["tag"] = _ollypython.t_reg_tag_get
    if _newclass:tag = property(_ollypython.t_reg_tag_get, _ollypython.t_reg_tag_set)
    __swig_setmethods__["fst"] = _ollypython.t_reg_fst_set
    __swig_getmethods__["fst"] = _ollypython.t_reg_fst_get
    if _newclass:fst = property(_ollypython.t_reg_fst_get, _ollypython.t_reg_fst_set)
    __swig_setmethods__["fcw"] = _ollypython.t_reg_fcw_set
    __swig_getmethods__["fcw"] = _ollypython.t_reg_fcw_get
    if _newclass:fcw = property(_ollypython.t_reg_fcw_get, _ollypython.t_reg_fcw_set)
    __swig_setmethods__["s"] = _ollypython.t_reg_s_set
    __swig_getmethods__["s"] = _ollypython.t_reg_s_get
    if _newclass:s = property(_ollypython.t_reg_s_get, _ollypython.t_reg_s_set)
    __swig_setmethods__["base"] = _ollypython.t_reg_base_set
    __swig_getmethods__["base"] = _ollypython.t_reg_base_get
    if _newclass:base = property(_ollypython.t_reg_base_get, _ollypython.t_reg_base_set)
    __swig_setmethods__["limit"] = _ollypython.t_reg_limit_set
    __swig_getmethods__["limit"] = _ollypython.t_reg_limit_get
    if _newclass:limit = property(_ollypython.t_reg_limit_get, _ollypython.t_reg_limit_set)
    __swig_setmethods__["big"] = _ollypython.t_reg_big_set
    __swig_getmethods__["big"] = _ollypython.t_reg_big_get
    if _newclass:big = property(_ollypython.t_reg_big_get, _ollypython.t_reg_big_set)
    __swig_setmethods__["dr6"] = _ollypython.t_reg_dr6_set
    __swig_getmethods__["dr6"] = _ollypython.t_reg_dr6_get
    if _newclass:dr6 = property(_ollypython.t_reg_dr6_get, _ollypython.t_reg_dr6_set)
    __swig_setmethods__["threadid"] = _ollypython.t_reg_threadid_set
    __swig_getmethods__["threadid"] = _ollypython.t_reg_threadid_get
    if _newclass:threadid = property(_ollypython.t_reg_threadid_get, _ollypython.t_reg_threadid_set)
    __swig_setmethods__["lasterror"] = _ollypython.t_reg_lasterror_set
    __swig_getmethods__["lasterror"] = _ollypython.t_reg_lasterror_get
    if _newclass:lasterror = property(_ollypython.t_reg_lasterror_get, _ollypython.t_reg_lasterror_set)
    __swig_setmethods__["ssevalid"] = _ollypython.t_reg_ssevalid_set
    __swig_getmethods__["ssevalid"] = _ollypython.t_reg_ssevalid_get
    if _newclass:ssevalid = property(_ollypython.t_reg_ssevalid_get, _ollypython.t_reg_ssevalid_set)
    __swig_setmethods__["ssemodified"] = _ollypython.t_reg_ssemodified_set
    __swig_getmethods__["ssemodified"] = _ollypython.t_reg_ssemodified_get
    if _newclass:ssemodified = property(_ollypython.t_reg_ssemodified_get, _ollypython.t_reg_ssemodified_set)
    __swig_setmethods__["ssereg"] = _ollypython.t_reg_ssereg_set
    __swig_getmethods__["ssereg"] = _ollypython.t_reg_ssereg_get
    if _newclass:ssereg = property(_ollypython.t_reg_ssereg_get, _ollypython.t_reg_ssereg_set)
    __swig_setmethods__["mxcsr"] = _ollypython.t_reg_mxcsr_set
    __swig_getmethods__["mxcsr"] = _ollypython.t_reg_mxcsr_get
    if _newclass:mxcsr = property(_ollypython.t_reg_mxcsr_get, _ollypython.t_reg_mxcsr_set)
    __swig_setmethods__["selected"] = _ollypython.t_reg_selected_set
    __swig_getmethods__["selected"] = _ollypython.t_reg_selected_get
    if _newclass:selected = property(_ollypython.t_reg_selected_get, _ollypython.t_reg_selected_set)
    __swig_setmethods__["drlin"] = _ollypython.t_reg_drlin_set
    __swig_getmethods__["drlin"] = _ollypython.t_reg_drlin_get
    if _newclass:drlin = property(_ollypython.t_reg_drlin_get, _ollypython.t_reg_drlin_set)
    __swig_setmethods__["dr7"] = _ollypython.t_reg_dr7_set
    __swig_getmethods__["dr7"] = _ollypython.t_reg_dr7_get
    if _newclass:dr7 = property(_ollypython.t_reg_dr7_get, _ollypython.t_reg_dr7_set)
    def __init__(self, *args): 
        """__init__(self) -> t_reg"""
        this = _ollypython.new_t_reg(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_reg
    __del__ = lambda self : None;
t_reg_swigregister = _ollypython.t_reg_swigregister
t_reg_swigregister(t_reg)

class t_operand(_object):
    """Proxy of C t_operand struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_operand, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_operand, name)
    __repr__ = _swig_repr
    __swig_setmethods__["optype"] = _ollypython.t_operand_optype_set
    __swig_getmethods__["optype"] = _ollypython.t_operand_optype_get
    if _newclass:optype = property(_ollypython.t_operand_optype_get, _ollypython.t_operand_optype_set)
    __swig_setmethods__["opsize"] = _ollypython.t_operand_opsize_set
    __swig_getmethods__["opsize"] = _ollypython.t_operand_opsize_get
    if _newclass:opsize = property(_ollypython.t_operand_opsize_get, _ollypython.t_operand_opsize_set)
    __swig_setmethods__["regscale"] = _ollypython.t_operand_regscale_set
    __swig_getmethods__["regscale"] = _ollypython.t_operand_regscale_get
    if _newclass:regscale = property(_ollypython.t_operand_regscale_get, _ollypython.t_operand_regscale_set)
    __swig_setmethods__["seg"] = _ollypython.t_operand_seg_set
    __swig_getmethods__["seg"] = _ollypython.t_operand_seg_get
    if _newclass:seg = property(_ollypython.t_operand_seg_get, _ollypython.t_operand_seg_set)
    __swig_setmethods__["opconst"] = _ollypython.t_operand_opconst_set
    __swig_getmethods__["opconst"] = _ollypython.t_operand_opconst_get
    if _newclass:opconst = property(_ollypython.t_operand_opconst_get, _ollypython.t_operand_opconst_set)
    def __init__(self, *args): 
        """__init__(self) -> t_operand"""
        this = _ollypython.new_t_operand(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_operand
    __del__ = lambda self : None;
t_operand_swigregister = _ollypython.t_operand_swigregister
t_operand_swigregister(t_operand)

class t_disasm(_object):
    """Proxy of C t_disasm struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_disasm, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_disasm, name)
    __repr__ = _swig_repr
    __swig_setmethods__["ip"] = _ollypython.t_disasm_ip_set
    __swig_getmethods__["ip"] = _ollypython.t_disasm_ip_get
    if _newclass:ip = property(_ollypython.t_disasm_ip_get, _ollypython.t_disasm_ip_set)
    __swig_setmethods__["dump"] = _ollypython.t_disasm_dump_set
    __swig_getmethods__["dump"] = _ollypython.t_disasm_dump_get
    if _newclass:dump = property(_ollypython.t_disasm_dump_get, _ollypython.t_disasm_dump_set)
    __swig_setmethods__["result"] = _ollypython.t_disasm_result_set
    __swig_getmethods__["result"] = _ollypython.t_disasm_result_get
    if _newclass:result = property(_ollypython.t_disasm_result_get, _ollypython.t_disasm_result_set)
    __swig_setmethods__["comment"] = _ollypython.t_disasm_comment_set
    __swig_getmethods__["comment"] = _ollypython.t_disasm_comment_get
    if _newclass:comment = property(_ollypython.t_disasm_comment_get, _ollypython.t_disasm_comment_set)
    __swig_setmethods__["opinfo"] = _ollypython.t_disasm_opinfo_set
    __swig_getmethods__["opinfo"] = _ollypython.t_disasm_opinfo_get
    if _newclass:opinfo = property(_ollypython.t_disasm_opinfo_get, _ollypython.t_disasm_opinfo_set)
    __swig_setmethods__["cmdtype"] = _ollypython.t_disasm_cmdtype_set
    __swig_getmethods__["cmdtype"] = _ollypython.t_disasm_cmdtype_get
    if _newclass:cmdtype = property(_ollypython.t_disasm_cmdtype_get, _ollypython.t_disasm_cmdtype_set)
    __swig_setmethods__["memtype"] = _ollypython.t_disasm_memtype_set
    __swig_getmethods__["memtype"] = _ollypython.t_disasm_memtype_get
    if _newclass:memtype = property(_ollypython.t_disasm_memtype_get, _ollypython.t_disasm_memtype_set)
    __swig_setmethods__["nprefix"] = _ollypython.t_disasm_nprefix_set
    __swig_getmethods__["nprefix"] = _ollypython.t_disasm_nprefix_get
    if _newclass:nprefix = property(_ollypython.t_disasm_nprefix_get, _ollypython.t_disasm_nprefix_set)
    __swig_setmethods__["indexed"] = _ollypython.t_disasm_indexed_set
    __swig_getmethods__["indexed"] = _ollypython.t_disasm_indexed_get
    if _newclass:indexed = property(_ollypython.t_disasm_indexed_get, _ollypython.t_disasm_indexed_set)
    __swig_setmethods__["jmpconst"] = _ollypython.t_disasm_jmpconst_set
    __swig_getmethods__["jmpconst"] = _ollypython.t_disasm_jmpconst_get
    if _newclass:jmpconst = property(_ollypython.t_disasm_jmpconst_get, _ollypython.t_disasm_jmpconst_set)
    __swig_setmethods__["jmptable"] = _ollypython.t_disasm_jmptable_set
    __swig_getmethods__["jmptable"] = _ollypython.t_disasm_jmptable_get
    if _newclass:jmptable = property(_ollypython.t_disasm_jmptable_get, _ollypython.t_disasm_jmptable_set)
    __swig_setmethods__["adrconst"] = _ollypython.t_disasm_adrconst_set
    __swig_getmethods__["adrconst"] = _ollypython.t_disasm_adrconst_get
    if _newclass:adrconst = property(_ollypython.t_disasm_adrconst_get, _ollypython.t_disasm_adrconst_set)
    __swig_setmethods__["immconst"] = _ollypython.t_disasm_immconst_set
    __swig_getmethods__["immconst"] = _ollypython.t_disasm_immconst_get
    if _newclass:immconst = property(_ollypython.t_disasm_immconst_get, _ollypython.t_disasm_immconst_set)
    __swig_setmethods__["zeroconst"] = _ollypython.t_disasm_zeroconst_set
    __swig_getmethods__["zeroconst"] = _ollypython.t_disasm_zeroconst_get
    if _newclass:zeroconst = property(_ollypython.t_disasm_zeroconst_get, _ollypython.t_disasm_zeroconst_set)
    __swig_setmethods__["fixupoffset"] = _ollypython.t_disasm_fixupoffset_set
    __swig_getmethods__["fixupoffset"] = _ollypython.t_disasm_fixupoffset_get
    if _newclass:fixupoffset = property(_ollypython.t_disasm_fixupoffset_get, _ollypython.t_disasm_fixupoffset_set)
    __swig_setmethods__["fixupsize"] = _ollypython.t_disasm_fixupsize_set
    __swig_getmethods__["fixupsize"] = _ollypython.t_disasm_fixupsize_get
    if _newclass:fixupsize = property(_ollypython.t_disasm_fixupsize_get, _ollypython.t_disasm_fixupsize_set)
    __swig_setmethods__["jmpaddr"] = _ollypython.t_disasm_jmpaddr_set
    __swig_getmethods__["jmpaddr"] = _ollypython.t_disasm_jmpaddr_get
    if _newclass:jmpaddr = property(_ollypython.t_disasm_jmpaddr_get, _ollypython.t_disasm_jmpaddr_set)
    __swig_setmethods__["condition"] = _ollypython.t_disasm_condition_set
    __swig_getmethods__["condition"] = _ollypython.t_disasm_condition_get
    if _newclass:condition = property(_ollypython.t_disasm_condition_get, _ollypython.t_disasm_condition_set)
    __swig_setmethods__["error"] = _ollypython.t_disasm_error_set
    __swig_getmethods__["error"] = _ollypython.t_disasm_error_get
    if _newclass:error = property(_ollypython.t_disasm_error_get, _ollypython.t_disasm_error_set)
    __swig_setmethods__["warnings"] = _ollypython.t_disasm_warnings_set
    __swig_getmethods__["warnings"] = _ollypython.t_disasm_warnings_get
    if _newclass:warnings = property(_ollypython.t_disasm_warnings_get, _ollypython.t_disasm_warnings_set)
    __swig_setmethods__["optype"] = _ollypython.t_disasm_optype_set
    __swig_getmethods__["optype"] = _ollypython.t_disasm_optype_get
    if _newclass:optype = property(_ollypython.t_disasm_optype_get, _ollypython.t_disasm_optype_set)
    __swig_setmethods__["opsize"] = _ollypython.t_disasm_opsize_set
    __swig_getmethods__["opsize"] = _ollypython.t_disasm_opsize_get
    if _newclass:opsize = property(_ollypython.t_disasm_opsize_get, _ollypython.t_disasm_opsize_set)
    __swig_setmethods__["opgood"] = _ollypython.t_disasm_opgood_set
    __swig_getmethods__["opgood"] = _ollypython.t_disasm_opgood_get
    if _newclass:opgood = property(_ollypython.t_disasm_opgood_get, _ollypython.t_disasm_opgood_set)
    __swig_setmethods__["opaddr"] = _ollypython.t_disasm_opaddr_set
    __swig_getmethods__["opaddr"] = _ollypython.t_disasm_opaddr_get
    if _newclass:opaddr = property(_ollypython.t_disasm_opaddr_get, _ollypython.t_disasm_opaddr_set)
    __swig_setmethods__["opdata"] = _ollypython.t_disasm_opdata_set
    __swig_getmethods__["opdata"] = _ollypython.t_disasm_opdata_get
    if _newclass:opdata = property(_ollypython.t_disasm_opdata_get, _ollypython.t_disasm_opdata_set)
    __swig_setmethods__["op"] = _ollypython.t_disasm_op_set
    __swig_getmethods__["op"] = _ollypython.t_disasm_op_get
    if _newclass:op = property(_ollypython.t_disasm_op_get, _ollypython.t_disasm_op_set)
    __swig_setmethods__["regdata"] = _ollypython.t_disasm_regdata_set
    __swig_getmethods__["regdata"] = _ollypython.t_disasm_regdata_get
    if _newclass:regdata = property(_ollypython.t_disasm_regdata_get, _ollypython.t_disasm_regdata_set)
    __swig_setmethods__["regstatus"] = _ollypython.t_disasm_regstatus_set
    __swig_getmethods__["regstatus"] = _ollypython.t_disasm_regstatus_get
    if _newclass:regstatus = property(_ollypython.t_disasm_regstatus_get, _ollypython.t_disasm_regstatus_set)
    __swig_setmethods__["addrdata"] = _ollypython.t_disasm_addrdata_set
    __swig_getmethods__["addrdata"] = _ollypython.t_disasm_addrdata_get
    if _newclass:addrdata = property(_ollypython.t_disasm_addrdata_get, _ollypython.t_disasm_addrdata_set)
    __swig_setmethods__["addrstatus"] = _ollypython.t_disasm_addrstatus_set
    __swig_getmethods__["addrstatus"] = _ollypython.t_disasm_addrstatus_get
    if _newclass:addrstatus = property(_ollypython.t_disasm_addrstatus_get, _ollypython.t_disasm_addrstatus_set)
    __swig_setmethods__["regstack"] = _ollypython.t_disasm_regstack_set
    __swig_getmethods__["regstack"] = _ollypython.t_disasm_regstack_get
    if _newclass:regstack = property(_ollypython.t_disasm_regstack_get, _ollypython.t_disasm_regstack_set)
    __swig_setmethods__["rststatus"] = _ollypython.t_disasm_rststatus_set
    __swig_getmethods__["rststatus"] = _ollypython.t_disasm_rststatus_get
    if _newclass:rststatus = property(_ollypython.t_disasm_rststatus_get, _ollypython.t_disasm_rststatus_set)
    __swig_setmethods__["nregstack"] = _ollypython.t_disasm_nregstack_set
    __swig_getmethods__["nregstack"] = _ollypython.t_disasm_nregstack_get
    if _newclass:nregstack = property(_ollypython.t_disasm_nregstack_get, _ollypython.t_disasm_nregstack_set)
    __swig_setmethods__["reserved"] = _ollypython.t_disasm_reserved_set
    __swig_getmethods__["reserved"] = _ollypython.t_disasm_reserved_get
    if _newclass:reserved = property(_ollypython.t_disasm_reserved_get, _ollypython.t_disasm_reserved_set)
    def __init__(self, *args): 
        """__init__(self) -> t_disasm"""
        this = _ollypython.new_t_disasm(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_disasm
    __del__ = lambda self : None;
t_disasm_swigregister = _ollypython.t_disasm_swigregister
t_disasm_swigregister(t_disasm)

class t_result_union1(_object):
    """Proxy of C t_result_union1 struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_result_union1, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_result_union1, name)
    __repr__ = _swig_repr
    __swig_setmethods__["data"] = _ollypython.t_result_union1_data_set
    __swig_getmethods__["data"] = _ollypython.t_result_union1_data_get
    if _newclass:data = property(_ollypython.t_result_union1_data_get, _ollypython.t_result_union1_data_set)
    __swig_setmethods__["u"] = _ollypython.t_result_union1_u_set
    __swig_getmethods__["u"] = _ollypython.t_result_union1_u_get
    if _newclass:u = property(_ollypython.t_result_union1_u_get, _ollypython.t_result_union1_u_set)
    __swig_setmethods__["l"] = _ollypython.t_result_union1_l_set
    __swig_getmethods__["l"] = _ollypython.t_result_union1_l_get
    if _newclass:l = property(_ollypython.t_result_union1_l_get, _ollypython.t_result_union1_l_set)
    __swig_setmethods__["f"] = _ollypython.t_result_union1_f_set
    __swig_getmethods__["f"] = _ollypython.t_result_union1_f_get
    if _newclass:f = property(_ollypython.t_result_union1_f_get, _ollypython.t_result_union1_f_set)
    def __init__(self, *args): 
        """__init__(self) -> t_result_union1"""
        this = _ollypython.new_t_result_union1(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_result_union1
    __del__ = lambda self : None;
t_result_union1_swigregister = _ollypython.t_result_union1_swigregister
t_result_union1_swigregister(t_result_union1)

class t_result_union2(_object):
    """Proxy of C t_result_union2 struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_result_union2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_result_union2, name)
    __repr__ = _swig_repr
    __swig_setmethods__["value"] = _ollypython.t_result_union2_value_set
    __swig_getmethods__["value"] = _ollypython.t_result_union2_value_get
    if _newclass:value = property(_ollypython.t_result_union2_value_get, _ollypython.t_result_union2_value_set)
    __swig_setmethods__["wvalue"] = _ollypython.t_result_union2_wvalue_set
    __swig_getmethods__["wvalue"] = _ollypython.t_result_union2_wvalue_get
    if _newclass:wvalue = property(_ollypython.t_result_union2_wvalue_get, _ollypython.t_result_union2_wvalue_set)
    def __init__(self, *args): 
        """__init__(self) -> t_result_union2"""
        this = _ollypython.new_t_result_union2(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_result_union2
    __del__ = lambda self : None;
t_result_union2_swigregister = _ollypython.t_result_union2_swigregister
t_result_union2_swigregister(t_result_union2)

class t_result(_object):
    """Proxy of C t_result struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_result, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_result, name)
    __repr__ = _swig_repr
    __swig_setmethods__["type"] = _ollypython.t_result_type_set
    __swig_getmethods__["type"] = _ollypython.t_result_type_get
    if _newclass:type = property(_ollypython.t_result_type_get, _ollypython.t_result_type_set)
    __swig_setmethods__["dtype"] = _ollypython.t_result_dtype_set
    __swig_getmethods__["dtype"] = _ollypython.t_result_dtype_get
    if _newclass:dtype = property(_ollypython.t_result_dtype_get, _ollypython.t_result_dtype_set)
    __swig_setmethods__["data"] = _ollypython.t_result_data_set
    __swig_getmethods__["data"] = _ollypython.t_result_data_get
    if _newclass:data = property(_ollypython.t_result_data_get, _ollypython.t_result_data_set)
    __swig_setmethods__["value"] = _ollypython.t_result_value_set
    __swig_getmethods__["value"] = _ollypython.t_result_value_get
    if _newclass:value = property(_ollypython.t_result_value_get, _ollypython.t_result_value_set)
    __swig_setmethods__["lvaddr"] = _ollypython.t_result_lvaddr_set
    __swig_getmethods__["lvaddr"] = _ollypython.t_result_lvaddr_get
    if _newclass:lvaddr = property(_ollypython.t_result_lvaddr_get, _ollypython.t_result_lvaddr_set)
    def __init__(self, *args): 
        """__init__(self) -> t_result"""
        this = _ollypython.new_t_result(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_result
    __del__ = lambda self : None;
t_result_swigregister = _ollypython.t_result_swigregister
t_result_swigregister(t_result)

class t_thread(_object):
    """Proxy of C t_thread struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_thread, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_thread, name)
    __repr__ = _swig_repr
    __swig_setmethods__["threadid"] = _ollypython.t_thread_threadid_set
    __swig_getmethods__["threadid"] = _ollypython.t_thread_threadid_get
    if _newclass:threadid = property(_ollypython.t_thread_threadid_get, _ollypython.t_thread_threadid_set)
    __swig_setmethods__["dummy"] = _ollypython.t_thread_dummy_set
    __swig_getmethods__["dummy"] = _ollypython.t_thread_dummy_get
    if _newclass:dummy = property(_ollypython.t_thread_dummy_get, _ollypython.t_thread_dummy_set)
    __swig_setmethods__["type"] = _ollypython.t_thread_type_set
    __swig_getmethods__["type"] = _ollypython.t_thread_type_get
    if _newclass:type = property(_ollypython.t_thread_type_get, _ollypython.t_thread_type_set)
    __swig_setmethods__["thread"] = _ollypython.t_thread_thread_set
    __swig_getmethods__["thread"] = _ollypython.t_thread_thread_get
    if _newclass:thread = property(_ollypython.t_thread_thread_get, _ollypython.t_thread_thread_set)
    __swig_setmethods__["datablock"] = _ollypython.t_thread_datablock_set
    __swig_getmethods__["datablock"] = _ollypython.t_thread_datablock_get
    if _newclass:datablock = property(_ollypython.t_thread_datablock_get, _ollypython.t_thread_datablock_set)
    __swig_setmethods__["entry"] = _ollypython.t_thread_entry_set
    __swig_getmethods__["entry"] = _ollypython.t_thread_entry_get
    if _newclass:entry = property(_ollypython.t_thread_entry_get, _ollypython.t_thread_entry_set)
    __swig_setmethods__["stacktop"] = _ollypython.t_thread_stacktop_set
    __swig_getmethods__["stacktop"] = _ollypython.t_thread_stacktop_get
    if _newclass:stacktop = property(_ollypython.t_thread_stacktop_get, _ollypython.t_thread_stacktop_set)
    __swig_setmethods__["stackbottom"] = _ollypython.t_thread_stackbottom_set
    __swig_getmethods__["stackbottom"] = _ollypython.t_thread_stackbottom_get
    if _newclass:stackbottom = property(_ollypython.t_thread_stackbottom_get, _ollypython.t_thread_stackbottom_set)
    __swig_setmethods__["context"] = _ollypython.t_thread_context_set
    __swig_getmethods__["context"] = _ollypython.t_thread_context_get
    if _newclass:context = property(_ollypython.t_thread_context_get, _ollypython.t_thread_context_set)
    __swig_setmethods__["reg"] = _ollypython.t_thread_reg_set
    __swig_getmethods__["reg"] = _ollypython.t_thread_reg_get
    if _newclass:reg = property(_ollypython.t_thread_reg_get, _ollypython.t_thread_reg_set)
    __swig_setmethods__["regvalid"] = _ollypython.t_thread_regvalid_set
    __swig_getmethods__["regvalid"] = _ollypython.t_thread_regvalid_get
    if _newclass:regvalid = property(_ollypython.t_thread_regvalid_get, _ollypython.t_thread_regvalid_set)
    __swig_setmethods__["oldreg"] = _ollypython.t_thread_oldreg_set
    __swig_getmethods__["oldreg"] = _ollypython.t_thread_oldreg_get
    if _newclass:oldreg = property(_ollypython.t_thread_oldreg_get, _ollypython.t_thread_oldreg_set)
    __swig_setmethods__["oldregvalid"] = _ollypython.t_thread_oldregvalid_set
    __swig_getmethods__["oldregvalid"] = _ollypython.t_thread_oldregvalid_get
    if _newclass:oldregvalid = property(_ollypython.t_thread_oldregvalid_get, _ollypython.t_thread_oldregvalid_set)
    __swig_setmethods__["suspendcount"] = _ollypython.t_thread_suspendcount_set
    __swig_getmethods__["suspendcount"] = _ollypython.t_thread_suspendcount_get
    if _newclass:suspendcount = property(_ollypython.t_thread_suspendcount_get, _ollypython.t_thread_suspendcount_set)
    __swig_setmethods__["usertime"] = _ollypython.t_thread_usertime_set
    __swig_getmethods__["usertime"] = _ollypython.t_thread_usertime_get
    if _newclass:usertime = property(_ollypython.t_thread_usertime_get, _ollypython.t_thread_usertime_set)
    __swig_setmethods__["systime"] = _ollypython.t_thread_systime_set
    __swig_getmethods__["systime"] = _ollypython.t_thread_systime_get
    if _newclass:systime = property(_ollypython.t_thread_systime_get, _ollypython.t_thread_systime_set)
    __swig_setmethods__["reserved"] = _ollypython.t_thread_reserved_set
    __swig_getmethods__["reserved"] = _ollypython.t_thread_reserved_get
    if _newclass:reserved = property(_ollypython.t_thread_reserved_get, _ollypython.t_thread_reserved_set)
    def __init__(self, *args): 
        """__init__(self) -> t_thread"""
        this = _ollypython.new_t_thread(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_thread
    __del__ = lambda self : None;
t_thread_swigregister = _ollypython.t_thread_swigregister
t_thread_swigregister(t_thread)

MM_RESTORE = _ollypython.MM_RESTORE
MM_SILENT = _ollypython.MM_SILENT
MM_DELANAL = _ollypython.MM_DELANAL
MM_RESILENT = _ollypython.MM_RESILENT
class t_memory(_object):
    """Proxy of C t_memory struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_memory, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_memory, name)
    __repr__ = _swig_repr
    __swig_setmethods__["base"] = _ollypython.t_memory_base_set
    __swig_getmethods__["base"] = _ollypython.t_memory_base_get
    if _newclass:base = property(_ollypython.t_memory_base_get, _ollypython.t_memory_base_set)
    __swig_setmethods__["size"] = _ollypython.t_memory_size_set
    __swig_getmethods__["size"] = _ollypython.t_memory_size_get
    if _newclass:size = property(_ollypython.t_memory_size_get, _ollypython.t_memory_size_set)
    __swig_setmethods__["type"] = _ollypython.t_memory_type_set
    __swig_getmethods__["type"] = _ollypython.t_memory_type_get
    if _newclass:type = property(_ollypython.t_memory_type_get, _ollypython.t_memory_type_set)
    __swig_setmethods__["owner"] = _ollypython.t_memory_owner_set
    __swig_getmethods__["owner"] = _ollypython.t_memory_owner_get
    if _newclass:owner = property(_ollypython.t_memory_owner_get, _ollypython.t_memory_owner_set)
    __swig_setmethods__["initaccess"] = _ollypython.t_memory_initaccess_set
    __swig_getmethods__["initaccess"] = _ollypython.t_memory_initaccess_get
    if _newclass:initaccess = property(_ollypython.t_memory_initaccess_get, _ollypython.t_memory_initaccess_set)
    __swig_setmethods__["access"] = _ollypython.t_memory_access_set
    __swig_getmethods__["access"] = _ollypython.t_memory_access_get
    if _newclass:access = property(_ollypython.t_memory_access_get, _ollypython.t_memory_access_set)
    __swig_setmethods__["threadid"] = _ollypython.t_memory_threadid_set
    __swig_getmethods__["threadid"] = _ollypython.t_memory_threadid_get
    if _newclass:threadid = property(_ollypython.t_memory_threadid_get, _ollypython.t_memory_threadid_set)
    __swig_setmethods__["sect"] = _ollypython.t_memory_sect_set
    __swig_getmethods__["sect"] = _ollypython.t_memory_sect_get
    if _newclass:sect = property(_ollypython.t_memory_sect_get, _ollypython.t_memory_sect_set)
    __swig_setmethods__["copy"] = _ollypython.t_memory_copy_set
    __swig_getmethods__["copy"] = _ollypython.t_memory_copy_get
    if _newclass:copy = property(_ollypython.t_memory_copy_get, _ollypython.t_memory_copy_set)
    __swig_setmethods__["reserved"] = _ollypython.t_memory_reserved_set
    __swig_getmethods__["reserved"] = _ollypython.t_memory_reserved_get
    if _newclass:reserved = property(_ollypython.t_memory_reserved_get, _ollypython.t_memory_reserved_set)
    def __init__(self, *args): 
        """__init__(self) -> t_memory"""
        this = _ollypython.new_t_memory(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_memory
    __del__ = lambda self : None;
t_memory_swigregister = _ollypython.t_memory_swigregister
t_memory_swigregister(t_memory)

class t_heap(_object):
    """Proxy of C t_heap struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_heap, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_heap, name)
    __repr__ = _swig_repr
    __swig_setmethods__["base"] = _ollypython.t_heap_base_set
    __swig_getmethods__["base"] = _ollypython.t_heap_base_get
    if _newclass:base = property(_ollypython.t_heap_base_get, _ollypython.t_heap_base_set)
    __swig_setmethods__["size"] = _ollypython.t_heap_size_set
    __swig_getmethods__["size"] = _ollypython.t_heap_size_get
    if _newclass:size = property(_ollypython.t_heap_size_get, _ollypython.t_heap_size_set)
    __swig_setmethods__["type"] = _ollypython.t_heap_type_set
    __swig_getmethods__["type"] = _ollypython.t_heap_type_get
    if _newclass:type = property(_ollypython.t_heap_type_get, _ollypython.t_heap_type_set)
    __swig_setmethods__["parent"] = _ollypython.t_heap_parent_set
    __swig_getmethods__["parent"] = _ollypython.t_heap_parent_get
    if _newclass:parent = property(_ollypython.t_heap_parent_get, _ollypython.t_heap_parent_set)
    def __init__(self, *args): 
        """__init__(self) -> t_heap"""
        this = _ollypython.new_t_heap(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_heap
    __del__ = lambda self : None;
t_heap_swigregister = _ollypython.t_heap_swigregister
t_heap_swigregister(t_heap)

NVERS = _ollypython.NVERS
JT_JUMP = _ollypython.JT_JUMP
JT_COND = _ollypython.JT_COND
JT_SWITCH = _ollypython.JT_SWITCH
class t_ahint(_object):
    """Proxy of C t_ahint struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_ahint, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_ahint, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_ahint_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_ahint_addr_get
    if _newclass:addr = property(_ollypython.t_ahint_addr_get, _ollypython.t_ahint_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_ahint_size_set
    __swig_getmethods__["size"] = _ollypython.t_ahint_size_get
    if _newclass:size = property(_ollypython.t_ahint_size_get, _ollypython.t_ahint_size_set)
    __swig_setmethods__["type"] = _ollypython.t_ahint_type_set
    __swig_getmethods__["type"] = _ollypython.t_ahint_type_get
    if _newclass:type = property(_ollypython.t_ahint_type_get, _ollypython.t_ahint_type_set)
    def __init__(self, *args): 
        """__init__(self) -> t_ahint"""
        this = _ollypython.new_t_ahint(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_ahint
    __del__ = lambda self : None;
t_ahint_swigregister = _ollypython.t_ahint_swigregister
t_ahint_swigregister(t_ahint)

class t_stringtable(_object):
    """Proxy of C t_stringtable struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_stringtable, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_stringtable, name)
    __repr__ = _swig_repr
    __swig_setmethods__["name"] = _ollypython.t_stringtable_name_set
    __swig_getmethods__["name"] = _ollypython.t_stringtable_name_get
    if _newclass:name = property(_ollypython.t_stringtable_name_get, _ollypython.t_stringtable_name_set)
    __swig_setmethods__["language"] = _ollypython.t_stringtable_language_set
    __swig_getmethods__["language"] = _ollypython.t_stringtable_language_get
    if _newclass:language = property(_ollypython.t_stringtable_language_get, _ollypython.t_stringtable_language_set)
    __swig_setmethods__["addr"] = _ollypython.t_stringtable_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_stringtable_addr_get
    if _newclass:addr = property(_ollypython.t_stringtable_addr_get, _ollypython.t_stringtable_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_stringtable_size_set
    __swig_getmethods__["size"] = _ollypython.t_stringtable_size_get
    if _newclass:size = property(_ollypython.t_stringtable_size_get, _ollypython.t_stringtable_size_set)
    def __init__(self, *args): 
        """__init__(self) -> t_stringtable"""
        this = _ollypython.new_t_stringtable(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_stringtable
    __del__ = lambda self : None;
t_stringtable_swigregister = _ollypython.t_stringtable_swigregister
t_stringtable_swigregister(t_stringtable)

class t_fixup(_object):
    """Proxy of C t_fixup struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_fixup, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_fixup, name)
    __repr__ = _swig_repr
    __swig_setmethods__["base"] = _ollypython.t_fixup_base_set
    __swig_getmethods__["base"] = _ollypython.t_fixup_base_get
    if _newclass:base = property(_ollypython.t_fixup_base_get, _ollypython.t_fixup_base_set)
    __swig_setmethods__["size"] = _ollypython.t_fixup_size_set
    __swig_getmethods__["size"] = _ollypython.t_fixup_size_get
    if _newclass:size = property(_ollypython.t_fixup_size_get, _ollypython.t_fixup_size_set)
    def __init__(self, *args): 
        """__init__(self) -> t_fixup"""
        this = _ollypython.new_t_fixup(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_fixup
    __del__ = lambda self : None;
t_fixup_swigregister = _ollypython.t_fixup_swigregister
t_fixup_swigregister(t_fixup)

class t_symvar_union1(_object):
    """Proxy of C t_symvar_union1 struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_symvar_union1, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_symvar_union1, name)
    __repr__ = _swig_repr
    __swig_setmethods__["type"] = _ollypython.t_symvar_union1_type_set
    __swig_getmethods__["type"] = _ollypython.t_symvar_union1_type_get
    if _newclass:type = property(_ollypython.t_symvar_union1_type_get, _ollypython.t_symvar_union1_type_set)
    __swig_setmethods__["regs"] = _ollypython.t_symvar_union1_regs_set
    __swig_getmethods__["regs"] = _ollypython.t_symvar_union1_regs_get
    if _newclass:regs = property(_ollypython.t_symvar_union1_regs_get, _ollypython.t_symvar_union1_regs_set)
    def __init__(self, *args): 
        """__init__(self) -> t_symvar_union1"""
        this = _ollypython.new_t_symvar_union1(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_symvar_union1
    __del__ = lambda self : None;
t_symvar_union1_swigregister = _ollypython.t_symvar_union1_swigregister
t_symvar_union1_swigregister(t_symvar_union1)

class t_symvar_union2(_object):
    """Proxy of C t_symvar_union2 struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_symvar_union2, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_symvar_union2, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_symvar_union2_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_symvar_union2_addr_get
    if _newclass:addr = property(_ollypython.t_symvar_union2_addr_get, _ollypython.t_symvar_union2_addr_set)
    __swig_setmethods__["offset"] = _ollypython.t_symvar_union2_offset_set
    __swig_getmethods__["offset"] = _ollypython.t_symvar_union2_offset_get
    if _newclass:offset = property(_ollypython.t_symvar_union2_offset_get, _ollypython.t_symvar_union2_offset_set)
    def __init__(self, *args): 
        """__init__(self) -> t_symvar_union2"""
        this = _ollypython.new_t_symvar_union2(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_symvar_union2
    __del__ = lambda self : None;
t_symvar_union2_swigregister = _ollypython.t_symvar_union2_swigregister
t_symvar_union2_swigregister(t_symvar_union2)

class t_symvar(_object):
    """Proxy of C t_symvar struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_symvar, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_symvar, name)
    __repr__ = _swig_repr
    __swig_setmethods__["next"] = _ollypython.t_symvar_next_set
    __swig_getmethods__["next"] = _ollypython.t_symvar_next_get
    if _newclass:next = property(_ollypython.t_symvar_next_get, _ollypython.t_symvar_next_set)
    __swig_setmethods__["kind"] = _ollypython.t_symvar_kind_set
    __swig_getmethods__["kind"] = _ollypython.t_symvar_kind_get
    if _newclass:kind = property(_ollypython.t_symvar_kind_get, _ollypython.t_symvar_kind_set)
    __swig_setmethods__["type"] = _ollypython.t_symvar_type_set
    __swig_getmethods__["type"] = _ollypython.t_symvar_type_get
    if _newclass:type = property(_ollypython.t_symvar_type_get, _ollypython.t_symvar_type_set)
    __swig_setmethods__["addr"] = _ollypython.t_symvar_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_symvar_addr_get
    if _newclass:addr = property(_ollypython.t_symvar_addr_get, _ollypython.t_symvar_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_symvar_size_set
    __swig_getmethods__["size"] = _ollypython.t_symvar_size_get
    if _newclass:size = property(_ollypython.t_symvar_size_get, _ollypython.t_symvar_size_set)
    __swig_setmethods__["optvar"] = _ollypython.t_symvar_optvar_set
    __swig_getmethods__["optvar"] = _ollypython.t_symvar_optvar_get
    if _newclass:optvar = property(_ollypython.t_symvar_optvar_get, _ollypython.t_symvar_optvar_set)
    __swig_setmethods__["nameaddr"] = _ollypython.t_symvar_nameaddr_set
    __swig_getmethods__["nameaddr"] = _ollypython.t_symvar_nameaddr_get
    if _newclass:nameaddr = property(_ollypython.t_symvar_nameaddr_get, _ollypython.t_symvar_nameaddr_set)
    def __init__(self, *args): 
        """__init__(self) -> t_symvar"""
        this = _ollypython.new_t_symvar(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_symvar
    __del__ = lambda self : None;
t_symvar_swigregister = _ollypython.t_symvar_swigregister
t_symvar_swigregister(t_symvar)

class t_jdest(_object):
    """Proxy of C t_jdest struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_jdest, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_jdest, name)
    __repr__ = _swig_repr
    __swig_setmethods__["type"] = _ollypython.t_jdest_type_set
    __swig_getmethods__["type"] = _ollypython.t_jdest_type_get
    if _newclass:type = property(_ollypython.t_jdest_type_get, _ollypython.t_jdest_type_set)
    __swig_setmethods__["_from"] = _ollypython.t_jdest__from_set
    __swig_getmethods__["_from"] = _ollypython.t_jdest__from_get
    if _newclass:_from = property(_ollypython.t_jdest__from_get, _ollypython.t_jdest__from_set)
    __swig_setmethods__["to"] = _ollypython.t_jdest_to_set
    __swig_getmethods__["to"] = _ollypython.t_jdest_to_get
    if _newclass:to = property(_ollypython.t_jdest_to_get, _ollypython.t_jdest_to_set)
    def __init__(self, *args): 
        """__init__(self) -> t_jdest"""
        this = _ollypython.new_t_jdest(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_jdest
    __del__ = lambda self : None;
t_jdest_swigregister = _ollypython.t_jdest_swigregister
t_jdest_swigregister(t_jdest)

class t_module(_object):
    """Proxy of C t_module struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_module, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_module, name)
    __repr__ = _swig_repr
    __swig_setmethods__["base"] = _ollypython.t_module_base_set
    __swig_getmethods__["base"] = _ollypython.t_module_base_get
    if _newclass:base = property(_ollypython.t_module_base_get, _ollypython.t_module_base_set)
    __swig_setmethods__["size"] = _ollypython.t_module_size_set
    __swig_getmethods__["size"] = _ollypython.t_module_size_get
    if _newclass:size = property(_ollypython.t_module_size_get, _ollypython.t_module_size_set)
    __swig_setmethods__["type"] = _ollypython.t_module_type_set
    __swig_getmethods__["type"] = _ollypython.t_module_type_get
    if _newclass:type = property(_ollypython.t_module_type_get, _ollypython.t_module_type_set)
    __swig_setmethods__["codebase"] = _ollypython.t_module_codebase_set
    __swig_getmethods__["codebase"] = _ollypython.t_module_codebase_get
    if _newclass:codebase = property(_ollypython.t_module_codebase_get, _ollypython.t_module_codebase_set)
    __swig_setmethods__["codesize"] = _ollypython.t_module_codesize_set
    __swig_getmethods__["codesize"] = _ollypython.t_module_codesize_get
    if _newclass:codesize = property(_ollypython.t_module_codesize_get, _ollypython.t_module_codesize_set)
    __swig_setmethods__["resbase"] = _ollypython.t_module_resbase_set
    __swig_getmethods__["resbase"] = _ollypython.t_module_resbase_get
    if _newclass:resbase = property(_ollypython.t_module_resbase_get, _ollypython.t_module_resbase_set)
    __swig_setmethods__["ressize"] = _ollypython.t_module_ressize_set
    __swig_getmethods__["ressize"] = _ollypython.t_module_ressize_get
    if _newclass:ressize = property(_ollypython.t_module_ressize_get, _ollypython.t_module_ressize_set)
    __swig_setmethods__["stringtable"] = _ollypython.t_module_stringtable_set
    __swig_getmethods__["stringtable"] = _ollypython.t_module_stringtable_get
    if _newclass:stringtable = property(_ollypython.t_module_stringtable_get, _ollypython.t_module_stringtable_set)
    __swig_setmethods__["nstringtable"] = _ollypython.t_module_nstringtable_set
    __swig_getmethods__["nstringtable"] = _ollypython.t_module_nstringtable_get
    if _newclass:nstringtable = property(_ollypython.t_module_nstringtable_get, _ollypython.t_module_nstringtable_set)
    __swig_setmethods__["maxstringtable"] = _ollypython.t_module_maxstringtable_set
    __swig_getmethods__["maxstringtable"] = _ollypython.t_module_maxstringtable_get
    if _newclass:maxstringtable = property(_ollypython.t_module_maxstringtable_get, _ollypython.t_module_maxstringtable_set)
    __swig_setmethods__["entry"] = _ollypython.t_module_entry_set
    __swig_getmethods__["entry"] = _ollypython.t_module_entry_get
    if _newclass:entry = property(_ollypython.t_module_entry_get, _ollypython.t_module_entry_set)
    __swig_setmethods__["database"] = _ollypython.t_module_database_set
    __swig_getmethods__["database"] = _ollypython.t_module_database_get
    if _newclass:database = property(_ollypython.t_module_database_get, _ollypython.t_module_database_set)
    __swig_setmethods__["idatatable"] = _ollypython.t_module_idatatable_set
    __swig_getmethods__["idatatable"] = _ollypython.t_module_idatatable_get
    if _newclass:idatatable = property(_ollypython.t_module_idatatable_get, _ollypython.t_module_idatatable_set)
    __swig_setmethods__["idatabase"] = _ollypython.t_module_idatabase_set
    __swig_getmethods__["idatabase"] = _ollypython.t_module_idatabase_get
    if _newclass:idatabase = property(_ollypython.t_module_idatabase_get, _ollypython.t_module_idatabase_set)
    __swig_setmethods__["edatatable"] = _ollypython.t_module_edatatable_set
    __swig_getmethods__["edatatable"] = _ollypython.t_module_edatatable_get
    if _newclass:edatatable = property(_ollypython.t_module_edatatable_get, _ollypython.t_module_edatatable_set)
    __swig_setmethods__["edatasize"] = _ollypython.t_module_edatasize_set
    __swig_getmethods__["edatasize"] = _ollypython.t_module_edatasize_get
    if _newclass:edatasize = property(_ollypython.t_module_edatasize_get, _ollypython.t_module_edatasize_set)
    __swig_setmethods__["reloctable"] = _ollypython.t_module_reloctable_set
    __swig_getmethods__["reloctable"] = _ollypython.t_module_reloctable_get
    if _newclass:reloctable = property(_ollypython.t_module_reloctable_get, _ollypython.t_module_reloctable_set)
    __swig_setmethods__["relocsize"] = _ollypython.t_module_relocsize_set
    __swig_getmethods__["relocsize"] = _ollypython.t_module_relocsize_get
    if _newclass:relocsize = property(_ollypython.t_module_relocsize_get, _ollypython.t_module_relocsize_set)
    __swig_setmethods__["name"] = _ollypython.t_module_name_set
    __swig_getmethods__["name"] = _ollypython.t_module_name_get
    if _newclass:name = property(_ollypython.t_module_name_get, _ollypython.t_module_name_set)
    __swig_setmethods__["path"] = _ollypython.t_module_path_set
    __swig_getmethods__["path"] = _ollypython.t_module_path_get
    if _newclass:path = property(_ollypython.t_module_path_get, _ollypython.t_module_path_set)
    __swig_setmethods__["nsect"] = _ollypython.t_module_nsect_set
    __swig_getmethods__["nsect"] = _ollypython.t_module_nsect_get
    if _newclass:nsect = property(_ollypython.t_module_nsect_get, _ollypython.t_module_nsect_set)
    __swig_setmethods__["sect"] = _ollypython.t_module_sect_set
    __swig_getmethods__["sect"] = _ollypython.t_module_sect_get
    if _newclass:sect = property(_ollypython.t_module_sect_get, _ollypython.t_module_sect_set)
    __swig_setmethods__["headersize"] = _ollypython.t_module_headersize_set
    __swig_getmethods__["headersize"] = _ollypython.t_module_headersize_get
    if _newclass:headersize = property(_ollypython.t_module_headersize_get, _ollypython.t_module_headersize_set)
    __swig_setmethods__["fixupbase"] = _ollypython.t_module_fixupbase_set
    __swig_getmethods__["fixupbase"] = _ollypython.t_module_fixupbase_get
    if _newclass:fixupbase = property(_ollypython.t_module_fixupbase_get, _ollypython.t_module_fixupbase_set)
    __swig_setmethods__["nfixup"] = _ollypython.t_module_nfixup_set
    __swig_getmethods__["nfixup"] = _ollypython.t_module_nfixup_get
    if _newclass:nfixup = property(_ollypython.t_module_nfixup_get, _ollypython.t_module_nfixup_set)
    __swig_setmethods__["fixup"] = _ollypython.t_module_fixup_set
    __swig_getmethods__["fixup"] = _ollypython.t_module_fixup_get
    if _newclass:fixup = property(_ollypython.t_module_fixup_get, _ollypython.t_module_fixup_set)
    __swig_setmethods__["codedec"] = _ollypython.t_module_codedec_set
    __swig_getmethods__["codedec"] = _ollypython.t_module_codedec_get
    if _newclass:codedec = property(_ollypython.t_module_codedec_get, _ollypython.t_module_codedec_set)
    __swig_setmethods__["codecrc"] = _ollypython.t_module_codecrc_set
    __swig_getmethods__["codecrc"] = _ollypython.t_module_codecrc_get
    if _newclass:codecrc = property(_ollypython.t_module_codecrc_get, _ollypython.t_module_codecrc_set)
    __swig_setmethods__["hittrace"] = _ollypython.t_module_hittrace_set
    __swig_getmethods__["hittrace"] = _ollypython.t_module_hittrace_get
    if _newclass:hittrace = property(_ollypython.t_module_hittrace_get, _ollypython.t_module_hittrace_set)
    __swig_setmethods__["hittracecopy"] = _ollypython.t_module_hittracecopy_set
    __swig_getmethods__["hittracecopy"] = _ollypython.t_module_hittracecopy_get
    if _newclass:hittracecopy = property(_ollypython.t_module_hittracecopy_get, _ollypython.t_module_hittracecopy_set)
    __swig_setmethods__["datadec"] = _ollypython.t_module_datadec_set
    __swig_getmethods__["datadec"] = _ollypython.t_module_datadec_get
    if _newclass:datadec = property(_ollypython.t_module_datadec_get, _ollypython.t_module_datadec_set)
    __swig_setmethods__["namelist"] = _ollypython.t_module_namelist_set
    __swig_getmethods__["namelist"] = _ollypython.t_module_namelist_get
    if _newclass:namelist = property(_ollypython.t_module_namelist_get, _ollypython.t_module_namelist_set)
    __swig_setmethods__["symvar"] = _ollypython.t_module_symvar_set
    __swig_getmethods__["symvar"] = _ollypython.t_module_symvar_get
    if _newclass:symvar = property(_ollypython.t_module_symvar_get, _ollypython.t_module_symvar_set)
    __swig_setmethods__["nsymvar"] = _ollypython.t_module_nsymvar_set
    __swig_getmethods__["nsymvar"] = _ollypython.t_module_nsymvar_get
    if _newclass:nsymvar = property(_ollypython.t_module_nsymvar_get, _ollypython.t_module_nsymvar_set)
    __swig_setmethods__["maxsymvar"] = _ollypython.t_module_maxsymvar_set
    __swig_getmethods__["maxsymvar"] = _ollypython.t_module_maxsymvar_get
    if _newclass:maxsymvar = property(_ollypython.t_module_maxsymvar_get, _ollypython.t_module_maxsymvar_set)
    __swig_setmethods__["globaltypes"] = _ollypython.t_module_globaltypes_set
    __swig_getmethods__["globaltypes"] = _ollypython.t_module_globaltypes_get
    if _newclass:globaltypes = property(_ollypython.t_module_globaltypes_get, _ollypython.t_module_globaltypes_set)
    __swig_setmethods__["mainentry"] = _ollypython.t_module_mainentry_set
    __swig_getmethods__["mainentry"] = _ollypython.t_module_mainentry_get
    if _newclass:mainentry = property(_ollypython.t_module_mainentry_get, _ollypython.t_module_mainentry_set)
    __swig_setmethods__["realsfxentry"] = _ollypython.t_module_realsfxentry_set
    __swig_getmethods__["realsfxentry"] = _ollypython.t_module_realsfxentry_get
    if _newclass:realsfxentry = property(_ollypython.t_module_realsfxentry_get, _ollypython.t_module_realsfxentry_set)
    __swig_setmethods__["updatenamelist"] = _ollypython.t_module_updatenamelist_set
    __swig_getmethods__["updatenamelist"] = _ollypython.t_module_updatenamelist_get
    if _newclass:updatenamelist = property(_ollypython.t_module_updatenamelist_get, _ollypython.t_module_updatenamelist_set)
    __swig_setmethods__["origcodesize"] = _ollypython.t_module_origcodesize_set
    __swig_getmethods__["origcodesize"] = _ollypython.t_module_origcodesize_get
    if _newclass:origcodesize = property(_ollypython.t_module_origcodesize_get, _ollypython.t_module_origcodesize_set)
    __swig_setmethods__["sfxbase"] = _ollypython.t_module_sfxbase_set
    __swig_getmethods__["sfxbase"] = _ollypython.t_module_sfxbase_get
    if _newclass:sfxbase = property(_ollypython.t_module_sfxbase_get, _ollypython.t_module_sfxbase_set)
    __swig_setmethods__["sfxsize"] = _ollypython.t_module_sfxsize_set
    __swig_getmethods__["sfxsize"] = _ollypython.t_module_sfxsize_get
    if _newclass:sfxsize = property(_ollypython.t_module_sfxsize_get, _ollypython.t_module_sfxsize_set)
    __swig_setmethods__["issystemdll"] = _ollypython.t_module_issystemdll_set
    __swig_getmethods__["issystemdll"] = _ollypython.t_module_issystemdll_get
    if _newclass:issystemdll = property(_ollypython.t_module_issystemdll_get, _ollypython.t_module_issystemdll_set)
    __swig_setmethods__["processed"] = _ollypython.t_module_processed_set
    __swig_getmethods__["processed"] = _ollypython.t_module_processed_get
    if _newclass:processed = property(_ollypython.t_module_processed_get, _ollypython.t_module_processed_set)
    __swig_setmethods__["dbghelpsym"] = _ollypython.t_module_dbghelpsym_set
    __swig_getmethods__["dbghelpsym"] = _ollypython.t_module_dbghelpsym_get
    if _newclass:dbghelpsym = property(_ollypython.t_module_dbghelpsym_get, _ollypython.t_module_dbghelpsym_set)
    __swig_setmethods__["version"] = _ollypython.t_module_version_set
    __swig_getmethods__["version"] = _ollypython.t_module_version_get
    if _newclass:version = property(_ollypython.t_module_version_get, _ollypython.t_module_version_set)
    __swig_setmethods__["jddata"] = _ollypython.t_module_jddata_set
    __swig_getmethods__["jddata"] = _ollypython.t_module_jddata_get
    if _newclass:jddata = property(_ollypython.t_module_jddata_get, _ollypython.t_module_jddata_set)
    __swig_setmethods__["njddata"] = _ollypython.t_module_njddata_set
    __swig_getmethods__["njddata"] = _ollypython.t_module_njddata_get
    if _newclass:njddata = property(_ollypython.t_module_njddata_get, _ollypython.t_module_njddata_set)
    __swig_setmethods__["reserved"] = _ollypython.t_module_reserved_set
    __swig_getmethods__["reserved"] = _ollypython.t_module_reserved_get
    if _newclass:reserved = property(_ollypython.t_module_reserved_get, _ollypython.t_module_reserved_set)
    def __init__(self, *args): 
        """__init__(self) -> t_module"""
        this = _ollypython.new_t_module(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_module
    __del__ = lambda self : None;
t_module_swigregister = _ollypython.t_module_swigregister
t_module_swigregister(t_module)

DU_ESCAPABLE = _ollypython.DU_ESCAPABLE
DU_BACKUP = _ollypython.DU_BACKUP
DU_TYPE = _ollypython.DU_TYPE
DU_COUNT = _ollypython.DU_COUNT
DU_SIZE = _ollypython.DU_SIZE
DU_UNDEF = _ollypython.DU_UNDEF
DU_HEXTEXT = _ollypython.DU_HEXTEXT
DU_TEXT = _ollypython.DU_TEXT
DU_UNICODE = _ollypython.DU_UNICODE
DU_INT = _ollypython.DU_INT
DU_UINT = _ollypython.DU_UINT
DU_IHEX = _ollypython.DU_IHEX
DU_FLOAT = _ollypython.DU_FLOAT
DU_ADDR = _ollypython.DU_ADDR
DU_DISASM = _ollypython.DU_DISASM
DU_HEXUNI = _ollypython.DU_HEXUNI
DU_ADRASC = _ollypython.DU_ADRASC
DU_ADRUNI = _ollypython.DU_ADRUNI
DU_SPEC = _ollypython.DU_SPEC
MT_BACKUP = _ollypython.MT_BACKUP
MT_COPY = _ollypython.MT_COPY
MT_EDIT = _ollypython.MT_EDIT
MT_SEARCH = _ollypython.MT_SEARCH
MT_DHISTORY = _ollypython.MT_DHISTORY
class t_dump(_object):
    """Proxy of C t_dump struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_dump, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_dump, name)
    __repr__ = _swig_repr
    __swig_setmethods__["table"] = _ollypython.t_dump_table_set
    __swig_getmethods__["table"] = _ollypython.t_dump_table_get
    if _newclass:table = property(_ollypython.t_dump_table_get, _ollypython.t_dump_table_set)
    __swig_setmethods__["dimmed"] = _ollypython.t_dump_dimmed_set
    __swig_getmethods__["dimmed"] = _ollypython.t_dump_dimmed_get
    if _newclass:dimmed = property(_ollypython.t_dump_dimmed_get, _ollypython.t_dump_dimmed_set)
    __swig_setmethods__["threadid"] = _ollypython.t_dump_threadid_set
    __swig_getmethods__["threadid"] = _ollypython.t_dump_threadid_get
    if _newclass:threadid = property(_ollypython.t_dump_threadid_get, _ollypython.t_dump_threadid_set)
    __swig_setmethods__["dumptype"] = _ollypython.t_dump_dumptype_set
    __swig_getmethods__["dumptype"] = _ollypython.t_dump_dumptype_get
    if _newclass:dumptype = property(_ollypython.t_dump_dumptype_get, _ollypython.t_dump_dumptype_set)
    __swig_setmethods__["specdump"] = _ollypython.t_dump_specdump_set
    __swig_getmethods__["specdump"] = _ollypython.t_dump_specdump_get
    if _newclass:specdump = property(_ollypython.t_dump_specdump_get, _ollypython.t_dump_specdump_set)
    __swig_setmethods__["menutype"] = _ollypython.t_dump_menutype_set
    __swig_getmethods__["menutype"] = _ollypython.t_dump_menutype_get
    if _newclass:menutype = property(_ollypython.t_dump_menutype_get, _ollypython.t_dump_menutype_set)
    __swig_setmethods__["itemwidth"] = _ollypython.t_dump_itemwidth_set
    __swig_getmethods__["itemwidth"] = _ollypython.t_dump_itemwidth_get
    if _newclass:itemwidth = property(_ollypython.t_dump_itemwidth_get, _ollypython.t_dump_itemwidth_set)
    __swig_setmethods__["showstackframes"] = _ollypython.t_dump_showstackframes_set
    __swig_getmethods__["showstackframes"] = _ollypython.t_dump_showstackframes_get
    if _newclass:showstackframes = property(_ollypython.t_dump_showstackframes_get, _ollypython.t_dump_showstackframes_set)
    __swig_setmethods__["showstacklocals"] = _ollypython.t_dump_showstacklocals_set
    __swig_getmethods__["showstacklocals"] = _ollypython.t_dump_showstacklocals_get
    if _newclass:showstacklocals = property(_ollypython.t_dump_showstacklocals_get, _ollypython.t_dump_showstacklocals_set)
    __swig_setmethods__["commentmode"] = _ollypython.t_dump_commentmode_set
    __swig_getmethods__["commentmode"] = _ollypython.t_dump_commentmode_get
    if _newclass:commentmode = property(_ollypython.t_dump_commentmode_get, _ollypython.t_dump_commentmode_set)
    __swig_setmethods__["filename"] = _ollypython.t_dump_filename_set
    __swig_getmethods__["filename"] = _ollypython.t_dump_filename_get
    if _newclass:filename = property(_ollypython.t_dump_filename_get, _ollypython.t_dump_filename_set)
    __swig_setmethods__["base"] = _ollypython.t_dump_base_set
    __swig_getmethods__["base"] = _ollypython.t_dump_base_get
    if _newclass:base = property(_ollypython.t_dump_base_get, _ollypython.t_dump_base_set)
    __swig_setmethods__["size"] = _ollypython.t_dump_size_set
    __swig_getmethods__["size"] = _ollypython.t_dump_size_get
    if _newclass:size = property(_ollypython.t_dump_size_get, _ollypython.t_dump_size_set)
    __swig_setmethods__["addr"] = _ollypython.t_dump_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_dump_addr_get
    if _newclass:addr = property(_ollypython.t_dump_addr_get, _ollypython.t_dump_addr_set)
    __swig_setmethods__["lastaddr"] = _ollypython.t_dump_lastaddr_set
    __swig_getmethods__["lastaddr"] = _ollypython.t_dump_lastaddr_get
    if _newclass:lastaddr = property(_ollypython.t_dump_lastaddr_get, _ollypython.t_dump_lastaddr_set)
    __swig_setmethods__["sel0"] = _ollypython.t_dump_sel0_set
    __swig_getmethods__["sel0"] = _ollypython.t_dump_sel0_get
    if _newclass:sel0 = property(_ollypython.t_dump_sel0_get, _ollypython.t_dump_sel0_set)
    __swig_setmethods__["sel1"] = _ollypython.t_dump_sel1_set
    __swig_getmethods__["sel1"] = _ollypython.t_dump_sel1_get
    if _newclass:sel1 = property(_ollypython.t_dump_sel1_get, _ollypython.t_dump_sel1_set)
    __swig_setmethods__["startsel"] = _ollypython.t_dump_startsel_set
    __swig_getmethods__["startsel"] = _ollypython.t_dump_startsel_get
    if _newclass:startsel = property(_ollypython.t_dump_startsel_get, _ollypython.t_dump_startsel_set)
    __swig_setmethods__["captured"] = _ollypython.t_dump_captured_set
    __swig_getmethods__["captured"] = _ollypython.t_dump_captured_get
    if _newclass:captured = property(_ollypython.t_dump_captured_get, _ollypython.t_dump_captured_set)
    __swig_setmethods__["reladdr"] = _ollypython.t_dump_reladdr_set
    __swig_getmethods__["reladdr"] = _ollypython.t_dump_reladdr_get
    if _newclass:reladdr = property(_ollypython.t_dump_reladdr_get, _ollypython.t_dump_reladdr_set)
    __swig_setmethods__["relname"] = _ollypython.t_dump_relname_set
    __swig_getmethods__["relname"] = _ollypython.t_dump_relname_get
    if _newclass:relname = property(_ollypython.t_dump_relname_get, _ollypython.t_dump_relname_set)
    __swig_setmethods__["filecopy"] = _ollypython.t_dump_filecopy_set
    __swig_getmethods__["filecopy"] = _ollypython.t_dump_filecopy_get
    if _newclass:filecopy = property(_ollypython.t_dump_filecopy_get, _ollypython.t_dump_filecopy_set)
    __swig_setmethods__["backup"] = _ollypython.t_dump_backup_set
    __swig_getmethods__["backup"] = _ollypython.t_dump_backup_get
    if _newclass:backup = property(_ollypython.t_dump_backup_get, _ollypython.t_dump_backup_set)
    __swig_setmethods__["runtraceoffset"] = _ollypython.t_dump_runtraceoffset_set
    __swig_getmethods__["runtraceoffset"] = _ollypython.t_dump_runtraceoffset_get
    if _newclass:runtraceoffset = property(_ollypython.t_dump_runtraceoffset_get, _ollypython.t_dump_runtraceoffset_set)
    __swig_setmethods__["reserved"] = _ollypython.t_dump_reserved_set
    __swig_getmethods__["reserved"] = _ollypython.t_dump_reserved_get
    if _newclass:reserved = property(_ollypython.t_dump_reserved_get, _ollypython.t_dump_reserved_set)
    def __init__(self, *args): 
        """__init__(self) -> t_dump"""
        this = _ollypython.new_t_dump(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_dump
    __del__ = lambda self : None;
t_dump_swigregister = _ollypython.t_dump_swigregister
t_dump_swigregister(t_dump)

MAXTRACK = _ollypython.MAXTRACK
MOVETOP = _ollypython.MOVETOP
MOVEBOTTOM = _ollypython.MOVEBOTTOM
CONT_BROADCAST = _ollypython.CONT_BROADCAST
STOP_BROADCAST = _ollypython.STOP_BROADCAST
BKUP_CREATE = _ollypython.BKUP_CREATE
BKUP_VIEWDATA = _ollypython.BKUP_VIEWDATA
BKUP_VIEWCOPY = _ollypython.BKUP_VIEWCOPY
BKUP_LOADCOPY = _ollypython.BKUP_LOADCOPY
BKUP_SAVEDATA = _ollypython.BKUP_SAVEDATA
BKUP_SAVECOPY = _ollypython.BKUP_SAVECOPY
BKUP_DELETE = _ollypython.BKUP_DELETE
class t_ref(_object):
    """Proxy of C t_ref struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_ref, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_ref, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_ref_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_ref_addr_get
    if _newclass:addr = property(_ollypython.t_ref_addr_get, _ollypython.t_ref_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_ref_size_set
    __swig_getmethods__["size"] = _ollypython.t_ref_size_get
    if _newclass:size = property(_ollypython.t_ref_size_get, _ollypython.t_ref_size_set)
    __swig_setmethods__["type"] = _ollypython.t_ref_type_set
    __swig_getmethods__["type"] = _ollypython.t_ref_type_get
    if _newclass:type = property(_ollypython.t_ref_type_get, _ollypython.t_ref_type_set)
    __swig_setmethods__["dest"] = _ollypython.t_ref_dest_set
    __swig_getmethods__["dest"] = _ollypython.t_ref_dest_get
    if _newclass:dest = property(_ollypython.t_ref_dest_get, _ollypython.t_ref_dest_set)
    def __init__(self, *args): 
        """__init__(self) -> t_ref"""
        this = _ollypython.new_t_ref(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_ref
    __del__ = lambda self : None;
t_ref_swigregister = _ollypython.t_ref_swigregister
t_ref_swigregister(t_ref)

MEMBP_READ = _ollypython.MEMBP_READ
MEMBP_WRITE = _ollypython.MEMBP_WRITE
MEMBP_SFX = _ollypython.MEMBP_SFX
TR_HIT = _ollypython.TR_HIT
TR_SET = _ollypython.TR_SET
TR_RTSKIP = _ollypython.TR_RTSKIP
TR_RTRACE = _ollypython.TR_RTRACE
TR_TRACE = _ollypython.TR_TRACE
HB_FREE = _ollypython.HB_FREE
HB_CODE = _ollypython.HB_CODE
HB_ACCESS = _ollypython.HB_ACCESS
HB_WRITE = _ollypython.HB_WRITE
HB_IO = _ollypython.HB_IO
HB_ONESHOT = _ollypython.HB_ONESHOT
HB_STOPAN = _ollypython.HB_STOPAN
HB_TEMP = _ollypython.HB_TEMP
ATR_ADD = _ollypython.ATR_ADD
ATR_ADDPROC = _ollypython.ATR_ADDPROC
ATR_RESET = _ollypython.ATR_RESET
ATR_REMOVE = _ollypython.ATR_REMOVE
ATR_REMOVEALL = _ollypython.ATR_REMOVEALL
ATR_RESTORE = _ollypython.ATR_RESTORE
ATR_RTRADD = _ollypython.ATR_RTRADD
ATR_RTRJUMPS = _ollypython.ATR_RTRJUMPS
ATR_RTRENTRY = _ollypython.ATR_RTRENTRY
ATR_RTREMOVE = _ollypython.ATR_RTREMOVE
ATR_RTSKIP = _ollypython.ATR_RTSKIP
COND_NOBREAK = _ollypython.COND_NOBREAK
COND_LOGTRUE = _ollypython.COND_LOGTRUE
COND_LOGALWAYS = _ollypython.COND_LOGALWAYS
COND_ARGTRUE = _ollypython.COND_ARGTRUE
COND_ARGALWAYS = _ollypython.COND_ARGALWAYS
COND_BRKALWAYS = _ollypython.COND_BRKALWAYS
COND_MASK = _ollypython.COND_MASK
COND_FILLING = _ollypython.COND_FILLING
class t_bpoint(_object):
    """Proxy of C t_bpoint struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_bpoint, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_bpoint, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_bpoint_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_bpoint_addr_get
    if _newclass:addr = property(_ollypython.t_bpoint_addr_get, _ollypython.t_bpoint_addr_set)
    __swig_setmethods__["dummy"] = _ollypython.t_bpoint_dummy_set
    __swig_getmethods__["dummy"] = _ollypython.t_bpoint_dummy_get
    if _newclass:dummy = property(_ollypython.t_bpoint_dummy_get, _ollypython.t_bpoint_dummy_set)
    __swig_setmethods__["type"] = _ollypython.t_bpoint_type_set
    __swig_getmethods__["type"] = _ollypython.t_bpoint_type_get
    if _newclass:type = property(_ollypython.t_bpoint_type_get, _ollypython.t_bpoint_type_set)
    __swig_setmethods__["cmd"] = _ollypython.t_bpoint_cmd_set
    __swig_getmethods__["cmd"] = _ollypython.t_bpoint_cmd_get
    if _newclass:cmd = property(_ollypython.t_bpoint_cmd_get, _ollypython.t_bpoint_cmd_set)
    __swig_setmethods__["passcount"] = _ollypython.t_bpoint_passcount_set
    __swig_getmethods__["passcount"] = _ollypython.t_bpoint_passcount_get
    if _newclass:passcount = property(_ollypython.t_bpoint_passcount_get, _ollypython.t_bpoint_passcount_set)
    def __init__(self, *args): 
        """__init__(self) -> t_bpoint"""
        this = _ollypython.new_t_bpoint(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_bpoint
    __del__ = lambda self : None;
t_bpoint_swigregister = _ollypython.t_bpoint_swigregister
t_bpoint_swigregister(t_bpoint)

class t_hardbpoint(_object):
    """Proxy of C t_hardbpoint struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_hardbpoint, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_hardbpoint, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_hardbpoint_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_hardbpoint_addr_get
    if _newclass:addr = property(_ollypython.t_hardbpoint_addr_get, _ollypython.t_hardbpoint_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_hardbpoint_size_set
    __swig_getmethods__["size"] = _ollypython.t_hardbpoint_size_get
    if _newclass:size = property(_ollypython.t_hardbpoint_size_get, _ollypython.t_hardbpoint_size_set)
    __swig_setmethods__["type"] = _ollypython.t_hardbpoint_type_set
    __swig_getmethods__["type"] = _ollypython.t_hardbpoint_type_get
    if _newclass:type = property(_ollypython.t_hardbpoint_type_get, _ollypython.t_hardbpoint_type_set)
    __swig_setmethods__["reserved"] = _ollypython.t_hardbpoint_reserved_set
    __swig_getmethods__["reserved"] = _ollypython.t_hardbpoint_reserved_get
    if _newclass:reserved = property(_ollypython.t_hardbpoint_reserved_get, _ollypython.t_hardbpoint_reserved_set)
    def __init__(self, *args): 
        """__init__(self) -> t_hardbpoint"""
        this = _ollypython.new_t_hardbpoint(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_hardbpoint
    __del__ = lambda self : None;
t_hardbpoint_swigregister = _ollypython.t_hardbpoint_swigregister
t_hardbpoint_swigregister(t_hardbpoint)

class t_inject(_object):
    """Proxy of C t_inject struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_inject, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_inject, name)
    __repr__ = _swig_repr
    __swig_setmethods__["codesize"] = _ollypython.t_inject_codesize_set
    __swig_getmethods__["codesize"] = _ollypython.t_inject_codesize_get
    if _newclass:codesize = property(_ollypython.t_inject_codesize_get, _ollypython.t_inject_codesize_set)
    __swig_setmethods__["code"] = _ollypython.t_inject_code_set
    __swig_getmethods__["code"] = _ollypython.t_inject_code_get
    if _newclass:code = property(_ollypython.t_inject_code_get, _ollypython.t_inject_code_set)
    __swig_setmethods__["stacksize"] = _ollypython.t_inject_stacksize_set
    __swig_getmethods__["stacksize"] = _ollypython.t_inject_stacksize_get
    if _newclass:stacksize = property(_ollypython.t_inject_stacksize_get, _ollypython.t_inject_stacksize_set)
    __swig_setmethods__["datatype"] = _ollypython.t_inject_datatype_set
    __swig_getmethods__["datatype"] = _ollypython.t_inject_datatype_get
    if _newclass:datatype = property(_ollypython.t_inject_datatype_get, _ollypython.t_inject_datatype_set)
    def __init__(self, *args): 
        """__init__(self) -> t_inject"""
        this = _ollypython.new_t_inject(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_inject
    __del__ = lambda self : None;
t_inject_swigregister = _ollypython.t_inject_swigregister
t_inject_swigregister(t_inject)

CPU_ASMHIST = _ollypython.CPU_ASMHIST
CPU_ASMCENTER = _ollypython.CPU_ASMCENTER
CPU_ASMFOCUS = _ollypython.CPU_ASMFOCUS
CPU_DUMPHIST = _ollypython.CPU_DUMPHIST
CPU_DUMPFIRST = _ollypython.CPU_DUMPFIRST
CPU_DUMPFOCUS = _ollypython.CPU_DUMPFOCUS
CPU_REGAUTO = _ollypython.CPU_REGAUTO
CPU_RUNTRACE = _ollypython.CPU_RUNTRACE
CPU_STACKFOCUS = _ollypython.CPU_STACKFOCUS
CPU_NOCREATE = _ollypython.CPU_NOCREATE
CPU_REDRAW = _ollypython.CPU_REDRAW
CPU_NOFOCUS = _ollypython.CPU_NOFOCUS
STEP_SAME = _ollypython.STEP_SAME
STEP_RUN = _ollypython.STEP_RUN
STEP_OVER = _ollypython.STEP_OVER
STEP_IN = _ollypython.STEP_IN
STEP_SKIP = _ollypython.STEP_SKIP
STAT_NONE = _ollypython.STAT_NONE
STAT_STOPPED = _ollypython.STAT_STOPPED
STAT_EVENT = _ollypython.STAT_EVENT
STAT_RUNNING = _ollypython.STAT_RUNNING
STAT_FINISHED = _ollypython.STAT_FINISHED
STAT_CLOSING = _ollypython.STAT_CLOSING
DASC_TEST = _ollypython.DASC_TEST
DASC_NOHEX = _ollypython.DASC_NOHEX
DASC_ASCII = _ollypython.DASC_ASCII
DASC_PASCAL = _ollypython.DASC_PASCAL
FIRST_CODE_LINE = _ollypython.FIRST_CODE_LINE
class t_sourceline(_object):
    """Proxy of C t_sourceline struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_sourceline, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_sourceline, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_sourceline_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_sourceline_addr_get
    if _newclass:addr = property(_ollypython.t_sourceline_addr_get, _ollypython.t_sourceline_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_sourceline_size_set
    __swig_getmethods__["size"] = _ollypython.t_sourceline_size_get
    if _newclass:size = property(_ollypython.t_sourceline_size_get, _ollypython.t_sourceline_size_set)
    __swig_setmethods__["srcname"] = _ollypython.t_sourceline_srcname_set
    __swig_getmethods__["srcname"] = _ollypython.t_sourceline_srcname_get
    if _newclass:srcname = property(_ollypython.t_sourceline_srcname_get, _ollypython.t_sourceline_srcname_set)
    __swig_setmethods__["line"] = _ollypython.t_sourceline_line_set
    __swig_getmethods__["line"] = _ollypython.t_sourceline_line_get
    if _newclass:line = property(_ollypython.t_sourceline_line_get, _ollypython.t_sourceline_line_set)
    def __init__(self, *args): 
        """__init__(self) -> t_sourceline"""
        this = _ollypython.new_t_sourceline(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_sourceline
    __del__ = lambda self : None;
t_sourceline_swigregister = _ollypython.t_sourceline_swigregister
t_sourceline_swigregister(t_sourceline)

MAXNEST = _ollypython.MAXNEST
class t_window(_object):
    """Proxy of C t_window struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_window, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_window, name)
    __repr__ = _swig_repr
    __swig_setmethods__["hwnd"] = _ollypython.t_window_hwnd_set
    __swig_getmethods__["hwnd"] = _ollypython.t_window_hwnd_get
    if _newclass:hwnd = property(_ollypython.t_window_hwnd_get, _ollypython.t_window_hwnd_set)
    __swig_setmethods__["dummy"] = _ollypython.t_window_dummy_set
    __swig_getmethods__["dummy"] = _ollypython.t_window_dummy_get
    if _newclass:dummy = property(_ollypython.t_window_dummy_get, _ollypython.t_window_dummy_set)
    __swig_setmethods__["type"] = _ollypython.t_window_type_set
    __swig_getmethods__["type"] = _ollypython.t_window_type_get
    if _newclass:type = property(_ollypython.t_window_type_get, _ollypython.t_window_type_set)
    __swig_setmethods__["parenthw"] = _ollypython.t_window_parenthw_set
    __swig_getmethods__["parenthw"] = _ollypython.t_window_parenthw_get
    if _newclass:parenthw = property(_ollypython.t_window_parenthw_get, _ollypython.t_window_parenthw_set)
    __swig_setmethods__["winproc"] = _ollypython.t_window_winproc_set
    __swig_getmethods__["winproc"] = _ollypython.t_window_winproc_get
    if _newclass:winproc = property(_ollypython.t_window_winproc_get, _ollypython.t_window_winproc_set)
    __swig_setmethods__["threadid"] = _ollypython.t_window_threadid_set
    __swig_getmethods__["threadid"] = _ollypython.t_window_threadid_get
    if _newclass:threadid = property(_ollypython.t_window_threadid_get, _ollypython.t_window_threadid_set)
    __swig_setmethods__["exstyle"] = _ollypython.t_window_exstyle_set
    __swig_getmethods__["exstyle"] = _ollypython.t_window_exstyle_get
    if _newclass:exstyle = property(_ollypython.t_window_exstyle_get, _ollypython.t_window_exstyle_set)
    __swig_setmethods__["style"] = _ollypython.t_window_style_set
    __swig_getmethods__["style"] = _ollypython.t_window_style_get
    if _newclass:style = property(_ollypython.t_window_style_get, _ollypython.t_window_style_set)
    __swig_setmethods__["id"] = _ollypython.t_window_id_set
    __swig_getmethods__["id"] = _ollypython.t_window_id_get
    if _newclass:id = property(_ollypython.t_window_id_get, _ollypython.t_window_id_set)
    __swig_setmethods__["classproc"] = _ollypython.t_window_classproc_set
    __swig_getmethods__["classproc"] = _ollypython.t_window_classproc_get
    if _newclass:classproc = property(_ollypython.t_window_classproc_get, _ollypython.t_window_classproc_set)
    __swig_setmethods__["child"] = _ollypython.t_window_child_set
    __swig_getmethods__["child"] = _ollypython.t_window_child_get
    if _newclass:child = property(_ollypython.t_window_child_get, _ollypython.t_window_child_set)
    __swig_setmethods__["level"] = _ollypython.t_window_level_set
    __swig_getmethods__["level"] = _ollypython.t_window_level_get
    if _newclass:level = property(_ollypython.t_window_level_get, _ollypython.t_window_level_set)
    __swig_setmethods__["sibling"] = _ollypython.t_window_sibling_set
    __swig_getmethods__["sibling"] = _ollypython.t_window_sibling_get
    if _newclass:sibling = property(_ollypython.t_window_sibling_get, _ollypython.t_window_sibling_set)
    __swig_setmethods__["byparent"] = _ollypython.t_window_byparent_set
    __swig_getmethods__["byparent"] = _ollypython.t_window_byparent_get
    if _newclass:byparent = property(_ollypython.t_window_byparent_get, _ollypython.t_window_byparent_set)
    __swig_setmethods__["title"] = _ollypython.t_window_title_set
    __swig_getmethods__["title"] = _ollypython.t_window_title_get
    if _newclass:title = property(_ollypython.t_window_title_get, _ollypython.t_window_title_set)
    __swig_setmethods__["classname"] = _ollypython.t_window_classname_set
    __swig_getmethods__["classname"] = _ollypython.t_window_classname_get
    if _newclass:classname = property(_ollypython.t_window_classname_get, _ollypython.t_window_classname_set)
    __swig_setmethods__["tree"] = _ollypython.t_window_tree_set
    __swig_getmethods__["tree"] = _ollypython.t_window_tree_get
    if _newclass:tree = property(_ollypython.t_window_tree_get, _ollypython.t_window_tree_set)
    def __init__(self, *args): 
        """__init__(self) -> t_window"""
        this = _ollypython.new_t_window(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_window
    __del__ = lambda self : None;
t_window_swigregister = _ollypython.t_window_swigregister
t_window_swigregister(t_window)

class t_patch(_object):
    """Proxy of C t_patch struct"""
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, t_patch, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, t_patch, name)
    __repr__ = _swig_repr
    __swig_setmethods__["addr"] = _ollypython.t_patch_addr_set
    __swig_getmethods__["addr"] = _ollypython.t_patch_addr_get
    if _newclass:addr = property(_ollypython.t_patch_addr_get, _ollypython.t_patch_addr_set)
    __swig_setmethods__["size"] = _ollypython.t_patch_size_set
    __swig_getmethods__["size"] = _ollypython.t_patch_size_get
    if _newclass:size = property(_ollypython.t_patch_size_get, _ollypython.t_patch_size_set)
    __swig_setmethods__["type"] = _ollypython.t_patch_type_set
    __swig_getmethods__["type"] = _ollypython.t_patch_type_get
    if _newclass:type = property(_ollypython.t_patch_type_get, _ollypython.t_patch_type_set)
    __swig_setmethods__["orig"] = _ollypython.t_patch_orig_set
    __swig_getmethods__["orig"] = _ollypython.t_patch_orig_get
    if _newclass:orig = property(_ollypython.t_patch_orig_get, _ollypython.t_patch_orig_set)
    __swig_setmethods__["mod"] = _ollypython.t_patch_mod_set
    __swig_getmethods__["mod"] = _ollypython.t_patch_mod_get
    if _newclass:mod = property(_ollypython.t_patch_mod_get, _ollypython.t_patch_mod_set)
    def __init__(self, *args): 
        """__init__(self) -> t_patch"""
        this = _ollypython.new_t_patch(*args)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _ollypython.delete_t_patch
    __del__ = lambda self : None;
t_patch_swigregister = _ollypython.t_patch_swigregister
t_patch_swigregister(t_patch)

VAL_HINST = _ollypython.VAL_HINST
VAL_HWMAIN = _ollypython.VAL_HWMAIN
VAL_HWCLIENT = _ollypython.VAL_HWCLIENT
VAL_NCOLORS = _ollypython.VAL_NCOLORS
VAL_COLORS = _ollypython.VAL_COLORS
VAL_BRUSHES = _ollypython.VAL_BRUSHES
VAL_PENS = _ollypython.VAL_PENS
VAL_NFONTS = _ollypython.VAL_NFONTS
VAL_FONTS = _ollypython.VAL_FONTS
VAL_FONTNAMES = _ollypython.VAL_FONTNAMES
VAL_FONTWIDTHS = _ollypython.VAL_FONTWIDTHS
VAL_FONTHEIGHTS = _ollypython.VAL_FONTHEIGHTS
VAL_NFIXFONTS = _ollypython.VAL_NFIXFONTS
VAL_DEFFONT = _ollypython.VAL_DEFFONT
VAL_NSCHEMES = _ollypython.VAL_NSCHEMES
VAL_SCHEMES = _ollypython.VAL_SCHEMES
VAL_DEFSCHEME = _ollypython.VAL_DEFSCHEME
VAL_DEFHSCROLL = _ollypython.VAL_DEFHSCROLL
VAL_RESTOREWINDOWPOS = _ollypython.VAL_RESTOREWINDOWPOS
VAL_HPROCESS = _ollypython.VAL_HPROCESS
VAL_PROCESSID = _ollypython.VAL_PROCESSID
VAL_HMAINTHREAD = _ollypython.VAL_HMAINTHREAD
VAL_MAINTHREADID = _ollypython.VAL_MAINTHREADID
VAL_MAINBASE = _ollypython.VAL_MAINBASE
VAL_PROCESSNAME = _ollypython.VAL_PROCESSNAME
VAL_EXEFILENAME = _ollypython.VAL_EXEFILENAME
VAL_CURRENTDIR = _ollypython.VAL_CURRENTDIR
VAL_SYSTEMDIR = _ollypython.VAL_SYSTEMDIR
VAL_DECODEANYIP = _ollypython.VAL_DECODEANYIP
VAL_PASCALSTRINGS = _ollypython.VAL_PASCALSTRINGS
VAL_ONLYASCII = _ollypython.VAL_ONLYASCII
VAL_DIACRITICALS = _ollypython.VAL_DIACRITICALS
VAL_GLOBALSEARCH = _ollypython.VAL_GLOBALSEARCH
VAL_ALIGNEDSEARCH = _ollypython.VAL_ALIGNEDSEARCH
VAL_IGNORECASE = _ollypython.VAL_IGNORECASE
VAL_SEARCHMARGIN = _ollypython.VAL_SEARCHMARGIN
VAL_KEEPSELSIZE = _ollypython.VAL_KEEPSELSIZE
VAL_MMXDISPLAY = _ollypython.VAL_MMXDISPLAY
VAL_WINDOWFONT = _ollypython.VAL_WINDOWFONT
VAL_TABSTOPS = _ollypython.VAL_TABSTOPS
VAL_MODULES = _ollypython.VAL_MODULES
VAL_MEMORY = _ollypython.VAL_MEMORY
VAL_THREADS = _ollypython.VAL_THREADS
VAL_BREAKPOINTS = _ollypython.VAL_BREAKPOINTS
VAL_REFERENCES = _ollypython.VAL_REFERENCES
VAL_SOURCELIST = _ollypython.VAL_SOURCELIST
VAL_WATCHES = _ollypython.VAL_WATCHES
VAL_CPUFEATURES = _ollypython.VAL_CPUFEATURES
VAL_TRACEFILE = _ollypython.VAL_TRACEFILE
VAL_ALIGNDIALOGS = _ollypython.VAL_ALIGNDIALOGS
VAL_CPUDASM = _ollypython.VAL_CPUDASM
VAL_CPUDDUMP = _ollypython.VAL_CPUDDUMP
VAL_CPUDSTACK = _ollypython.VAL_CPUDSTACK
VAL_APIHELP = _ollypython.VAL_APIHELP
VAL_HARDBP = _ollypython.VAL_HARDBP
VAL_PATCHES = _ollypython.VAL_PATCHES
VAL_HINTS = _ollypython.VAL_HINTS
PM_MAIN = _ollypython.PM_MAIN
PM_DUMP = _ollypython.PM_DUMP
PM_MODULES = _ollypython.PM_MODULES
PM_MEMORY = _ollypython.PM_MEMORY
PM_THREADS = _ollypython.PM_THREADS
PM_BREAKPOINTS = _ollypython.PM_BREAKPOINTS
PM_REFERENCES = _ollypython.PM_REFERENCES
PM_RTRACE = _ollypython.PM_RTRACE
PM_WATCHES = _ollypython.PM_WATCHES
PM_WINDOWS = _ollypython.PM_WINDOWS
PM_DISASM = _ollypython.PM_DISASM
PM_CPUDUMP = _ollypython.PM_CPUDUMP
PM_CPUSTACK = _ollypython.PM_CPUSTACK
PM_CPUREGS = _ollypython.PM_CPUREGS
PP_MAIN = _ollypython.PP_MAIN
PP_EVENT = _ollypython.PP_EVENT
PP_PAUSE = _ollypython.PP_PAUSE
PP_TERMINATED = _ollypython.PP_TERMINATED
PP_BYPROGRAM = _ollypython.PP_BYPROGRAM
PP_INT3BREAK = _ollypython.PP_INT3BREAK
PP_MEMBREAK = _ollypython.PP_MEMBREAK
PP_HWBREAK = _ollypython.PP_HWBREAK
PP_SINGLESTEP = _ollypython.PP_SINGLESTEP
PP_EXCEPTION = _ollypython.PP_EXCEPTION
PP_ACCESS = _ollypython.PP_ACCESS
PP_GUARDED = _ollypython.PP_GUARDED
MI_SIGNATURE = _ollypython.MI_SIGNATURE
MI_VERSION = _ollypython.MI_VERSION
MI_FILENAME = _ollypython.MI_FILENAME
MI_FILESIZE = _ollypython.MI_FILESIZE
MI_TIMESTAMP = _ollypython.MI_TIMESTAMP
MI_SFXENTRY = _ollypython.MI_SFXENTRY
MI_PATCH = _ollypython.MI_PATCH
MI_USER = _ollypython.MI_USER
MI_PROCDATA = _ollypython.MI_PROCDATA
MI_SWDATA = _ollypython.MI_SWDATA
MI_CALLFINT = _ollypython.MI_CALLFINT
MI_CALLFMOD = _ollypython.MI_CALLFMOD
MI_CALLFABS = _ollypython.MI_CALLFABS
MI_INT3BREAK = _ollypython.MI_INT3BREAK
MI_INT3BRKC = _ollypython.MI_INT3BRKC
MI_HDWRBREAK = _ollypython.MI_HDWRBREAK
MI_JDDATA = _ollypython.MI_JDDATA
MI_ANALYSIS = _ollypython.MI_ANALYSIS
MI_ANALPACK = _ollypython.MI_ANALPACK
MI_AHINT = _ollypython.MI_AHINT
MI_TRACE = _ollypython.MI_TRACE
MI_TRACEPACK = _ollypython.MI_TRACEPACK
MI_CODECRC = _ollypython.MI_CODECRC
MI_SAVEAREA = _ollypython.MI_SAVEAREA
MI_END = _ollypython.MI_END
MI_WINJUG = _ollypython.MI_WINJUG
MI_WINJU1 = _ollypython.MI_WINJU1
MI_WINJU2 = _ollypython.MI_WINJU2
MI_WINJU3 = _ollypython.MI_WINJU3
MI_APPST = _ollypython.MI_APPST

def Addtolist(*args):
  """Addtolist(long addr, int highlight, char format, v(...) ?)"""
  return _ollypython.Addtolist(*args)

def Updatelist(*args):
  """Updatelist()"""
  return _ollypython.Updatelist(*args)

def Createlistwindow(*args):
  """Createlistwindow() -> HWND"""
  return _ollypython.Createlistwindow(*args)

def Error(*args):
  """Error(char format, v(...) ?)"""
  return _ollypython.Error(*args)

def Message(*args):
  """Message(ulong addr, char format, v(...) ?)"""
  return _ollypython.Message(*args)

def Infoline(*args):
  """Infoline(char format, v(...) ?)"""
  return _ollypython.Infoline(*args)

def Progress(*args):
  """Progress(int promille, char format, v(...) ?)"""
  return _ollypython.Progress(*args)

def Flash(*args):
  """Flash(char format, v(...) ?)"""
  return _ollypython.Flash(*args)

def Followcall(*args):
  """Followcall(ulong addr) -> ulong"""
  return _ollypython.Followcall(*args)

def IstextA(*args):
  """IstextA(char c) -> int"""
  return _ollypython.IstextA(*args)

def Stringtotext(*args):
  """Stringtotext(char data, int ndata, char text, int ntext) -> int"""
  return _ollypython.Stringtotext(*args)

def Getlong(*args):
  """
    Getlong(char title, ulong data, int datasize, char letter, 
        int mode) -> int
    """
  return _ollypython.Getlong(*args)

def Getlongxy(*args):
  """
    Getlongxy(char title, ulong data, int datasize, char letter, 
        int mode, int x, int y) -> int
    """
  return _ollypython.Getlongxy(*args)

def Getregxy(*args):
  """Getregxy(char title, ulong data, char letter, int x, int y) -> int"""
  return _ollypython.Getregxy(*args)

def Getline(*args):
  """Getline(char title, ulong data) -> int"""
  return _ollypython.Getline(*args)

def Getlinexy(*args):
  """Getlinexy(char title, ulong data, int x, int y) -> int"""
  return _ollypython.Getlinexy(*args)

def Getfloat10(*args):
  """
    Getfloat10(char title, long double fdata, uchar tag, char letter, 
        int mode) -> int
    """
  return _ollypython.Getfloat10(*args)

def Getfloat10xy(*args):
  """
    Getfloat10xy(char title, long double fdata, char tag, char letter, 
        int mode, int x, int y) -> int
    """
  return _ollypython.Getfloat10xy(*args)

def Getfloat(*args):
  """Getfloat(char title, void fdata, int size, char letter, int mode) -> int"""
  return _ollypython.Getfloat(*args)

def Getfloatxy(*args):
  """
    Getfloatxy(char title, void fdata, int size, char letter, int mode, 
        int x, int y) -> int
    """
  return _ollypython.Getfloatxy(*args)

def Getasmfindmodel(*args):
  """Getasmfindmodel(t_asmmodel model, char letter, int searchall)"""
  return _ollypython.Getasmfindmodel(*args)

def Getasmfindmodelxy(*args):
  """
    Getasmfindmodelxy(t_asmmodel model, char letter, int searchall, int x, 
        int y)
    """
  return _ollypython.Getasmfindmodelxy(*args)

def Gettext(*args):
  """Gettext(char title, char text, char letter, int type, int fontindex) -> int"""
  return _ollypython.Gettext(*args)

def Gettextxy(*args):
  """
    Gettextxy(char title, char text, char letter, int type, int fontindex, 
        int x, int y) -> int
    """
  return _ollypython.Gettextxy(*args)

def Gethexstring(*args):
  """Gethexstring(char title, t_hexstr hs, int mode, int fontindex, char letter) -> int"""
  return _ollypython.Gethexstring(*args)

def Gethexstringxy(*args):
  """
    Gethexstringxy(char title, t_hexstr hs, int mode, int fontindex, char letter, 
        int x, int y) -> int
    """
  return _ollypython.Gethexstringxy(*args)

def Getmmx(*args):
  """Getmmx(char title, uchar data, int mode) -> int"""
  return _ollypython.Getmmx(*args)

def Getmmxxy(*args):
  """Getmmxxy(char title, char data, int mode, int x, int y) -> int"""
  return _ollypython.Getmmxxy(*args)

def Get3dnow(*args):
  """Get3dnow(char title, uchar data, int mode) -> int"""
  return _ollypython.Get3dnow(*args)

def Get3dnowxy(*args):
  """Get3dnowxy(char title, char data, int mode, int x, int y) -> int"""
  return _ollypython.Get3dnowxy(*args)

def OpenEXEfile(*args):
  """OpenEXEfile(char path, int dropped) -> int"""
  return _ollypython.OpenEXEfile(*args)

def Attachtoactiveprocess(*args):
  """Attachtoactiveprocess(int newprocessid) -> int"""
  return _ollypython.Attachtoactiveprocess(*args)

def Animate(*args):
  """Animate(int animation)"""
  return _ollypython.Animate(*args)

def Createsorteddata(*args):
  """
    Createsorteddata(t_sorted sd, char name, int itemsize, int nmax, SORTFUNC sortfunc, 
        DESTFUNC destfunc) -> int
    """
  return _ollypython.Createsorteddata(*args)

def Destroysorteddata(*args):
  """Destroysorteddata(t_sorted sd)"""
  return _ollypython.Destroysorteddata(*args)

def Addsorteddata(*args):
  """Addsorteddata(t_sorted sd, void item) -> void"""
  return _ollypython.Addsorteddata(*args)

def Deletesorteddata(*args):
  """Deletesorteddata(t_sorted sd, ulong addr)"""
  return _ollypython.Deletesorteddata(*args)

def Deletesorteddatarange(*args):
  """Deletesorteddatarange(t_sorted sd, ulong addr0, ulong addr1)"""
  return _ollypython.Deletesorteddatarange(*args)

def Deletenonconfirmedsorteddata(*args):
  """Deletenonconfirmedsorteddata(t_sorted sd) -> int"""
  return _ollypython.Deletenonconfirmedsorteddata(*args)

def Findsorteddata(*args):
  """Findsorteddata(t_sorted sd, ulong addr) -> void"""
  return _ollypython.Findsorteddata(*args)

def Findsorteddatarange(*args):
  """Findsorteddatarange(t_sorted sd, ulong addr0, ulong addr1) -> void"""
  return _ollypython.Findsorteddatarange(*args)

def Findsorteddataindex(*args):
  """Findsorteddataindex(t_sorted sd, ulong addr0, ulong addr1) -> int"""
  return _ollypython.Findsorteddataindex(*args)

def Sortsorteddata(*args):
  """Sortsorteddata(t_sorted sd, int sort) -> int"""
  return _ollypython.Sortsorteddata(*args)

def Getsortedbyselection(*args):
  """Getsortedbyselection(t_sorted sd, int index) -> void"""
  return _ollypython.Getsortedbyselection(*args)

def Defaultbar(*args):
  """Defaultbar(t_bar pb)"""
  return _ollypython.Defaultbar(*args)

def Tablefunction(*args):
  """Tablefunction(t_table pt, HWND hw, UINT msg, WPARAM wp, LPARAM lp) -> int"""
  return _ollypython.Tablefunction(*args)

def Painttable(*args):
  """Painttable(HWND hw, t_table pt, DRAWFUNC getline)"""
  return _ollypython.Painttable(*args)

def Gettableselectionxy(*args):
  """Gettableselectionxy(t_table pt, int column, int px, int py) -> int"""
  return _ollypython.Gettableselectionxy(*args)

def Selectandscroll(*args):
  """Selectandscroll(t_table pt, int index, int mode)"""
  return _ollypython.Selectandscroll(*args)

def Insertname(*args):
  """Insertname(ulong addr, int type, char name) -> int"""
  return _ollypython.Insertname(*args)

def Quickinsertname(*args):
  """Quickinsertname(ulong addr, int type, char name) -> int"""
  return _ollypython.Quickinsertname(*args)

def Mergequicknames(*args):
  """Mergequicknames()"""
  return _ollypython.Mergequicknames(*args)

def Discardquicknames(*args):
  """Discardquicknames()"""
  return _ollypython.Discardquicknames(*args)

def Deletenamerange(*args):
  """Deletenamerange(ulong addr0, ulong addr1, int type)"""
  return _ollypython.Deletenamerange(*args)

def Disassembleback(*args):
  """
    Disassembleback(uchar block, ulong base, ulong size, ulong ip, int n, 
        int usedec) -> ulong
    """
  return _ollypython.Disassembleback(*args)

def Disassembleforward(*args):
  """
    Disassembleforward(uchar block, ulong base, ulong size, ulong ip, int n, 
        int usedec) -> ulong
    """
  return _ollypython.Disassembleforward(*args)

def Isfilling(*args):
  """Isfilling(ulong offset, char data, ulong size, ulong align) -> int"""
  return _ollypython.Isfilling(*args)

def Checkcondition(*args):
  """Checkcondition(int code, ulong flags) -> int"""
  return _ollypython.Checkcondition(*args)

def Createthreadwindow(*args):
  """Createthreadwindow() -> HWND"""
  return _ollypython.Createthreadwindow(*args)

def Findthread(*args):
  """Findthread(ulong threadid) -> t_thread"""
  return _ollypython.Findthread(*args)

def Getcputhreadid(*args):
  """Getcputhreadid() -> ulong"""
  return _ollypython.Getcputhreadid(*args)

def Runsinglethread(*args):
  """Runsinglethread(ulong threadid) -> ulong"""
  return _ollypython.Runsinglethread(*args)

def Restoreallthreads(*args):
  """Restoreallthreads()"""
  return _ollypython.Restoreallthreads(*args)

def Listmemory(*args):
  """Listmemory() -> int"""
  return _ollypython.Listmemory(*args)

def Findmemory(*args):
  """Findmemory(ulong addr) -> t_memory"""
  return _ollypython.Findmemory(*args)

def Guardmemory(*args):
  """Guardmemory(ulong base, ulong size, int guard) -> int"""
  return _ollypython.Guardmemory(*args)

def Havecopyofmemory(*args):
  """Havecopyofmemory(uchar copy, ulong base, ulong size)"""
  return _ollypython.Havecopyofmemory(*args)

def Readmemory(*args):
  """Readmemory(void buf, ulong addr, ulong size, int mode) -> ulong"""
  return _ollypython.Readmemory(*args)

def Writememory(*args):
  """Writememory(void buf, ulong addr, ulong size, int mode) -> ulong"""
  return _ollypython.Writememory(*args)

def Findmodule(*args):
  """Findmodule(ulong addr) -> t_module"""
  return _ollypython.Findmodule(*args)

def Findfixup(*args):
  """Findfixup(t_module pmod, ulong addr) -> t_fixup"""
  return _ollypython.Findfixup(*args)

def Finddecode(*args):
  """Finddecode(ulong addr, ulong psize) -> uchar"""
  return _ollypython.Finddecode(*args)

def Findfileoffset(*args):
  """Findfileoffset(t_module pmod, ulong addr) -> ulong"""
  return _ollypython.Findfileoffset(*args)

def Analysecode(*args):
  """Analysecode(t_module pm) -> int"""
  return _ollypython.Analysecode(*args)

def Newtablewindow(*args):
  """
    Newtablewindow(t_table pt, int nlines, int maxcolumns, char winclass, 
        char wintitle) -> HWND
    """
  return _ollypython.Newtablewindow(*args)

def Quicktablewindow(*args):
  """
    Quicktablewindow(t_table pt, int nlines, int maxcolumns, char winclass, 
        char wintitle) -> HWND
    """
  return _ollypython.Quicktablewindow(*args)

def Createdumpwindow(*args):
  """
    Createdumpwindow(char name, ulong base, ulong size, ulong addr, int type, 
        SPECFUNC specdump) -> HWND
    """
  return _ollypython.Createdumpwindow(*args)

def Setdumptype(*args):
  """Setdumptype(t_dump pd, int dumptype)"""
  return _ollypython.Setdumptype(*args)

def Dumpbackup(*args):
  """Dumpbackup(t_dump pd, int action)"""
  return _ollypython.Dumpbackup(*args)

def Broadcast(*args):
  """Broadcast(UINT msg, WPARAM wp, LPARAM lp) -> int"""
  return _ollypython.Broadcast(*args)

def Compress(*args):
  """Compress(uchar bufin, ulong nbufin, uchar bufout, ulong nbufout) -> ulong"""
  return _ollypython.Compress(*args)

def Getoriginaldatasize(*args):
  """Getoriginaldatasize(char bufin, ulong nbufin) -> ulong"""
  return _ollypython.Getoriginaldatasize(*args)

def Decompress(*args):
  """Decompress(uchar bufin, ulong nbufin, uchar bufout, ulong nbufout) -> ulong"""
  return _ollypython.Decompress(*args)

def Calculatecrc(*args):
  """
    Calculatecrc(uchar copy, ulong base, ulong size, t_module pmod, 
        ulong fixupoffset) -> ulong
    """
  return _ollypython.Calculatecrc(*args)

def Findreferences(*args):
  """
    Findreferences(ulong base, ulong size, ulong addr0, ulong addr1, ulong origin, 
        int recurseonjump, char title) -> int
    """
  return _ollypython.Findreferences(*args)

def Findstrings(*args):
  """Findstrings(ulong base, ulong size, ulong origin, char title) -> int"""
  return _ollypython.Findstrings(*args)

def Findalldllcalls(*args):
  """Findalldllcalls(t_dump pd, ulong origin, char title) -> int"""
  return _ollypython.Findalldllcalls(*args)

def Findallcommands(*args):
  """Findallcommands(t_dump pd, t_asmmodel model, ulong origin, char title) -> int"""
  return _ollypython.Findallcommands(*args)

def Findallsequences(*args):
  """Findallsequences(t_dump pd, t_extmodel model, ulong origin, char title) -> int"""
  return _ollypython.Findallsequences(*args)

def Walkreference(*args):
  """Walkreference(int dir) -> ulong"""
  return _ollypython.Walkreference(*args)

def Walkreferenceex(*args):
  """Walkreferenceex(int dir, ulong size) -> ulong"""
  return _ollypython.Walkreferenceex(*args)

def Setbreakpoint(*args):
  """Setbreakpoint(ulong addr, ulong type, uchar cmd) -> int"""
  return _ollypython.Setbreakpoint(*args)

def Setbreakpointext(*args):
  """Setbreakpointext(ulong addr, ulong type, char cmd, ulong passcount) -> int"""
  return _ollypython.Setbreakpointext(*args)

def Manualbreakpoint(*args):
  """
    Manualbreakpoint(ulong addr, int key, int shiftkey, ulong nametype, 
        int font) -> int
    """
  return _ollypython.Manualbreakpoint(*args)

def Deletebreakpoints(*args):
  """Deletebreakpoints(ulong addr0, ulong addr1, int silent)"""
  return _ollypython.Deletebreakpoints(*args)

def Getbreakpointtype(*args):
  """Getbreakpointtype(ulong addr) -> ulong"""
  return _ollypython.Getbreakpointtype(*args)

def Getnextbreakpoint(*args):
  """Getnextbreakpoint(ulong addr, ulong type, int cmd) -> ulong"""
  return _ollypython.Getnextbreakpoint(*args)

def Tempbreakpoint(*args):
  """Tempbreakpoint(ulong addr, int mode)"""
  return _ollypython.Tempbreakpoint(*args)

def Hardbreakpoints(*args):
  """Hardbreakpoints(int closeondelete) -> int"""
  return _ollypython.Hardbreakpoints(*args)

def Sethardwarebreakpoint(*args):
  """Sethardwarebreakpoint(ulong addr, int size, int type) -> int"""
  return _ollypython.Sethardwarebreakpoint(*args)

def Deletehardwarebreakpoint(*args):
  """Deletehardwarebreakpoint(int index) -> int"""
  return _ollypython.Deletehardwarebreakpoint(*args)

def Deletehardwarebreakbyaddr(*args):
  """Deletehardwarebreakbyaddr(ulong addr) -> int"""
  return _ollypython.Deletehardwarebreakbyaddr(*args)

def Setmembreakpoint(*args):
  """Setmembreakpoint(int type, ulong addr, ulong size) -> int"""
  return _ollypython.Setmembreakpoint(*args)

def Modifyhittrace(*args):
  """Modifyhittrace(ulong addr0, ulong addr1, int mode) -> int"""
  return _ollypython.Modifyhittrace(*args)

def Creatertracewindow(*args):
  """Creatertracewindow() -> HWND"""
  return _ollypython.Creatertracewindow(*args)

def Settracecondition(*args):
  """
    Settracecondition(char cond, int onsuspicious, ulong in0, ulong in1, 
        ulong out0, ulong out1)
    """
  return _ollypython.Settracecondition(*args)

def Settracecount(*args):
  """Settracecount(ulong count)"""
  return _ollypython.Settracecount(*args)

def Settracepauseoncommands(*args):
  """Settracepauseoncommands(char cmdset)"""
  return _ollypython.Settracepauseoncommands(*args)

def Startruntrace(*args):
  """Startruntrace(t_reg preg) -> int"""
  return _ollypython.Startruntrace(*args)

def Deleteruntrace(*args):
  """Deleteruntrace()"""
  return _ollypython.Deleteruntrace(*args)

def Runtracesize(*args):
  """Runtracesize() -> int"""
  return _ollypython.Runtracesize(*args)

def Findprevruntraceip(*args):
  """Findprevruntraceip(ulong ip, int startback) -> int"""
  return _ollypython.Findprevruntraceip(*args)

def Findnextruntraceip(*args):
  """Findnextruntraceip(ulong ip, int startback) -> int"""
  return _ollypython.Findnextruntraceip(*args)

def Getruntraceprofile(*args):
  """Getruntraceprofile(ulong addr, ulong size, ulong profile) -> int"""
  return _ollypython.Getruntraceprofile(*args)

def Scrollruntracewindow(*args):
  """Scrollruntracewindow(int back)"""
  return _ollypython.Scrollruntracewindow(*args)

def Createprofilewindow(*args):
  """Createprofilewindow(ulong base, ulong size) -> HWND"""
  return _ollypython.Createprofilewindow(*args)

def Injectcode(*args):
  """
    Injectcode(ulong threadid, t_inject inject, char data, ulong datasize, 
        ulong parm1, ulong parm2, INJECTANSWER answerfunc) -> int
    """
  return _ollypython.Injectcode(*args)

def Setcpu(*args):
  """
    Setcpu(ulong threadid, ulong asmaddr, ulong dumpaddr, ulong stackaddr, 
        int mode)
    """
  return _ollypython.Setcpu(*args)

def Setdisasm(*args):
  """Setdisasm(ulong asmaddr, ulong selsize, int mode)"""
  return _ollypython.Setdisasm(*args)

def Redrawdisassembler(*args):
  """Redrawdisassembler()"""
  return _ollypython.Redrawdisassembler(*args)

def Findprocbegin(*args):
  """Findprocbegin(ulong addr) -> ulong"""
  return _ollypython.Findprocbegin(*args)

def Findprocend(*args):
  """Findprocend(ulong addr) -> ulong"""
  return _ollypython.Findprocend(*args)

def Findprevproc(*args):
  """Findprevproc(ulong addr) -> ulong"""
  return _ollypython.Findprevproc(*args)

def Findnextproc(*args):
  """Findnextproc(ulong addr) -> ulong"""
  return _ollypython.Findnextproc(*args)

def Sendshortcut(*args):
  """
    Sendshortcut(int where, ulong addr, int msg, int ctrl, int shift, 
        int vkcode)
    """
  return _ollypython.Sendshortcut(*args)

def Getstatus(*args):
  """Getstatus() -> int"""
  return _ollypython.Getstatus(*args)

def Go(*args):
  """
    Go(ulong threadid, ulong tilladdr, int stepmode, int givechance, 
        int backupregs) -> int
    """
  return _ollypython.Go(*args)

def Suspendprocess(*args):
  """Suspendprocess(int processevents) -> int"""
  return _ollypython.Suspendprocess(*args)

def Decodeknownargument(*args):
  """
    Decodeknownargument(ulong addr, uchar arg, ulong value, int valid, char s, 
        char mask, uchar pset) -> int
    """
  return _ollypython.Decodeknownargument(*args)

def Findunknownfunction(*args):
  """Findunknownfunction(ulong ip, char code, char dec, ulong size, char fname) -> char"""
  return _ollypython.Findunknownfunction(*args)

def Showsourcefromaddress(*args):
  """Showsourcefromaddress(ulong addr, int show) -> HWND"""
  return _ollypython.Showsourcefromaddress(*args)

def Getlinefromaddress(*args):
  """Getlinefromaddress(ulong addr) -> t_sourceline"""
  return _ollypython.Getlinefromaddress(*args)

def Getaddressfromline(*args):
  """Getaddressfromline(ulong addr0, ulong addr1, char path, ulong line) -> ulong"""
  return _ollypython.Getaddressfromline(*args)

def Decodefullvarname(*args):
  """Decodefullvarname(t_module pmod, t_symvar psym, int offset, char name) -> int"""
  return _ollypython.Decodefullvarname(*args)

def Getbprelname(*args):
  """Getbprelname(t_module pmod, ulong addr, long offset, char s, int nsymb) -> int"""
  return _ollypython.Getbprelname(*args)

def Createwatchwindow(*args):
  """Createwatchwindow() -> HWND"""
  return _ollypython.Createwatchwindow(*args)

def Deletewatch(*args):
  """Deletewatch(int indexone) -> int"""
  return _ollypython.Deletewatch(*args)

def Insertwatch(*args):
  """Insertwatch(int indexone, char text) -> int"""
  return _ollypython.Insertwatch(*args)

def Createwinwindow(*args):
  """Createwinwindow() -> HWND"""
  return _ollypython.Createwinwindow(*args)

def Createpatchwindow(*args):
  """Createpatchwindow() -> HWND"""
  return _ollypython.Createpatchwindow(*args)

def Unregisterpluginclass(*args):
  """Unregisterpluginclass(char classname)"""
  return _ollypython.Unregisterpluginclass(*args)

def Pluginwriteinttoini(*args):
  """Pluginwriteinttoini(HINSTANCE dllinst, char key, int value) -> int"""
  return _ollypython.Pluginwriteinttoini(*args)

def Pluginwritestringtoini(*args):
  """Pluginwritestringtoini(HINSTANCE dllinst, char key, char s) -> int"""
  return _ollypython.Pluginwritestringtoini(*args)

def Pluginreadintfromini(*args):
  """Pluginreadintfromini(HINSTANCE dllinst, char key, int def) -> int"""
  return _ollypython.Pluginreadintfromini(*args)

def Pluginsaverecord(*args):
  """Pluginsaverecord(ulong tag, ulong size, void data) -> int"""
  return _ollypython.Pluginsaverecord(*args)

def Plugingetvalue(*args):
  """Plugingetvalue(int type) -> int"""
  return _ollypython.Plugingetvalue(*args)


