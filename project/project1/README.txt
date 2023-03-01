Jia Huang
jzh9076@nyu.edu

My work is complete and has no known bugs.

I noticed that some of the opcodes had similar assembly formats and machine formats. For example, 'add', 
'sub', 'or', 'and', and 'slt' had the same assembly format {opcode, $dst, $srcA, $srcB} and the 
same machine format {srcA, srcB, dst, opcode}. I wrote a converter function called 
add_sub_etc(opcode, arguments) that would handle all 'add', 'sub', 'or', 'and', and 'slt' instructions 
because they had the same input and output formula. I did the same for other collections of opcodes 
with similar formats like 'lw' and 'sw', 'slti' and 'addi', etc. As I parsed through a list of assembly 
instructions, the instructions would be passed through one of the converter functions based on the opcode 
in the instruction. For example, if the opcode was 'add' or 'sub' I would pass the instructions through
 add_sub_etc() but if the opcode was 'jal' I'd pass the instructions through jal_j(). Each converter 
 function returned the corresponding machine code which would be added to a list of translated machine code 
 that would be outputted to the terminal.

I feel that this design cut back on a lot of redundancy. By using one function for multiple opcodes that 
are essentially the same, I am cutting a lot of repetitive code. Having separate functions for each collection 
of opcodes also makes the code more readable to me as I can easily find the function associated with an opcode 
and there is less code to read. 