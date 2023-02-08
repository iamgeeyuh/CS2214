module binary_to_gray(B, G);

    input [3:0] B;
    output [3:0] G;

    wire G0, G1, G2, G3;

    assign G3 = B[3];
    assign G2 = B[2] ^ B[3];
    assign G1 = B[1] ^ B[2];
    assign G0 = B[0] ^ B[1];

    assign G = {G3, G2, G1, G0};

endmodule