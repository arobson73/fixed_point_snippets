function dat = plot_test_data(filename, range)

fid = fopen(filename,'rb');
dat = fread(fid,inf,'int16'); 
fclose(fid);
%check figure exists
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

figure(next_fig);
if length(range) > 1
    plot(dat(range),'r');
else
    plot(dat,'.r');
end
title('Input test Data');
whitebg(gcf,'k');
