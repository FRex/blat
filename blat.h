#ifndef BLAT_H_FILE_INCLUDED
#define BLAT_H_FILE_INCLUDED

double blat_seconds(void);
long long blat_microseconds(void);

#endif /* BLAT_H_FILE_INCLUDED */

#ifdef BLAT_DO_IMPLEMENTATION

#ifdef _WIN32

#include <profileapi.h> /* for QueryPerformanceFrequency, QueryPerformanceCounter */
#include <synchapi.h> /* for Sleep */

static LARGE_INTEGER blat_gs_frequency;

double blat_seconds(void)
{
    LARGE_INTEGER ret;
    if(blat_gs_frequency.QuadPart == 0) QueryPerformanceFrequency(&blat_gs_frequency);
    QueryPerformanceCounter(&ret);
    return ret.QuadPart / (double)blat_gs_frequency.QuadPart;
}

long long blat_microseconds(void)
{
    LARGE_INTEGER ret;
    if(blat_gs_frequency.QuadPart == 0) QueryPerformanceFrequency(&blat_gs_frequency);
    QueryPerformanceCounter(&ret);
    return (1000 * 1000 * ret.QuadPart) / blat_gs_frequency.QuadPart;
}

#endif /* _WIN32 */

#endif /* BLAT_DO_IMPLEMENTATION */
