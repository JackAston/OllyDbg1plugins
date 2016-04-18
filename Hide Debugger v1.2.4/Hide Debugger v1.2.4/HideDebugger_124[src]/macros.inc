
WM_CASE MACRO reg:REQ, msgs:REQ
 IRP msg, <msgs>
      cmp reg, WM_&msg
      je @&msg
 ENDM
ENDM


$invoke MACRO vars:VARARG
     invoke vars
     EXITM <eax>
ENDM


date MACRO
local pos, month

	;; Month
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 1)
			month TEXTEQU @SubStr(%@Date, 1 , 2)
			IF month EQ 01
				db "Jan "
			ELSEIF month EQ 02
				db "Feb "
			ELSEIF month EQ 03
				db "Mar "
			ELSEIF month EQ 04
				db "Apr "
			ELSEIF month EQ 05
				db "May "
			ELSEIF month EQ 06
				db "Jun "
			ELSEIF month EQ 07
				db "Jul "
			ELSEIF month EQ 08
				db "Aug "
			ELSEIF month EQ 09
				db "Sep "
			ELSEIF month EQ 10
				db "Oct "
			ELSEIF month EQ 11
				db "Nov "
			ELSEIF month EQ 12
				db "Dec "
			ENDIF
		ENDIF
		pos = pos + 1
	ENDM

	;; Day
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 4) OR (pos EQ 5)
			db "&chr"
		ENDIF
		pos = pos + 1
	ENDM

	;; Year
	db " 20"
	pos = 1
	% FORC chr, @Date
		IF (pos EQ 7) OR (pos EQ 8)
			db "&chr"
		ENDIF
		pos = pos + 1
	ENDM

ENDM


time MACRO
% FORC chr, @Time
db "&chr"
ENDM
ENDM

year MACRO
  db "20"
  pos = 1
  % FORC chr, @Date
    IF (pos EQ 7) OR (pos EQ 8)
      db "&chr"
    ENDIF
    pos = pos + 1
  ENDM
ENDM
