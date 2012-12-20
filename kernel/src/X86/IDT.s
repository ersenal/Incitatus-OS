;
; Copyright(C) 2012 Ali Ersenal
; License: WTFPL v2
; URL: http://sam.zoy.org/wtfpl/COPYING
;
;--------------------------------------------------------------------------
; IDT.s
;--------------------------------------------------------------------------
;
; DESCRIPTION:  Low level interrupt(exception and irq) handlers.
;
; AUTHOR:       Ali Ersenal, aliersenal@gmail.com
;--------------------------------------------------------------------------


[EXTERN IDT_handlerException] ; Common C-level exception handler in IDT.c
[EXTERN IDT_handlerIRQ] ; Common C-level IRQ handler in IDT.c

;Exceptions
[GLOBAL IDT_handler0]
[GLOBAL IDT_handler1]
[GLOBAL IDT_handler2]
[GLOBAL IDT_handler3]
[GLOBAL IDT_handler4]
[GLOBAL IDT_handler5]
[GLOBAL IDT_handler6]
[GLOBAL IDT_handler7]
[GLOBAL IDT_handler8]
[GLOBAL IDT_handler9]
[GLOBAL IDT_handler10]
[GLOBAL IDT_handler11]
[GLOBAL IDT_handler12]
[GLOBAL IDT_handler13]
[GLOBAL IDT_handler14]
[GLOBAL IDT_handler15]
[GLOBAL IDT_handler16]
[GLOBAL IDT_handler17]
[GLOBAL IDT_handler18]
[GLOBAL IDT_handler19]
[GLOBAL IDT_handler20]
[GLOBAL IDT_handler21]
[GLOBAL IDT_handler22]
[GLOBAL IDT_handler23]
[GLOBAL IDT_handler24]
[GLOBAL IDT_handler25]
[GLOBAL IDT_handler26]
[GLOBAL IDT_handler27]
[GLOBAL IDT_handler28]
[GLOBAL IDT_handler29]
[GLOBAL IDT_handler30]
[GLOBAL IDT_handler31]

;IRQs
[GLOBAL IDT_request0]
[GLOBAL IDT_request1]
[GLOBAL IDT_request2]
[GLOBAL IDT_request3]
[GLOBAL IDT_request4]
[GLOBAL IDT_request5]
[GLOBAL IDT_request6]
[GLOBAL IDT_request7]
[GLOBAL IDT_request8]
[GLOBAL IDT_request9]
[GLOBAL IDT_request10]
[GLOBAL IDT_request11]
[GLOBAL IDT_request12]
[GLOBAL IDT_request13]
[GLOBAL IDT_request14]
[GLOBAL IDT_request15]

; Common exception handler
; Save processor state, set segments and call C-handler and then restore stack frame
IDT_exceptionHandlerCommon:

    ; Save register state: Push EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    pusha

    ; Save segment selectors
    mov eax, ds
    push eax

    mov eax, es
    push eax

    mov eax, fs
    push eax

    mov eax, gs
    push eax

    ; Switch to kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp                   ; Push stack pointer(Regs* in IDT.c)
    call IDT_handlerException  ; Call C-level common exception handler
    add esp, 4                 ; Drop stack pointer

    ; Restore the segment selectors
    pop gs
    pop fs
    pop es
    pop ds

    popa                     ; Restore register state, Pop EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    add esp, 8               ; Drop interrupt number and error code
    iret                     ; Restore CS:IP and EFLAGS

; Common IRQ handler
; Save processor state, set segments and call C-handler and then restore stack frame
IDT_irqHandlerCommon:

    ; Save register state: Push EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    pusha

    ; Save segment selectors
    mov eax, ds
    push eax

    mov eax, es
    push eax

    mov eax, fs
    push eax

    mov eax, gs
    push eax

    ; Switch to kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp             ; Push stack pointer(Regs* in IDT.c)
    call IDT_handlerIRQ  ; Call C-level common exception handler
    add esp, 4           ; Drop stack pointer

    mov eax, DR0
    mov esp, eax

    ; Restore the segment selectors
    pop gs
    pop fs
    pop es
    pop ds

    popa                     ; Restore register state, Pop EDI, ESI, EBP, ESP, EBX, EDX, ECX, EAX
    add esp, 8               ; Drop interrupt number and error code
    iret                     ; Restore CS:IP and EFLAGS

; Only interrupts 8, 10-14 inclusive push error codes onto the stack
; For others use a dummy error code 0
IDT_handler0:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 0 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler1:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 1 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler


IDT_handler2:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 2 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler3:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 3 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler4:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 4 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler5:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 5 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler6:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 6 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler7:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 7 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler8:

    cli ; Disable interrupts
    push byte 8 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler9:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 9 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler10:

    cli ; Disable interrupts
    push byte 10 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler11:

    cli ; Disable interrupts
    push byte 11 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler12:

    cli ; Disable interrupts
    push byte 12 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler13:

    cli ; Disable interrupts
    push byte 13 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler14:

    cli ; Disable interrupts
    push byte 14 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler15:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 15 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler16:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 16 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler17:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 17 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler18:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 18 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler19:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 19 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler20:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 20 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler21:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 21 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler22:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 22 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler23:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 23 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler24:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 24 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler25:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 25 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler26:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 26 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler27:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 27 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler28:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 28 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler29:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 29 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler30:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 30 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_handler31:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 31 ; Push interrupt number
    jmp IDT_exceptionHandlerCommon ; Go to common handler

IDT_request0:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 32 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request1:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 33 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request2:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 34 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request3:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 35 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request4:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 36 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request5:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 37 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request6:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 38 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request7:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 39 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request8:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 40 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request9:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 41 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request10:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 42 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request11:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 43 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request12:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 44 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request13:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 45 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request14:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 46 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

IDT_request15:

    cli ; Disable interrupts
    push byte 0 ; Push a dummy error code
    push byte 47 ; Push interrupt number
    jmp IDT_irqHandlerCommon ; Go to common handler

