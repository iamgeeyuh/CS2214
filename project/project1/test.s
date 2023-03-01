movi $1, foo # ram[0]
addi $1, $1, 1 # ram[1]
jr $1 # ram[2]
halt # ram[3]
foo:
movi $2, 2 # ram[4]
movi $3, 3 # ram[5]
halt # ram[6]