上次我们 Disable 了 OD 的 DebugPrivilege 权限, 可能会有问题.
正好想学学写 OD 的插件, 就有了这个东东, 只修改被调试进程的 DebugPrivilege 权限.

插件参考了 IsDebugPresent 的 ASM 代码, 用C 实现, 有三个命令.

1. 手动 Disable 子进程 DebugPrivlege
2. 手动 Enable  子进程 DebugPrivlege
3. 程序加载和 Restart 时可选择自动 Disable DebugPrivlege

程序在 VC6 下编译通过, 有一个问题花了我好长时间才解决,
OD 手册中提到的 DllEntryPoint() 编译能通过, 但就是不执行.
看了看 OD 自带的 VC 例子 和其他的例子, 都是 DllEntryPoint().

后来我改成 DllMain() 才正确, 这个是不是 OD Pulgin 的一个小问题?





Because OD enable DebugPrivilege itself, 
so debugee created by OD also has DebugPrivilege.
Some packer will detect OD by checking debugee's DebugPrivilege.

This plugin can control debugee's DebugPrivilege.

1. Disable
    
   Manual disable debugee's DebugPrivilege.

2. Enable
   
   Manual enable debugee's DebugPrivilege.

3. Option
   You can set auto disable debugee's DebugPrivilege when open or restart.

   Thank 2oo3SV for your IsDebugPresent src code.