#ifndef __TESTING__H
#define __TESTING__H
#include<stdio.h>
#include<stdlib.h>
#include "basic_op.h"
/*flag to enable the testing */
#define TESTING
#define NUM_ROUTINES 2 
#define CORDIC       0
#define FSF          1
#define ALWAYS       0xFF /*always run this */ 

typedef struct dummy {
  char *name;
  char *info;
  int   testing;
} routine_info_t;

typedef struct counts_ {
    unsigned int hits;
    unsigned int o_flows;
} of_count;
    
void print_test_names (void);
void print_test_info (char *which_one);
void set_test_routine (char *routine_name);
void overflow_count(const char* ref, const char* func,of_count* cnt);
void test_writef (FILE *f, double *x, int n, int routine);
void test_writef2 (FILE *f, void *x, int len,int type,int data_type);
void de_norm(FILE* f,void* buffer,Word16 buffer_size, Word16 shiftin, Word16 q, int routine,int type);
void test_read_then_q(FILE *f, void* buff, int len,int q ,int routine, int type);
Word16 test_read_then_q_and_norm(FILE *f, void* buff, int len,int q ,int routine, int type);

extern FILE *testinfp, *testoutfp, *testscrfp,*testreadfp;
extern int routine_index;
extern routine_info_t routine_info[NUM_ROUTINES];
#endif
