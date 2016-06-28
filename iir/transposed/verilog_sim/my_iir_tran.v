module my_iir_tran
(
    input clk,
    input reset,
    input load,
    input signed [15:0] in,
    input signed [15:0] cina,
    input signed [15:0] cinb,
    output signed [15:0] out,
    //debug
    output signed [31:0] outb1w,
    output signed [31:0] outc1w,
    output signed [31:0] outa1w,
    output signed [31:0] del2w,
    output signed [31:0] del3w,
    output signed [15:0] coeff_b0w,
    output signed [31:0] del0w


);
//not this is not pipelined, needs a combinatoril
//multiply and add combo


integer k;
parameter COEFF_SIZE = 3;
parameter DELAY_LINE_SIZE = COEFF_SIZE-1;
reg signed [15:0] outc1_reg;
reg signed [15:0] coeff_a [COEFF_SIZE-1:0];
reg signed [15:0] coeff_b [COEFF_SIZE-1:0];
reg signed [31:0] delay_line[DELAY_LINE_SIZE-1:0];
wire signed [31:0] outa1,outb1,outa2,outb2,outc2,del2,del3;
wire signed [31:0] outa3,outb3;
wire signed [15:0] outc1,outn;
reg [1:0] counter;

//debug
assign outa1w = outa1;
assign outb1w = outb1;
assign outc1w = outc1;
assign del2w = del2;
assign del3w = del3;
assign coeff_b0w = coeff_b[0];
assign del0w = delay_line[0];

//instantiate
lmult_c multa1
(.a(coeff_b[0]), .b(in), .c(outa1));
L_add_c adda1
(.a(delay_line[0]), .b(outa1), .c(outb1));
roundB_c rounda1
(.in(outb1), .out(outc1));

negate_c negate
(.in(outc1), .out(outn));
lmult_c multa2
(.a(coeff_b[1]), .b(in), .c(outa2));
lmult_c multb2
(.a(outn), .b(coeff_a[1]), .c(outb2));
L_add_c adda2
(.a(outa2), .b(outb2), .c(outc2));
L_add_c addb2
(.a(delay_line[1]), .b(outc2), .c(del2));

lmult_c multa3
(.a(coeff_b[2]), .b(in), .c(outa3));
lmult_c multb3
(.a(outn), .b(coeff_a[2]), .c(outb3));
L_add_c adda3
(.a(outa3), .b(outb3), .c(del3));

assign out = outc1_reg;
always @(posedge clk or posedge reset)
begin
    if(reset)
    begin
        outc1_reg <= 0;
        for(k=0;k < DELAY_LINE_SIZE; k=k+1) delay_line[k] <= 0;        
        counter <= 0;
    end
    else if (! load)
    begin
        //high
        coeff_a[2] <=cina;
        coeff_a[1] <= coeff_a[2];
        coeff_a[0] <= coeff_a[1];
        //low
        coeff_b[2] <= cinb;
        coeff_b[1] <= coeff_b[2];
        coeff_b[0] <= coeff_b[1];
    end
    else
    begin
        if (counter < 1'b1)
        begin
            delay_line[0] <= 0;
            delay_line[1] <= 0;
            counter <= counter + 1'b1;
        end
        else
        begin
            delay_line[0] <= del2;
            delay_line[1] <= del3;
            outc1_reg <= outc1;
        end

    end
end

endmodule

