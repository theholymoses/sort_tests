global bubble_sort_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
align 16
times -(bubble_sort_uint64.outer - bubble_sort_uint64) % 16 nop
bubble_sort_uint64:
    cmp rsi, 1
    jbe .end

    mov edx, 8
    lea rsi, [rdi + rsi * 8]    ; ptr to last element + 1

    .outer:
        lea r9, [rdi + rdx] ; ptr to 2nd element
        mov r8, rdi         ; ptr to 1st element
        xor r10, r10        ; count of swaps

        cmp r9, rsi
        jae .end

        .inner:
            mov rax, [r8]
            cmp rax, [r9]
            jbe .next

            xchg rax, [r9]
            mov [r8], rax
            inc r10

        .next:
            add r9, rdx
            add r8, rdx
            cmp r9, rsi
            jb .inner

        test r10, r10   ; no swaps occured, array is sorted
        jz .end

        sub rsi, 8
        jmp .outer

.end:
    ret

