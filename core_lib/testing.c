#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "typedef.h"
#include "testing.h"
#include <math.h>

FILE *testinfp, *testoutfp, *testscrfp,*testreadfp;
int routine_index = -1;
double bg_buff[2048];/*just a buffer for denorm debugging*/

routine_info_t routine_info[NUM_ROUTINES] = {

    { "cordic_sine", 
        "series of arguments between -pi/2 and pi/2",
        0 },
    { "fsf", 
        "input is noisy sine data, which is filtered by fsf filter",
        0 }

};


void print_test_names (void) 
{
    int i;

    fprintf(stderr, "valid routines to generate test vectors from:\n");
    for (i=0; i<NUM_ROUTINES; i++)
        fprintf(stderr, "%s\n", routine_info[i].name);

    /*exit(0);*/
}

void print_test_info (char *which_one)
{
    int i = 0;

    while (i<NUM_ROUTINES) {
        if (strcmp(which_one, routine_info[i].name) == 0) {
            fprintf(stderr, "%s : %s\n", routine_info[i].name, 
                    routine_info[i].info);
            /*exit(0);*/
            return;
        }
        i++;
    }

    fprintf(stderr, "%s : no such test routine available\n", which_one);

    exit(1);
}

void set_test_routine (char *routine_name)
{
    int i = 0;

    while (i<NUM_ROUTINES) {
        if (strcmp(routine_name, routine_info[i].name) == 0) {

            routine_info[i].testing = 1;
            routine_index = i;
            if ((testscrfp = fopen("sdx.script", "wb")) == NULL) {
                perror("sdx.script");
                fprintf(stderr, "couldn't open logfile for writing\n");
                exit(1);
            }

            if ((testinfp = fopen("sdx.in", "w")) == NULL) {
                perror("sdx.in");
                fprintf(stderr, "couldn't open logfile for writing\n");
                exit(1);
            }
            if ((testoutfp = fopen("sdx.ref", "w")) == NULL) {
                perror("sdx.ref");
                fprintf(stderr, "couldn't open logfile for writing\n");
                exit(1);
            }
            /*
               if ((testreadfp = fopen("read.in","rb")) == NULL)
               {
               perror("read.in");
               fprintf(stderr, "couldn't open logfile for reading\n");
               exit(1);

               }*/

            setbuf (testinfp, NULL);
            setbuf (testoutfp, NULL);
            setbuf (testscrfp, NULL);

            printf("setting test %s\n",routine_name);
            return;
        }
        i++;
    }

    fprintf(stderr, "no such routine for test vector, try -tnames option\n");
    exit(1);
}

void overflow_count(const char* ref, const char* func,of_count* cnt)
{
    cnt->hits++;
    if (Overflow)
    {
        cnt->o_flows++;
        Overflow=0;
        printf("%s, %s, %u,%u,%f%%\n",ref,func,cnt->hits,cnt->o_flows,(float)cnt->o_flows*100/(float)cnt->hits);
    }
}



void de_norm(FILE* f,void* buffer,Word16 buffer_size, Word16 shiftin, Word16 q, int routine,int type)
{

    int i;
    double x;

    if (routine_info[routine].testing == 1 || routine == ALWAYS )
    {

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
}


/*this will read a file of doubles, and convert to appropriate q. This can be used as data for testing */
void test_read_then_q(FILE *f, void* buff, int len,int q ,int routine, int type)
{
    int i;

    if ((routine_info[routine].testing == 1) || routine == ALWAYS )
    {
        fread(&bg_buff, sizeof(FLOAT) , len, f);

        /*now convert to its desired q format */
        for(i=0; i < len; i++)
        {
            if(type == 0)
            {
                /* printf("%f ",bg_buff[i]);*/
                *((Word16*)buff+i) = (q >= 0) ? ((Word16)(bg_buff[i] * (1LL << q))) : ((Word16)(bg_buff[i] / (1LL << -q)));
            }
            else
            {
                *((Word32*)buff+i) = (q >= 0) ? ((Word32)(bg_buff[i] * (1LL << q))) : ((Word32)(bg_buff[i] / (1LL << -q)));
            } 
        }
    }

}
/*this will read a file of doubles, and convert to appropriate q, then it will normalise it. This can be used as data for testing */
Word16 test_read_then_q_and_norm(FILE *f, void* buff, int len,int q ,int routine, int type)
{
    int i;
    Word16 mx16=0,val16;
    Word32 mx32 = 0,val32;
    Word16 sh1=0;
    Word16 temp16;
    Word32 temp32;
    if ((routine_info[routine].testing == 1) || routine == ALWAYS )
    {
        fread(&bg_buff, sizeof(FLOAT) , len, f);

        /*now convert to its desired q format */
        for(i=0; i < len; i++)
        {
            if(type == 0)
            {
                /* printf("%f ",bg_buff[i]);*/
                val16 = *((Word16*)buff+i) = (q >= 0) ? ((Word16)(bg_buff[i] * (1LL << q))) : ((Word16)(bg_buff[i] / (1LL << -q)));
                if (abs_s(val16) > mx16) mx16 = abs_s(val16);
            }
            else
            {
                val32 =  *((Word32*)buff+i) = (q >= 0) ? ((Word32)(bg_buff[i] * (1LL << q))) : ((Word32)(bg_buff[i] / (1LL << -q)));
                if (L_abs(val32) > mx32) mx32 = abs_s(val32);
            }
        }
        if(type == 0)
        {
            sh1 = norm_s(mx16);
        }
        else
        {
            sh1 = norm_l(mx32);
        }
        /*now normalise*/
        for(i=0; i < len ; i++)
        {
            if(type == 0)
            {
                temp16 = *((Word16*)buff+i);
                temp16 = shl(temp16,sh1);
                *((Word16*)buff+i) = temp16;
            }
            else
            {
                temp32 = *((Word32*)buff+i);
                temp32 = shl(temp32,sh1);
                *((Word32*)buff+i) = temp32;
            }

        } 

    }
    return sh1;

}

void test_writef (FILE *f, double *x, int n, int routine)
{
    if ((routine_info[routine].testing == 1) || routine == ALWAYS )
    {
        fwrite(x,sizeof(double),n,f);
    }
}
void test_writef2 (FILE *f, void *x, int len,int type,int data_type)
{
    int i;
    double buf[2048];/*assume 2048 is big enough*/

    if ((routine_info[type].testing == 1) || (type == ALWAYS))
    {
        if(data_type == 1)/*its an integer data type*/
        {
            for(i=0; i < len;i++)
            {
                buf[i] = (double)(*((int*)x+i));                                      
            }
            fwrite(buf,sizeof(double),len,f);
        }
        else if(data_type == 2)/*its an Word16 data type*/
        {
            for(i=0; i < len;i++)
            {
                buf[i] = (double)(*((Word16*)x+i));                                      
            }
            fwrite(buf,sizeof(double),len,f);
        }
        else
            fwrite(x,sizeof(double),len,f);
    }
}
