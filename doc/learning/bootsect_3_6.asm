;
; A simple boot sector that demonstrates the stack
;

  mov ah, 0x0e      ; int 10/ah = 0eh -> scrolling teletype BIOS routine

  mov bp, 0x8000    ; Set the base of the stack a little above where the BIOS
  mov sp, bp        ; loads our boot sector - so it wont overwrite us.

  push 'A'          ; Push some characters on the stack for later
  push 'B'          ; retreival. Note, these are pushed on as
  push 'C'          ; 16-bit values, so the most significant byte
                    ; will be added by our assembler as 0x00.

  pop bx            ; Note, we can only pop 16-bits, so pop to bx
  mov al, bl        ; then copy bl (i.e. 8-bit char) to al
  int 0x10          ; print(al)

  pop bx            ; Pop the next value
  mov al, bl
  int 0x10          ; print(al)

  mov al, [0x7ffe]  ; To prove out stack grows downwards from bp,
                    ; fetch the char at 0x8000 - 0x2 (i.e. 16-bits)
  int 0x10          ; print(al)

  jmp $             ; Jump to the current address (i.e. forever)

;
; Padding and magic BIOS number.
;

  times 510-($-$$) db 0
  dw 0xaa55