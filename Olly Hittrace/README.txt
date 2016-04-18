
HitTrace - david zimmer <dzzie@yahoo.com>

This is a simple plugin based on my modulebpx code.

You set breakpoints in the UI and it will then run
the app automating it and logging which ones were hit.

To set breakpoints in the main module use "main"
(no quotes) as mod name

For dlls you can enter partial strings as long
as they are unique and as long as they are the 
dlls actual name as found in its export table.

Addresses of breakpoints are set in rva's (in case
a dll gets rebased)

The LogExp is optional..it accepts any type of
expression the ollydbg expression window takes
such as [ebp+4] or eax or whatever. If this is 
set and is valid then it will shoot the results to the 
log window on breakpoint. Sorry only supports one
expression to evaluate per bpx right now.

Each breakpoint is assigned an index which is visible
in the listbox. You can use one of these indexes
in the abort box to have it bail on tracing when
that bp is reached.

For example you can run it on looper.exe with the 
following settings

main	1030	[esp+8]
main	1070	[esp+8]
main	1136	

abort on index: 2

Then hit View results to see hit count and log window

or load the test.htl sample provided (abort on index is not saved to htl files)

if you recompile looper offsets may change disasm to find new ones.