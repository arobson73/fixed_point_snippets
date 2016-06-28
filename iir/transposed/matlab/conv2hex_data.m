function conv2hex_data(data, outfile)
%take a integer vector and convert to hex for verilog
%simulator use
fid  = fopen(outfile,'w');
if fid ~= -1
    for i=1:length(data)
       fprintf(fid,'%x ', typecast(int16(data(i)),'uint16'));
    end
    fclose(fid);
end
