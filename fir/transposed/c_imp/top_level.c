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
//#include "testing.h"
#define BUFF_SIZE 1000
#define COEFF_SIZE 4
#define DELAY_LINE COEFF_SIZE-1 
#define TESTING
/*structure for checking overflow*/
typedef struct counts_ {
    unsigned int hits;
    unsigned int o_flows;
} of_count;

static long  readData( double *pbuff, int len, FILE *fp ,long len_remain);
static long getFileLength(FILE*, char data_type);
static void dump_buffer(void* buff, int len, char data_type);
static void transposed_fir_filter(double* in, double* coeff, double* out,double* delay_line,int len);
static void transposed_fir_filter_fix(Word16* in, Word32* coeff, Word16* out,Word32* delay_line,int len);
static long  readDataFix( Word16 *pbuff, int len, FILE *fp , long len_remain);
static void overflow_count(const char* ref, const char* func,of_count* cnt);
static void de_norm(FILE* f,void* buffer,Word16 buffer_size, Word16 shiftin, Word16 q,int type);

/*for debugging float and fixed*/
FILE *fdb_fix,*fdb_float;

#define DUMP_DOUBLE(x,n) fwrite(x,sizeof(double),n,fdb_float)
#define DUMP_FIX(x,sz,sft,q,t) de_norm(fdb_fix,x,sz,sft,q,t) 
int main(int argc,char *argv[])
{
    /* no frames here just a block of 1000 samples*/
    /* input buffer out buffer and coeff pointer*/
    double in[BUFF_SIZE];
    Word16 in_fix[BUFF_SIZE];
    double out[BUFF_SIZE];
    Word16 out_fix[BUFF_SIZE];
    double delay_line[DELAY_LINE];
    Word32 delay_line_fix[DELAY_LINE];
    long file_length;
    FILE *fin; /*input file data*/
    FILE *fout; /*output file data*/
    FILE *fin_fix;
    FILE *fout_fix;
    int i;
    /*open input file*/
    fin = fopen("../matlab/indata_double.pcm","rb");
    fin_fix = fopen("../matlab/indata_fixed.pcm","rb");
    if (fin == NULL || fin_fix == NULL)
    {
        printf("could not open input file, exiting\n");
        return 1;
    }
    /*open output file*/
    fout = fopen("output_data.pcm","wb");
    fout_fix = fopen("output_data_fix.pcm","wb");
    if (fout == NULL || fout_fix == NULL)
    {
        printf("could not open output file, exiting\n");
        return 1;
    }
    /*open debug files*/
    fdb_fix = fopen("fix_debug.pcm","wb");
    fdb_float = fopen("float_debug.pcm","wb");

    if (fdb_fix == NULL || fdb_float == NULL)
    {
        printf("could not open output debug file, exiting\n");
        return 1;
    }


    /*get the length of data in file*/
    file_length = getFileLength(fin,'d');
    printf("\nfile length is %ld\n", file_length);
    /*read the all input data into buffer*/
    readData(in,BUFF_SIZE,fin,file_length);

    /*same for fixed point versions*/
    file_length = getFileLength(fin_fix,'w');
    readDataFix(in_fix,BUFF_SIZE,fin_fix,file_length);

    /*zero delay line */
    for (i=0; i < COEFF_SIZE-1; i++)
    {
        delay_line[i]=0.0;
        delay_line_fix[i]=0;
    }

    /*dump data to test*/
    //    dump_buffer(in_fix,BUFF_SIZE,'w');

    transposed_fir_filter(in, &coeff_data[0], out, delay_line,BUFF_SIZE);
   // for(i=0; i < BUFF_SIZE; i++)
   //     printf("%.10f \n", out[i]);

    transposed_fir_filter_fix(in_fix, &coeff_data_fixQ31[0], out_fix, delay_line_fix,BUFF_SIZE);

    /*write results to file*/
    fwrite(&out[0],sizeof(double),BUFF_SIZE,fout);
    fwrite(&out_fix[0],sizeof(Word16),BUFF_SIZE,fout_fix);

    //  dump_buffer(out,BUFF_SIZE);
    return 0;
}

static void transposed_fir_filter(double* in, double* coeff, double* out,double* delay_line,int len)
{
    /*we have 41 coeffcients */
    int i,j;


    for (i =0; i < len; i++)
    {
        /*new value for start of delay line*/
        double temp = coeff[0] * in[i];
//        DUMP_DOUBLE(&temp,1);
        out[i] = temp + delay_line[DELAY_LINE-1];

    //    printf("%.10f , %.10f\n", temp,out[i]);
        //out[i] = (coeff[0] * in[i]) + delay_line[DELAY_LINE-1]; 
        /*delay line */      
        for(j=1;j < DELAY_LINE; j++)
        {
            /*d[39] = coeff[1] * in + d[38]
             * d[38] = coeff[2] * in + d[37];
             * d[1] = coeff[39] * in + d[0];*/
            delay_line[DELAY_LINE-j] = ( coeff[j] * in[i] ) + delay_line[DELAY_LINE-1-j];
        } 
        /*update first delay line value*/ 
        delay_line[0] = coeff[COEFF_SIZE-1] * in[i];
    //    DUMP_DOUBLE(&delay_line[0],1);
    }
}

static void transposed_fir_filter_fix(Word16* in, Word32* coeff, Word16* out,Word32* delay_line,int len)
{
    int i,j;
    Word32 acc0;
    Word16 t;
#ifdef TESTING
    static of_count of1,of2,of3,of4,of5;
#endif 
#if 0
    for (i= 0; i < len; i++)
    {
        /*out[i] = (coeff[0] * in[i]) + delay_line[DELAY_LINE-1];*/ /*q16 * q14 => q31*/
        t = mult_r(coeff[0],in[i]); /*q16*/
        t = shr(t,1); /*q15*/
#ifdef TESTING
        overflow_count("a",__func__,&of1);
#endif
        out[i] = add(t,delay_line[DELAY_LINE-1]);
#ifdef TESTING
        overflow_count("b",__func__,&of2);
#endif
        for(j=1;j < DELAY_LINE-1; j++)
        {
            /*delay_line[DELAY_LINE-j] = ( coeff[j] * in[i] ) + delay_line[DELAY_LINE-1-j];*/
            t = mult_r(coeff[j],in[i]);
            t = shr(t,1);
#ifdef TESTING
            overflow_count("c",__func__,&of3);
#endif
            delay_line[DELAY_LINE-j] = add(t,delay_line[DELAY_LINE-1-j]);
#ifdef TESTING
            overflow_count("d",__func__,&of4);
#endif
        }
        delay_line[0] = shr(mult_r(coeff[COEFF_SIZE-1],in[i]),1);

        //DUMP_FIX(&delay_line[0],1,0,15,0); /*q16 * q14 =q31 => q16 =>q15*/
#ifdef TESTING
        overflow_count("d",__func__,&of5);
#endif
    }
    //only getting 6db snr with above 
    //
#endif
#if 0
    for (i= 0; i < len; i++)
    {
        acc0 = L_mult(coeff[0],in[i]); /*q16 * q14 => q31 */
        acc0 = L_shr(acc0,1); /*q30*/
        acc0 = L_add(acc0,delay_line[DELAY_LINE-1]);
        out[i] = roundB(acc0);
        for(j=1;j < DELAY_LINE-1; j++)
        {
            acc0 = L_mult(coeff[j],in[i]);
            acc0 = L_shr(acc0,1);
            delay_line[DELAY_LINE-j] = L_add(acc0,delay_line[DELAY_LINE-1-j]);
        }
        acc0 = L_mult(coeff[COEFF_SIZE-1],in[i]);
        delay_line[0] = L_shr(acc0,1);
    }
#ifdef TESTING
    overflow_count("d",__func__,&of5);
#endif
#endif
    //again 6db with above
    //try making coefficients 32 bit
    //
    Word16 hi,lo;
    for (i= 0; i < len; i++)
    {
        L_Extract(coeff[0],&hi, &lo);
        acc0 = Mpy_32_16(hi,lo,in[i]); /*q31 *q14 => q45 => q30  */
        printf("--------------------------------------------------------------\n");
        printf("coeff[0] * in[%d] = %d \n", i, acc0); 
        
    //    printf("coeff*in=%d, del_line[39]=%d,in[%d]=%d,coeff[0]=%d %d\n",acc0,delay_line[DELAY_LINE-1],i,in[i],hi,lo);
       // DUMP_FIX(&acc0,1,0,30,1); /*50db*/ 
        acc0 = L_add(acc0, delay_line[DELAY_LINE-1]);
        printf(" + delay_line[%d] = %d = %d\n",DELAY_LINE-1,delay_line[DELAY_LINE-1],acc0);
        out[i] = roundB(acc0); /*2,30 => 2,14 q14*/
        //printf("in[%d]=%d,coeff0_hi = %d, coeff0_lo=%d, out[%d]=%d ,acc=%d,del[%d]=%d\n",i,in[i],hi,lo,i,out[i],acc0,DELAY_LINE-1,delay_line[DELAY_LINE-1]);
        printf("out[%d] = %d\n",i,out[i]);
        for(j=1;j < DELAY_LINE ; j++)
        {
            L_Extract(coeff[j],&hi,&lo);
            acc0 = Mpy_32_16(hi,lo,in[i]);
            printf("coeff[%d] * in[%d] = %d\n",j,i,acc0);
            delay_line[DELAY_LINE-j] = L_add(acc0,delay_line[DELAY_LINE-1-j]);
            printf("del_line[%d] = %d + (delay_line[%d] = %d) = %d\n",DELAY_LINE-j,acc0,DELAY_LINE-1-j,delay_line[DELAY_LINE-1-j], delay_line[DELAY_LINE-j]); 
          //  printf("hi=%d,lo=%d,del[%d] = %d, coeff[%d]*in[%d] = acc0=%d + del[%d] = %d\n",hi,lo,DELAY_LINE-j,delay_line[DELAY_LINE-j],j,i,acc0,DELAY_LINE-1-j,delay_line[DELAY_LINE-1-j]); 
         //   delay_line[DELAY_LINE-j] = roundB(acc0);
        }
        L_Extract(coeff[COEFF_SIZE-1],&hi,&lo);
        delay_line[0] = Mpy_32_16(hi,lo,in[i]);
        printf("del_line[0] = coeff[%d] * in[%d] = %d\n",COEFF_SIZE-1,i,delay_line[0]);
        //printf("hi=%d,lo=%d,del[0] = %d, coeff[%d]*in[%d] \n",hi,lo,delay_line[0], COEFF_SIZE-1,i);
      //  delay_line[0] = roundB(acc0); /*q30 is 2,30 => q14 when rounded*/

    //    DUMP_FIX(&delay_line[0],1,0,30,1);  /*60db*/
        overflow_count("d",__func__,&of5);
    }
    //above gave output of  61.9 db, not sure how i
    //get that, its more like 46.9 db
    //an obvious optimisation step would be to make 
    //the delay line 16 bit instead of 32
}


static long getFileLength(FILE* fp,char data_type)
{
    long flen;
    fseek(fp,0L,SEEK_END);
    flen = ftell(fp);
    rewind(fp);
    if (data_type == 'd')
        flen /= sizeof(double);
    else
        flen  /= sizeof(Word16);
    return flen;
}

static void dump_buffer(void* buff, int len, char data_type)
{
    int i;
    for(i=0; i < len ; i++)
    {
        if(data_type == 'd') 
            printf(" %lf ", (double)(*((double*)buff+i)));     
        else
            printf(" %d ", (Word16)(*((Word16*)buff+i)));     
    }
    printf("\n");
} 

static long  readData( double *pbuff, int len, FILE *fp , long len_remain)
{
    int   i  ;
    long read_length;

    for ( i = 0 ; i < len ; i ++ )
    {
        pbuff[i] = (double) 0.0 ;
    }
    if (len > len_remain)
        len = len_remain;

    read_length = fread ( (char *)pbuff, sizeof(double), len, fp ) ;

    return read_length;
}
static long  readDataFix( Word16 *pbuff, int len, FILE *fp , long len_remain)
{
    int   i  ;
    long read_length;

    for ( i = 0 ; i < len ; i ++ )
    {
        pbuff[i] = (Word16) 0.0 ;
    }
    if (len > len_remain)
        len = len_remain;

    read_length = fread ( (char *)pbuff, sizeof(Word16), len, fp ) ;

    return read_length;
}

static void overflow_count(const char* ref, const char* func,of_count* cnt)
{
    cnt->hits++;
    if (Overflow)
    {
        cnt->o_flows++;
        Overflow=0;
        printf("%s, %s, %u,%u,%f%%\n",ref,func,cnt->hits,cnt->o_flows,(float)cnt->o_flows*100/(float)cnt->hits);
    }
}


static void de_norm(FILE* f,void* buffer,Word16 buffer_size, Word16 shiftin, Word16 q,int type)
{
    int i;
    double x;
    double bg_buff[2048];/*just a buffer for denorm debugging*/

    for(i =0; i < buffer_size; i++)
    {
        Word16 shift=shiftin;
        /*first un q it */
        if(type == 0)
            x = q >= 0 ? ((double) (*((Word16*)buffer+i)))/(1LL << q) : ((double) (*((Word16*)buffer+i))) * (1LL << -q);
        else
            x = q >= 0 ? ((double) (*((Word32*)buffer+i)))/(1LL << q) : ((double) (*((Word32*)buffer+i))) * (1LL << -q);
        /*now unshift it a positive shift means we scale down, a negative shift is scale up */
        if (shift > 0)
        {
            while (shift > 0)
            {
                int s = shift > 32 ? 32 : shift;
                x /= (1LL << s);
                shift -= s;
            }
        }
        else
        {
            while (shift < 0)
            {
                int s = shift < -32 ? -32 : shift;
                x *= (1LL << (-s));
                shift += -s;
            }
        }
        bg_buff[i]=x;
    }
    /*now write to file*/
    fwrite(&bg_buff[0],sizeof(double),buffer_size,f);
}
