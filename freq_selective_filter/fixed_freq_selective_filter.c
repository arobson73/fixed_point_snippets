#include <stdio.h>
#include "typedef.h"
#include "basic_op.h"
#include "testing.h"
#include "fixed_freq_selective_filter.h"

void freqSelectiveFilterFixed(Word16* pbuff, int frame_size, Fsf_fdata* fdata)
{
    int i,j;
    Word32 yc32,acc0,acc1;
    Word16 res,yc;
#ifdef TESTING
    static of_count fsf0,fsf1,fsf2,fsf3;
#endif 

    for(i=0; i < frame_size; i++)
    {
        res = mult_r(pbuff[i],fdata->mul);/*15,0 * 0,20 => 15,21, really 10,21=>10,5*/
#ifdef TESTING_NO
        de_norm(testinfp,&res,1, 0, 5, FSF,0);/*91.15 db*/
#endif
        fdata->cbuffer[0] =res;
        yc = sub(res,fdata->cbuffer[fdata->N]);
#ifdef TESTING_NO
        de_norm(testinfp,&yc,1, 0, 5, FSF,0);/*84.02db*/
#endif

        for(j=fdata->N; j >= 1 ; j--)
        {
            fdata->cbuffer[j] = fdata->cbuffer[j-1];
        }
#ifdef TESTING
        overflow_count("a",__func__,&fsf0);
#endif

        /*
         * resonator feedback part
        ur = yc +  (f_data->cm * f_data->yfb[0]) - (f_data->r2 * f_data->yfb[1]);*/
        yc32 =  L_mult(yc,(Word16)512);/*10,5 *2*2^9=> 10,15*/
        acc1 = L_mult(fdata->cm,fdata->yfb[0]);/*1,14 *14,1  => 15,16*/
        acc0 = L_mult(fdata->r2,fdata->yfb[1]);/*1,14 * 14,1 => 15,16*/
        acc1 = L_shr(acc1,1);/*16,15*/
        acc0 = L_shr(acc0,1);/*16,15*/
        acc0 = L_sub(acc1,acc0);
        acc0 = L_add(yc32,acc0);/*ur*/
#ifdef TESTING
        overflow_count("b",__func__,&fsf1);
#endif
#ifdef TESTING_NO
        de_norm(testinfp,&acc0,1, 0, 15, FSF,1);/*22.9db*/
#endif

        /*resonator feedforward part
        f_data->yc[i] = ur - (f_data->cm * f_data->yfb[0]);*/
        acc1 = L_sub(acc0 , acc1);/*16,15*/
        fdata->yc[i] = roundB(acc1);/*16,-1*/
#ifdef TESTING
        overflow_count("c",__func__,&fsf2);
#endif
#ifdef TESTING_NO
        de_norm(testinfp,&acc1,1, 0, 15, FSF,1);/*22.9db*/
#endif
#ifdef TESTING_NO
        de_norm(testinfp,&fdata->yc[i],1, 0, -1, FSF,0);/**/
#endif

        /*update resonator delay line*/
        fdata->yfb[1] = fdata->yfb[0];
        fdata->yfb[0] = shl(roundB(acc0),2);/*16,15 => 14,17*/

#ifdef TESTING
        overflow_count("d",__func__,&fsf3);
#endif

    }
}

