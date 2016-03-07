function d = generate_angle_data(N,mi,mx,filename)
%N is number of samples to generate
%mi is minimum angle in radians
%mx is maximum angle in radians

d = mi + (mx-mi).*rand(N,1);

%convert to q14
d14 = fix(d*2^14);

%write to file
fid = fopen(filename,'wb');
count = fwrite(fid,d14,'int16');
fprintf('\n%d bytes written to %s\n',count*2,filename);    
