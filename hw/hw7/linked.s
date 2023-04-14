main:
    movi $2, head           # $2 = curr
loop:
    lw $3, 0($2)            # $3 = curr.val
    add $1, $1, $3          # $1 += $3
    lw $3, 1($2)            # $3 = next
    jeq $3, $0, end         # if next == 0, end
    add $2, $0, $3          # $2 = curr = next   
    j loop     
end:
    halt
chain4:
    .fill 100
    .fill chain5
    .fill 300
chain7:
    .fill 16384
    .fill 909
    .fill 0
chain1:
    .fill 23
    .fill chain2
head:                       # beginning of linked list
    .fill 34
    .fill chain1
    .fill 82
    .fill 10
chain3:
    .fill 4
    .fill chain4
    .fill 229
    .fill 449
chain2:
    .fill 0
    .fill chain3
chain5:
    .fill 12
    .fill 0                 # end of linked list
    .fill 9999
chain6:
    .fill 99
    .fill 49
