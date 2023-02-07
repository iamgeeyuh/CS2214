module recitation3a(A, B, X);

    input A. B;
    output X;

    assign X = ~(~(~(~B | A) | (A & B)) & (A | B));

endmodule