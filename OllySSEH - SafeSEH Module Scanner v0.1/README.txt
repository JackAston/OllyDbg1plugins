######################################################################
Software     : OllySSEH /SafeSEH Module Scanner
Date         : 21 May 2007
Description  : OllyDbg /SafeSEH in-memory module scanner
Author       : Mario Ballano (mballano~gmail.com)
Web          : http://www.48bits.com/
######################################################################

  This plugin does an in-memory scanning of process loaded modules
  checking if they were compiled with /SafeSEH, if so it can list 
  the registered handlers (you can follow them at CPU window doing 
  double click ). 
  
  This plugin is useful when exploiting vulnerabilities in 
  systems with Software DEP protection. 
  
  The plugin lists all loaded modules and can give you the 
  following results for each one:
  
  /SafeSEH Off : No /SafeSEH, jump party ;-)
  
  /SafeSEH ON  : /SafeSEH active, you can list registered handles
                 using rigth click. 
                 
  No SEH       : SEH is not active for this module, take a look
                 at PE/COFF specs (IMAGE_DLLCHARACTERISTICS_ NO_SEH)
                 
  Error        : There was an error reading module structure :-(

######################################################################

References : 

http://msdn2.microsoft.com/en-us/library/9a89h429(VS.80).aspx
http://www.microsoft.com/whdc/system/platform/firmware/PECOFF.mspx

Greets : 

Ruben Santamarta, from http://www.reversemode.com/, for helping me
with an anoying unicode/mdi bug in my Visual Studio project! :-)


Cheers, 

Mario Ballano


