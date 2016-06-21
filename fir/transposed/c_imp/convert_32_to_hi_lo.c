#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include "typedef.h"
#include "basic_op.h"
#include "oper_32b.h"
#include "transposed.h"


int main(int argc, char* argv[])
{
    FILE* fp_result, *fp_h, *fp_l;
    int i;
    int len=41;
    Word16 hi,lo;
    Word16 ty = -10;
    printf("ty = %x\n",ty & 0xFFFF);
    if (argc > 1)
    {
        printf("length supplied , new length is %d\n",atoi(argv[1]));
        len = atoi(argv[1]);
    }
    fp_result = fopen("out_hi_lo.txt","w");
    if (fp_result == NULL)
    {
        printf("could not open file for output\n");
        return 1;
    }
    //also 2 text files for high and low for verilog sim
    //
    fp_h = fopen("out_hi.hex","w");
    if (fp_h == NULL)
    {
        printf("could not open file 1 for output\n");
        return 1;
    }
    fp_l = fopen("out_lo.hex","w");
    if (fp_l == NULL)
    {
        printf("could not open file 2 for output\n");
        return 1;
    }
    for(i=0; i < len; i++)
    {
        L_Extract(coeff_data_fixQ31[i],&hi,&lo);
       // printf("hi = %d, lo= %d \n",hi,lo);
        fprintf(fp_result,"%d \t\t %d\n",hi,lo);
        fprintf(fp_h,"%x ",0xFFFF & hi);
        fprintf(fp_l,"%x ",0xFFFF & lo);
       // fwrite(&hi,sizeof(Word16),1,fp_h);
       // fwrite(&lo,sizeof(Word16),1,fp_l);
    }



    return 0;
}
