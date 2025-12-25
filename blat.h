#ifndef BLAT_H_FILE_INCLUDED
#define BLAT_H_FILE_INCLUDED

/*
    blat - bla time, a small pure C89 wrapper for monotonic time and sleep
    functions on Windows and Linux. It can be useful for simple programs
    that don't require specific sleep or timing features and want to measure
    time a bit better than time and clock functions from C header time.h can.

    See https://github.com/FRex/blat for latest version and end of this file
    for licensing information (Unlicense license).

    To use, put #define BLAT_DO_IMPLEMENTATION a single source file before
    including this header, and in all other sources just include this file
    and use functions described below.

    Each function is self-contained so if you need just one of them you can
    copy paste it into your own code and include right OS headers yourself.

    Windows is detected with #ifdef _WIN32 and Linux with #ifdef __linux__.

    For seconds and milliseconds a double was chosen, to balance between
    convenience, accuracy and range. For microseconds long long (signed
    64-bit int on most/all platforms) was chosen.

    There is no nanosecond precision but it might be added in the future.

    There is no defined start of the clock so the values from timing functions make
    sense only in context of deltas, e.g. taking time before and after some operation.

    On Windows - Sleep and QueryPerformanceCounter/QueryPerformanceFrequency are used.
    QueryPerformanceFrequency is called once and the result stored in a global variable.
    There is no error checking since on Windows XP and later these functions can't fail.

    On Linux clock_gettime with CLOCK_MONOTONIC_RAW and nanosleep are used.

    Things that are not done or handled:
    1. resuming nanosleep after a signal using rem (rem IS NULL) on Linux.
    2. alertable SleepEx to interact with I/O completions on Windows.
    3. Wrapping Sleep in timeBeginPeriod/timeEndPeriod with TIMECAPS.wPeriodMin from timeGetDevCaps.

    If you need this functionality you're probably already writing OS specific
    code and can call relevant extra functions yourself.
*/

/* short string saying what kind of implementaiton is used */
const char * blat_type(void);

/* get current monotonic time, start is not specified, so only deltas are meaningful */
double blat_seconds(void);
double blat_milliseconds(void);
long long blat_microseconds(void);

/* sleep for (around) a given time, not accounting for signals and OS clock granulity */
void blat_sleepseconds(double seconds);
void blat_sleepmilliseconds(double milliseconds);
void blat_sleepmicroseconds(long long microseconds);

#endif /* BLAT_H_FILE_INCLUDED */


#ifdef BLAT_DO_IMPLEMENTATION

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h> /* for QueryPerformanceFrequency, QueryPerformanceCounter, Sleep */

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
    return 1000.0 * ts.tv_sec + ts.tv_nsec / 1000000.0;
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

/* LICENSE INFORMATION:

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
*/
