
    cpu 8086
    bits 16

    org 0x800

    mov ax,0
hello:
    jmp there
    nop
    nop
there:
    inc ah
    int 12h
    cmp ah,37h
    jnz hello
    int 11h
    int 17h
    jmp hello
