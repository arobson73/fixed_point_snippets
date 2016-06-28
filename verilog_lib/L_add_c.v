module L_add_c
(
    input signed [31:0] a,
    input signed [31:0] b,
    output signed [31:0] c
);
assign c = a + b;
endmodule
