function [b,a,y,yme] = script(filename)
%note this filter order is way too low to be effective
%its low order just to make it easy to implement in verilog
close all;
N=2;
Rp=0.5;
Rs=20;
Fs = 8000;
%bandpass filter
%fre = ([600 1600]/4000) ;
%lowpass filter
fre = 1400/4000;
[b,a] = ellip(N,Rp,Rs,fre);
h = fvtool(b,a);whitebg(gcf,'white');
%convert filter to direct form 2 transpose
%note transpose 2 form not easy
%to implement in verilog, try transpose 1 form
Hd = dfilt.df2t(b,a);
Hd2 = dfilt.df1t(b,a);
%note the coefficients are identical from ellip as coeffs from
%df2t
btat = cell2mat(Hd2.coefficients);

%open the input data

filename_double = strcat(filename,'_double.pcm');

fid = fopen(filename_double,'rb');
sigd = fread(fid,'double');
fprintf('\n%d bytes read from %s\n',length(sigd)*8,filename_double);
fclose(fid);
%look at spectrum of signal before filtering
figure(2);
pwelch(sigd,[],[],[],8000,'onesided');
whitebg(gcf,'w');
%sound(sigd, Fs);
%pause;
%filter the data
y = filter(Hd2,sigd);
size(y)
%look at spectrum after filtering
figure(3);
pwelch(y,[],[],[],8000,'onesided');
whitebg(gcf,'w');
%sound(y, Fs);

%compare matlab default filter with my own filter
yme = simple_tran_iir('indata')';
size(yme)
figure(4);
pwelch(yme,[],[],[],8000,'onesided');
whitebg(gcf,'w');

display('snr with default matlab and my own');
snr(y(:),y(:)-yme(:))

%compare the c double implementation results
fid = fopen('../c_imp/output_data.pcm','rb');
if fid ~= -1
    cdob = fread(fid,'double');
    fprintf('\n%d bytes read from %s\n',length(cdob)*8,'output_data.pcm');
    fclose(fid);
    display('snr with c double version');
    snr(y(:),y(:)-cdob(:))
else
display('could not open the c double output file');
end

%compare the c fixed implementation results
fid = fopen('../c_imp/output_data_fix.pcm','rb');
if fid ~= -1
    cfix = fread(fid,'int16');
    fprintf('\n%d bytes read from %s\n',length(cfix)*2,'output_data_fix.pcm');
    fclose(fid);
    display('snr with c fixed version');
    snr(y(:),y(:)-(cfix(:)/16384.0))
else
display('could not open the c double output file');
end






