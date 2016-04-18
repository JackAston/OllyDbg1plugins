import sys, re
from datetime import datetime

		
sys.stderr = file('ollypython_error.log', 'a')

import ollypython

def write_clean(str, hilight):
	if "\r\n" in str:
		lines = str.split('\r\n')
	else:
		lines = str.split('\n')
		
	if len(lines) > 1:
		lines = lines[:-1]
		
	for line in lines:
		ollypython.Addtolist(0, hilight, line)

class OStdout:
	def write(self, str):
		write_clean(str, 0)
		
class OStderr:
	def write(self, str):
		write_clean(str, 1)
		
class OStdin:
	def read(self):
		ollypython.Addtolist(0, 0, "Stdin unsupported")
		return None
		
	def readline(self):
		ollypython.Addtolist(0, 0, "Stdin unsupported")
		return None
		
sys.stdout = OStdout()
sys.stderr = OStderr()
sys.stdin = OStdin()

print "OllyPython initialized"