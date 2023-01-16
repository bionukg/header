#pragma once
#ifndef _bionukg_timer_h
#define _bionukg_timer_h

#include<windows.h>
#include<process.h>
#include<mmsystem.h>
#include<time.h>
#pragma comment(lib,"winmm.lib")

class timer
{
public:
	timer()
	{
		QueryPerformanceCounter(&t_us);
		t_lus.QuadPart = t_us.QuadPart;
		QueryPerformanceFrequency(&freq);
	};
	~timer() {};
	void from_start(time_t* time_s)
	{
		*time_s = time(0) - t_s;
	}
	void from_start(DWORD* time_ms)
	{
		*time_ms = timeGetTime() - t_ms;
	}
	void from_start(LARGE_INTEGER* time_us)
	{
		LARGE_INTEGER  tem;
		QueryPerformanceCounter(&tem);
		time_us->QuadPart = tem.QuadPart - t_us.QuadPart;
	}

	time_t s_form_last()
	{
		time_t tem = time(0),ret=tem-t_ls;
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
private:
	time_t t_s = time(0),t_ls=t_s;
	DWORD t_ms = timeGetTime(),t_lms=t_ms;
	LARGE_INTEGER  t_us,freq,t_lus;

};




#endif // !_bionukg_timer_h
