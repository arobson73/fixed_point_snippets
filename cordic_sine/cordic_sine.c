#include <stdio.h>
#include "typedef.h"
#include "testing.h"
#include "basic_op.h"
#include "cordic_sine.h"
#if 0
//this gives snr of 153.51db for 5000 random
//samples between -pi/2 and pi/2, with N=25 iterations
void cordicSine(Word16* pbuff)
{
    int i;
    int N = 25;//number of iterations
    /*q30 lut data*/
    Word32 atan_lut[25] = {
        843314856,
        497837829,
        263043836,
        133525158,
        67021686,
        33543515,
        16775850,
        8388437,
        4194282,
        2097149,
        1048575,
        524287,
        262143,
        131071,
        65535,
        32767,
        16383,
        8191,
        4095,
        2047,
        1023,
        511,
        255,
        127,
        63};
    Word32 x0 = 1304065748L;/*q31*/
    Word32 y0 = 0L;
    Word32 z0 = L_deposit_h(pbuff[0]);/*q14 to q30*/
    Word32 x1,y1,z1;

#ifdef TESTING
    static of_count cord;
#endif 

    for(i = 0;i < N-1; i++)
    {
        if (z0 < (Word32)0)
        {
            x1 = L_add(x0,L_shr(y0,i));
            y1 = L_sub(y0,L_shr(x0,i));
            z1 = L_add(z0,atan_lut[i]);
        }
        else
        {
            x1 = L_sub(x0,L_shr(y0,i));
            y1 = L_add(y0,L_shr(x0,i));
            z1 = L_sub(z0,atan_lut[i]);
        }
        x0=x1;
        y0=y1;
        z0=z1;
#ifdef TESTING
        overflow_count("a",__func__,&cord);
#endif

    }
#ifdef TESTING
    de_norm(testscrfp,&y0,1,0 ,31,CORDIC,1);/**/
    de_norm(testscrfp,&x0,1,0 ,31,CORDIC,1);/**/
#endif

}

#endif

//this gives about 71.52 db using q14 data
void cordicSine(Word16* pbuff)
{
    int i;
    int N = 14;//number of iterations
    /*q30 lut data*/
    Word16 atan_lut[14] = {
        12867,
        7596,
        4013,
        2037,
        1022,
        511,
        255,
        127,
        63,
        31,
        15,
        7,
        3,1};

    Word16 x0 = 9949;/*q14*/
    Word16 y0 = 0;
    Word16 z0 = pbuff[0];/*q14*/
    Word16 x1,y1,z1;

#ifdef TESTING
    static of_count cord;
#endif 

    for(i = 0;i < N-1; i++)
    {
        if (z0 < (Word16)0)
        {
            x1 = add(x0,shr(y0,i));
            y1 = sub(y0,shr(x0,i));
            z1 = add(z0,atan_lut[i]);
        }
        else
        {
            x1 = sub(x0,shr(y0,i));
            y1 = add(y0,shr(x0,i));
            z1 = sub(z0,atan_lut[i]);
        }
        x0=x1;
        y0=y1;
        z0=z1;
#ifdef TESTING
        overflow_count("a",__func__,&cord);
#endif

    }
#ifdef TESTING
    de_norm(testscrfp,&y0,1,0 ,14,CORDIC,0);/**/
    de_norm(testscrfp,&x0,1,0 ,14,CORDIC,0);/**/
#endif

}
