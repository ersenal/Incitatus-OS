;
; Copyright(C) 2012 Ali Ersenal
; License: WTFPL v2
; URL: http://sam.zoy.org/wtfpl/COPYING
;
;--------------------------------------------------------------------------
; GDT.s
;--------------------------------------------------------------------------
;
; DESCRIPTION:  Far jump is used while installing GDT in order to set
;               CS and EIP registers.
;
; AUTHOR:       Ali Ersenal, aliersenal@gmail.com
;--------------------------------------------------------------------------


[GLOBAL GDT_farJump]

GDT_farJump:

   jmp 0x08:dummy ; Set CS and EIP

dummy:
   ret
