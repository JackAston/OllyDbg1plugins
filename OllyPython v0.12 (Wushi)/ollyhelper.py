"""
    ollypython for ollydbg
    wrote by wooshi@gmail.com
    version: 0.1
    2007.1.2
"""
from ollypy import *

def StepIn():
    """
    As Press F7 in ollydbg.
    """
    threadid = Getcputhreadid()
    
    if threadid != None :
        Animate(ANIMATE_OFF)
        Go(threadid, 0, STEP_IN, 0, 0)
        
def StepOver():
    """
    As Press F8 in ollydbg.
    """
    threadid = Getcputhreadid()
    
    if threadid != None :
        Animate(ANIMATE_OFF)
        Go(threadid, 0, STEP_OVER, 0, 0)

def Run():
    """
    Run,:)
    as press F9 in ollydbg.
    """
    threadid = Getcputhreadid()
    if threadid != None :
            Animate(ANIMATE_OFF)
            Go(threadid, 0, STEP_RUN, 0, 0)
        
def RunTo(tilladdr):
    """
    Run to the tilladdr .
    """
    threadid = Getcputhreadid()
    if threadid != None :
            Animate(ANIMATE_OFF)
            Go(threadid, tilladdr, STEP_SKIP, 0, 0)
                
def RunRet():
    """
    As press Ctrl+F9 in ollydbg.
    """
    
    threadid = Getcputhreadid()
    if threadid != None :
            Animate(ANIMATE_RET)
            Go(threadid, 0, STEP_RUN, 0, 0)

def AnIn():
    """
    As press Ctrl+F7 in ollydbg.
    """
    threadid = Getcputhreadid()
    if threadid != None :
        Animate(ANIMATE_IN)
        Go(threadid, 0, STEP_RUN, 0, 0)
        
def AnOver():
    """
    As press Ctrl+F8 in ollydbg.
    """
    threadid = Getcputhreadid()
    if threadid != None :
        Animate(ANIMATE_OVER)
        Go(threadid, 0, STEP_RUN, 0, 0)

def TraceCIn(Cond):
    """
    You can set the trace cond here.
    cond must be like this:
    cond = [cnd, onsus, in0, in1, out0, out1]
    the work like press Ctrl+F11 in ollydbg.
    """
    threadid = Getcputhreadid()
    if threadid != None :
        Animate(ANIMATE_TRIN)
        
        cnd, onsus, in0, in1, out0, out1 = Cond
        Settracecondition(cnd, onsus, in0, in1, out0, out1)
        reg = t_reg()
        Startruntrace(reg)
        Go(threadid, 0, STEP_RUN, 0, 0)
                        
def TraceCOver(Cond):
    """
    You can set the trace cond here.
    cond must be like this:
    cond = [cnd, onsus, in0, in1, out0, out1]
    the work like press Ctrl+F11 in ollydbg.
    """
    threadid = Getcputhreadid()
    if threadid != None :
        Animate(ANIMATE_TROVER)
        cnd, onsus, in0, in1, out0, out1 = Cond
        Settracecondition(cnd, onsus, in0, in1, out0, out1)
        reg = t_reg()
        Startruntrace(reg)
        Go(threadid, 0, STEP_RUN, 0, 0)
        
def TraceNIn(num):
    """
    Trace In function.
    will stop after trace num asm codes.
    num is the length of the trace in codes.
    """
    threadid = Getcputhreadid()
    if threadid != None :
        Animate(ANIMATE_TRIN)
        Settracecount(num)
        reg = t_reg()
        Startruntrace(reg)
        Go(threadid, 0, STEP_RUN, 0, 0)

def TraceNOver(num):
    """
    Trace Over function.
    will stop after trace num asm codes.
    num is the length of the trace over codes.
    """
    threadid = Getcputhreadid()
    if threadid != None :
        Animate(ANIMATE_TROVER)
        Settracecount(num)
        reg = t_reg()
        Startruntrace(reg)
        Go(threadid, 0, STEP_RUN, 0, 0)
        
def StopTrace():
    """
    StopTrace
    """
    Deleteruntrace()
        
def AddTempBP(addr):
    """
    add the temp oneshot breakpoint.
    the param is the address need to add the BP.
    """
    Tempbreakpoint(addr, TY_ONESHOT|TY_KEEPCOND)
        
def AddTempTRBP(addr):
    """
    add a temp oneshot trace breakpoint.
    the param is the address need to add the BP.
    
    """
    Tempbreakpoint(addr, TY_ONESHOT|TY_KEEPCOND|TY_STOPAN)
    
def AnalyzeCode(addr):
    """
    as press Ctrl+A @addr
    
    """
    return Analysecode(Findmodule(addr))
        
def GetCurrentThreadEnv():
    """
    return the t_thread struct
    """
    t = t_thread()
    t = Findthread(Getcputhreadid())
    return  t
                                   
def SetReg(regname, value):
    """
    change the reg's current value.
    you can add the others reg as the below.
    I'm lazy,:)
    """
    
    threadid = Getcputhreadid()
    
    if threadid != None :
            thread = Findthread(threadid)
            m = thread.reg.r
            n =  thread.reg.s
            if regname.lower() == "eax":
                 set_ulongarray_value(m, 0, value)
            elif regname.lower() == "ecx":
                    set_ulongarray_value(m, 1, value)
            elif regname.lower() == "edx":
                    set_ulongarray_value(m, 2, value)
            elif regname.lower() == "ebx":
                    set_ulongarray_value(m, 3, value)
            elif regname.lower() == "esp":
                    set_ulongarray_value(m, 4, value)
            elif regname.lower() == "ebp":
                    set_ulongarray_value(m, 5, value)
            elif regname.lower() == "esi":
                    set_ulongarray_value(m, 6, value)
            elif regname.lower() == "edi":
                    set_ulongarray_value(m, 7, value)
           elif regname.lower() == "eip":
                    thread.reg.ip = value
            elif regname.lower() == "efl":
                    thread.reg.flags = value
                
def GetReg(regname):
    """
    the param is the reg's name
    return the reg's current value.
    you can add the others reg as the below.
    I'm lazy,:)
    """
    threadid = Getcputhreadid()
    d = 0
    if threadid != None :
            thread = Findthread(threadid)
            m = thread.reg.r
            n =  thread.reg.s
            
            if regname.lower() == "eax":
                 d = get_ulongarray_value(m, 0)
            elif regname.lower() == "ecx":
                 d    = get_ulongarray_value(m, 1)
            elif regname.lower() == "edx":
                d =     get_ulongarray_value(m, 2)
            elif regname.lower() == "ebx":
                d =     get_ulongarray_value(m, 3)
            elif regname.lower() == "esp":
                d = get_ulongarray_value(m, 4)
            elif regname.lower() == "ebp":
                d =    get_ulongarray_value(m, 5)
            elif regname.lower() == "esi":
                d =    get_ulongarray_value(m, 6)
            elif regname.lower() == "edi":
                d = get_ulongarray_value(m, 7)
            elif regname.lower() == "es":
                d =    get_ulongarray_value(n, 0)
            elif regname.lower() == "cs":
                d =    get_ulongarray_value(n, 1)
            elif regname.lower() == "ss":
                d =    get_ulongarray_value(n, 2)
            elif regname.lower() == "ds":
                d =    get_ulongarray_value(n, 3)
            elif regname.lower() == "fs":
                d =    get_ulongarray_value(n, 4)
            elif regname.lower() == "gs":
                d =    get_ulongarray_value(n, 5)    
            elif regname.lower() == "eip":
                d =    thread.reg.ip
            elif regname.lower() == "efl":
                d =    thread.reg.flags
                            
    return d
        

def GetAsm(addr):
    """
    addr is the address need disasm 
    return the asm code size and code
    """
        
    size, code = Readcommand(addr)
    dm = t_disasm()
    threadid = Getcputhreadid()
    size, data = Disasm(code, size , addr, dm, DISASM_CODE, threadid)
    return size, dm.result

def SetAsm(addr,asmcode):
    """
    addr is the address need to change 
    return the modified asm's len and errcode
    """
    m = t_asmmodel()
    leng, err= Assemble(asmcode, addr, m, 0, 0)
    Writememory(m.code, addr, leng, MM_RESTORE)
    return leng, err

def Readmem(addr, lens):
    """
    Read the memory from addr,
    the mem's lenth is lens.
    return the value list.
    """
    List = []
    len, text = Readmemory(addr, lens, MM_RESTORE)
    if len > 0 :
        for i in range(0, len):
            List.append(ord(get_chararray_value(text, i)))
                        
    return List

def Writemem(addr, text):
    """
    write the text to the memory from addr.
    """
    if len(text) > 0 :
       lens =  Writememory(text, addr, len(text), MM_RESTORE)
       if lens == len(text):
           
           return 1
       else:
           return 0
    else:
        return 0