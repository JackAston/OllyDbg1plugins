		      __..::ди OllyGHOST+ ид::..__
			ии░"^\  SynApsus  /^"░ии

This little tool has 2 buttons.

1.	The first, "Hide", hooks th IsDebuggerPresent API and makes it unuseful
against debuggers. The Armadillo software protection system is owned by
this trick ! After having hidden your debuggers, you can restore the 
first verion of the API by re-clicking the button, which caption had 
changed to "Un-Hide".

2.	The second button enables you to activate breakpoints on Windows APIs in
OllyDbg under 9x systems, thing which was impossible. Caution, it makes your
Kernel32 in memory WRITEABLE, so a simple line of code can kill your most 
basic Windows functions, until next reboot. 
	After having set breakpoints with OllyDBG, if you are not sure your Kernel
is clean, you can fix all the APIs's first byte by clicking "Fix". After that a 
messagebox appears, asking you if you want to COMPLETELY clean you Kernel. 
 If you answer YES, you will be able to execute ALL applications, the 
no-imports ones too. 
 If you answer NO, you will be able to re-fix your Kernel as you want, when 
you want, until you click YES.

	All these tricks work, even if you close OllyGHOST. It detects if you have 
the bps enabled or IsDebuggerPresent hooked, and inits itself, following the 
different cases.

Hope it will be useful for you,    

			
			:д SynApsus д:  [N-GeN]
			Contact : EFNet, #n-gen
			
	Greetz : Gbillou, y0da, [SkY], RaX|, Oleh Yuschuk, and my little pixador :)
	
	
	-
Closing Link: SynApsus by Amsterdam2.NL.EU.undernet.org (Ping timeout)
-