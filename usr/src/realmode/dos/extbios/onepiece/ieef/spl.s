
	TITLE   spl.s

_TEXT	SEGMENT  BYTE PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT  WORD PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT  WORD PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT  WORD PUBLIC 'BSS'
_BSS	ENDS
DGROUP	GROUP	CONST,	_BSS,	_DATA
	ASSUME  CS: _TEXT, DS: NOTHING, SS: NOTHING, ES: NOTHING
_TEXT      SEGMENT

	PUBLIC  _splhi
_splhi  PROC    NEAR
	pushf
	cli
	pop	ax
	ret
_splhi  ENDP

	PUBLIC  _splx
_splx   PROC    NEAR
	push    bp
	mov     bp, sp
	mov     dx, 4[bp]
	push    dx
	popf
	pop     bp
	ret
_splx   ENDP

_TEXT	ENDS
END
