#ifndef BLAT_H_FILE_INCLUDED
#define BLAT_H_FILE_INCLUDED

double blat_seconds(void);
long long blat_microseconds(void);

double blat_secondsdelta(double * start);
long long blat_microsecondsdelta(long long * start);

#endif /* BLAT_H_FILE_INCLUDED */

#ifdef BLAT_DO_IMPLEMENTATION

#ifdef _WIN32

#include <profileapi.h> /* for QueryPerformanceFrequency, QueryPerformanceCounter */
#include <synchapi.h> /* for Sleep */

static LARGE_INTEGER blat_priv_gs_frequency;

double blat_seconds(void)
{
    LARGE_INTEGER ret;
    if(blat_priv_gs_frequency.QuadPart == 0) QueryPerformanceFrequency(&blat_priv_gs_frequency);
    QueryPerformanceCounter(&ret);
    return ret.QuadPart / (double)blat_priv_gs_frequency.QuadPart;
}

long long blat_microseconds(void)
{
    LARGE_INTEGER ret;
    if(blat_priv_gs_frequency.QuadPart == 0) QueryPerformanceFrequency(&blat_priv_gs_frequency);
    QueryPerformanceCounter(&ret);
    return (1000 * 1000 * ret.QuadPart) / blat_priv_gs_frequency.QuadPart;
}

#endif /* _WIN32 */

/* non platform specific one liner helper functions */
double blat_secondsdelta(double * start) {return blat_seconds() - (start ? *start : 0.0);}
long long blat_microsecondsdelta(long long * start) {return blat_microseconds() - (start ? *start : 0);}

#endif /* BLAT_DO_IMPLEMENTATION */
