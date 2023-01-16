#pragma once
#ifndef _WINDOWS_
#include <Windows.h>
#endif

#pragma comment(lib,"winmm.lib")




class hardware_inputs
{
#define INPUT_DELAY UINT16_MAX
private:	
	INPUT* inputs = 0;
	DWORD* delay_ms = 0;
	uint32_t length = 0;
	uint32_t capacity = 0;
public:
	hardware_inputs(const hardware_inputs&copy)
	{
		length = copy.length;
		capacity = copy.capacity;
		inputs = new INPUT[copy.capacity]{};
		memcpy(inputs, copy.inputs, length * sizeof(inputs[0]));
		delay_ms = new DWORD[copy.capacity]{};
		memcpy(delay_ms, copy.delay_ms, length * sizeof(delay_ms[0]));
	}
	hardware_inputs()
	{
		inputs = new INPUT[32]{};
		delay_ms = new DWORD[32]{};
		length = 0;
		capacity = 32;
	};
	~hardware_inputs() 
	{
		if (inputs != 0)
			delete[] inputs;
		if (delay_ms != 0)
			delete[] delay_ms;

	};
	//delay_ms[i]解释为距离首次调用的毫秒数(需要保证排序)
	void run_timestamp(void)
	{
		DWORD time0 = timeGetTime(), time1;
		for (uint32_t count = 0; count < length;)
		{
			time1 = timeGetTime() - time0;
			if (time1 >= delay_ms[count])
			{								
				UINT uSent1 = SendInput(1, &inputs[count], sizeof(INPUT));
				if (uSent1 != 1)
				{
					printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
				}
				count++;
			}
		}
		return;
	}
	//delay_ms[i]解释为距离上次调用的毫秒数
	void run_interval(void)
	{
		DWORD time0 = timeGetTime(), time1;
		for (uint32_t count = 0; count < length;)
		{
			time1 = timeGetTime() - time0;
			if (time1 >= delay_ms[count])
			{
				if (inputs[count].type!= INPUT_DELAY) 
				{
					uint32_t multiinput = 1;
					while (delay_ms[count] == 0)
					{
						count++;
						if (count >= length)
						{
							count--;
							break;
						}
						multiinput++;
					}
					UINT uSent1 = SendInput(multiinput, &inputs[count - multiinput + 1], sizeof(INPUT) * multiinput);
					if (uSent1 != 1)
					{
						printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
					}
				}

				time0 = timeGetTime();
				count++;
			}
		}
		return;
	}

	void timestamp2interval(void)
	{
		this->timestamp_sort();
		for (uint32_t i = length - 1; i > 0; i--)
		{
			delay_ms[i] -= delay_ms[i - 1];
		}
		return;
	}

	void timestamp_sort(void)
	{
		for (int sorted = 1; sorted == 1;) 
		{
			sorted = 1;
			for (uint32_t i = 0; i < length - 1; i++)
			{
				if (delay_ms[i] > delay_ms[i + 1])
				{
					this->exchange(i, i + 1);
					sorted = 0;
				}
			}
		}

		return;
	}

	void replace(const INPUT& in, const DWORD& delay, uint32_t index)
	{
		memcpy(&inputs[index], &in, sizeof(INPUT));
		delay_ms[index] = delay;
		return;
	}
	void insert(const INPUT& in, const DWORD& delay, uint32_t index)
	{
		if (length == capacity)
			this->resize(length + 8);
		for (uint32_t i = length; i > index; i--)
		{
			this->replace(inputs[i - 1], delay_ms[i - 1], i);
		}
		this->replace(in, delay, index);
		length++;
		return;
	}
	void remove(uint32_t index)
	{
		for (uint32_t i = index; i < length-1; i++)
		{
			this->replace(inputs[i + 1], delay_ms[i + 1], i);
		}
		length--;
		return;
	}
	void exchange(uint32_t index0, uint32_t index1)
	{		
		auto temi = inputs[index0];
		auto temd = delay_ms[index0];
		inputs[index0] = inputs[index1];
		delay_ms[index0] = delay_ms[index1];
		inputs[index1] = temi;
		delay_ms[index1] = temd;
		return;
	}
	void resize(uint32_t new_size)
	{
		INPUT* new_inputs = new INPUT[new_size]{};
		DWORD* new_input_time = new DWORD[new_size]{};
		if (length < new_size)
			length = new_size;

		if (inputs != 0)
		{
			memcpy(new_inputs, inputs, sizeof(inputs[0]) * length);
			delete[] inputs;
		}
		inputs = new_inputs;

		if (delay_ms != 0)
		{
			memcpy(new_input_time, delay_ms, sizeof(delay_ms[0]) * length);
			delete[] delay_ms;
		}
		delay_ms = new_input_time;

		capacity = new_size;

	}


	void keyprint_interval(WORD key, DWORD duration)
	{
		if (length + 2 >= capacity)
			resize(capacity + 8);
		inputs[length].type = INPUT_KEYBOARD;
		inputs[length].ki.wVk = key;
		inputs[length].ki.dwFlags = 0;
		delay_ms[length] = 0;
		length++;
		inputs[length].type = INPUT_KEYBOARD;
		inputs[length].ki.wVk = key;
		inputs[length].ki.dwFlags = KEYEVENTF_KEYUP;
		delay_ms[length] = duration;
		length++;
	}
	void mousemove_internal(DWORD dx,DWORD dy, DWORD duration)
	{
		if (length + 2 >= capacity)
			resize(capacity + 8);
		inputs[length].type = INPUT_MOUSE;
		inputs[length].mi.dx = dx;
		inputs[length].mi.dy = dy;
		inputs[length].mi.dwFlags = MOUSEEVENTF_MOVE;
		delay_ms[length] = duration;
		length++;
		return;
	}
	void mouseclick_internal(DWORD duration,DWORD MouseEventF_whichDOWN)
	{
		if (MouseEventF_whichDOWN != MOUSEEVENTF_LEFTDOWN &&
			MouseEventF_whichDOWN != MOUSEEVENTF_RIGHTDOWN &&
			MouseEventF_whichDOWN != MOUSEEVENTF_MIDDLEDOWN &&
			MouseEventF_whichDOWN != MOUSEEVENTF_XDOWN)
		{
			return;
		}
		if (length + 2 >= capacity)
			resize(capacity + 8);
		inputs[length].type = INPUT_MOUSE;
		inputs[length].mi.dx = 0;
		inputs[length].mi.dy = 0;
		inputs[length].mi.dwFlags = MouseEventF_whichDOWN;
		delay_ms[length] = 0;
		length++;
		inputs[length].type = INPUT_MOUSE;
		inputs[length].mi.dx = 0;
		inputs[length].mi.dy = 0;
		inputs[length].mi.dwFlags = MouseEventF_whichDOWN << 1;
		delay_ms[length] = duration;
		length++;

		return;
	}
	   


};
class hardware_getinput
{
private:
	uint8_t keystate[256];
public:
	BOOL get() {
		GetKeyState(0);
		return GetKeyboardState(keystate);
	}
	enum keycode
	{
		LMOUSE = 1,		//鼠标左键
		RMOUSE,			//鼠标右键
		MMOUSE = 4,		//鼠标中键
		BMOUSE,			//侧键后滚
		FMOUSE,			//侧键前滚



		LSHIFT = 160,	//左shift
		RSHIFT,
		LCTRL,
		RCTRL,
		LALT,
		RALT    
		

	};
	void prt(int width)
	{
		for (size_t i = 0; i < 256; i++)
		{
			if (i % width == 0)
				printf("%3d:", i);
			printf("%2x%c", keystate[i], (i + 1) % width == 0 ? '\n' : ' ');
		}
	}
};
