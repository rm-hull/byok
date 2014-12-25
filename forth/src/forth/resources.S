    .global system_forth
    .global resource_size
    .section .rodata
system_forth:
    .incbin "src/forth/system.fth"
    .ascii "\0"
resource_size:
    .int . - system_forth