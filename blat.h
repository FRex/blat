#ifndef BLAT_H_FILE_INCLUDED
#define BLAT_H_FILE_INCLUDED

const char * blat_type(void);

double blat_seconds(void);
double blat_milliseconds(void);
long long blat_microseconds(void);

void blat_sleepseconds(double seconds);
void blat_sleepmilliseconds(double milliseconds);
void blat_sleepmicroseconds(long long microseconds);

#endif /* BLAT_H_FILE_INCLUDED */

#ifdef BLAT_DO_IMPLEMENTATION

#ifdef _WIN32

#include <profileapi.h> /* for QueryPerformanceFrequency, QueryPerformanceCounter */
#include <synchapi.h> /* for Sleep */

static LARGE_INTEGER blat_priv_gs_frequency;

const char * blat_type(void) {return "blat_winapi";}

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
void blat_sleepmicroseconds(long long microseconds) {Sleep((DWORD)(microseconds / 1000));}

#endif /* _WIN32 */

#ifdef __linux__

/* to allow using non-std functions from posix from time.h */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h> /* for nanosleep, clock_gettime */

const char * blat_type(void) {return "blat_linux";}

double blat_seconds(void)
{
    struct timespec ts;
    /* TODO: consider using CLOCK_MONOTONIC outside */
    clock_gettime(CLOCK_MONOTONIC_RAW , &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

double blat_milliseconds(void)
{
    struct timespec ts;
    /* TODO: consider using CLOCK_MONOTONIC outside */
    clock_gettime(CLOCK_MONOTONIC_RAW , &ts);
    return 1000.0 * ts.tv_sec + ts.tv_nsec / 100000.0;
}

long long blat_microseconds(void)
{
    struct timespec ts;
    /* TODO: consider using CLOCK_MONOTONIC outside */
    clock_gettime(CLOCK_MONOTONIC_RAW , &ts);
    return 1000000ll * ts.tv_sec + ts.tv_nsec / 1000;
}

void blat_sleepseconds(double seconds)
{
    struct timespec ts;
    ts.tv_sec = seconds;
    ts.tv_nsec = 1000000000ll * (seconds - ts.tv_sec);
    nanosleep(&ts, NULL);
}

void blat_sleepmilliseconds(double milliseconds) {blat_sleepseconds(milliseconds / 1000.0);}
void blat_sleepmicroseconds(long long microseconds) {blat_sleepseconds(microseconds / 1000000.0);}

#endif /* __linux__ */

#endif /* BLAT_DO_IMPLEMENTATION */
