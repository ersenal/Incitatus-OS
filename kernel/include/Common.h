/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Common.h
|--------------------------------------------------------------------------
|
| DESCRIPTION:  Provides common definitions and typedefs.
|
|
| AUTHOR:       Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#ifndef COMMON_H
#define COMMON_H

/* Get the number of elements in an array */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

/* Null type */
#define NULL   ((void *)0)

/* Mark a variable as unused */
#define UNUSED(var) ((void) var)

/** Used mainly for casting structs */
#define FORCE_CAST(var, type) *(type*)&var

/** 32 bit X86 types */
typedef unsigned long long  u64int;
typedef unsigned int        u32int;
typedef unsigned short      u16int;
typedef unsigned char       u8int;

typedef unsigned int size_t;
typedef signed int   ptrdiff_t;

/** Boolean type **/
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

/** Scope, helps with code documentation/structure(used in .c files)
**  PUBLIC - Functions or variables can be accessed from other compilation units
**  PRIVATE - Functions or variables can NOT be accessed from other compilation units
**/
#define PRIVATE static
#define PUBLIC

#endif
