extern main

[bits 64]
KernelStub:

    call main
    .hlt:
    hlt
    jmp .hlt
    ret