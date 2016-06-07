%this is a test script that creates a noisy signal for 
%processing by a low pass transposed filter. 
%the output result will remove some / most of 
%the noise. the signal of interest is 
%in a low portion of the spectrum compared
%to the noise, so can be easily removed with
%lowpass filtering. This is just to demonstrate
%lowpass filtering with transposed form filter


close all;
clear all;
%create some white noise 
white_noise = wgn(1000,1,0); %1000 samples at 1 watt
figure(1);whitebg(gcf,'k');
pwelch(white_noise,[],[],[],8000,'onesided');

%band limited noise operates between 2k and 3 k
fd = 2000;
fu = 3000;
fs = 8000; %sampling frequency
fn = fs/2;
%create band pass filter for this noise
[b,a] = fir1(34,[fd fu]/fn);
figure(2);
%check its frequency response
freqz(b,a,512);
band_limited_noise = filter(b,a,white_noise);
whitebg(gcf,'k');
figure(3);
%plot band limited noise spectrum
pwelch(band_limited_noise,[],[],[],8000,'onesided');
whitebg(gcf,'k');

%%create a dtmf tone
f=[770 1336];
Fs = 8000;
n = [0:999];
omega=2*pi*f/Fs;
dtmf = sin(omega(1)*n)+sin(omega(2)*n);
dtmf=dtmf/max(dtmf(:));
%listen to it
sound(dtmf, Fs);
%check dtmf power spectrum
figure(4);
pwelch(dtmf,[],[],[],8000,'onesided');
whitebg(gcf,'k');
size(dtmf)
size(band_limited_noise)
%add the dtmf and noise together
sig = dtmf(:) + band_limited_noise(:);
%check its power spectrum
figure(5);
pwelch(sig,[],[],[],8000,'onesided');
whitebg(gcf,'k');

%quick listen to the noisy signal
%sound(sig,Fs);

%create a low pass filter to remove the higher frequency noise
fre = ( [0 1500 1700 4000]/4000) ;
msk = [1 1 0 0];
b = firpm(40,fre,msk);
figure(6);
freqz(b,1,2048);
%convert filter to direct form transposed and filter the noisy signal
Hd = dfilt.dffirt(b);
y = filter(Hd,sig);
figure(7);
pwelch(y,[],[],[],8000,'onesided');whitebg(gcf,'k');
%now check the filtered power spectrum

sound(y, Fs); %sounds ok
filename = 'input_data.pcm';
%now save the input data for the c code inplementation
fid = fopen(filename,'wb');   % Save signal to xn_int.dat
count = fwrite(fid,sig,'double'); %
fprintf('\n%d bytes written to %s\n',count*8,filename);    
fclose(fid);

%read the results from the c float code if any
filename = 'output_data.pcm';
fid = fopen(filename,'rb');
if fid ~= -1
    data = fread(fid,1000,'double');
    fprintf('\n%d bytes read from %s\n',1000*8,filename); 
    %get power spectrum of c float version
    figure(8);
    pwelch(data,[],[],[],8000,'onesided');whitebg(gcf,'k');
end
fclose(fid);

%also for the fixed point version data in needs to be q14
filename = 'input_data_fix.pcm';
fid =fopen(filename,'wb');
count = fwrite(fid,int16(sig*16384),'int16');
fprintf('\n%d bytes written to %s\n',count*2,filename); 
fclose(fid);

%check the fixed point results
filename = 'output_data_fix.pcm'
fid = fopen(filename,'rb');
if fid ~= -1
    dataf = fread(fid,1000,'int16');
    dataf = dataf/16384.0 %convert from q14 to double
    fprintf('\n%d bytes read from %s\n',1000*2,filename); 
    %get power spectrum of c float version
    figure(9);
    pwelch(dataf,[],[],[],8000,'onesided');whitebg(gcf,'k');
end
fclose(fid);

%check snr
display('snr');
snr(data,data-dataf)

