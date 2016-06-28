module saturate_c
(
    input signed [31:0] in,
    output signed [15:0] out
);
wire signed [15:0] out_e1;
//instantiate

extract_l_c extract_l_unit
(.in(in), .out(out_e1));

assign out = (in > 32767) ? 32767 : (in < 32767) ? -32768 : out_e1;
endmodule
