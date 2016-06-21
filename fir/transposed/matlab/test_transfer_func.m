function test_transfer_function(filename)
%this is mainly to see the affect of adding delays
%to a transfer function
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
b = cell2mat(Hd.coefficients);
%add delay of 1
bd1 = [0 b 0];

%fvtool(b,1,bd1,1); %magnitude response is still same

bd2 = [0 0 0 0 b(1) b(2) b(3) b(4)];
%this also is better less latency.
%bd2 = [0 0 0 b(1) b(2) b(3) b(4)];

fvtool(b,1,bd2,1); %magnitude response is still same
legend('orig','modded');

