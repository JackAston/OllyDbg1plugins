
import os
import shutil
import subprocess

def main():
	OLLY_PATH='D:\\...\\odbg110'
	dllDir='bin\\Release'
	#find .dll file in bin\Release
	dlls=[]
	for tmp in os.listdir(dllDir):
		if tmp.endswith('.dll'):
			dlls.append(tmp)
	if len(dlls)!=1:
		print('Error: there should be one and only one dll present in "'+dllDir+'"')
		return
	shutil.copy2(os.path.join(dllDir,dlls[0]),os.path.join(OLLY_PATH,'plugins'))
	#use olly to debug itself
	subprocess.call([os.path.join(OLLY_PATH,'OLLYDBG.EXE'),os.path.join(OLLY_PATH,'OLLYDBG.EXE')])

if __name__=='__main__':
	main()
