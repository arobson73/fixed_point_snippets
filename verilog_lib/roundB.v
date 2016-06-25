module roundB
(
    input clk,
    input reset,
    input signed [31:0] in,
    output signed [15:0] out
);
wire signed [31:0] ao2;
wire signed [15:0] eo3;

//delay of 2
L_add add_unit
(.clk(clk), .reset(reset), .a(in), .b(32'h00008000), .c(ao2), .overflow());
//delay of 1
extract_h extract_unit
(.clk(clk), .reset(reset), .in(ao2), .out(out));

endmodule
