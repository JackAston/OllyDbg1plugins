Processes with manipulated PEB.LoaderData don't show in the OllyDbg "Select process to attach" dialogue box.

The plugin first checks for the integrity of the target process's _PEB_LDR_DATA structure. If a manipulated structure is detected, the plugin tries to create a new typical one.

http://code.google.com/p/ollytlscatch/

Waliedassar