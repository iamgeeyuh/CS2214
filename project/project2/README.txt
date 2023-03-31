Jia Huang
jzh9076@nyu.edu

My work is complete and has no known bugs.

Resources: E20 manual, w3schools, google, stackoverflow

I wrote my program based on the fetch, decode, and execute stages of the multicycle E20 implementation. I first fetched the instructions and stored them in RAM. Then I used a while loop to go through the instructions until a halt was encountered. In each iteration of the loop, I decoded the opcode and arguments and used them the execute the proper actions. Then I would increment the pc accordingly and move on to the next iteration of the while loop. I could've combined the decode and execute functions and removed the middle man of determining the opcode from decode() and passing on the opcode to execute(), but I think separating the functions made the program much more readable and easy to debug. 

In my decode function, I used switch case to find the opcode. I did not include a default return at the end of the code even though not all cases were covered. For example, if the input was 000 000 000 000 0111, decode() would return nothing which would result in an error. This affects execute() as well because execute() depends on the opcode returned from decode(). Assuming only valid instructions are inputted, the program should run without errors.

