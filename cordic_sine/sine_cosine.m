function [x,y,z,s,c] = sine_cosine(N,ang)
%compute sine and cosine for a given angle
%N is number of iterations
%ang is the input angle between -pi/2 and pi/2
%s is the sine result for the given angle
%c is the cosine result for the given angle

%create LUT for arctan (2^ -i)
lut = atan(2 .^ -(0:N-1));
%set intial values 
z(1) = ang;
x(1) = 1/(prod(sqrt(1 + (2.^ -((0:N-1)*2)))));
y(1) = 0;
%iterate N-1 times
for i= 1:N-1
    if z(i) < 0
        x(i+1) = x(i) + bitsra(y(i),(i-1));
        y(i+1) = y(i) - bitsra(x(i),(i-1));
        z(i+1) = z(i) + lut(i);
    else
        x(i+1) = x(i) - bitsra(y(i),(i-1));
        y(i+1) = y(i) + bitsra(x(i),(i-1));
        z(i+1) = z(i) - lut(i);
    end 
    
    
end
c = x(N);
s = y(N);
