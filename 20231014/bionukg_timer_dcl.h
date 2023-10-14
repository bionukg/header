#pragma once
#ifndef _bionukg_timer_dcl_h
#define _bionukg_timer_dcl_h

#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")
#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

    class timer
    {
    private:
        time_t t_s, t_ls;
        DWORD t_ms, t_lms;
        LARGE_INTEGER t_us, freq, t_lus;

    public:
        timer();
        ~timer()=default;

        time_t from_start(time_t *time_s);
        DWORD from_start(DWORD *time_ms);
        LARGE_INTEGER from_start(LARGE_INTEGER *time_us);
        time_t s_from_last();
        DWORD ms_from_last();
        LONGLONG us_from_last();
    };
    class sleeper
    {
    public:
        sleeper(UINT time_period = 5);
        void sleep(DWORD t);
        void wait(DWORD t);
        ~sleeper();
    };
#ifndef operator_bionukg
    time_t operator""_s(unsigned long long s)
	{
		return s;
	}
    DWORD operator""_ms(unsigned long long ms)
	{
		return ms;
	}
    LARGE_INTEGER operator""_us(unsigned long long us)
    {
        LARGE_INTEGER ret;
        ret.QuadPart = us;
        return ret;
    }
#endif // operator_bionukg


#ifdef namespace_bionukg
}
#endif // namespace_bionukg

#ifdef bionukg_include_imp
#include "./bionukg_timer_imp.h"
#endif

#endif // !_bionukg_timer_h
