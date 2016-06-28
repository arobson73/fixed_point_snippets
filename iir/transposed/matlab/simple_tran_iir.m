function y = simple_tran_iir(filename)
%simple iir transpose form

%open the input data
filename_double = strcat(filename,'_double.pcm');
fid = fopen(filename_double,'rb');
sigd = fread(fid,'double');
fprintf('\n%d bytes read from %s\n',length(sigd)*8,filename_double);
fclose(fid);

del_line = zeros(1,2);
b =[0.271275515076715 0.376532612594379 0.271275515076715];
a = [1 -0.343320701630157 0.316863473960287];

x=sigd;
% 303 db
for i = 1: length(x)
    y(i) = b(1) * x(i) + del_line(1);
    del_line(1) = -y(i)*a(2) + b(2)*x(i) + del_line(2);
    del_line(2) = -y(i)*a(3) + b(3)*x(i);
end
if 0
x_del = zeros(1,2);
y_del = zeros(1,2);
% 303 db
for i=1:length(x)
    y(i) = b(1)*x(i) - a(2)* y_del(1) + b(2)* x_del(1) - a(3)*y_del(2) + b(3)*x_del(2);
    y_del(2) = y_del(1);
    y_del(1) = y(i);
    x_del(2) = x_del(1);
    x_del(1) = x(i);
end
end

if 0
%try to make these 2 more pipelined
for i = 1: length(x)
    y(i) = b(1) * x(i) + del_line(1); %1 clk cycle

    del_line(1) = -y(i)*a(2) + b(2)*x(i) + del_line(2);
    del_line(2) = -y(i)*a(3) + b(3)*x(i);
end
end
