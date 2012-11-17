/**
| Copyright(C) 2012 Ali Ersenal
| License: WTFPL v2
| URL: http://sam.zoy.org/wtfpl/COPYING
|
|--------------------------------------------------------------------------
| Math.c
|--------------------------------------------------------------------------
|
| DESCRIPTION:   Very simple kernel math library.
|
| AUTHOR:        Ali Ersenal, aliersenal@gmail.com
\------------------------------------------------------------------------*/


#include <Lib/Math.h>

double Math_abs(double number) {

    asm volatile("fld %0;"
                 "fabs;"
                 "fst %0"
                 : "+t" (number));

    return number;
}

double Math_sqrt(double number) {

    asm volatile("fld %0;"
                 "fsqrt;"
                 "fst %0"
                 : "+t" (number));

    return number;

}

double Math_sin(double radian) {

    asm volatile("fld %0;"
                 "fsin;"
                 "fst %0"
                 : "+t" (radian));

    return radian;

}

double Math_cos(double radian) {

    asm volatile("fld %0;"
                 "fcos;"
                 "fst %0"
                 : "+t" (radian));

    return radian;

}

double Math_tan(double radian) {

    asm volatile("fld %0;"
                 "fptan;"
                 "fstp %0;"
                 "fst  %0"
                 : "+t" (radian));

    return radian;

}