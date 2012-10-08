[BITS 32]

; Setup multiboot
MBOOT_PAGE_ALIGN    equ 1<<0
MBOOT_MEM_INFO      equ 1<<1
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

STACKSIZE   equ 0xFF00; Kernel stack size, 64 bytes

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
        dd  Start                     ; Multiboot will call this

    Start:
        mov  esp, stack + STACKSIZE ; Set up the stack
        cli                         ; Disable interrupts
        push eax                    ; Push multiboot magic number
        push ebx                    ; Push multiboot info pointer
        call Kernel                 ; Jump to Kernel() in Kernel.c

section .bss

    stack:
        resb STACKSIZE; Reserve STACKSIZE bytes in bss segment