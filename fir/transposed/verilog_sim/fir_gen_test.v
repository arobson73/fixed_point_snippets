`timescale 1 ns/ 10 ps

module fir_gen_test;

//signals
reg clk;
localparam T = 20;
localparam BUFF_SIZE = 1000;
localparam COEFF_SIZE = 4;
reg reset,load;
integer count,i;
reg signed [15:0] in, cin_hi, cin_lo;
reg signed [15:0] xin [BUFF_SIZE-1:0];
reg signed [15:0] chigh [COEFF_SIZE-1:0];
reg signed [15:0] clow [COEFF_SIZE-1:0];
reg [9:0] countx_in;
reg [2:0] count_coeff;
wire signed [15:0] out;
wire signed [31:0] c0x9,c1x9,c2x9,c3x9,c3x10w,c3_c2_inw,c3_c2_c1_inw;
wire signed [31:0] c3_c2_out, c3_c2_c1_out,y_out;

my_fir_tran fir
(.clk(clk), .reset(reset), .load(load), .cin_hi(cin_hi), .cin_lo(cin_lo), .in(in), .out(out), .c0x9(c0x9), .c1x9(c1x9), .c2x9(c2x9), .c3x9(c3x9), .c3x10w(c3x10w), .c3_c2_inw(c3_c2_inw), .c3_c2_c1_inw(c3_c2_c1_inw), .c3_c2_out(c3_c2_out), .c3_c2_c1_out(c3_c2_c1_out) , .y_out(y_out));

//clock
//
always
begin
    clk = 1'b1;
    #(T/2);
    clk = 1'b0;
    #(T/2);
end

initial
begin
    reset = 1'b1;
    #(T/2);
    reset = 1'b0;
end

initial
begin
    
   load=0;
   $readmemh("input_data_fix.hex",xin);
   $readmemh("out_hi.hex",chigh);
   $readmemh("out_lo.hex",clow);
   countx_in =0;
   count_coeff=0;
  
end

always @(posedge clk)
begin
    
    if (countx_in < (BUFF_SIZE-1) && count_coeff == COEFF_SIZE && load==1)
    begin
        in = xin[countx_in];
        countx_in = countx_in +1;
        if (countx_in == (BUFF_SIZE-1))
            $stop;
    end
    if (! load)
    begin
        if(count_coeff < COEFF_SIZE)
        begin
        cin_hi = chigh[count_coeff];
        cin_lo = clow[count_coeff];
        count_coeff = count_coeff + 1;
        end
        else
            load = 1;
    end
    
end
endmodule

