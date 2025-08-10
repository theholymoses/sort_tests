global odd_even_sort_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
times -(odd_even_sort_uint64.odd - odd_even_sort_uint64) % 16 nop
odd_even_sort_uint64:
    cmp rsi, 1
    jbe .end

    mov rdx, 8
    mov rcx, 16
    lea rsi, [rdi + rsi * 8]
    sub rsi, rdx

    .odd:
        mov r9, 1

        lea r8, [rdi + rdx]
        cmp r8, rsi
        je .even

        .odd_inner:
            mov rax, [r8]
            cmp rax, [r8 + rdx]
            jbe .odd_next

            xchg rax, [r8 + rdx]
            mov [r8], rax
            xor r9, r9

        .odd_next:
            add r8, rcx
            cmp r8, rsi
            jb .odd_inner

    .even:
        mov r8, rdi

        .even_inner:
            mov rax, [r8]
            cmp rax, [r8 + rdx]
            jbe .even_next

            xchg rax, [r8 + rdx]
            mov [r8], rax
            xor r9, r9

        .even_next:
            add r8, rcx
            cmp r8, rsi
            jb .even_inner

        test r9, r9
        jz .odd
.end:
    ret
