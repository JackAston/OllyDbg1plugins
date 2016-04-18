About OllySafe:


In fact, the virus analysis tools OllySafe MiniSafe is the professional version is a virus analysts specifically for the OllyDbg plug-ins, it can operate on the virus files, registry, the process of interception to simplify the analysis of the virus more difficult to assist the work of a good virus .


Installation:


After the installation package extract olly_hardware_breakpoint.dll, MiniSafe.exe, OllySafe.sys three documents copied to the Plugin OllyDbg directory can complete the installation, to not more than 32 plug-in the ceiling, preferably in the installation directory OllyDbg Under a copy.


Usage:


Shell and instructions to spend more than icesword example.

OllyDbg icesword with open, we can see that the OllySafe run automatically.

Minimize OllySafe window.

Operation icesword by pressing F9.

First of all we can see some icesword loaded DLL file, regardless first, the click is loaded.

Icesword can then be intercepted on the registry's operations, must write \ SystemRoot \ System32 \ Drivers \ IsDrv120.sys, look at the first interception, click suspended.

And click on the revision is let the registry. Generally procedures will be parked in Ntdll.dll modules, use ALT + F9 return to the user code.

However, as is their direct call Icesword the Native Api, omit this step:)

Icesword operation can be seen on the relevant registry code. F9 continue running

IceSword_unpacked.ex trying to drive the process of loading \ Registry \ Machine \ System \ CurrentControlSet \ Services \ IsDrv120

The bar is driven to a loading operation.

Click suspended and returned to the user code, whether this point.

Read from top to bottom, can be found IceSword unsuccessful the first time after loading drive will be used random file name loading drive.

Well, how to use temporary first mentioned here.


Pay attention to matters:


1, in order to safety, OllySafe running prohibit the creation of any process. This is to prevent the virus from OllySafe monitoring Winexec create malicious programs, so you want to run in the debugging process OllySafe please start running before.