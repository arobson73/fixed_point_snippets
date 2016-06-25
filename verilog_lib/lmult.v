//this will multiply a 32bit number
//(represented as hi , low) by a 16 bit 
//number
module lmult 
(
    input clk,
    input reset,
    input signed [15:0] a, //high part of 32 bit num
    input signed [15:0] b, //low part of 32 bit num
    output signed [31:0] out,
    output overflow
);

reg signed [31:0] a1;
reg signed [31:0] a2, aa2;

always @(posedge clk or posedge reset)
    if (reset)
    begin
        a1 <= 0;
        a2 <= 0;
        aa2 <= 0;
    end
    else
    begin
        //a1 <= a * $signed({1'b0,b});
        a1 <= a * b;
        aa2 <= (a1 * 2);
        a2 <= a1;
    end

assign out = (a2 != 32'h40000000) ? aa2 : 32'h7fffffff;
assign overflow = (a2 == 32'h40000000);

endmodule
