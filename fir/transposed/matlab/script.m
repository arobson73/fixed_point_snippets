function [y,coeffs,coeffsQ15,coeffsQ16,coeffsQ31] = script(filename)
%main script
%filename is name of input data without the pcm extension

%get the input data
filename_double = strcat(filename,'_double.pcm');
%filename_fixed = strcat(filename,'_fixed.pcm');

fid = fopen(filename_double,'rb');
sigd = fread(fid,'double');
fprintf('\n%d bytes read from %s\n',length(sigd)*8,filename_double);
fclose(fid);

%create a low pass filter to remove the higher frequency noise
%obviously this filter will be crap since its only
%has 4 coefficients. This is intended so we can have
%a simple verilog model (less coding), which could then be extended 
%to have man more coeffcients if necessary

fre = ( [0 1500 1700 4000]/4000) ;
msk = [1 1 0 0];
b = firpm(3,fre,msk);

%convert filter to direct form transposed and filter the noisy signal
Hd = dfilt.dffirt(b);
coeffs = cell2mat(Hd.coefficients);
coeffsQ15 = int16(coeffs*32767);
coeffsQ31 = int32(coeffs*(2^31-1));
coeffsQ16= int16(coeffs*(2^16-1));
y = filter(Hd,sigd);

%read the results from the c float code if any
filename = '../c_imp/output_data.pcm';
fid = fopen(filename,'rb');
if fid ~= -1
    resd = fread(fid,1000,'double');
    fprintf('\n%d bytes read from %s\n',1000*8,filename); 
    %get power spectrum of c float version
    fclose(fid);
end

%check snr of c float
display('snr c float');
snr(y,y-resd)

%read the rsults from the c fixed code if any
filename = '../c_imp/output_data_fix.pcm';
fid = fopen(filename,'rb');
if fid ~= -1
    resf = fread(fid,1000,'int16');
    resf = resf/16384.0;
    fprintf('\n%d bytes read from %s\n',1000*2,filename); 
    %get power spectrum of c float version
    fclose(fid);
end

display('snr c fixed');
snr(y,y-resf)







