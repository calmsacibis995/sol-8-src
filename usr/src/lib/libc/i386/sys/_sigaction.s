.ident	"@(#)_sigaction.s	1.9	98/07/08 SMI"

/ SYS library -- _sigaction
/ error = _sigaction(sig, act, oact);

	
	.file "_sigaction.s"
	
	.text

	.globl	__sigaction
	.globl	__cerror

_fgdef_(__sigaction):
	MCOUNT
	movl	$SIGACTION,%eax
	_prologue_
	movl	_daref_(_sigreturn),%edx
	_epilogue_
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
noerror:
	ret
	_fg_setsize_(`__sigaction')


/ The following is used as a return address from user level interrupt
/ catching routines to clear the args from the interrupt routine call.
/ Then it enters the system to do the interrupt return to restore the 
/ stack to the proper state. Its address sent to kernel only on signal
/ sigset.
/ The signal handling is done in the kernel because the floating point
/ emulation is done there also.
/
_sigreturn:
	addl	$4,%esp		/ remove args to user interrupt routine
	lcall   $SIGCLEAN_TRAPNUM,$0    / return to kernel to return to user
