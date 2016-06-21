function gen_input_data(n,filename)
%this will just generate some 
%dtmf tone in band limited noise in double
%and q14 fixed point
%filename is just name, this code will add .pcm extension
Fs=8000;
white_noise = wgn(n,1,0);
fd = 2000;
fu = 3000;
fn = Fs/2;
[b,a] = fir1(34,[fd fu]/fn);
band_lim_noise = filter(b,a,white_noise);

%create dtmf
%%create a dtmf tone
f=[770 1336];
Fs = 8000;
n = [0:999];
omega=2*pi*f/Fs;
dtmf = sin(omega(1)*n)+sin(omega(2)*n);
dtmf=dtmf/max(dtmf(:));

%add dtmf and noise together
sig = dtmf(:) + band_lim_noise(:);
filename_double = strcat(filename,'_double.pcm');
filename_fixed = strcat(filename,'_fixed.pcm');
%save to file
fid = fopen(filename_double,'wb');
count = fwrite(fid,sig,'double');
fprintf('\n%d bytes written to %s\n',count*8,filename_double);    
fclose(fid);
fid = fopen(filename_fixed,'wb');
count = fwrite(fid,int16(sig*16384),'int16');
fprintf('\n%d bytes written to %s\n',count*2,filename_fixed);    
fclose(fid);
