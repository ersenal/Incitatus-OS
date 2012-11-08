[GLOBAL GDT_farJump]

GDT_farJump:

   jmp 0x08:dummy ; Set CS and EIP

dummy:
   ret
