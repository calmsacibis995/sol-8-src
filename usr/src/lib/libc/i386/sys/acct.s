
.ident	"@(#)acct.s	1.8	98/07/08 SMI"

/ acct function


	.file	"acct.s"

	.text

	.globl  __cerror

_fwdef_(`acct'):
	MCOUNT
	movl	$ACCT,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp 	_fref_(__cerror)
noerror:
	xorl	%eax,%eax
	ret
	_fw_setsize_(`acct')
