function [x_int] = gen_noisey_sine(filename,k,N)
%k is index from omega (2*pi*k/N).
%N is number of samples around unit circle
omega = 2*pi *k/N;
%create a sinewave with some noise added
n= 0:6000;
sine = sin(omega*n);
noi = sqrt(0.1) .*randn(1,length(n));%noisy sine with variance 0.1
x = sine+noi;
x_int = round(32768*x./max(abs(x)));
plot(1:255,x_int(1:255),'g'); axis([1 256 -32768 32767]);

ylabel('Amplitude'); xlabel('Sample index, n');
whitebg(gcf,'k');
fid = fopen(filename,'wb');   % Save signal to xn_int.dat
count = fwrite(fid,x_int,'int16');
fprintf('\n%d bytes written to %s\n',count*2,filename);    
fclose(fid);
