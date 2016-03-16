function [ycf,yt] = test_filter(filename,k,N,range,frame_length)

x = plot_test_data(filename,range);
figure(2);
[h,w] = check_freq_response(x(range),1024);
title('frequency response of noisy sine signal');

%create the FSF filter
[bc,br,b,a] = freq_selective_filter(N,k);
figure(3);
subplot(311);
[hf,wf] = freqz(b,a);
plot(wf/pi,20*log10(abs(hf)),'.g');
title('frequency response of FSF filter');
subplot(312);
plot(wf/pi,angle(hf), '.g');
title('phase response of FSF filter');
subplot(313);
zplane(b,a);

title('zplane of FSF filter');
whitebg(gcf,'k');

%filter the comb first
Hd1 = dfilt.dffir(bc);
%freqz(Hd1);
%whitebg(gcf,'k');
%filter the resonator next
Hd = dfilt.df2(br,a);
%freqz(Hd);
%whitebg(gcf,'k');
Hcas = dfilt.cascade(Hd1,Hd);
%freqz(Hcas);
%whitebg(gcf,'k');
yt = filter(Hcas,x);
%figure(7);plot(yt(1:255));whitebg(gcf,'k');
        
%check the filtered signal frequency response
y = yt(range);
figure(4);
[h1,w1] = check_freq_response(y,1024);
title('frequency response of filtered signal');

%plot the filtered signal from c floating point code
ycf = get_script('sdx.ref','double','double',range);
%plot the filtered signal
figure(6);
%plot(range,y,'g',range,x(range),'r');whitebg(gcf,'k');
plot(range,y,'g',range,ycf(range),'r');whitebg(gcf,'k');
legend('matlab', 'c float');
title('plot of filtered signal from matlab sim and c float');


%plot the frequency response of c float filtered signal
figure(7);
[hc,wc] = check_freq_response(ycf(range),1024);
end
