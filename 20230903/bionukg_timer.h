#pragma once
#ifndef _bionukg_timer_h
#define _bionukg_timer_h

#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#include <time.h>
#include "./bionukg_meta_base.h"
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
		~timer(){};

		time_t from_start(time_t *time_s)
		{
			time_t tm = time(0) - t_ms;
			if (time_s != 0)
				*time_s = tm;
			return tm;
		}
		DWORD from_start(DWORD *time_ms)
		{
			DWORD tm = timeGetTime() - t_ms;
			if (time_ms != 0)
				*time_ms = tm;
			return tm;
		}
		LARGE_INTEGER from_start(LARGE_INTEGER *time_us)
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
		sleeper(UINT time_period)
		{
			timeBeginPeriod(time_period);
		}

		void sleep(DWORD t)
		{
			Sleep(t);
		}
		void wait(DWORD t)
		{
			Sleep(t);

		}
		~sleeper()
		{
			timeEndPeriod(1);
		}
	};


	//template <uint_any_t uint_t, typename funcptr_t>
	//class test_bench
	//{
	//public:
	//	volatile funcptr_t func0, func1;
	//	volatile void *d = 0;

	//	const uint_t imax = sizeof(uint_t) > 2 ? 0xfffffff : (~uint_t(0));

	//	test_bench(funcptr_t func_)
	//	{
	//		func0 = func_;
	//		func1 = func_;
	//	};
	//	test_bench(funcptr_t func_, funcptr_t func1_)
	//	{
	//		func0 = func_;
	//		func1 = func1_;
	//	};

	//	~test_bench(){};

	//	LONGLONG time_test(const int fn_ = 0)
	//	{
	//		funcptr_t f = fn_ == 0 ? func0 : func1;
	//		timer t;
	//		volatile auto r = f(0);
	//		d = &r;
	//		for (uint_t i = imax; i > 0; i--)
	//		{
	//			r = f(i);
	//		}
	//		d = 0;
	//		return t.us_form_last() * (0x10000000ui64 / ((uint64_t)imax + 1));
	//	};

	//	bool result_test() // compare if func and func1 is the same
	//	{
	//		if (func0 == func1)
	//			return true;
	//		for (uint_t i = imax;;)
	//		{
	//			if (func0(i) != func1(i))
	//				return false;
	//			if (i == 0)
	//				break;
	//			i--;
	//		}
	//		return true;
	//	};
	//};

#ifdef namespace_bionukg
}
#endif // namespace_bionukg

#endif // !_bionukg_timer_h
