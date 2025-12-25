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

static long long microseconds;
static double seconds, milliseconds;

static void saveTimes(void)
{
    microseconds = blat_microseconds();
    milliseconds = blat_milliseconds();
    seconds = blat_seconds();
}

static void printTimes(void)
{
    printf(
        "elapsed: %.6fs %.3fms %lldus\n\n",
        blat_seconds() - seconds,
        blat_milliseconds() - milliseconds,
        blat_microseconds() - microseconds
    );
}

int main(void)
{
    int i;

    /* spend some time calculating high up fib terms */
    saveTimes();
    for(i = 38; i < 41; ++i) printf("fib(%d) = %lld\n", i, fib(i));
    printTimes();

    /* try various sleeping functions and amounts */
#define DO_PRINT_AND_TIME(code) do{saveTimes(); printf("doing " #code "\n"); code; printTimes();}while(0)
    DO_PRINT_AND_TIME(blat_sleepseconds(1.234));
    DO_PRINT_AND_TIME(blat_sleepmilliseconds(1234));
    DO_PRINT_AND_TIME(blat_sleepmicroseconds(1234000));
#undef DO_PRINT_AND_TIME

    return 0;
}
