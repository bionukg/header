#pragma once
#ifndef _bionukg_timer_h
#define _bionukg_timer_h

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
		time_t t_s = time(0), t_ls = t_s;
		DWORD t_ms = timeGetTime(), t_lms = t_ms;
		LARGE_INTEGER t_us, freq, t_lus;

	public:
		timer()
		{
			QueryPerformanceCounter(&t_us);
			t_lus.QuadPart = t_us.QuadPart;
			QueryPerformanceFrequency(&freq);
		};
		~timer() {};

		time_t from_start(time_t* time_s)
		{
			time_t tm = time(0) - t_ms;
			if (time_s != 0)
				*time_s = tm;
			return tm;
		}
		DWORD from_start(DWORD* time_ms)
		{
			DWORD tm = timeGetTime() - t_ms;
			if (time_ms != 0)
				*time_ms = tm;
			return tm;
		}
		LARGE_INTEGER from_start(LARGE_INTEGER* time_us)
		{
			LARGE_INTEGER tm;
			QueryPerformanceCounter(&tm);
			if (time_us != 0)
				time_us->QuadPart = tm.QuadPart - t_us.QuadPart;
			return tm;
		}

		time_t s_form_last()
		{
			time_t tem = time(0), ret = tem - t_ls;
			t_ls = tem;
			return ret;
		}
		DWORD ms_form_last()
		{
			DWORD tem = timeGetTime(), ret = tem - t_lms;
			t_lms = tem;
			return ret;
		}
		LONGLONG us_form_last()
		{
			LARGE_INTEGER tem;
			LONGLONG ret;
			QueryPerformanceCounter(&tem);
			ret = tem.QuadPart - t_lus.QuadPart;
			t_lus.QuadPart = tem.QuadPart;
			return ret;
		}
	};

	class sleeper
	{
	public:
		sleeper(UINT time_period = 5);
		void sleep(DWORD t);
		void wait(DWORD t);
		~sleeper();
	};
	sleeper::sleeper(UINT time_period/*=5*/)
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
};
#endif // namespace_bionukg

#endif // !_bionukg_timer_h
