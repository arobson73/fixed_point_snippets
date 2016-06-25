module saturate
(
    input clk,
    input reset,
    input signed [31:0] in,
    output signed [15:0] out
);

reg signed [15:0] out_reg1;
reg signed [15:0] out_rega2,out_regb2;
reg choice_reg1, choice_reg2;
wire signed [15:0] out_e1;
wire choice0a,choice0b;

//instantiate
extract_l extract_l_unit
(.clk(clk), .reset(reset), .in(in), .out(out_e1));

always @(posedge clk or posedge reset)
    if (reset)
    begin
        out_reg1 <= 0;
        out_rega2 <= 0;
        out_regb2 <= 0;
        choice_reg1 <= 0;
        choice_reg2 <= 0;
    end
    else
    begin
        if (choice0a)
        begin
            out_reg1 <= 16'h7fff;
            choice_reg1 <= 1;
        end
        else if(choice0b) 
        begin
            out_reg1 <= 16'h8000;
            choice_reg1 <= 1;
        end
        else
        begin
            out_reg1 <= 0;
            choice_reg1 <= 0;
        end

        choice_reg2 <= choice_reg1;
        out_rega2 <= out_reg1;
        out_regb2 <= out_e1;

    end


assign out = (choice_reg2) ? out_rega2 : out_regb2;    
assign choice0a = (in > 32767);
assign choice0b = (in < -32768);

endmodule  
