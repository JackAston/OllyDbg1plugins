1. copy ollypy.py,ollyhelper.py to your python2.4's lib directory.
2. copy _ollypy.dll to your python2.4's DLLs directory
3. copy ollypython.dll,ollypy.dll to your ollydbg's plugin directory , when ollydbg haven't a plugin dir, put to ollydbg dir.
4. use tests/*.py and tests/crash.exe(being debugged ) to test the plugin . 
5. see tests/*.py and ollydbg plugin api help v1.10 to get the funcs using method.