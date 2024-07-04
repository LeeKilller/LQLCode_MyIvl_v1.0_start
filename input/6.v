module op_exp0(a,b,c,d,e,q);
input a,b,c,d,e;
output q;
wire sum1,sum2;
assign sum1 = c + b * a;
assign sum2 = d + b * a;
assign sum3 = e + b * a;
assign q = sum1 & sum2 & sum3;
endmodule
