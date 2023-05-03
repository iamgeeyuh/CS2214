section .data
numbers_size:
dd 8
numbers:
dd 10, 34, 55, 106, 44, 0, 46, 400

section .text
global _start

_start:
    ; initialize variables
    mov ebx, [numbers_size]  ; store size of array in ebx
    mov ecx, numbers        ; store address of array in ecx
    mov edx, 0              ; initialize edx to 0 for division

    ; calculate sum of array
    mov eax, 0              ; initialize eax to 0 for sum
sum_loop:
    cmp ebx, 0              ; check if we've processed all elements
    je calc_avg             ; if so, calculate the average
    add eax, [ecx]          ; add current element to sum
    add ecx, 4              ; increment pointer to next element
    dec ebx                 ; decrement counter
    jmp sum_loop            ; repeat until all elements are processed

    ; calculate average and exit
calc_avg:
    mov ebx, eax            ; copy sum to ebx for division
    mov eax, 0              ; zero out eax before division
    div dword [numbers_size]; divide sum by array size
    mov eax, 1              ; select exit syscall
    int 0x80                ; invoke the syscall
