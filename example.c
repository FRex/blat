#define BLAT_DO_IMPLEMENTATION
#include "blat.h"
#include <stdio.h> /* printf */

static long long fib(int n)
{
    /* fib(0) is 0, fib(1) is 1, like at https://oeis.org/A000045 */
    if(n < 1) return 0;
    if(n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

int main(void)
{
    long long microseconds;
    double seconds, milliseconds;
    int i;

    microseconds = blat_microseconds();
    milliseconds = blat_milliseconds();
    seconds = blat_seconds();

    /* spend some time calculating high up fib terms */
    for(i = 35; i < 41; ++i) printf("fib(%d) = %lld\n", i, fib(i));

    printf("elapsed s ms us = %.6f %.3f %lld\n", blat_seconds() - seconds, blat_milliseconds() - milliseconds, blat_microseconds() - microseconds);
    return 0;
}
