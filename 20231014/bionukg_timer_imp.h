#pragma once
#ifndef _bionukg_timer_imp_h
#define _bionukg_timer_imp_h

#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")

#ifndef _bionukg_timer_dcl_h
#include "./bionukg_timer_dcl.h"
#endif

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

    timer::timer()
    {
        t_s = time(0);
        t_ls = t_s;
        t_ms = timeGetTime();
        t_lms = t_ms;
        QueryPerformanceCounter(&t_us);
        t_lus.QuadPart = t_us.QuadPart;
        QueryPerformanceFrequency(&freq);
    };
    time_t timer::from_start(time_t *time_s)
    {
        time_t tm = time(0) - t_s;
        if (time_s != 0)
            *time_s = tm;
        return tm;
    }
    DWORD timer::from_start(DWORD *time_ms)
    {
        DWORD tm = timeGetTime() - t_ms;
        if (time_ms != 0)
            *time_ms = tm;
        return tm;
    }
    LARGE_INTEGER timer::from_start(LARGE_INTEGER *time_us)
    {
        LARGE_INTEGER tm;
        QueryPerformanceCounter(&tm);
        if (time_us != 0)
            time_us->QuadPart = tm.QuadPart - t_us.QuadPart;
        return tm;
    }
    time_t timer::s_from_last()
    {
        time_t tem = time(0), ret = tem - t_ls;
        t_ls = tem;
        return ret;
    }
    DWORD timer::ms_from_last()
    {
        DWORD tem = timeGetTime(), ret = tem - t_lms;
        t_lms = tem;
        return ret;
    }
    LONGLONG timer::us_from_last()
    {
        LARGE_INTEGER tem;
        LONGLONG ret;
        QueryPerformanceCounter(&tem);
        ret = tem.QuadPart - t_lus.QuadPart;
        t_lus.QuadPart = tem.QuadPart;
        return ret;
    }


    sleeper::sleeper(UINT time_period)
    {
        timeBeginPeriod(time_period);
    }

    void sleeper::sleep(DWORD t)
    {
        Sleep(t);
    }
    void sleeper::wait(DWORD t)
    {
        Sleep(t);
    }
    sleeper::~sleeper()
    {
        timeEndPeriod(1);
    }

#ifdef namespace_bionukg
}
#endif // namespace_bionukg

#endif // !_bionukg_timer_h
