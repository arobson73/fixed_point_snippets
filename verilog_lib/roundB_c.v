module roundB_c
(
    input signed [31:0] in,
    output signed [15:0] out
);
wire signed [31:0] ao2;
L_add_c add_unit
(.a(in), .b(32'h00008000), .c(ao2));
extract_h_c extract_unit
(.in(ao2), .out(out));

endmodule
