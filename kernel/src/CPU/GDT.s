[GLOBAL GDT_farJump]

GDT_farJump:

   jmp 0x08:.flush ; Set CS and EIP

.flush:
   ret
