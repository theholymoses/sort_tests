global cycle_sort_uint64

section .text

; -------------------------------------------------------------
; rdi - addr
; rsi - len
cycle_sort_uint64:
    cmp rsi, 1
    jbe end

    mov rdx, 8
    lea rsi, [rdi + rsi * 8]
    sub rsi, rdx

    seek_correct_position_of_element:
        cmp rdi, rsi
        je end

        lea r9,  [rdi + rdx]        ; next element position
        mov rax, [rdi]              ; element value
        mov r8,  rdi                ; initial element position

        .loop:
            cmp rax, [r9]
            jbe .next
            add r8, rdx
        .next:
            add r9, rdx
            cmp r9, rsi
            jbe .loop

        cmp r8, rdi
        je next

        .skip_duplicates:
            cmp rax, [r8]
            jne .swap

            cmp r8, rsi
            ja next

            add r8, rdx
            jmp .skip_duplicates

    .swap:
        xchg rax, [r8]

    seek_correct_position_of_swapped:
        cmp r8, rdi
        je next

        lea r9,  [rdi + rdx]
        mov r8,  rdi

        .loop:
            cmp rax, [r9]
            jbe .next
            add r8, rdx
        .next:
            add r9, rdx
            cmp r9, rsi
            jbe .loop

        .skip_duplicates:
            cmp rax, [r8]
            jne .swap

            cmp r8, rsi
            ja next

            add r8, rdx
            jmp .skip_duplicates

    .swap:
        xchg rax, [r8]
        jmp seek_correct_position_of_swapped

    next:
        add rdi, rdx
        jmp seek_correct_position_of_element

end:
    ret
