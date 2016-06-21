function simple_transpose_fir_delayed(filename)
%this filter we try to replicate the verilog
%pipeline issue where our add part involves
%using the xin * coeff + delay_line[i] = delay_line[i+1]
%ideally this would be no problem if the addition
%was done in one clock cycle, but if our addition takes
%more than one clock cycle, then we need to change
%the filter structure add more delay lines basically.

close all;
%get the input data
filename_double = strcat(filename,'_double.pcm');
%filename_fixed = strcat(filename,'_fixed.pcm');

fid = fopen(filename_double,'rb');
x = fread(fid,'double');
fprintf('\n%d bytes read from %s\n',length(x)*8,filename_double);
fclose(fid);

%this is an fir transpose filter
fre = ( [0 1500 1700 4000]/4000) ;
msk = [1 1 0 0];
b = firpm(3,fre,msk);

%convert filter to direct form transposed and filter the noisy signal
Hd = dfilt.dffirt(b);
c = cell2mat(Hd.coefficients);
del_line = zeros(1,7);
del_line_top = zeros(1,4);
for i = 1: length(x)
    y(i) = c(1)*del_line_top(4) + del_line(7);
    del_line(7) = del_line(6);
    del_line(6) = del_line(5);
    del_line(5) = c(2)*del_line_top(2) + del_line(4);
    del_line(4) = del_line(3);
    del_line(3) = del_line(2);
    del_line(2) = c(3)*x(i) + del_line(1);
    del_line(1) = c(4) *x(i);
    del_line_top(4) = del_line_top(3);
    del_line_top(3) = del_line_top(2);
    del_line_top(2) = del_line_top(1);
    del_line_top(1) = x(i);
end
%do matlab filtering
yy = filter(Hd,x);
%check snr
display('snr of matlab and my simple implementation');
snr(yy(:),yy(:)-y(:))
range=200
figure(1);
plot(1:range,yy(1:range), '-')
hold on
plot(1:range,y(1:range),'')
whitebg(gcf,'k');
