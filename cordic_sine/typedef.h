#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__
/*
   Types definitions
*/
#if  defined(_MSC_VER)
typedef  long  int   Word32;
typedef  short int   Word16;
typedef  short int   Flag;
#elif defined(linux)
typedef short Word16;
typedef int   Word32;
typedef int   Flag;
#else
#error  COMPILER NOT TESTED typedef.h needs to be updated
#endif
#if defined (_single) || defined (single)
typedef  float  FLOAT;
#else
typedef  double  FLOAT;
#endif

#endif
