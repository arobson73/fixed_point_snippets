module extract_h_c
(
    input signed [31:0] in,
    output signed [15:0] out
);

assign out = in[31:16];
endmodule
