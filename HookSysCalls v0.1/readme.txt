HookSysCalls by Waliedassar
waliedassar@gmail.com

http://waleedassar.blogspot.com/

You can use this OllyDbg v1.10 plugin to hook calls to:
1) ZwQueryInformationProcess with the "ProcessInformationClass" parameter set to ProcessDebugPort (0x7).
2) ZwQueryInformationProcess with the "ProcessInformationClass" parameter set to ProcessDebugObjectHandle (0x1E).
3) ZwQueryInformationProcess with the "ProcessInformationClass" parameter set to ProcessDebugFlags (0x1F).
4) ZwSetInformationThread with the "ThreadInformationClass" parameter set to ThreadHideFromDebugger (0x11).

Target: OllyDbg v1.10 on Windows 7 SP0/SP1 64-bit and Windows XP SP2/SP3.