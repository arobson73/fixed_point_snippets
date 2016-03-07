function [bits,mxx,fst16,snd16,snd32] = get_q_data(x)
%this will return range of numbers,
%max and min, and q in 32 and 16 bit
mx = max(x);
mi = min(x);
mxx = max(abs(x));
bits = log2(mxx);

%get 16 and 32 bit q
init = 32767;
q=15;
while true 
    val = mxx * init;
    if(val < 32767)
        break;
    end
    q=q-1;
    init=init/2;

end
%disp('16 bit q is')
fst16 = 15 - q;
snd16 = q;
%disp('32 bit q is')
fst32 = 15 -q;
snd32 = 16+q;
