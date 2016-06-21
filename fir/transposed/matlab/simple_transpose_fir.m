function simple_transpose_fir(filename)

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
del_line = zeros(1,length(c)-1);
for i = 1: length(x)
    y(i) = x(i)*c(1) + del_line(3);
    del_line(3) = x(i)*c(2) + del_line(2);
    del_line(2) = x(i)*c(3) + del_line(1);
    del_line(1) = x(i)*c(4);

end

%do matlab filtering
yy = filter(Hd,x);
%check snr
display('snr of matlab and my simple implementation');
snr(yy(:),yy(:)-y(:))
