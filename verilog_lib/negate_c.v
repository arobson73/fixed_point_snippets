module negate_c
(
    input signed [15:0] in,
    output signed [15:0] out
);

assign out = (in == -16'd32768) ? 16'd32767 : -in; 

endmodule
