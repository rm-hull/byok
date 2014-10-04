;
; A boot sector that prints a string using our function
;
[org 0x7c00]        ; Tell the assembler where this code will be loaded

  mov bx, HELLO_MSG     ; Use BX as a parameter to our function, so
  call print_string     ; se can specify the address of a string.

  mov bx, CRLF
  call print_string

  mov bx, GOODBYE_MSG
  call print_string

  mov bx, CRLF
  call print_string

  mov dx, 0x2fb6        ; store the value to print in DX
  call print_hex        ; call the function

  mov bx, CRLF
  call print_string

  jmp $                 ; Hang

%include "src/learning/print_string.asm"

; Data

CRLF:
  db 0xa, 0xd, 0

HELLO_MSG:
  db 'Hello, World!', 0   ; <-- The zero on the end tells our routine
                          ;     when to stop printing

GOODBYE_MSG:
  db 'Goodbye!', 0

; Padding and magic BIOS number.
  times 510-($-$$) db 0
  dw 0xaa55