from ollypy import *

def Run():
    """
    Run,:)
    as press F9 in ollydbg.
    """
    threadid = Getcputhreadid()
    if threadid != None :
            Animate(ANIMATE_OFF)
            Go(threadid, 0, STEP_RUN, 0, 0)
            


def bpcallback():
		Addtolist(0,-1,"bpcallback found")  
		return
def expcallback():
		Addtolist(0,-1,"expcallback found")  
		return		
if __name__ == "__main__" :
		address = 0x4166d5
		Sethardwarebreakpoint(address,4,HB_CODE)
		Run()
