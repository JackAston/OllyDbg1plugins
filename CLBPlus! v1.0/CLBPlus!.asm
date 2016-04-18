		.386
		.model flat, stdcall
		option casemap:none

		includelib \masm32\lib\user32.lib
		includelib \masm32\lib\kernel32.lib
		includelib \masm32\lib\ollydbg.lib	

		include \masm32\include\windows.inc
		include \masm32\include\user32.inc
		include \masm32\include\kernel32.inc
		include plugin.inc
		include Strings.mac

GetParamFromCmd proto	cmd:ptr char, param_num:dword		; returns parameter from cmd:
								; EAX = t_result.u
								; ESI > ASCIIZ param

;**************************************************************************************
		.data

LOG_ALREADY_STARTED	db	0
AUTO_CONTINUE_PROCESS	db	1
log_counter		dd	1
template		db	13,10,13,10,13,10,'%.2i. [%.2i:%.2i:%.2i]',09,'Breakpoint at %s %s',13,10
			db	09,09,'Dump base: %#.8lX | Dump size: %i bytes',13,10,13,10,0
;**************************************************************************************	


;**************************************************************************************	
		.data?

hwmain		dd		?
hinst		HINSTANCE	?
log_fd		dd		?
param2		dd		?
param3		dd		?
pdump_buf	dd		?

threadid	dd		?
reg_eip		dd		?
pmod_base	dd		?
mod_size	dd		?
hProcess	dd		?
log_buf		db		128 dup (?)
symb		db		128 dup (?)
nsymb		equ		$-symb
scomment	db		TEXTLEN dup (?)
;**************************************************************************************	


;**************************************************************************************
		.code

DllEntryPoint	proc 	hi:HINSTANCE, reason:dword, res:dword

		.IF reason == DLL_PROCESS_ATTACH
			m2m	hinst, hi
		.ENDIF

		return	1

DllEntryPoint	endp
;**************************************************************************************

;**************************************************************************************
ODBG_Plugindata	proc 	C shortname:ptr byte

		invoke	lstrcpy, 						\
			shortname, 						\
			$CTA0("CLBPlus\:")

		return	PLUGIN_VERSION

ODBG_Plugindata	endp
;**************************************************************************************

;**************************************************************************************
ODBG_Plugininit proc 	C ollydbgversion:dword, hw:HWND, features:ptr dword 
      
		.IF ollydbgversion < PLUGIN_VERSION
            		jmp	_bad_exit
		.ENDIF

		invoke	Addtolist, 						\
			0, 							\
			0, 							\
			$CTA0("CLBPlus\: v1.0")
		invoke  Addtolist, 						\
			0, 							\
			-1, 							\
			$CTA0("  Copyright (C) 2005 by Robert Ayrapetyan")
		m2m	hwmain, hw

		return	0
_bad_exit:
		return  -1

ODBG_Plugininit endp
;**************************************************************************************

;**************************************************************************************
ODBG_Pluginmenu	proc 	C origin:dword, data:ptr byte, item:dword

        	.IF origin == PM_MAIN
            		invoke	lstrcpy, 					\
			data, 							\
			$CTA0("0 &Options|1 &About")
			return	1
		.ENDIF

            	return	0

ODBG_Pluginmenu endp
;**************************************************************************************

;**************************************************************************************
ODBG_Pluginaction proc 	C origin:dword, action:dword, item:dword

                .IF origin == PM_MAIN
            		.IF action == 0
				
				; ShowOptionsWindow

            		.ELSEIF action == 1

				invoke	MessageBox, 				\
					hwmain,					\
					$CTA0("CLBPlus\: plugin v1.0\nCopyright (C) 2005 by Robert Ayrapetyan"),\
					$CTA0("CLBPlus\:"),			\
					MB_OK or MB_ICONINFORMATION
			.ENDIF
		.ENDIF

		ret

ODBG_Pluginaction endp
;**************************************************************************************

;**************************************************************************************
ODBG_Plugincmd	proc	C uses EBX reason:dword, reg:ptr t_reg, cmd:ptr char

LOCAL temp:DWORD
LOCAL sys_time:SYSTEMTIME

		mov	EAX, dword ptr [cmd]

		invoke	CharUpper,		\
			EAX

		.IF	dword ptr [EAX] != 'PBLC'
			jmp	_fin
		.ENDIF		

		invoke	GetParamFromCmd, cmd, 1
		mov	EAX, [ESI]

		.IF	EAX == 'PMUD'					; DUMP

			.IF	LOG_ALREADY_STARTED == 0				

				mov	EBX, reg
				assume	EBX: ptr t_reg
				m2m	threadid, [EBX].threadid
				m2m	reg_eip, [EBX].ip

				invoke	Findmodule,			\
					reg_eip

				mov	EBX, EAX
				assume	EBX: ptr t_module
				m2m	pmod_base, [EBX].base
				m2m	mod_size, [EBX].size_

				invoke	GetParamFromCmd, cmd, 2
				mov	param2, EAX				
				invoke	GetParamFromCmd, cmd, 3
				mov	param3, EAX				
				
				invoke	Plugingetvalue,			\
					VAL_HPROCESS

				mov	hProcess, EAX

				invoke	Plugingetvalue,			\
					VAL_PROCESSNAME

				mov	EDI, EAX
				mov	ESI, EAX
				mov	ECX, MAX_PATH
				xor	AL, AL
				cld
				repnz	scasb
				dec	EDI
				mov	EAX, 'gol.'
				stosd
				mov	EAX, '+'
				stosd

				invoke	CreateFile,			\
					ESI,				\
					GENERIC_WRITE,			\
					NULL,				\
					NULL,				\
					CREATE_ALWAYS,			\
					NULL,				\
					NULL

				mov	log_fd, EAX				

				invoke  VirtualAlloc, 		\
					NULL, 			\
					1000h, 			\
					MEM_RESERVE+MEM_COMMIT,	\
					PAGE_READWRITE

				mov	pdump_buf, EAX

				mov	LOG_ALREADY_STARTED, 1
			.ENDIF

			invoke	ReadProcessMemory,	\
				hProcess,		\
				param2,			\
				pdump_buf,		\
				param3,			\
				addr temp
			
			invoke	GetLocalTime, 		\
				addr sys_time

			invoke	Decodeaddress,		\
				reg_eip,		\
				pmod_base,		\
				NULL,			\
				addr symb,		\
				nsymb,			\
				addr scomment				

			invoke	wsprintf,		\
				addr log_buf,		\
				addr template,		\
				log_counter,		\
				sword ptr sys_time.wHour,	\
				sword ptr sys_time.wMinute,	\
				sword ptr sys_time.wSecond,	\
				addr symb,		\
				addr scomment,		\
				param2,			\
				param3

				mov	EBX, EAX
				inc	log_counter				

			invoke 	WriteFile, 		\
				log_fd, 		\
				addr log_buf,		\
				EBX,			\
				addr temp,		\
				NULL

			invoke 	WriteFile, 		\
				log_fd, 		\
				pdump_buf,		\
				param3,			\
				addr temp,		\
				NULL

			.IF	AUTO_CONTINUE_PROCESS == 1

				invoke	Sendshortcut,		\
					PM_MAIN,		\
					NULL,			\
					WM_KEYDOWN,		\
					NULL,			\
					NULL,			\
					VK_F9
			.ENDIF


		.ELSEIF	EAX == 'TSET'
			; test
			invoke	MessageBox, 			\
				hwmain,				\
				$CTA0("TEST"),			\
				$CTA0("CLBPlus\:"),		\
				MB_OK or MB_ICONINFORMATION		
		.ELSE
			invoke	Addtolist, 			\
				0, 				\
				0, 				\
				$CTA0("CLBPlus\:: Unrecognized command")
		.ENDIF

		return	1	
_fin:
		return	0

ODBG_Plugincmd	endp
;**************************************************************************************

;**************************************************************************************
ODBG_Plugindestroy	proc	C
		.IF	LOG_ALREADY_STARTED == 1		
			invoke  VirtualFree,	\
	       			pdump_buf,	\
				0,		\
				MEM_RELEASE

			invoke	CloseHandle,	\
				log_fd
		.ENDIF
		ret
ODBG_Plugindestroy	endp
;**************************************************************************************

;**************************************************************************************
GetParamFromCmd	proc uses EDX EBX cmd:ptr char, param_num:dword

LOCAL cmd_buf[TEXTLEN]:BYTE
LOCAL pres:t_result

		mov	ESI, cmd
		lea	EDI, cmd_buf
_next_cmd_char:
		lodsb
		stosb
		test	AL, AL
		jnz	_next_cmd_char

		lea	EDI, cmd_buf
		add	EDI, 4				; jump over identifier
		mov	ECX, TEXTLEN-4	
_next_param:
		mov	AL, ' '
		repz	scasb
		dec	EDI
		push	EDI
		repnz	scasb
		dec	EDI
		xor	AL, AL
		stosb					; make ASCIIZ expression
		pop	ESI
		
		dec	param_num
		jnz	_next_param

		invoke	Expression,			\
			addr pres,			\
			ESI,				\
			NULL,				\
			NULL,				\
			NULL,				\
			pmod_base,			\
			mod_size,			\
			threadid

		mov	EAX, dword ptr [pres.u]
		ret
GetParamFromCmd	endp
;**************************************************************************************

end DllEntryPoint