OllyTiper is a plugin for OllyDbg v1.10 to make it easier to work with keyboard. 
First release by Ryokou with src, with thanks to KanXue and HeXer.
Update by DhtFish[CCG].
Totally remade and BUGFIXed by vptrlx.

Version 2.0:

Changes:
1)Settings menu allow you to change operation keys or disable them.
	/*Code Information and Disassembly shortcuts may be the same;
	DblClick in stack cannot be disabled or changed*/
2)Enhanced Usability
3)Enhanced Operation
4)BIG BUG fixes


Operation:
1. Disassembly Window 
(1) View data 
In everything here "Ctrl+D" instead of "SHIFT" makes results to be shown in Stack Window.
 
push A480033  / / If you press Shift, then the data window will display the data A480033
mov eax, 401000 / / this line by Shift, the data window to display data on 401,000 
mov eax, [401000] / / this line by Shift, the data window to display data on 401,000 
mov [ebp-4], esp / / this line press Shift, then the data window to display the value of ebp-4 (note the EIP must point to the current line) 
mov eax, [esp +10] / / this line by Shift, the data window to display the value of esp +10 (Note that the EIP must point to the current line) 
JNZ 401000 / / this line by Shift, the data window to display data on 401,000 
 
(2) copies the current address 
00401092 68 00000080 PUSH 80000000 / / select this going-rate, press Ctrl + Q, will address "00,401,092" copy to the clipboard. 
 
(3) Calculate the size of code: select some piece and push and release Ctrl.
--------------------------------------------- 
2. Data Window 
Suppose such data in the window:
00406000 01 02 10 40 00 00 00 00 00 00 00 00 00 CA 2E 40
and the cursor set to "02 10 40 00"
Then
(1)Ctrl: Disassembly window goes to 00401002
(2)Shift: Data window goes to 00401002
(3)Ctrl+D: Stack window goes to 00401002
(4)Ctrl+Q: Moves 00401002 to Clipboard
 
--------------------------------------------- 
3. Stack Window 
0012FF44 00401D8A / / 
(1)Ctrl or double-click, disassemble the window displays the contents of the address 0401D8A;
(2)Shift, Data window displays the contents of the address 0401D8A 
(3)Ctrl+D: Stack window goes to 0401D8A
(4)Ctrl+Q: Moves 0401D8A to Clipboard
 
---------------------------------------------
4. Registers Window:
 
EAX 00000000
ECX 0012FFB0
EDX 7C90E4F4 ntdll.KiFastSystemCallRet
EBX 7FFDC000
ESP 0012FFC4
EBP 0012FFF0
ESI FFFFFFFF
EDI 7C910208 ntdll.7C910208
 
For example, select EDX. Then
(1)Ctrl: Disassembly displays 7C90E4F4
(2)Shift: Data displays 7C90E4F4
(3)Ctrl+D: Stack displays 7C90E4F4
(4)Ctrl+Q: Copies 7C90E4F4 to the Clipboard
/*works only with eax..edi*/ 
--------------------------------------------
5. A funny "feature" that apeeared by chance:
If some operation is invoked with use only of Ctrl/Shift/Alt then
you don't need the window that you want to capture address/value from to be focused -
just push ctrl/shift button and before releasing it move mouse into the window you want.


Known Issues:
0) Key shortcuts with the only Alt system key may not work
