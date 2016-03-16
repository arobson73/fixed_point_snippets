
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>
#include "typedef.h"
#include "testing.h"
#include "float_freq_selective_filter.h"
#include "fixed_freq_selective_filter.h"
#define PI 3.14159265358979

#define BUFF_SIZE 1000//make this as big as necessary

Flag use_pr = 1;//flag for debugging
static long checkArgsAndFiles(FILE **ifp, int argc, char *argv[], int* frame_size, int* N, int* k);
static long  readData( Word16 *pbuff, int len, FILE *fp ,long len_remain);
static void error(char *format, ...);
static Word16 convert2Q(double in,double m);


int main(int argc, char *argv[])
{

    FILE  *ifp;       /* I/O File pointers */
    long  len_remain,total_len;
    long  rlen; /*read length*/
    int frame_size,N,k;
    double* comb_buffer;
    Word16* comb_fbuffer;
    double* ybuff;
    Word16* yfbuff;
    Word16  data_buff[BUFF_SIZE];
    double r = 0.9999; /*ensure poles are inside the unit circle*/
    Fsf_data f_data;
    Fsf_fdata fx_data;
    /* Check command line arguments and files */
    total_len = checkArgsAndFiles(&ifp, argc, argv, &frame_size,&N,&k);
    len_remain = total_len;
    /*allocate comb delay line, and filter output buffer */
    comb_buffer = (double*) calloc(N+1,sizeof(double));
    comb_fbuffer = (Word16*) calloc(N+1,sizeof(Word16));
    ybuff = (double*)calloc(frame_size,sizeof(double));
    yfbuff = (Word16*)calloc(frame_size,sizeof(Word16));
    
    if(comb_buffer == NULL || comb_fbuffer == NULL || ybuff == NULL || yfbuff == NULL)
    {
        fprintf(stdout,"Cannot allocate double comb buffer\n");
        fflush(stdout);
        exit(1);
    }
    /*compute 1/N and r^N and multiply them, used in comb filter;*/
    f_data.N = N;
    f_data.k = k;
    f_data.Nr = 1.0/N;
    printf("Nr = %f\n",f_data.Nr);
    f_data.rp = pow(r,N);
    printf("rp = %f\n",f_data.rp);
    f_data.mul = f_data.Nr * f_data.rp;
    printf("mul = %f\n",f_data.mul);
    /*compute the cosine coeff for resonator*/
    f_data.cm =2*r*cos(2*PI*k/N);

    printf("cm = %f\n",f_data.cm);
    f_data.r2 = r*r;

    printf("r2 = %f\n",f_data.r2);
    f_data.cbuffer = &comb_buffer[0];
    f_data.yfb[0]=0.0;f_data.yfb[1]=0.0;
    f_data.yc = &ybuff[0];

    /*compute fixed point multipliers struct*/
    fx_data.N = N;
    fx_data.k = k;
    fx_data.Nr = convert2Q(f_data.Nr,  32768.0);


    printf("fx Nr = %d\n",fx_data.Nr);
    fx_data.rp = convert2Q(f_data.rp , 32768.0); 

    printf("fx rp = %d\n",fx_data.rp);
    fx_data.mul =convert2Q(f_data.mul , 1048576.0);/*q20, not much advantage 
    in this scaling, since needs to be scaled back when used in the feedback part of the filter,
    almost equal results obtained at filter outputwhen this is q15*/
    printf("fx mul = %d\n",fx_data.mul);
    fx_data.cm = convert2Q(f_data.cm,16384.0);
    fx_data.r2 = convert2Q(f_data.r2,16384.0);
    fx_data.cbuffer = &comb_fbuffer[0];
    fx_data.yfb[0]=0;fx_data.yfb[1]=0;
    fx_data.yc = &yfbuff[0];



    /* Process */
    do {

        rlen = readData(data_buff,frame_size,ifp,len_remain);

        //add the function to be tested here!!
        freqSelectiveFilterFloat(data_buff,rlen,&f_data);
        freqSelectiveFilterFixed(data_buff,rlen,&fx_data);

        //write out float data to file
        test_writef(testoutfp,&f_data.yc[0], rlen, FSF);
        //write out fixed data to file
        de_norm(testinfp,&fx_data.yc[0],rlen, 0, -1, FSF,0);


        if( use_pr)
        {
            fprintf(stdout, "Done  : %6ld %3ld\r", total_len - len_remain,
                    (len_remain > 0) ? (total_len - len_remain)*100/total_len : 100);
            fflush(stdout);
        }
        len_remain -= rlen;
    }
    while (len_remain > 0);

    if (use_pr)
        fprintf(stdout,"\n");

    if(ifp) { (void) fclose(ifp); }
    free(comb_buffer);
    free(ybuff);

    return 0;
}

/* parse command line */
static long checkArgsAndFiles(FILE **ifp,int argc, char *argv[], int* frame_size,int *N, int *k)
{
    int   i;
    long  flen;
    char *usage = "Usage: fsf.exe [-t names]\t (lists all test points) [-t info NAME]\t(prints info of the test)\n\t\t[-t test NAME]\t(cause logging of the routine NAME)\n(input_file_name)\nframe_size\t (frame length or sample size)\ncomb_delay N\t(delay of comb filter)\nk index\t(the index of 2*pi*k/N)";
    /* Process the argument list, if any */

    if (argc < 5) {
        /* printf("Usage: %s [options] inputfile sample_size \n", Argv[0]);*/
        error(usage);
        exit(1);
    }
    for (i=1; i < argc-4; i++) {

        if (!strcmp("-t",argv[i++])) /*test some routine */
        {
            if(i < argc)
            {
                if (strcmp(argv[i], "names") == 0) {
                    print_test_names();
                    continue;
                    /* NOTREACHED */
                }
                if (strcmp(argv[i], "info") == 0) {

                    if (++i < argc)
                    { 
                        print_test_info(argv[i]);
                    }
                    else
                        error(usage);
                    continue;
                    /* NOTREACHED */
                }
                if (strcmp(argv[i], "test") == 0) {
                    if (++i < argc) 
                        set_test_routine(argv[i]);
                    else
                        error(usage);
                    continue;
                }
            }

        }
        else
        {
            printf("unknown command\n");
            fprintf(stderr, "unknown argument, %s\n", argv[i]);
            exit(1);
        }
    }

    /*  Open I/O files  */
    *ifp = fopen(argv[argc-4], "rb");
    if (*ifp == NULL) {
        fprintf(stderr, "Invalid input file name: %s\n", argv[argc-2]);
        exit(1);
    }
    if ( use_pr )
        printf("Input file:    %s\n", argv[argc-2]);

    *frame_size = atoi(argv[argc-3]);
    *N = atoi(argv[argc-2]);
    *k = atoi(argv[argc-1]);

    /*  get file length  */

    fseek(*ifp, 0L, SEEK_END);
    flen = ftell(*ifp);
    rewind(*ifp);
    flen /= sizeof(Word16) ;

    return flen;
}

static long  readData( Word16 *pbuff, int len, FILE *fp , long len_remain)
{
    int   i  ;
    long read_length;

    for ( i = 0 ; i < len ; i ++ )
        pbuff[i] = (Word16) 0 ;
    if (len > len_remain)
        len = len_remain;

    read_length = fread ( (char *)pbuff, sizeof(Word16), len, fp ) ;

    return read_length;
}

/*for displaying error results */
static void error(char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr,format, ap);
    va_end(ap);
    exit(1);
}
static
Word16 convert2Q(double in,double m)
{
    Word16 res;
    res = (Word16)(in*m);
    return res;
}
