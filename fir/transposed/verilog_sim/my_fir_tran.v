module my_fir_tran
(
    input clk,
    input reset,
    input load,
    input signed [15:0] in,
    input signed [15:0] cin_hi,
    input signed [15:0] cin_lo,
    output signed [15:0] out,
    //debug
    output signed [31:0] c0x9,c1x9,c2x9,c3x9,c3x10w, c3_c2_inw,c3_c2_c1_inw,
    output signed [31:0] c3_c2_out, c3_c2_c1_out,y_out     
);

parameter COEFF_SIZE = 4;
parameter DELAY_LINE_SIZE = COEFF_SIZE-1;
parameter COUNTA = 12;
parameter COUNTB = 14;
parameter COUNTC = 16;

integer k;
reg signed [15:0] coeff_hi [COEFF_SIZE-1:0];
reg signed [15:0] coeff_lo [COEFF_SIZE-1:0];
reg signed [15:0] in1,in2,in3,in4;
reg signed [31:0] c3x10;
reg signed [31:0] c3_c2_in,c3_c2_c1_in;
reg [4:0] counter0;
assign c3x10w = c3x10;
assign c3_c2_inw = c3_c2_in;
assign c3_c2_c1_inw = c3_c2_c1_in;
//wire signed [31:0] c3_c2_out,c3_c2_c1_out,y_out;

//wire signed [31:0] c3x9,c2x9,c1x9,c0x9;//,del_line0,del_line1,del_line2;


//debug

//coeff (32 bit or 2 16 bits * in (16 bit))
mpy_32_16 mult1
(.clk(clk), .reset(reset), .hi(coeff_hi[0]), .lo(coeff_lo[0]), .n(in4), .outf(c0x9));
mpy_32_16 mult2
(.clk(clk), .reset(reset), .hi(coeff_hi[1]), .lo(coeff_lo[1]), .n(in2), .outf(c1x9));
mpy_32_16 mult3
(.clk(clk), .reset(reset), .hi(coeff_hi[2]), .lo(coeff_lo[2]), .n(in), .outf(c2x9));
mpy_32_16 mult4
(.clk(clk), .reset(reset), .hi(coeff_hi[3]), .lo(coeff_lo[3]), .n(in), .outf(c3x9));

//round
roundB round
(.clk(clk), .reset(reset), .in(y_out), .out(out));


//adds
L_add add0
(.clk(clk), .reset(reset), .a(c3x10), .b(c2x9), .c(c3_c2_out), .overflow());
L_add add1
(.clk(clk), .reset(reset), .a(c3_c2_in), .b(c1x9), .c(c3_c2_c1_out), .overflow());
L_add add2
(.clk(clk), .reset(reset), .a(c3_c2_c1_in), .b(c0x9), .c(y_out),   .overflow());
always @(posedge clk or posedge reset)
    if(reset)
    begin
        in1 <= 0;
        in2 <= 0;
        in3 <= 0;
        in4 <= 0;
        counter0 <=0;
        c3x10 <= 0;
        c3_c2_in <= 0;
        c3_c2_c1_in <=0;
   end
    else if (! load)
    begin
        //high
        coeff_hi[3] <=cin_hi;
        coeff_hi[2] <= coeff_hi[3];
        coeff_hi[1] <= coeff_hi[2];
        coeff_hi[0] <= coeff_hi[1];
        //low
        coeff_lo[3] <= cin_lo;
        coeff_lo[2] <= coeff_lo[3];
        coeff_lo[1] <= coeff_lo[2];
        coeff_lo[0] <= coeff_lo[1];
    end
    else
    begin
        in1 <= in;
        in2 <= in1;
        in3 <= in2;
        in4 <= in3;
        if (counter0 < COUNTC)
        begin
            counter0 <= counter0 + 4'd1;
            c3_c2_c1_in <= 0;
        end
        else
            c3_c2_c1_in <= c3_c2_c1_out;

        if (counter0 < COUNTB)
        begin
            c3_c2_in <=0;
        end
        else
            c3_c2_in <= c3_c2_out;

        if(counter0 < COUNTA)
            c3x10 <= 0;
        else
            c3x10 <= c3x9;

    end


endmodule

