Sometimes (especially when dealing with packers) you may need to run
OllyDbg's code analysis function, only to find it's not available to
you because the EIP is currently outside the code segment as defined
by the PE header. AnalyzeThis! is an OllyDbg plugin to allow OllyDbg's 
analysis function to operate outside of the marked code segment, by
telling OllyDbg the current segment *is* the code segment. 

Caveats: If the EIP is outside the range of a known executable module,
AnalyzeThis! will not work. Also, OllyDbg can only store one analysis
table, so if you analyze a new segment, it will remove any existing
analysis that has been done.

Source code has not been included; not because I don't want to release
it at this time, but because I can't find it offhand. If you really need 
it, email me and I'll look harder for it.

Joe Stewart
joe@joestewart.org

