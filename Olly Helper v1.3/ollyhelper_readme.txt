ollyhelper是一个ollydbg插件，它能完成一些辅助性的功能，
比如去除debug位，有了它其实就不用isdebug插件了。

最初我是看技术上isdebug插件用新建thread加sleep的方式来禁止debug位不爽，才想到要开发这个插件的。ollyhelper去除是在最必要的时候。

菜单说明：
set bit: 设置debug位，这样被调试程序能用isdebugpresent函数查到调试器
clear bit: 置debug位为0,被调试程序不能用isdebugpresent函数查到调试器
(注：现在对调试位的修改应该是支持xpsp2的)
options: 一些选项，比如是否在进程开始的时候自动清除调试位,以及要在哪些dll的dllmain里断下来。
alloc memory: 分配内存，如果要临时写一些代码或数据，可以不用找程序的空隙了，用allocmemory分配的内存吧，它肯定是属于你的！
insert dll: 插入dll到目标进程，可以自写dll配合自己的调试。
notes: 调用记事本打开一个固定文件，要写点什么可以写在里面。
detach: 在xp或win2k3上调用DebugActiveProcessStop函数停止调试，使被调试程序继续运行
delete udds: 删除udd文件。
about : 显示关于信息。
另外：选项里的break on dlls是很好用的功能，比如你想中断在foo.dll的入口处，你直接调试这个dll,因为ollydbg有bug，不一定能做到中断在入口处，但你在选项里加上foo.dll，就一定能中断在入口处，对于调试dll是很有帮助的。

除了alloc memory,insert dll,detach(?)，其它的功能都有别的插件可以实现，我为了自己用着方便，这些功能是慢慢写出来的，其实那些小功能的实现可能比找一个插件更容易，所以我这样做并不觉得有重复造轮子之嫌。也许大家认为用专门的插件更好一点，不过自认为alloc memory,insert dll和break on dlls的功能是很诱人的。

作者：goldenegg
2005-03-07,10:59
http://bbs.pediy.com/showthread.php?s=&threadid=11804