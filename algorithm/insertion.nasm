global insertion_sort_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
align 16
times -(insertion_sort_uint64.select - insertion_sort_uint64) % 16 nop
insertion_sort_uint64:
    cmp rsi, 1
    jbe .end

    dec rsi         ; use rsi as last index

    mov r8, 1       ; select index

    .select:
        mov rax, [rdi + r8 * 8] ; inserted value

        mov r10, r8             ; prev prev element index
        mov r9,  r8             ; prev element index
        dec r10

        .insert:
            cmp r9, 0
            je .next

            mov rdx, [rdi + r10 * 8]

            cmp rdx, rax
            jbe .next

            xchg rdx, [rdi + r9 * 8]
            mov [rdi + r10 * 8], rdx
 
            dec r10
            dec r9
            jnz .insert

    .next:
        mov [rdi + r9 * 8], rax
        inc r8
        cmp r8, rsi
        jbe .select

.end:
    ret

