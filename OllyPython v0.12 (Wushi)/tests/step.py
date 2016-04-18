from ollyhelper import *

#WM_KEYDOWN = 0x0100
#VK_F7 = 0x76
#Addtolist(0,-1,"0")
#Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 0, VK_F7) 
#Addtolist(0,-1,"1")
#Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 0, VK_F7) 
#Addtolist(0,-1,"2")
#Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 0, VK_F7) 
#Addtolist(0,-1,"3")
#Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 0, VK_F7) 
def sstepcallback():
		Go(Getcputhreadid(), 0, STEP_IN, 0, 0)
		Addtolist(0,-1,"sstepcallback()")  
		return
if __name__ == "__main__" :		
		Addtolist(0,-1,"0")
		Go(Getcputhreadid(), 0, STEP_IN, 0, 0)


