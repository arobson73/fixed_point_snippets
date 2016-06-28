module extract_l_c
(
    input signed [31:0] in,
    output signed [15:0] out
);

assign out = in[15:0];

endmodule
