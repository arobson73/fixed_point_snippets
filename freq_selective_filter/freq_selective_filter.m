function [bc,br,b,a] = freq_selective_filter(N,k)
%this will produce a bandpass filter  (FSF) which is a
%comb filter in cascade with a resonator.
%see lyons dsp book for explanation

%k is the radians index which gives 2*pi*k /N 
%N is the number of sample point around unit circle
%hence is k = 3 and N= N = 32, then if k =4, then
%the bandpass would be located half way between fs/2 and 0

%this gives single section complex FSF 
if 0
b = [1 zeros(1,N-1) -1] * (1/N); % comb filter
a = [1 -exp(j*omega) zeros(1,N-1)];%single section resonator
end


%this is gives single section FSF real response

%resonator part- make sure poles are inside the unit circle
r = 0.9999;

bc = [1 zeros(1,N-1) -1] * (1/N) * r^N; %comb filter
br = [1 -r*cos(-2*pi*k/N)]*2; %resonator b coeffs
b = conv(bc ,br);% since comb and resonator b coeffs are in cascade, then convolve
a = [1 -2*r*cos(2*pi*k/N) (r^2)];%resonator a coeffs

if 0
figure(1);
freqz(b,a,512);
whitebg(gcf,'k');
figure(2);
zplane(b,a,512);

whitebg(gcf,'k');
end

end
