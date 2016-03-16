#ifndef __FREQ_SEL_FILTER__
#define __FREQ_SEL_FILTER__
#include "typedef.h"

typedef struct
{
    int N;//number of samples around unit circle
    int k;//index of 2*pi*k/N
    double Nr;//1/N
    double rp;// r^N
    double mul;//(1/N) * (r^N)
    double cm;//cos(2*pi*k/N)
    double r2;//r*r - r used to keep poles inside unit circle
    double* cbuffer; //delay line comb
    double yfb[2];//delay line resonator
    double* yc;//resonator output buffer
    
}Fsf_data;

void freqSelectiveFilterFloat(Word16* pbuff,int frame_size,Fsf_data* f_data);
#endif
