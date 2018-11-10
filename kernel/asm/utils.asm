GLOBAL DebugBreak

DebugBreak:
    push rbp
    mov rbp, rsp

    xchg bx, bx

    pop rbp
    ret