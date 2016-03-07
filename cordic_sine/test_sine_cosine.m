%test sine cosine

N=14;%number of iterations

fp = fopen('test_data.bin','r');
in1 = fread(fp,'int16');
fclose(fp);

if length(in1) == 0
    disp('Empty file');
    return;
end


res =[];
for i=1:length(in1)
    [x,y,z,s(i),c(i)] = sine_cosine(N,in1(i)/16384);
    res = [res s(i) c(i)];
end
%get the q data
[bits,mxx,fst16,snd16,snd32] = get_q_data(res);



%plot the float data

%check figure exists
fig_info = findall(0,'Type','Figure');
num_figs = size(fig_info,1);
indx=zeros(1,100);;%assume there is never 100 figures open
%get  used figure indexs
for i=1:num_figs
    indx(fig_info(i).Number) =1; 
end
%get first unused index
next_fig=1;
for i = 1:length(indx)
    if indx(i) == 0
        next_fig=i;
        break;
    end

end
figure(next_fig);plot(res,'.r');whitebg(gcf,'k');
title(['float',',bits=',num2str(bits),',max=',num2str(mxx),',Q',num2str(fst16),'.',num2str(snd16),'or Q',num2str(fst16),'.',num2str(snd32)],'Color','w');

fp = fopen('sdx.script','r');
out = fread(fp,'double');
fclose(fp);


if length(out) == 0
    disp('Empty file');
    return;
end

%get the q data
[bits,mxx,fst16,snd16,snd32] = get_q_data(out);

%check figure exists
fig_info = findall(0,'Type','Figure');
num_figs = size(fig_info,1);
indx=zeros(1,100);;%assume there is never 100 figures open
%get  used figure indexs
for i=1:num_figs
    indx(fig_info(i).Number) =1; 
end
%get first unused index
next_fig=1;
for i = 1:length(indx)
    if indx(i) == 0
        next_fig=i;
        break;
    end

end

figure(next_fig);plot(out,'.r');whitebg(gcf,'k');
title(['fixed',',bits=',num2str(bits),',max=',num2str(mxx),',Q',num2str(fst16),'.',num2str(snd16),'or Q',num2str(fst16),'.',num2str(snd32)],'Color','w');

%check the snr
fprintf('SNR is %12.8f db\n',snr(res,res - out'));

