module mult
(
    input clk,
    input reset,
    input signed [15:0] a,
    input signed [15:0] b,
    output signed [15:0] c
);

reg signed [31:0] prod1;
reg signed [31:0] prod2;
reg signed [31:0] prod3a,prod3b;
wire signed [31:0] prod3w;
reg check3;

//instantiate saturate
//delay of 2, so mult has total delay of 5
saturate saturate_unit
(.clk(clk), .reset(reset), .in(prod3w), .out(c));

always @(posedge clk or posedge reset)
    if (reset)
    begin
        prod1 <= 0;
        prod2 <= 0;
        prod3a <= 0;
        prod3b <= 0;
        check3 <= 0;
    end
    else
    begin
        prod1 <= a * b;
        prod2 <= ((prod1 & 32'hffff8000) >> 15); 
        prod3a <= prod2 | 32'hffff0000;
        prod3b <= prod2;
        check3 <= (prod2 & 32'h00010000) != 32'h00000000;
    end

assign prod3w = check3 ? prod3a : prod3b;

endmodule
