#ifndef BLAT_H_FILE_INCLUDED
#define BLAT_H_FILE_INCLUDED

double blat_seconds(void);
double blat_milliseconds(void);
long long blat_microseconds(void);

void blat_sleepseconds(double seconds);
void blat_sleepmilliseconds(double milliseconds);
void blat_sleepmicroseconds(double microseconds);

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

double blat_milliseconds(void)
{
    LARGE_INTEGER ret;
    if(blat_priv_gs_frequency.QuadPart == 0) QueryPerformanceFrequency(&blat_priv_gs_frequency);
    QueryPerformanceCounter(&ret);
    return (1000 * ret.QuadPart) / (double)blat_priv_gs_frequency.QuadPart;
}

long long blat_microseconds(void)
{
    LARGE_INTEGER ret;
    if(blat_priv_gs_frequency.QuadPart == 0) QueryPerformanceFrequency(&blat_priv_gs_frequency);
    QueryPerformanceCounter(&ret);
    return (1000 * 1000 * ret.QuadPart) / blat_priv_gs_frequency.QuadPart;
}

/* TODO: consider using timeGetDevCaps then doing timeBeginPeriod/timeEndPeriod with wPeriodMin */
void blat_sleepseconds(double seconds) {Sleep((DWORD)(seconds * 1000.0));}
void blat_sleepmilliseconds(double milliseconds) {Sleep((DWORD)milliseconds);}
void blat_sleepmicroseconds(double microseconds) {Sleep((DWORD)(microseconds / 1000.0));}

#endif /* _WIN32 */

#endif /* BLAT_DO_IMPLEMENTATION */
