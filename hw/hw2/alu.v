`include "four_bit_adder.v"

module alu(A, B, Op, S);

   input [3:0]  A;
   input [3:0]  B;
   output [3:0] S;
   input [2:0]  Op;

   wire [3:0] adder_sum, subtractor_sum;
   wire adder_Cout, subtractor_Cout;

   four_bit_adder the_adder(A, B, 1'b0, adder_sum, adder_Cout);
   four_bit_adder the_subtractor(A, ~B, 1'b1, subtractor_sum, subtractor_Cout);

   assign S = (Op == 3'b000) ? A & B : 
              (Op == 3'b001) ? A | B : 
              (Op == 3'b010) ? adder_sum: 
              (Op == 3'b100) ? A & ~B : 
              (Op == 3'b101) ? A | ~B : 
              (Op == 3'b110) ? subtractor_sum : subtractor_sum[3];

endmodule
