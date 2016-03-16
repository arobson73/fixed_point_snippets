#ifndef __FIXED_FREQ_SEL_FILTER__
#define __FIXED_FREQ_SEL_FILTER__
#include "typedef.h"

typedef struct
{
    int N;//number of samples around unit circle
    int k;//index of 2*pi*k/N
    Word16 Nr;//1/N
    Word16 rp;// r^N
    Word16 mul;//(1/N) * (r^N)
    Word16 cm;//cos(2*pi*k/N)
    Word16 r2;//r*r - r used to keep poles inside unit circle
    Word16* cbuffer; //delay line comb
    Word16 yfb[2];//delay line resonator
    Word16* yc;//resonator output buffer
    
}Fsf_fdata;

void freqSelectiveFilterFixed(Word16* pbuff,int frame_size,Fsf_fdata* fdata);
#endif
