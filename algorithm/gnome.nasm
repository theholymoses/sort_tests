global gnome_sort_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
times -(gnome_sort_uint64.loop - gnome_sort_uint64) % 16 nop
gnome_sort_uint64:
    cmp rsi, 1
    jbe .end

    xor r8,  r8
    mov r9,  1
    mov r10, 2

    .loop:
        cmp r9, rsi
        jae .end

        mov rax, [rdi + r8 * 8]
        cmp rax, [rdi + r9 * 8]
        jbe .next

        xchg rax, [rdi + r9 * 8]
        mov [rdi + r8 * 8], rax
        dec r8
        dec r9
        jnz .loop

    .next:
        mov r9, r10
        mov r8, r10
        dec r8
        inc r10
        jmp .loop

.end:
    ret

