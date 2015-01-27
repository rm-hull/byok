    .global system_forth
    .global examples_forth
    .global mandlebrot_forth
    .global resource_size

    .section .rodata
system_forth:
    .incbin "src/forth/system.fth"
    .ascii "\0"

# For convenience, included here while under development
examples_forth:
    .incbin "src/forth/examples.fth"
    .ascii "\0"
mandlebrot_forth:
    .incbin "src/forth/mandlebrot.fth"
    .ascii "\0"
resource_size:
    .int . - system_forth