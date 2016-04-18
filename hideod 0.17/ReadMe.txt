v0.17
Clear HeapMagic
可以躲过Themida 1.8x的Anti.
code by heXer

v0.161
修正一个小bug

v0.16
稍改进一下代码，以避免一些可能出现的问题。

v0.15
1.修正附加进程时，插件会导致OD出现“无法写入到/读取被调试进程(xxxxxxxx..xxxxxxxx)的内存”的错误。（感谢vifun反馈这个bug）
2.为Alloc memory功能增加快捷键Alt+Q（多次按可以快速切换到所分配的空间处）。

感谢CoDe_Inject在修复第1个bug所给予的帮助！另外，也特别感谢goldenegg所给的帮助（虽然那方案这次没采用）。
2006.7.19

v0.13
增加分配临时空间的功能(Alloc memory)，主要是脱壳时方便键入代码。
这功能完全抄自goldenegg的ollyhelper插件。;)
2006.1.14


v0.12
修正ZwSetInformationThread。
2005.12.9


v0.11
ZwSetInformationThread这部分遗漏，ZwSetInformationProcess方法2在Win2000下有问题。己修正。
2005.12.6

v0.1
现在隐藏OllyDBG插件比较多，整合的loveboom写过一个，并放出源码。
编写这个插件主要目的是熟悉一下OllyDBG插件编程。本插件中的相关
源代码来自heXer,shoooo,loveboom,simonzh2000等各位的程序，谢谢！
我只不过整合了一下。另外，特别感谢shoooo，goldenegg在编写插件
方面给予的帮助。



目前该插件可以隐藏如下Anti-Debug检测：
IsDebuggerpresent,NtGlobalFlags,HeapFlags,ForceFlags,SetDebugPrivilege,OutDebugStringA,
CheckRemoteDebuggerPresent,ZwSetInformationThread,UnhandledExceptionFilter,Process32Next,
ZwQueryInformationProcess。


--------------------------------------------------------------------------------------
a website on reverse engineering

kanxue
http://www.pediy.com
http://bbs.pediy.com