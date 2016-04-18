from ollypython import *
import time

def test():

	print "OllyPython example"

	for i in range(0, 1001, 50):
		Progress(i, "Doing something important")
		time.sleep(0.001)
	Progress(0, "")

	(status, path) = Browsefilename("Select some executable", "c:\\winnt\\system32\\winmine.exe", "*.exe", 0)

	if status != 1:
		return False
	
	print "Opening file %s" % path
	if OpenEXEfile(path, 0) != 0:
		return False
	
test()
print "Done"