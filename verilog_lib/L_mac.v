module L_mac
(
    input clk,
    input reset,
    input signed [31:0] c,
    input signed [15:0] a,
    input signed [15:0] b,
    output signed [31:0] out
);

reg signed [31:0] c1_reg,c2_reg;
wire signed [31:0] mo2;

//this is 4 clk cycles delay

//instantiate
lmult lmult_unit
(.clk(clk), .reset(reset), .a(a), .b(b), .out(mo2), .overflow());

L_add add_unit
(.clk(clk), .reset(reset), .a(c2_reg), .b(mo2), .c(out), .overflow());

always @(posedge clk or posedge reset)
    if (reset)
    begin
        c1_reg <= 0;
        c2_reg <= 0;
    end
    else
    begin
        c1_reg <= c;
        c2_reg <= c1_reg;
    end
endmodule    
