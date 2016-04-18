from ODBG_pyth import *
#---------------------(C) 0x0c0de 2008-------------------------------------------
#--------------------ODBGPython library------------------------------------------
#-------------See examples in examlpe directory-------------------------------

def Run():
 """
 F9 
 """
 thrd = Getcputhreadid()
 Animate(ANIMATE_OFF)
 return Go(thrd, 0, STEP_RUN, 0, 0)

def StepF7():
  """
  F7
  """
  thrd = Getcputhreadid()
  Animate(ANIMATE_OFF)
  return Go(thrd, 0, STEP_IN, 0, 0)
        
def StepF8():
  """
  F8
  """
  thrd = Getcputhreadid()
  Animate(ANIMATE_OFF)
  return Go(thrd, 0, STEP_OVER, 0, 0) 

def RunRet():
 """
 Ctrl+F9
 """
 thrd = Getcputhreadid()
 Animate(ANIMATE_RET)
 return Go(thrd, 0, STEP_RUN, 0, 0)

def Log(data):
  """
  Adds a single line of ASCII text to the log window
  
  void Log(char * data)  
  """
  return Addtolist(0, 0,data)
 
def Info(infl):
  """ 
  Sets the status bar message

  void Info(char * data)
  """
  return Infoline(infl)
 
def Err(msg):
 """
 This function shows an Error dialog with a custom message.
 
 void Err(char * data)
 """
 return Error(msg)

def clearStatusBar():
 """
 
 Removes the current status bar message.
 
 """
 return Infoline("")
 
def Sethardbp(address,size,type):
 """ 
 
 Sets Hardware breakpoint
 
 int   Sethardbp(unsigned long addr,int size,int type)
 
 """
 return Sethardwarebreakpoint(address,size,type)

 
def DisableMemBreakpoint(addr):
  """
  
  Disable Memory Breakpoint.
  
  """
  return Setmembreakpoint(0, addr,0)
 
def Deletehardbp(index):
 """
 
 Disable hardware breakpoint by index.
 
 int   Deletehardbp(int index)
 
 """ 
 return Deletehardwarebreakpoint(index)
 
def Deletehardbpbyaddr(addr):
 """
 
 Disable hardware breakpoint by address.
 
 int   Deletehardbpbyaddr(unsigned long addr)
 """
 
 return Deletehardwarebreakbyaddr(addr)

def Getfuncaddr(func):
 """
 Get function address 
 
 unsigned long Getfuncaddr(char* funcdll)
 """ 
 return Getfuncaddress(func)

def Setbp(addr,type,cmd):
 """
 Sets breakpoint
 
 int   Setbp(unsigned long addr,unsigned long type,unsigned char cmd)
 """
 return Setbreakpoint(addr,type,cmd)

def GReg(reg_name):
 """
 Get reg value 
 
 unsigned long GReg(char * reg_name)
 
 *Argument is nOt cAse SensiTivE (for example "eIP" == "EiP")
 """
 return GetReg(reg_name)

def Asm(command,address):
 """
 Assemble command
 
 Asm(char * command,unsigned long address) 
 
 """
 m = t_asmmodel()
 Assemble(command,address, m, 0, 0,"Error asm")
 return Writememory(m.code,address,m.length,2)

def Writemem(addr,buf,size):
 """
 Write buffer to memory address.
 
 unsigned long  Writemem(unsigned long addr,void *buf,unsigned long size)
 """
 return Writememory(buf, addr,size,2)

def Browsefile(title,buf,ext,mode):
 """
 Show Open File dialog
  
 int  Browsefile(char *title,char *buf,char *ext,int mode)
 """
 return Browsefilename(title,buf,ext,mode)

def Tempbp(addr,type):
 """
 Sets temporary breakpoint
 
 void  Tempbp(unsigned long addr,int mode)
 """
 return Tempbreakpoint(addr,type)
 
def Setmembp(type,addr,size):
 """
 Sets memory breapoint
 
 int   Setmembp(int type,unsigned long addr,unsigned long size)
 """
 return Setmembreakpoint(type,addr,size)
 
def Findmod(addr):
 """
 Return structure with module's information
 
 t_module* Findmod(unsigned long addr)
 """ 
 return Findmodule(addr)
 
def Analyse(pmod):
 """
 Analyse module's code
 int   Analyse(t_module *pmod)
 """
 return Analysecode(pmod)
 
def Dltruntrace():
 """
 
 Delete run trace
 
 """
 return Deleteruntrace()
 
def Runfile(path):
 """
 Load file into debugger
 
 int  Runfile(char *path)
 """ 
 return OpenEXEfile(path,0)

def Attach(pid):
 """

 Attach to an active process by Process ID
 
 int  Attach(int pid)
 """
 return Attachtoactiveprocess(pid)
 
def Attachbyname(procname):
 """

 Attach to an active process by process name
 
 void Attachbyname(char * procname)
 """
 return Attach_by_name(procname)
 
def Suspend(event):
 """
 Suspend process
 
 int Suspend(int event)
 
 """ 
 return Suspendprocess(event)
 
def Restoreallthrd():
 """
 
 Restore all prosess's threads
 
 """
 return Restoreallthreads()
 
def Runthread(tid):
 """
 
 unsigned long  Runthread(unsigned long tid)
 
 """
 return Runsinglethread(tid)

def Flashmes(mess):
 """
 
 void  Flashmes(char *mess,...)
 
 """
 return Flash(mess)
 
def Uplist():
 """

 Updage log window
 
 """
 return Updatelist()
 
def msg(addr,msg):
 """
 
 Sets comment by address
 
 void  msg(unsigned long addr,char *msg,...);
 
 """
 return Message(addr,msg)
 
def Readcmd(addr,buf):
 """
 
 unsigned long  Readcmd(unsigned long addr,char *buf)
 
 """
 return Readcommand(addr,buf)
 
def AnalyseCurrentModule():
 """

 Analyse current module
 
 """
 return Analysecode(Findmodule(GetReg("eip")))
 
def Dumpreg(file_name,base,size):
 """
 Dumping memory region into file
 
 void Dumpreg(char * file_name,unsigned long base,unsigned long size)
 """
 return dump_mem(file_name,base,size)
 
def Findmem(addr):
 """
 t_memory* Findmem(unsigned long addr)
 """
 return Findmemory(addr)

def Disassm(addr,type):
 """
 Disassembly command
 
 t_disasm Disassm(unsigned long addr,int mode)
 
 """
 return Dis_asm(addr,type) 
 
def Getlng(title,size):
 """
 
 unsigned long Getlng(char * title,int datasize)
 
 """
 return Get_long(title,size)
 
def Findthrd(id):
 """
 
 t_thread* Findthrd(unsigned long id)
 
 """
 return Findthread(id)
 
def Getcputhrdid():
 """
 
 unsigned long  Getcputhrdid(void)
 
 """
 return Getcputhreadid()
 
def Decodethrdname(buf, tid,mode):
 """
 
 int    Decodethrdname(char *buf,unsigned long tid,int mode)
 
 """
 return Decodethreadname(buf,tid,mode)
 
def IsVmWare():
 """
 
 Detecting VmWare
 
 unsigned long IsVmWare(void)
 """
 return vmWare()
 
def Gettxt(title,text,letter,type,ind):
 """
 
 Gettxt(char *title,char *text, char letter,int type,int ind)
 
 """
 return Gettext(title,text,letter,type,ind)
 
def Disasmin(addr,str):
 """
 unsigned long Disasmin(unsigned long addr,char * str)
 
 Second parameter must be in UPPER case (for example "PUSH EBP","XOR EAX,EAX")
 """
 return find_instruction_in(addr,str)
 
def Disasmback(addr,str):
 """
 
 unsigned long Disasmback(unsigned long addr,char * str)
 
 Second parameter must be in UPPER case (for example "PUSH EBP","XOR EAX,EAX")
 """
 return find_instruction_back(addr,str)

def Disasminbyopcode(addr,str):
 """
 
 unsigned long Disasminbyopcode(unsigned long addr,char * str)
 
 Second parameter - opcode(for example "55","33C0")
 
 """
 return find_instruction_in_op(addr,str)
 
def Disasmbackbyopcode(addr,str):
 """
 
 unsigned long Disasmbackbyopcode(unsigned long addr,char * str)
 
 Second parameter - opcode(for example "55","33C0")
 
 """
 return find_instruction_back_op(addr,str)
 
def asmfindmodel(model):
 """
 
 void asmfindmodel(t_asmmodel model[])
 
 """
 return Get_asmfindmodel(model)
 
def Searchfileoffset(pmod,addr):
 """
 
 Converts address belonging to some module into offset in executable file. 
 
 Returns offset or 0 if offset cannot be calculated 
 (for example, address belongs to the gap between two sections)
 
 unsigned long Searchfileoffset(t_module *pmod,unsigned long addr)
 
 """
 return Findfileoffset(pmod,addr)
 
def Findallcmds(pd,model,origin,title):
 """

 Findallcmds(t_dump *pd,t_asmmodel *model,unsigned long origin,char *title)

 """
 return Findallcommands(pd,model,origin,title)
 
def GetMmx(title,data,mode):
 """
 
 Display dialog box allowing user to enter or edit 64-bit MMX number as a 
 combination of 8-, 16- or 32-bit integers in signed decimal, unsigned decimal or hexadecimal formats. 
 
 Return 0 on success and -1 on error or when user cancelled input. 
 
 GetMmx(char *title,char *data,int mode)
 
 """
 return Getmmx(title,data,mode)

def GetMmxXY(title,data,mode,x,y):
 """
 Display dialog box allowing user to enter or edit 64-bit MMX number as a 
 combination of 8-, 16- or 32-bit integers in signed decimal, unsigned decimal or hexadecimal formats. 
 
 Return 0 on success and -1 on error or when user cancelled input. 
 
 GetMmxXy(char *title,char *data,int mode,int x,int y)
 """
 return Getmmxxy(title,data,mode,x,y)

def GetHexString(title,hs,mode,fontindex,letter):
 """
 
 GetHexString(char *title,t_hexstr *hs,int mode,int fontindex,char letter)
 
 """
 return Gethexstring(title,hs,mode,fontindex,letter)

def GetHexStringXY(title,hs,mode,fontindex,letter,x,y):
 """
 
 GetHexStringXy(char *title,t_hexstr *hs,int mode,int fontindex,char letter,int x,int y)
 
 """
 return Gethexstringxy(title,hs,mode,fontindex,letter,x,y)
 
def Get3dNow(title,data,mode):
 """
 Display dialog box allowing user to enter or edit 64-bit 3DNow! number as a combination of 
 two floating-point or hexadecimal 32-bit numbers. 
 
 Return 0 on success and -1 on error or when user cancelled input.
 int Get3dNow(char *title,char *data,int mode)
 
 """
 return Get3dnow(title,data,mode)
 
def Get3dNowXY(title,data,mode,x,y):
 """
 Display dialog box allowing user to enter or edit 64-bit 3DNow! number as a combination of 
 two floating-point or hexadecimal 32-bit numbers. 
 
 Return 0 on success and -1 on error or when user cancelled input.
 
 Get3dNowXY(char *title,char *data,int mode,int x,int y)
 
 """
 return Get3dnowxy(title,data,mode,x,y)

def Setcomment(addr,comment):
 """
 Sets comment
 
 void Setcomment(unsigned long addr, char * comment)
 """
 return Addtolist(addr,0,comment)

def Findnxtproc(addr):
 """
 ulong Findnxtproc(ulong addr)
 
 Returns start address of the procedure that is next to addr, or 0 on error, for example,

 when module is not analyzed or address doesn't point to executable code.
 """
 return Findnextproc(addr)
 
def Findprocbgn(addr):
 """
 ulong Findprocbgn(ulong addr)
 
 Returns start address of the procedure that encloses addr, or 0 on error, for example, 
 
 when module is not analyzed or address points to no procedure.
 
 """
 return Findprocbegin(addr)

def Findprevprc(addr):
 """
 ulong Findprevprc(ulong addr)
 
 Returns start address of the procedure that precedes or encloses addr, or 0 on error, for example, 
 
 when module is not analyzed or address doesn't point to executable code.
 """
 return Findprevproc(addr)
 
def Findprcend(addr):
 """
 ulong Findprcend(ulong addr);
 
 Returns address of the last command of the procedure that encloses addr, or 0 on error, 
 
 for example, when module is not analyzed or address points to no procedure.
 
 """
 return Findprocend(addr)
 
def Getmodule(number):
 """
 Get loaded module info by index [see function Getnumberofmods] 
 
 mod_info Getall_modules(int n)
 """
 return Getall_modules(number)

def Getmemoryblock(number):
 """
 Return allocated memory block information by index
 
 mem_blocks Getall_blocks(int n)
 """
 return Getall_blocks(number)

def Getnumberofblocks():
 """
 Return  number of allocated memory blocks 
 
 int Getnumberofblocks(void)
 """
 return Getnumofblocks() 
 
def Getnumberofmods():
 """
 Return number of loaded modules
 
 int Getnumberofmods(void)
 """
 return Getnumofmods()
 
