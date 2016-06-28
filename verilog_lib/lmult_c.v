//same as lmult but this is combinatorial version
//note if the code in c was tested for overflow
//then no need to have this saturation check
module lmult_c 
(
    input signed [15:0] a, //high part of 32 bit num
    input signed [15:0] b, //low part of 32 bit num
    output signed [31:0] c 
); 
wire signed [31:0] outb;
assign outb = (a * b);
assign c = (outb != 32'h40000000) ? outb << 1 : 32'h7fffffff;

endmodule
