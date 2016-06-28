`timescale 1 ns/ 10 ps

module iir_gen_test;

//signals
reg clk;
localparam T = 20;
localparam BUFF_SIZE = 1000;
localparam COEFF_SIZE = 3;
reg reset,load;
integer count,i;
reg signed [15:0] in, cina, cinb;
reg signed [15:0] xin [BUFF_SIZE-1:0];
reg signed [15:0] ca [COEFF_SIZE-1:0];
reg signed [15:0] cb [COEFF_SIZE-1:0];
reg [9:0] countx_in;
reg [2:0] count_coeff;
wire signed [15:0] out;

wire signed [31:0] outb1w,outc1w,del2w,del3w,outa1w,del0w;
wire signed [15:0] coeff_b0w;

my_iir_tran iir
(.clk(clk), .reset(reset), .load(load), .in(in), .cina(cina), .cinb(cinb), .out(out), .outb1w(outb1w), .outc1w(outc1w), .outa1w(outa1w), .del2w(del2w), .del3w(del3w), .coeff_b0w(coeff_b0w), .del0w(del0w) );

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
   $readmemh("inhex.hex",xin);
   $readmemh("coeffa.hex",ca);
   $readmemh("coeffb.hex",cb);
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
    else if (! load)
    begin
        if(count_coeff < COEFF_SIZE)
        begin
        cina = ca[count_coeff];
        cinb = cb[count_coeff];
        count_coeff = count_coeff + 1;
        end
        else
            load = 1;
    end
end    
endmodule

