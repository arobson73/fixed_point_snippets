module L_add
(
    input clk,
    input reset,
    input signed [31:0] a,
    input signed [31:0] b,
    output signed [31:0] c,
    output overflow
);

//delay of 2
reg signed [31:0] s1_reg,s2_reg,a1_reg,a2_reg;
reg iffa1_reg,iffb2_reg,iffa2_reg;
wire iffa,iffb1;
wire signed [31:0] out2o;

always @(posedge clk or posedge reset)
    if (reset)
    begin
        s1_reg <=0;
        s2_reg <=0;
        a1_reg <=0;
        a2_reg <=0;
        iffa1_reg <=0;
        iffb2_reg <=0;
        iffa2_reg <=0;
    end
    else
    begin
        s1_reg <= a + b;
        s2_reg <= s1_reg;
        a1_reg <= a;
        a2_reg <= a1_reg;
        iffa1_reg <= iffa;
        iffb2_reg <= iffb1;
        iffa2_reg <= iffa1_reg;
    end

assign iffa = (((a ^ b) & 32'h80000000) == 0);    
assign iffb1 = (((s1_reg ^ a1_reg) & 32'h80000000) != 0);
assign out2o = (a2_reg < 0) ? 32'h80000000 : 32'h7fffffff;
assign overflow = (iffa2_reg && iffb2_reg);
assign c = overflow ? out2o : s2_reg;
endmodule
