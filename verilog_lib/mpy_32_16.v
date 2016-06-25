//this will multiply a 32bit number
//(represented as hi , low) by a 16 bit 
//number
module mpy_32_16
(
    input clk,
    input reset,
    input signed [15:0] hi, //high part of 32 bit num
    input signed [15:0] lo, //low part of 32 bit num
    input signed [15:0] n, // the other 16 bit number 
    output signed [31:0] outf
    //debug
    //output signed [15:0] multo7,
    //output signed [31:0] out2,    
    //output signed [31:0] out5
  //
);

reg signed [31:0] out3_reg,out4_reg,out5_reg,out6_reg,out7_reg;
reg signed [15:0] lo1_reg,lo2_reg;
reg signed [15:0] n1_reg, n2_reg;
wire signed [15:0] multo7;
wire signed [31:0] out2;

//instantiate lmult
//delay 2
lmult lmult_unit
(.clk(clk), .reset(reset), .a(hi), .b(n), .out(out2), .overflow());
//delay of 5 
mult mult_unit
(.clk(clk), .reset(reset), .a(lo2_reg), .b(n2_reg), .c(multo7));

//delay 2
L_mac mac_unit
(.clk(clk), .reset(reset), .c(out7_reg), .a(multo7), .b(16'd1), .out(outf));

always @(posedge clk or posedge reset)
    if (reset)
    begin
        out3_reg <= 0;
        out4_reg <= 0;
        out5_reg <= 0;
        out6_reg <= 0;
        out7_reg <= 0;
        lo1_reg <= 0;
        lo2_reg <= 0;
        n1_reg <= 0;
        n2_reg <= 0;
    end
    else
    begin
        n1_reg <= n;
        n2_reg <= n1_reg;
        lo1_reg <= lo;
        lo2_reg <= lo1_reg;
        out3_reg <= out2;
        out4_reg <= out3_reg;
        out5_reg <= out4_reg;
        out6_reg <= out5_reg;
        out7_reg <= out6_reg;
    end

endmodule
