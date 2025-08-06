global selection_sort_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
times -(selection_sort_uint64.outer - selection_sort_uint64) % 16 nop
selection_sort_uint64:
    cmp rsi, 1
    jbe .end

    mov rdx, 8                  ; data size
    lea rcx, [rdi + rsi * 8]    ; last element
    sub rcx, rdx

    .outer:
        cmp rdi, rcx
        je .end

        mov rax, [rdi]          ; min value
        mov r8,  rdi            ; min value ptr
        lea r9,  [rdi + rdx]    ; next value ptr

        .select:
            cmp rax, [r9]
            jbe .select_next

            mov rax, [r9]
            mov r8,  r9

        .select_next:
            add r9, rdx
            cmp r9, rcx
            jbe .select

        cmp r8, rdi
        je .next

        xchg rax, [rdi]
        mov [r8], rax

    .next:
        add rdi, rdx
        jmp .outer

.end:
    ret
