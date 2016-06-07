function [in,out] = check_debug_data(file1,file2,type)
%this will get the sdxin and sdx ref
%INPUT : file1, file2, type
fp = fopen(file1,'r');
in = fread(fp,type);
fclose(fp);
fp = fopen(file2,'r');
out = fread(fp,type);

%check figure exists

tf = isempty(findall(0,'Type','Figure'));
fig = gcf;
new_fig = fig.Number;
if ~tf
    new_fig = new_fig +1;
end


figure(new_fig);subplot(211);plot(in,'.r');subplot(212);plot(out,'.r');whitebg(gcf,'k');

ref = in;
non_ref = out;
snr(ref,ref-non_ref)
