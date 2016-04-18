from ollypy import *

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
"""
all OK, you can remove comment to test it.

"""   

if __name__ == "__main__" : 
		#Cond = ("EIP == 416736",0,0,0,0,0)
		#TraceCIn(Cond)
		Cond = ("EIP == 416787",0,0,0,0,0)
		TraceCOver(Cond)
		#TraceNOver(3)
		#TraceNIn(3)
		
		
		
		Creatertracewindow()
		#Scrollruntracewindow(2)
		#Createprofilewindow(0x416736,10)
		
		#StopTrace()
		
		size = Runtracesize()
		Addtolist(0,-1,"Runtracesize = %d"%(size))
		  
		size = Findprevruntraceip(0x416787,0)
		Addtolist(0,-1,"Findprevruntraceip = %d"%(size))
		
		size = Findnextruntraceip(0x416787,0)
		Addtolist(0,-1,"Findnextruntraceip = %d"%(size))
		
		"""
		the next Getruntraceregisters will cause ollydbg fault when quit the olyydbg.
		I don't know why.
		why??????????
		
		the stack show ollypy.free , maybe double free?
		maybe cmd = null or comment = null , ollypy free a null point.
		when I have time, I will fix this.
		
		oldreg = t_reg()
		newreg = t_reg()
		size,cmd,comment =  Getruntraceregisters(0,newreg,oldreg)
		Addtolist(0,-1,"size = %d , cmd = %s , comment = %s"%(size,cmd,comment))
		
		"""
		
		StopTrace()
		
		"""
		the follow funcs don't use
		
		char *Findhittrace(ulong addr,char **ptracecopy,ulong *psize);
		int Modifyhittrace(ulong addr0,ulong addr1,int mode);
		
		"""