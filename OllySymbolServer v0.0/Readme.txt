OllySymbolSever Plugin v0.0.0.0 (pre-beta)
by suburban_user


INSTALLATION:
-copy "symsrv.dll" and "dbghelp.dll"  from the 'redist' folder to your OllyDBG's main folder (these are unmodified Microsoft files taken from WinDBG).
-copy the plugin DLL, "OllySymbolServer.dll" to your OllyDBG 'Plugin' folder.


USAGE:
-start olly and check the log and/or the plugin menu to make sure that the OllySymbolServer plugin was loaded correctly.
-when you open a file the plugin will create a folder named "SYMBOLS" in your OllyDBG main folder, this is where your symbols will now automatically be downloaded to (yes, this folders name and location are hard-coded for this beta version).
-you should be prompted (only the very first time) to accept Microsoft symbol server terms.
-available symbols will be downloaded as they are loaded, olly will appear to freeze while symbols are downloading.. this may take some time.  Any symbol usage in olly will give a considerable performance drain, this performance drain is NOT caused by my plugin.
-AFTER DOWNLOADING SYMBOLS - YOU MUST ANALYZE EACH MODULE AND RELOAD OLLY BEFORE THESE SYMBOLS WILL BE USED CORRECTLY.


FEATURES:
-enable Microsoft symbol server download compatablilty in OllyDbg v1.10 (with no fussing around).


TECHNICAL:
-this plugin DLL was written in Visual C++ 6.0 with the modified Olly PDK (v1.10).
-this plugins is still in pre-beta.. (use at your own risk, error checking has yet to be implemented).


CHANGELOG:
-v0.0.0.0 - Initial version, proof of concept.


SPECIAL THANKS TO:
-gorgeous women.
-google.
-grandma.
