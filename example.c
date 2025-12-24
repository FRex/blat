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
    double seconds;
    int i;

    microseconds = blat_microseconds();
    seconds = blat_seconds();

    printf("start times = %f %lld\n", blat_seconds(), blat_microseconds());
    for(i = 0; i < 41; ++i) printf("fib(%d) = %lld\n", i, fib(i));
    printf("end times = %f %lld\n", blat_seconds(), blat_microseconds());
    printf("elapsed = %f %lld\n", blat_secondsdelta(&seconds), blat_microsecondsdelta(&microseconds));
    return 0;
}
