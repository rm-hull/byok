;
; A print function.
;
; The address of a null-terminated string should be placed in BX
;
print_string:
  pusha                 ; Push all register values to the stack

loop:
  mov al, [bx]          ; Move contents of BX into AL

  cmp al, 0             ; Found a null terminator?
  je the_end            ; If so, exit

  mov ah, 0x0e          ; int=10/ah=0x0e -> BIOS tele-type output
  int 0x10              ; print the character in al

  inc bx                ; increment BX for the next character
  jmp loop              ; and go back to print it

the_end:
  popa                  ; Restore original reguster values
  ret


; prints the value of DX as hex
print_hex:
  pusha

  mov al, dh
  shr al, 4
  call hex_digit_to_ascii
  mov [HEX_OUT+2], al

  mov al, dh
  and al, 0x0f
  call hex_digit_to_ascii
  mov [HEX_OUT+3], al

  mov al, dl
  shr al, 4
  call hex_digit_to_ascii
  mov [HEX_OUT+4], al

  mov al, dl
  and al, 0x0f
  call hex_digit_to_ascii
  mov [HEX_OUT+5], al

  mov bx, HEX_OUT       ; print the string poijnted to
  call print_string     ; by BX
  popa
  ret

; Converts a digit in range 0..15 to an ASCII representation of 0..9a..f, passed in AL, returned in AL 
hex_digit_to_ascii:
    cmp al, 9
    jle digit_char
    add al, 39
digit_char:
    add al, 48
    ret

; global variables
HEX_OUT: db '0x0000', 0