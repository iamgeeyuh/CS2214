    lw $2, multiplier($0)      # multiplier
    lw $3, multiplicand($0)    # multiplicand
    movi $4, 1                 # bitmask
loop:                        
    slt $5, $4, $3          
    jeq $0, $5, done           # end when multiplicand < bitmask
    and $6, $4, $3             # current bit of multiplicand
    jeq $0, $6, continue       # if current bit is 0, continue loop
    add $1, $1, $2             # answer = answer + multiplier
continue:
    add $4, $4, $4             # multiply bitmask by 2
    add $2, $2, $2             # multiply multiplier by 2
    j loop
done: 
    halt
multiplicand: 
    .fill 123
multiplier: 
    .fill 119

