This plugin is BETA - so don't expect too much.

Features:
-Auto-Hook on compileMethod()
-Shows the IL code of each method
-Break on jitted method (native code)
-All dumped methods (e.g. once shown in dump window) remain in BP window
-Manually dump IL code (In context menu of dump windows > Dump selection)

How to:
-Simply run a .net app in Olly and choose "Run ILLY"
-Run/Stop also via shortcut: CTRL + SHIFT + I
-Olly will remember, if to run the plugin from startup or not
-"SKIP" means closing the dump window and waiting for the next method
-"NEXT" means leaving the dump window open and waiting for the next method

Works on:
-All tested jithooking protectors: DNGuard, CliSecure, dotNet Protector,...
-Except for MaxToCode.. coz of the guarding DLL being protected with Themida :--/
ILLY works only after attaching then - not from startup !

ToDo:
-Resolve tokens
-Full method dump: Add method header and additional method sections (exception handlers) to dump