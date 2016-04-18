HiddenThreads by Waliedassar
waliedassar@gmail.com

http://waleedassar.blogspot.com/

You can use this tiny plugin which detect threads hidden when attaching to a target process.

Threads are usually hidden by calling the "ZwSetInformationThread" function with the "ThreadInformationClass" parameter set to ThreadHideFromDebugger, 0x11.

Tested on Windows 7.