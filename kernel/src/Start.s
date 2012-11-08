;
; Copyright(C) 2012 Ali Ersenal
; License: WTFPL v2
; URL: http://sam.zoy.org/wtfpl/COPYING
;
;--------------------------------------------------------------------------
; Start.s
;--------------------------------------------------------------------------
;
; DESCRIPTION:  This is the first entry point for the kernel before jumping
;               to Kernel() in Kernel.c. After GRUB finishes its job it jumps
;               to Start in this file(we tell this to GRUB using the mbHead
;               Multiboot struct).
;
;               GRUB drops us in this state:
;
;                   - CPU is in protected mode(A20 gate enabled)
;                   - Interrupts are disabled
;                   - Paging is disabled
;                   - EAX register contains multiboot magic value
;                   - EBX register contains pointer to multiboot info
;                   - ESP(stack) register is undefined
;
;
; AUTHOR:       Ali Ersenal, aliersenal@gmail.com
;--------------------------------------------------------------------------

[BITS 32]

; Setup multiboot
MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

STACKSIZE   equ 0xFF00; Kernel stack size, 64 Kilobytes

[GLOBAL mbHead]
[GLOBAL Start]
[EXTERN text]
[EXTERN bss]
[EXTERN end]
[EXTERN Kernel]; Main C kernel entry point

section .text

    mbHead:
        dd  MBOOT_HEADER_MAGIC        ; Magic value
        dd  MBOOT_HEADER_FLAGS        ; Multiboot flags
        dd  MBOOT_CHECKSUM            ; Multiboot checksum value

        dd  mbHead                    ; Address of this structure
        dd  text                      ; Kernel start address
        dd  bss                       ; End of data segment
        dd  end                       ; End of bss segment
        dd  Start                     ; GRUB will call this

    Start:
        mov  esp, stack + STACKSIZE ; Set up the stack
        push eax                    ; Push multiboot magic number
        push ebx                    ; Push multiboot info pointer
        call Kernel                 ; Jump to Kernel() in Kernel.c

section .bss

    stack:
        resb STACKSIZE ; Reserve STACKSIZE bytes in bss segment