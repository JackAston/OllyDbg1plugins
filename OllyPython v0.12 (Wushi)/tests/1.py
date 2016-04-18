from ollypy import *
if __name__ == "__main__" :
		xx = "test"
		yy = 0x1234
		zz = 0.1234
		
		Addtolist(0,-1,"xx=%s,yy = %d,zz=%f"%(xx,yy,zz))
		
		Updatelist()
		Createlistwindow()
		Error("xx=%s"%(xx))
		
		Message(0x7c80a644,"msg = %s"%(xx))
		Infoline("display bottom = %s"%(xx))
		Progress(800,"progress = %s"%(xx));
		Flash("flash=%d"%(yy))
