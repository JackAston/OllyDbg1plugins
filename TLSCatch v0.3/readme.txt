File Name: TLSCatch.dll
File Submitter: waliedassar
File Submitted: 31 Oct 2010
Topic Submitted: http://forum.tuts4you.com/index.php?showtopic=24255

This plugin simply intercepts any new module loaded into the current process address space ,searchs it for tlscallbacks and sets a one-shot breakpoint on every callback found. It lets the malware analyst catch any tls callback in ollydbg. Just copy the plugin dll into olly plugin directory then fire ollydbg. Tested on ollydbg v1 on windows xp and windows Vista.

http://waleedassar.blogspot.com/2010/10/quick-look-at-tls-callbacks.html