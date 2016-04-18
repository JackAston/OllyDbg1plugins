
CheckOS  PROTO

.DATA?

ALIGN DWORD
osvi  OSVERSIONINFO  <>

.CODE

ALIGN DWORD
CheckOS proc
option PROLOGUE:NONE
option EPILOGUE:NONE
  mov osvi.dwOSVersionInfoSize, SIZEOF OSVERSIONINFO
  invoke GetVersionEx, OFFSET osvi
  .IF eax != 0
     xor eax, eax
     cmp osvi.dwPlatformId, VER_PLATFORM_WIN32_NT
     sete al
  .ENDIF
  ret
option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF
CheckOS endp
