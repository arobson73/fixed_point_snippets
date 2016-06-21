function conv2hex(pcmfile,outhexfile)
%open and read the pcm file to be converted
fid = fopen(pcmfile,'rb');
if fid ~= -1
    datapcm = fread(fid,'int16');
    fprintf('\n%d bytes read from %s\n',length(datapcm)*2,pcmfile);
    fclose(fid);
end

%also generate input in hex text for the verilog
fid = fopen(outhexfile,'w');
for i=1:length(datapcm)
    %fprintf(fid,'%x ',conv2fix(i)); 
    %sprintf('%x ', typecast(int16(conv2fix(i)),'uint16'));
   fprintf(fid,'%x ', typecast(int16(datapcm(i)),'uint16'));
end
fclose(fid);
