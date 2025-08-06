global shaker_sort_oneloop_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
align 16
times -(shaker_sort_oneloop_uint64.outer - shaker_sort_oneloop_uint64) % 32 nop
shaker_sort_oneloop_uint64:
    cmp rsi, 1
    jbe .end

    mov rdx, 8                  ; ptr direction (8, -8)

    lea rsi, [rdi + rsi * 8]    ; upper bound (const)
    lea rdi, [rdi - 8]          ; lower bound (const)

    mov r11, rsi                ; upper bound (mutable)
    mov r10, rdi                ; lower bound (mutable)

    .outer:
        lea r8, [r10 + rdx]     ; 1st elem addr
        lea r9, [r8  + rdx]     ; 2nd elem addr

        cmp r8, rdi
        je .end
        cmp r9, rsi
        je .end

        xor rcx, rcx            ; new upper/lower bound

        cmp r8, r9
        jb .inner

        xchg r8, r9

        .inner:
            mov rax, [r8]
            cmp rax, [r9]
            jbe .next

            xchg rax, [r9]
            mov [r8], rax

            mov rcx, r9
            cmp rdx, -8
            cmove rcx, r8

        .next:
            add r8, rdx
            add r9, rdx

            cmp r8, rdi
            je .switch_direction
            cmp r9, rsi
            je .switch_direction
            jmp .inner

    .switch_direction:
        test rcx, rcx
        jz .end

        cmp    rdx, 8
        cmove  r11, rcx
        cmovne r10, rcx

        neg rdx
        xchg r10, r11
        jmp .outer

.end:
    ret

