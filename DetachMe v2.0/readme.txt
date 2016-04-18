Author: Waliedassar
Contact e-mail address: waliedassar@gmail.com
Contact website: http://waleedassar.blogspot.com

One of the new interesting features of OllyDbg v2.0 is the "Detach" functionality, which enables you to detach debuggees from OllyDbg at anytime and let them run freely outside control of OllyDbg.

Unfortunately, OllyDbg v1.10, the widely used version, lacks this features. Pedram Amini has created a nice plugin to fill this gap, but it does not satisfy my needs, though.

Pedram's plugin only works on debuggees in the running mode. It does not work on debuggees in the suspended mode. In addition, debugees will crash if software breakpoints are left.

This pushed me to create a new OllyDbg v1.10 plugin, in which i tried to create a similar functionality to the one in version 2.0.

Features introduced in my plugin:

 1) Disabling user's software and hardware breakpoints without affecting the corresponding .udd files.
 2) Detaching debuggees in the suspended mode.