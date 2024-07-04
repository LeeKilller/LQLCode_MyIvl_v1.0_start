module op_exp0(a,b,c,d,e,q);
input a,b,c,d,e;
output q;
wire sum1,sum2;
assign sum1 = a + b + c;
assign sum2 = b + c + d;
assign q = sum1 & sum2 & sum3;
endmodule
