
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "typedef.h"
#include "testing.h"
#include "cordic_sine.h"

#define BUFF_SIZE 1000//make this as big as necessary
Flag use_pr = 1;//flag for debugging
long checkArgsAndFiles(FILE **ifp, int argc, char *argv[], int* sample_size);
void  readData( Word16 *pbuff, int len, FILE *fp );
void error(char *format, ...);


int main(int argc, char *argv[])
{

    FILE  *ifp;       /* I/O File pointers */
    long  cnt = 0;
    long  len;
    int sample_size;
    Word16  data_buff[BUFF_SIZE];

    /* Check command line arguments and files */
    len = checkArgsAndFiles(&ifp, argc, argv, &sample_size);

    /* Process */
    do {

        readData(data_buff,sample_size,ifp);
        //add the function to be tested here!!
        cordicSine(data_buff);
        cnt++;

        if( use_pr)
        {
            fprintf(stdout, "Done  : %6ld %3ld\r", cnt,
                    len ? cnt*100/len : 0);
            fflush(stdout);
        }
       // printf("cnt= %d\n",cnt);
    }
    while (cnt < len);

    if (use_pr)
        fprintf(stdout,"\n");

    if(ifp) { (void) fclose(ifp); }

    return 0;
}

/* parse command line */
long checkArgsAndFiles(FILE **ifp,int argc, char *argv[], int* sample_size)
{
    int   i;
    long  flen;
    char *usage = "Usage: cordic_sine.exe [-t names]\t (lists all test points) [-t info NAME]\t(prints info of the test)\n\t\t[-t test NAME]\t(cause logging of the routine NAME)\n\t (input_file_name)\nsample_size (frame length or sample size)";
    /* Process the argument list, if any */

    if (argc < 3) {
        /* printf("Usage: %s [options] inputfile sample_size \n", Argv[0]);*/
        error(usage);
        exit(1);
    }
    for (i=1; i < argc-2; i++) {

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
    *ifp = fopen(argv[argc-2], "rb");
    if (*ifp == NULL) {
        fprintf(stderr, "Invalid input file name: %s\n", argv[argc-2]);
        exit(1);
    }
    if ( use_pr )
        printf("Input file:    %s\n", argv[argc-2]);

    *sample_size = atoi(argv[argc-1]);

    /*  get file length  */

    fseek(*ifp, 0L, SEEK_END);
    flen = ftell(*ifp);
    rewind(*ifp);
    flen /= sizeof(Word16)*(*sample_size) ;

    return flen;
}

void  readData( Word16 *pbuff, int len, FILE *fp )
{
    int   i  ;

    for ( i = 0 ; i < len ; i ++ )
        pbuff[i] = (Word16) 0 ;

    fread ( (char *)pbuff, sizeof(Word16), len, fp ) ;

    return;
}

/*for displaying error results */
void error(char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr,format, ap);
    va_end(ap);
    exit(1);
}
