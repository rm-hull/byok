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