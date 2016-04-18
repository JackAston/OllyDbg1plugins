;Small plugin to fix OllyDBG's handling of EXCEPTION_PAGE_GUARD
;By MOID/TSRh
;Assemble with NASM
;
;Like any useful plugin, it uses undocumented stuff :P
;It patches olly 1.10 so it can hook exception handling and breakpoint setting
;Probably won't work on any other version

;This program is free software; you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation; either version 2 of the License, or
;(at your option) any later version.
;
;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.
;
;You should have received a copy of the GNU General Public License
;along with this program; if not, write to the Free Software
;Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

%macro callW 1		;easy extern declaration
extern %1
call %1
%endmacro

%macro dll_export 1
GLOBAL %1
EXPORT %1
%endmacro

%DEFINE DEBUG 0

[SECTION MOIDTSRh USE32 CODE DATA]
..start:
xor eax, eax
inc eax
ret 3*4

dll_export _ODBG_Plugindata
_ODBG_Plugindata:
mov edx, [esp+1*4]
xor ecx, ecx

.copyloop:
mov al, [name+ecx]
mov [edx+ecx], al
inc ecx
test al, al
jnz .copyloop

xor eax, eax
mov al, 110
ret

dll_export _ODBG_Plugininit
_ODBG_Plugininit:
%if DEBUG == 1
int3
%endif

mov ecx, 256*3               ;initialize the buffer
mov edx, membps.startoflist
xor eax, eax

.clearloop:
mov [edx], eax
loop .clearloop

cmp dword [esp+1*4], 110     ;Is OllyDBG version 1.10 ?
jz .is_good_version

push 0x4 | 0x100 | 0x30         ;MB_YESNO or MB_DEFBUTTON2 or MB_ICONEXCLAMATION
push byte 0
push wrong_version
push byte 0
callW MessageBoxA

sub eax, byte 6
jz .is_good_version

xor eax, eax
dec eax
ret

.is_good_version:
push byte 0
callW GetModuleHandleA
push _Guardmemory
push eax
callW GetProcAddress

;now we're gonna inline it
cmp dword [eax], 0x83EC8B55
jnz .failure

cmp word [eax+4], 0xE0C4
jnz .failure

mov [replacement1.addy], eax
add dword [replacement1.addy], 6

push edi
xchg eax, edi

push oldprotect
push byte 0x40      ;PAGE_EXECUTE_READWRITE
push byte 6         ;size
push edi
callW VirtualProtect

mov al, 0x68
stosb
mov eax, replacement1
stosd
mov al, 0xC3
stosb

push oldprotect
push dword [oldprotect]
push byte 6         ;size
push edi
callW VirtualProtect

;Second patch
push byte 0
callW GetModuleHandleA
mov edx, eax

add eax, [eax+0x3C]
mov ecx, [eax+0x50]

.searchloop:
cmp dword [edx], 0x0001E981
jnz .next
cmp dword [edx+4], 0x840F8000
jnz .next
jmp .found

.next:
inc edx
loop .searchloop

pop edi

.failure:
push byte 0x10
push byte 0
push failuretext
push byte 0
callW MessageBoxA

xor eax, eax
dec eax
ret

.found:
add edx, byte 8
mov eax, edx     ;eax = offset

add edx, [edx]
add edx, byte 4  ;edx = destination
mov [replacement2.handleit], edx

mov edi, eax

push oldprotect
push byte 0x40      ;PAGE_EXECUTE_READWRITE
push byte 4         ;size
push edi
callW VirtualProtect

mov eax, edi
add eax, byte 4
mov [replacement2.donthandleit], eax

neg eax
lea edx, [eax+replacement2]
neg eax

mov [edi], edx

push oldprotect
push dword [oldprotect]
push byte 4         ;size
push edi
callW VirtualProtect

pop edi
xor eax, eax
ret

;firt replacement function, it is a hook for _Guardmemory
replacement1:
%if DEBUG == 1
int3
%endif

mov eax, [esp+3*4]          ;arg3 = set or remove flag
test eax, eax
jz .remove

;find end of list
mov ecx, membps

.findloop:
lea eax, [ecx+2*4]
mov ecx, [eax]
test ecx, ecx
jnz .findloop

;find empty space
mov edx, membps

.findloop2:
add edx, 3*4
mov ecx, [edx]
test ecx, ecx
jnz .findloop2

;add this one
mov ecx, [esp+1*4]
mov [edx], ecx
add ecx, [esp+2*4]
mov [edx+1*4], ecx
mov [eax], edx

jmp short .end_replacement_1

;remove a guarded page
.remove:
mov eax, membps
mov edx, [esp+1*4]

.findloop3:
cmp edx, [eax]
jz .found
lea ecx, [eax+2*4]
mov eax, [ecx]
test eax, eax
jnz .findloop3

;not found ??
jmp .end_replacement_1

.found:
mov edx, [eax+2*4]
mov [ecx], edx          ;remove from linked list

xor edx, edx
mov [eax], edx
mov [eax+4], edx
mov [eax+8], edx        ;clear the entry

.end_replacement_1:
push    ebp
mov     ebp, esp
sub     esp, byte 20h

db 0x68             ;push
.addy: dd 0
ret

;replacement 2, it checks if the guard page exception should be handled
replacement2:
%if DEBUG == 1
int3
%endif

mov edx, [edx+3*4]         ;EXCEPTION_DEBUG_INFO._EXCEPTION_RECORD.ExceptionAddress

mov ecx, membps

.findloop:
cmp edx, [ecx]
jb .next
cmp edx, [ecx+1*4]
jb .found

.next:
mov ecx, [ecx+2*4]
test ecx, ecx
jnz .findloop

;not found, so not handled either
xor ecx, ecx        ;it will be tested for some more exceptions

db 0x68             ;push
.donthandleit: dd 0
ret

.found:
db 0x68             ;push
.handleit: dd 0
ret


;Stuff for olly, aboutbox etc
dll_export _ODBG_Pluginmenu
_ODBG_Pluginmenu:

mov eax, [esp+1*4]
test eax, eax               ;PM_MAIN?
jz .main

xor eax, eax
ret

.main:
mov edx, [esp+2*4]
xor ecx, ecx
.copyloop:
mov al, [menu+ecx]
mov [edx+ecx], al
inc ecx
test al, al
jnz .copyloop

inc eax
ret

dll_export _ODBG_Pluginaction
_ODBG_Pluginaction:
mov eax, [esp+1*4]
test eax, eax               ;PM_MAIN?
jz .main

.wrong:
xor eax, eax
ret

.main:
mov eax, [esp+2*4]
test eax, eax               ;Is it the About button? Not like we have another button but anyway.
jnz .wrong

push 0
push title
push text
push 0
callW MessageBoxA

ret

dll_export _ODBG_Paused
_ODBG_Paused:
%if DEBUG == 1
int3
%endif

cmp dword [esp+1*4], 2      ;PP_TERMINATED
jz .clear

xor eax, eax
ret

.clear:
mov ecx, 256*3+1
mov edx, membps+2*4
xor eax, eax

.clearloop:
mov [edx], eax
loop .clearloop

ret

name: db "OllyGuard", 0
menu: db "0 &About", 0
title: db "OllyGuard Info", 0
text:
db "OllyGuard v0.1 by MOID / TSRh", 0xD, 0xA
db "Licensed under the GPL", 0xD, 0xA
db 0xD, 0xA
db "Makes olly only handle EXCEPTION_GUARD_PAGE if you set a memory breakpoint", 0xD, 0xA
db "You should ignore exception 80000001h if you do not want to break on application-triggered guard page exceptions.", 0
wrong_version: db "This plugin was made for Ollydbg 1.10 and uses patches in olly's code. These may not work on this version of Ollydbg. Continue anyway?", 0
failuretext:
db "OllyGuard failed to patch olly :(", 0

_Guardmemory: db "_Guardmemory", 0
oldprotect: dd 0

moduleinfo:
.base: dd 0
.sizeofimage: dd 0
.entrypoint: dd 0

membps:
;first a bogus entry
.start: dd 0xFFFFFFFF
.end:   dd 0xFFFFFFFF
.next:  dd 0

.startoflist:
resd 3*256           ;max 256 membps should be enough