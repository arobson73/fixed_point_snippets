module extract_h
(
    input clk,
    input reset,
    input signed [31:0] in,
    output signed [15:0] out
);

//delay of one

reg signed [15:0] out_reg;

assign out = out_reg;

always @(posedge clk or posedge reset)
    if (reset)
        out_reg <= 0;
    else
        out_reg <= in[31:16];


endmodule
