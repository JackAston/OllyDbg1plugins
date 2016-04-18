/*****************************************************************************/ 
/*****************************************************************************/ 
/*****************************************************************************/ 
 LCB plugin                                                            
 Description: Ollydbg plugin to export and import labels, comments and 
              breakpoints.                                             
                                                                       
 Authors: scherzo                                                      
(c) 2006                                                              
/*****************************************************************************/ 
/*****************************************************************************/ 
/*****************************************************************************/ 



This plugin is based on the Labelmaster plugin by Joe Stewart. These days, I was
analysing an application and I wanted to save my comments and labels of a DLL in
a file. But I couldn't do that because Joe's plugin use physical address to save 
comments and labels and each time the DLL was loaded in a different address.


So, I started to write this plugin and I noticed that there is another "bug" in
Labelmaster: if, for example, you have a label in the data section, it can't be
exported because Labelmaster uses size variable of the t_dump struct.


And while I was writing the plugin, I thought: why don't I write a breakpoint
export import function? If so, I won't use anymore Labelmaster for labels and 
comments and Breakpoint Manager plugin for breakpoints, but I'll use only one 
plugin.


So, here you are: LCB plugin. Use it if you find it useful. Any report of bugs 
and suggestions are welcome. Source is avaiable too.


scherzo