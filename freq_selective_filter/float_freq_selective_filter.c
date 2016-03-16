#include <stdio.h>
#include "typedef.h"
#include "float_freq_selective_filter.h"
#include "testing.h"



void freqSelectiveFilterFloat(Word16* pbuff, int frame_size,Fsf_data* f_data)
{

    int i=0,j;
    double yc=0.0;
    double ur=0.0;
    /*comb filter and resonator*/
    for(i =0; i < frame_size; i++)
    {
#ifdef TESTING_NO
        test_writef2(testscrfp,&pbuff[i],1,FSF,2);/*15,0*/
#endif

    //    printf("%f, ", (double)pbuff[i]); 
        /*comb part*/
        f_data->cbuffer[0] =(double) pbuff[i] * f_data->mul; /*q10.5 or 10.21*/
#ifdef TESTING_NO
        test_writef(testscrfp,&f_data->cbuffer[0],1,FSF);
#endif

    //    printf("%f, ", f_data->cbuffer[0]); 
        /*comb part*/
        yc = f_data->cbuffer[0]  - f_data->cbuffer[f_data->N]; 
#ifdef TESTING_NO
        test_writef(testscrfp,&yc,1,FSF);/*q10.5*/
#endif


        for(j=f_data->N; j >= 1 ; j--)
        {
            f_data->cbuffer[j] = f_data->cbuffer[j-1];
        }
        /*resonator feedback part*/
        ur = yc +  (f_data->cm * f_data->yfb[0]) - (f_data->r2 * f_data->yfb[1]);
#ifdef TESTING_NO
        test_writef(testscrfp,&ur,1,FSF);/*14.1*/
#endif


        /*resonator feedforward part*/
        f_data->yc[i] = ur - (f_data->cm * f_data->yfb[0]);
#ifdef TESTING_NO
        test_writef(testscrfp,&f_data->yc[i],1,FSF);/*14.1*/
#endif


        /*update resonator delay line*/
        f_data->yfb[1] = f_data->yfb[0];
        f_data->yfb[0] = ur;
    //    printf("%f, ", f_data->yc[i]); 
    }


}
