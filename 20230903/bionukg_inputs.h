#pragma once

#ifndef _bionukg_inputs_h
#define _bionukg_inputs_h
#define _CRT_SECURE_NO_WARNINGS
#ifndef _WINDOWS_
#include <Windows.h>
#endif


#pragma comment(lib,"winmm.lib")
#include"./bionukg_timer.h"
#include<vector>
#ifdef namespace_bionukg
namespace bionukg{
#endif //namespace_bionukg

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
enum keycode
{
	LMOUSE_KC = 1,	//鼠标左键
	RMOUSE_KC,		//鼠标右键
	MMOUSE_KC = 4,	//鼠标中键
	X1MOUSE_KC,		//侧键后滚
	X2MOUSE_KC,		//侧键前滚
#ifdef XBUTTON3
	X3MOUSE_KC,		//侧键莫须有
#else
	XBOXGAMEBAR_KC,//也许等同于Win+G组合键的gamebar键
#endif // XBUTTON3

	BACKSPACE_KC = 8,
	TAB_KC,

	ENTER_KC = 13,

	SHIFT_KC = 16,//不区分左右
	CTRL_KC,//不区分左右
	ALT_KC,//不区分左右

	CAPSLOCK_KC = 20,

	ESCAPE_KC = 27,//ESC

	SPACEBAR_KC = 32,//空格
	PAGEUP_KC,
	PAGEDOWN_KC,
	END_KC,
	HOME_KC,
	LEFTARROW_KC,
	UPARROW_KC,
	RIGHTARROW_KC,
	DOWNARROW_KC,

	PRTSCR_KC = 44,
	INSERT_KC,
	DELETE_KC,

	NUM0_KC = '0',//48
	NUM1_KC = '1',
	NUM2_KC = '2',
	NUM3_KC = '3',
	NUM4_KC = '4',
	NUM5_KC = '5',
	NUM6_KC = '6',
	NUM7_KC = '7',
	NUM8_KC = '8',
	NUM9_KC = '9',//57

	A_KC = 'A',//65
	B_KC = 'B',
	C_KC = 'C',
	D_KC = 'D',
	E_KC = 'E',
	F_KC = 'F',
	G_KC = 'G',
	H_KC = 'H',
	I_KC = 'I',
	J_KC = 'J',
	K_KC = 'K',
	L_KC = 'L',
	M_KC = 'M',
	N_KC = 'N',
	O_KC = 'O',
	P_KC = 'P',
	Q_KC = 'Q',
	R_KC = 'R',
	S_KC = 'S',
	T_KC = 'T',
	U_KC = 'U',
	V_KC = 'V',
	W_KC = 'W',
	X_KC = 'X',
	Y_KC = 'Y',
	Z_KC = 'Z',//90
	LWINLOGOKEY_KC,//田
	RWINLOGOKEY_KC,//田

	F1_KC = 112,
	F2_KC,
	F3_KC,
	F4_KC,
	F5_KC,
	F6_KC,
	F7_KC,
	F8_KC,
	F9_KC,
	F10_KC,
	F11_KC,
	F12_KC,//123

	LSHIFT_KC = 160,//区分左右
	RSHIFT_KC,
	LCTRL_KC,
	RCTRL_KC,
	LALT_KC,
	RALT_KC,

	SEMICOLON_KC = 186,	//符号';'
	EQUALS_KC = 187,	//符号'='
	COMMA_KC,			//符号','
	MINUS_DASH_KC,			//符号'-'
	PERIOD_KC,			//符号'.'
	FORWARDSLASH_KC,	//符号'/'
	BACKQUOTE_GRAVEACCENT_KC,//符号'`'
						


	LSUQAREBRAKET_KC = 219,//符号'[
						
	BACKSHALSH_KC,		//符号'\\'
	RSUQAREBRAKET_KC,	//符号']'
	SINGELQUOTE_KC			//符号'''



};

//只包含按下状态，不包含capslock等键的锁定状态
class prtio
{
public:
	uint32_t keydown[8];//256keys
	POINT mousepos;
#ifdef _bionukg_inputsdx_h
	LONG mouse_dxyz[3];
#endif
	void pack(_In_reads_(256) uint8_t keystate[256],_In_ POINT mousecoord
#ifdef _bionukg_inputsdx_h
		, _In_ DIMOUSESTATE mouseD
#endif
	)
	{
#ifdef _bionukg_inputsdx_h
		mouse_dxyz[0] = mouseD.lX;
		mouse_dxyz[1] = mouseD.lY;
		mouse_dxyz[2] = mouseD.lZ;
#endif
		mousepos.x = mousecoord.x;
		mousepos.y = mousecoord.y;
		for (size_t j = 0; j < 256; j++)
		{
			keydown[j / 32] <<= 1;
			keydown[j / 32] |= keystate[j] > 0x40 ? 1 : 0;
		}
	}
	void unpack(_Out_writes_(256) uint8_t* keystate, _Out_ POINT* mousecoord
#ifdef _bionukg_inputsdx_h
		, _Out_  DIMOUSESTATE* mouseD
#endif	
	)
	{
#ifdef _bionukg_inputsdx_h
		mouseD->lX = mouse_dxyz[0];
		mouseD->lY = mouse_dxyz[1];
		mouseD->lZ = mouse_dxyz[2];
#endif
		mousecoord->x = mousepos.x;
		mousecoord->y = mousepos.y;
		for (size_t j = 0; j < 256; j++)
		{
			keystate[j] = (uint8_t(keydown[j / 32] >> (31-j % 32)) & 1) * 0x80;
		}
	}
};
//获得用户的键盘鼠标设备操作
class outputs
{
protected:
	uint8_t keystate[256];
	POINT mousecoord;
#ifdef _bionukg_inputsdx_h
	mouse_d mouseD;
	keyboard_d keyboardD;
#endif
	void wait(DWORD time_ms)
	{
		/*timer t;
		uint8_t c = 0;
		while (t.from_start((DWORD*)0)<15)
		{
			c++;
		}*/
		Sleep(time_ms);
	}
public:
	const POINT& get_mousecoord() { return mousecoord; }
	void refresh() {
		GetKeyState(0);
		GetKeyboardState(keystate);
		GetCursorPos(&mousecoord);
		
		


#ifdef _bionukg_inputsdx_h
		mouseD.refresh();
		keyboardD.refresh();
#endif
	}
	void prt(int width = 16,_Out_writes_(4096) char* buffer = 0)
	{
		if (buffer == 0) {
			printf("x=%5d,y=%5d\n", mousecoord.x, mousecoord.y);
#ifdef _bionukg_inputsdx_h
			printf("dx=%5d,dy=%5d,dz=%5d\n", mouseD.mouseState.lX, mouseD.mouseState.lY, mouseD.mouseState.lZ);
			printf("rgbButtons=%8X\n", *(uint32_t*)mouseD.mouseState.rgbButtons);
#endif
			for (size_t i = 0; i < 256; i++)
			{
				if (i % width == 0)
					printf("%3d:", i);
				printf("%2x%c", keystate[i], (i + 1) % width == 0 ? '\n' : ' ');
			}
		}
		else
		{
			const int totalbuf = 4096;
			int bufrest = totalbuf;
			bufrest -= sprintf_s(buffer+(totalbuf-bufrest), bufrest, "x=%5d,y=%5d\n", mousecoord.x, mousecoord.y);
#ifdef _bionukg_inputsdx_h
			bufrest -= sprintf_s(buffer + (totalbuf - bufrest), bufrest, "dx=%5d,dy=%5d,dz=%5d\n", mouseD.mouseState.lX, mouseD.mouseState.lY, mouseD.mouseState.lZ);
			bufrest -= sprintf_s(buffer + (totalbuf - bufrest), bufrest, "rgbButtons=%8X\n", *(uint32_t*)mouseD.mouseState.rgbButtons);
#endif
			for (size_t i = 0; i < 256; i++)
			{
				if (i % width == 0)
					bufrest -= sprintf_s(buffer + (totalbuf - bufrest), bufrest, "%3d:", i);
				bufrest -= sprintf_s(buffer + (totalbuf - bufrest), bufrest, "%2x%c", keystate[i], (i + 1) % width == 0 ? '\n' : ' ');
			}
		}
	}
	void txt_record_sync(const char* path, size_t total_frames, DWORD deltat_ms = 15)
	{
		FILE* fp;
		if (path != 0)
			fopen_s(&fp,path, "wb+");
		if (fp == 0)
		{
			printf("fopen failed with path %s", path);
			system("pause");
			return;
		}

		refresh();
		for (size_t i = 0; i < total_frames; i++)
		{
			for (size_t j = 0; j < 256; j++)
			{
				if (keystate[j] > 0x80)
				{
					fprintf(fp, "%ld ", j);
				}
			}
			fprintf(fp, "\n%d %d\n", mousecoord.x, mousecoord.y);

#ifdef _bionukg_inputsdx_h
			fprintf(fp, "%d %d %d\n", mouseD.mouseState.lX, mouseD.mouseState.lY, mouseD.mouseState.lZ);
#endif

			wait(deltat_ms);
			refresh();
		}
		fclose(fp);
	}
	void bin_record_sync(const char* path, size_t total_frames, DWORD deltat_ms = 15)
	{
		FILE* fp;

		if (path != 0)
			fopen_s(&fp, path, "wb+");
		if (fp == 0)
		{
			printf("fopen failed with path %s", path);
			system("pause");
			return;
		}
		prtio ptem;
		for (size_t i = 0; i < total_frames; i++)
		{

			refresh();
			ptem.pack(keystate, mousecoord
#ifdef _bionukg_inputsdx_h
				,mouseD.mouseState
#endif
			);
			fwrite(&ptem, sizeof(ptem), 1, fp);

			wait(deltat_ms);

		}
		fclose(fp);
		
	};
	bool is_down(uint8_t keyid)
	{
		return keystate[keyid] > 0x40 ? true : false;
	}
	bool is_down(keycode keyid)
	{
		return keystate[keyid] > 0x40 ? true : false;
	}

	void get_mouse(LONG* x, LONG* y)
	{
		*x = mousecoord.x;
		*y = mousecoord.y;
	}
#ifdef _bionukg_inputsdx_h

	void get_mouse_move(LONG*x, LONG*y, LONG*z=0)
	{
		*x = mouseD.mouseState.lX;
		*y = mouseD.mouseState.lY;
		if(z!=0)
			*z= mouseD.mouseState.lZ;
	}
#endif


	~outputs()
	{

	}
};
//向电脑模拟用户输入
class inputs 
{
private:
	INPUT* input = 0;
	DWORD* delay_ms = 0;
	size_t length = 0;
	size_t size = 0;

private:
	void resize(size_t newsize)
	{
		if (length > newsize)
			length = newsize;
		INPUT* ninputs = new INPUT[newsize]{};
		DWORD* ndelay_ms = new DWORD[newsize]{};
		if (input!=0)
		{
			memcpy(ninputs, input, sizeof(INPUT) * length);
			delete[] input;
		}
		input = ninputs;
		if (delay_ms != 0)
		{
			memcpy(ndelay_ms, delay_ms, sizeof(DWORD) * length);
			delete[] delay_ms;
		}
		delay_ms = ndelay_ms;
		size = newsize;
		
	}
	void tryextend(size_t moresize)
	{
		if (length + moresize + 4 > size)
		{
			resize(length + moresize + 4);
		}
		
	}

		
#ifdef wait_precise
	void wait(DWORD time_ms)
	{
		timer t;
		volatile uint8_t c = 0;
		while (t.from_start((DWORD*)0) < time_ms)
		{
			c++;
		}
}
#else
	int wait_offset = 0;
	void wait(DWORD time_ms)
	{
		wait_offset += time_ms;
		if (wait_offset < 0)
			return;
		timer t;
		Sleep(wait_offset);
		DWORD dt = t.from_start((DWORD*)0);
		wait_offset -= dt;

	}
#endif // wait_precise

public:
	
	void execute_sync(void)
	{
		for (size_t i = 0; i < length; i++)
		{
			UINT uSent1 = SendInput(1, &input[i], sizeof(INPUT));
			if (uSent1 != 1)
			{
				printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
			}

			
			wait(delay_ms[i]);
		}
		return;
	}
	void add_key_press(char key)
	{
		if (key >= 'a' && key <= 'z')
			add_key_press(key + ('A' - 'a'), 15, 15);
	}
	//字母按键是大写
	void add_key_press(WORD key, DWORD downdelay = 15, DWORD updelay = 15)
	{
		tryextend(2);
		INPUT* in = &input[length];
		in[0].type = INPUT_KEYBOARD;
		in[0].ki.time = 0;
		in[0].ki.dwFlags = 0;
		in[0].ki.wVk = key;
		in[1].type = INPUT_KEYBOARD;
		in[1].ki.time = 0;
		in[1].ki.dwFlags = KEYEVENTF_KEYUP;
		in[1].ki.wVk = key;

		DWORD* de = &delay_ms[length];
		de[0] = downdelay;
		de[1] = updelay;
		length += 2;
	};
	int add_key_press(keycode key, DWORD downdelay = 15, DWORD updelay = 15)
	{
		int ret=0;
		tryextend(2);
		INPUT* in = &input[length];
		memset(in, 0, 2 * sizeof(INPUT));
		switch (key)
		{
		case LMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			in[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			goto mouse_button_click;
		case RMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			in[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			goto mouse_button_click;
		case MMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			in[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			goto mouse_button_click;
		case X1MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON1;
			in[1].mi.dwFlags = MOUSEEVENTF_XUP;
			in[1].mi.mouseData = XBUTTON1;
			goto mouse_button_click;
		case X2MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON2;
			in[1].mi.dwFlags = MOUSEEVENTF_XUP;
			in[1].mi.mouseData = XBUTTON2;
			goto mouse_button_click;
		mouse_button_click:
			in[0].type = INPUT_MOUSE;
			in[1].type = INPUT_MOUSE;
			break;
#ifdef XBUTTON3 
		case X3MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON3;
			in[1].mi.dwFlags = MOUSEEVENTF_XUP;
			in[1].mi.mouseData = XBUTTON3;
			goto mouse_button_click;
#else
		case XBOXGAMEBAR_KC:
#endif
		case BACKSPACE_KC:
		case TAB_KC:
		case ENTER_KC:
		case SHIFT_KC:
		case CTRL_KC:
		case ALT_KC:
		case CAPSLOCK_KC:
		case ESCAPE_KC:
		case SPACEBAR_KC:
		case PAGEUP_KC:
		case PAGEDOWN_KC:
		case END_KC:
		case HOME_KC:
		case LEFTARROW_KC:
		case UPARROW_KC:
		case RIGHTARROW_KC:
		case DOWNARROW_KC:
		case PRTSCR_KC:
		case INSERT_KC:
		case DELETE_KC:
		case NUM0_KC:
		case NUM1_KC:
		case NUM2_KC:
		case NUM3_KC:
		case NUM4_KC:
		case NUM5_KC:
		case NUM6_KC:
		case NUM7_KC:
		case NUM8_KC:
		case NUM9_KC:
		case A_KC:
		case B_KC:
		case C_KC:
		case D_KC:
		case E_KC:
		case F_KC:
		case G_KC:
		case H_KC:
		case I_KC:
		case J_KC:
		case K_KC:
		case L_KC:
		case M_KC:
		case N_KC:
		case O_KC:
		case P_KC:
		case Q_KC:
		case R_KC:
		case S_KC:
		case T_KC:
		case U_KC:
		case V_KC:
		case W_KC:
		case X_KC:
		case Y_KC:
		case Z_KC:
		case LWINLOGOKEY_KC:
		case RWINLOGOKEY_KC:
		case F1_KC:
		case F2_KC:
		case F3_KC:
		case F4_KC:
		case F5_KC:
		case F6_KC:
		case F7_KC:
		case F8_KC:
		case F9_KC:
		case F10_KC:
		case F11_KC:
		case F12_KC:
		case LSHIFT_KC:
		case RSHIFT_KC:
		case LCTRL_KC:
		case RCTRL_KC:
		case LALT_KC:
		case RALT_KC:
		case SEMICOLON_KC:
		case EQUALS_KC:
		case COMMA_KC:
		case MINUS_DASH_KC:
		case PERIOD_KC:
		case FORWARDSLASH_KC:
		case BACKQUOTE_GRAVEACCENT_KC:
		case LSUQAREBRAKET_KC:
		case BACKSHALSH_KC:
		case RSUQAREBRAKET_KC:
		case SINGELQUOTE_KC:
			in[0].type = INPUT_KEYBOARD;
			in[0].ki.dwFlags = 0;
			in[0].ki.wVk = key;
			in[1].type = INPUT_KEYBOARD;
			in[1].ki.dwFlags = KEYEVENTF_KEYUP;
			in[1].ki.wVk = key;
			break;
		default:
			ret = -1;
			break;
		}


		DWORD* de = &delay_ms[length];
		de[0] = downdelay;
		de[1] = updelay;
		length += 2;
		return ret;
	};
	int add_key_down(keycode key, DWORD downdelay = 15)
	{
		int ret = 0;
		tryextend(1);
		INPUT* in = &input[length];
		memset(in, 0,  sizeof(INPUT));
		switch (key)
		{
		case LMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			goto mouse_button_click;
		case RMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
			goto mouse_button_click;
		case MMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
			goto mouse_button_click;
		case X1MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON1;
			goto mouse_button_click;
		case X2MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON2;
			goto mouse_button_click;
		mouse_button_click:
			in[0].type = INPUT_MOUSE;
			break;
#ifdef XBUTTON3 
		case X3MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON3;
			goto mouse_button_click;
#else
		case XBOXGAMEBAR_KC:
#endif
		case BACKSPACE_KC:
		case TAB_KC:
		case ENTER_KC:
		case SHIFT_KC:
		case CTRL_KC:
		case ALT_KC:
		case CAPSLOCK_KC:
		case ESCAPE_KC:
		case SPACEBAR_KC:
		case PAGEUP_KC:
		case PAGEDOWN_KC:
		case END_KC:
		case HOME_KC:
		case LEFTARROW_KC:
		case UPARROW_KC:
		case RIGHTARROW_KC:
		case DOWNARROW_KC:
		case PRTSCR_KC:
		case INSERT_KC:
		case DELETE_KC:
		case NUM0_KC:
		case NUM1_KC:
		case NUM2_KC:
		case NUM3_KC:
		case NUM4_KC:
		case NUM5_KC:
		case NUM6_KC:
		case NUM7_KC:
		case NUM8_KC:
		case NUM9_KC:
		case A_KC:
		case B_KC:
		case C_KC:
		case D_KC:
		case E_KC:
		case F_KC:
		case G_KC:
		case H_KC:
		case I_KC:
		case J_KC:
		case K_KC:
		case L_KC:
		case M_KC:
		case N_KC:
		case O_KC:
		case P_KC:
		case Q_KC:
		case R_KC:
		case S_KC:
		case T_KC:
		case U_KC:
		case V_KC:
		case W_KC:
		case X_KC:
		case Y_KC:
		case Z_KC:
		case LWINLOGOKEY_KC:
		case RWINLOGOKEY_KC:
		case F1_KC:
		case F2_KC:
		case F3_KC:
		case F4_KC:
		case F5_KC:
		case F6_KC:
		case F7_KC:
		case F8_KC:
		case F9_KC:
		case F10_KC:
		case F11_KC:
		case F12_KC:
		case LSHIFT_KC:
		case RSHIFT_KC:
		case LCTRL_KC:
		case RCTRL_KC:
		case LALT_KC:
		case RALT_KC:
		case SEMICOLON_KC:
		case EQUALS_KC:
		case COMMA_KC:
		case MINUS_DASH_KC:
		case PERIOD_KC:
		case FORWARDSLASH_KC:
		case BACKQUOTE_GRAVEACCENT_KC:
		case LSUQAREBRAKET_KC:
		case BACKSHALSH_KC:
		case RSUQAREBRAKET_KC:
		case SINGELQUOTE_KC:
			in[0].type = INPUT_KEYBOARD;
			in[0].ki.dwFlags = 0;
			in[0].ki.wVk = key;
			break;
		default:
			ret = -1;
			break;
		}


		DWORD* de = &delay_ms[length];
		de[0] = downdelay;
		length += 1;
		return ret;
	};
	int add_key_up(keycode key, DWORD updelay = 15)
	{
		int ret = 0;
		tryextend(1);
		INPUT* in = &input[length];
		memset(in, 0, sizeof(INPUT));
		switch (key)
		{
		case LMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
			goto mouse_button_click;
		case RMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
			goto mouse_button_click;
		case MMOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
			goto mouse_button_click;
		case X1MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XUP;
			in[0].mi.mouseData = XBUTTON1;
			goto mouse_button_click;
		case X2MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XUP;
			in[0].mi.mouseData = XBUTTON2;
			goto mouse_button_click;
		mouse_button_click:
			in[0].type = INPUT_MOUSE;
			break;
#ifdef XBUTTON3 
		case X3MOUSE_KC:
			in[0].mi.dwFlags = MOUSEEVENTF_XDOWN;
			in[0].mi.mouseData = XBUTTON3;
			in[1].mi.dwFlags = MOUSEEVENTF_XUP;
			in[1].mi.mouseData = XBUTTON3;
			goto mouse_button_click;
#else
		case XBOXGAMEBAR_KC:
#endif
		case BACKSPACE_KC:
		case TAB_KC:
		case ENTER_KC:
		case SHIFT_KC:
		case CTRL_KC:
		case ALT_KC:
		case CAPSLOCK_KC:
		case ESCAPE_KC:
		case SPACEBAR_KC:
		case PAGEUP_KC:
		case PAGEDOWN_KC:
		case END_KC:
		case HOME_KC:
		case LEFTARROW_KC:
		case UPARROW_KC:
		case RIGHTARROW_KC:
		case DOWNARROW_KC:
		case PRTSCR_KC:
		case INSERT_KC:
		case DELETE_KC:
		case NUM0_KC:
		case NUM1_KC:
		case NUM2_KC:
		case NUM3_KC:
		case NUM4_KC:
		case NUM5_KC:
		case NUM6_KC:
		case NUM7_KC:
		case NUM8_KC:
		case NUM9_KC:
		case A_KC:
		case B_KC:
		case C_KC:
		case D_KC:
		case E_KC:
		case F_KC:
		case G_KC:
		case H_KC:
		case I_KC:
		case J_KC:
		case K_KC:
		case L_KC:
		case M_KC:
		case N_KC:
		case O_KC:
		case P_KC:
		case Q_KC:
		case R_KC:
		case S_KC:
		case T_KC:
		case U_KC:
		case V_KC:
		case W_KC:
		case X_KC:
		case Y_KC:
		case Z_KC:
		case LWINLOGOKEY_KC:
		case RWINLOGOKEY_KC:
		case F1_KC:
		case F2_KC:
		case F3_KC:
		case F4_KC:
		case F5_KC:
		case F6_KC:
		case F7_KC:
		case F8_KC:
		case F9_KC:
		case F10_KC:
		case F11_KC:
		case F12_KC:
		case LSHIFT_KC:
		case RSHIFT_KC:
		case LCTRL_KC:
		case RCTRL_KC:
		case LALT_KC:
		case RALT_KC:
		case SEMICOLON_KC:
		case EQUALS_KC:
		case COMMA_KC:
		case MINUS_DASH_KC:
		case PERIOD_KC:
		case FORWARDSLASH_KC:
		case BACKQUOTE_GRAVEACCENT_KC:
		case LSUQAREBRAKET_KC:
		case BACKSHALSH_KC:
		case RSUQAREBRAKET_KC:
		case SINGELQUOTE_KC:
			in[0].type = INPUT_KEYBOARD;
			in[0].ki.dwFlags = KEYEVENTF_KEYUP;
			in[0].ki.wVk = key;
			break;
		default:
			ret = -1;
			break;
		}


		DWORD* de = &delay_ms[length];
		de[0] = updelay;
		length += 1;
		return ret;
	};
	int add_mouse_move(LONG x, LONG y, DWORD delay = 15)
	{
		int ret = 0;
		tryextend(1);
		INPUT* in = &input[length];
		memset(in, 0, sizeof(INPUT));
		in[0].type = INPUT_MOUSE;
		in[0].mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
		in[0].mi.dx = x;
		in[0].mi.dy = y;
		DWORD* de = &delay_ms[length];
		de[0] = delay;
		length += 1;
		return ret;
	}
	int add_mouse_move_relative(LONG dx, LONG dy, DWORD delay = 15)
	{
		int ret = 0;
		tryextend(1);
		INPUT* in = &input[length];
		memset(in, 0, sizeof(INPUT));
		in[0].type = INPUT_MOUSE;
		in[0].mi.dwFlags = MOUSEEVENTF_MOVE;
		in[0].mi.dx = dx;
		in[0].mi.dy = dy;
		DWORD* de = &delay_ms[length];
		de[0] = delay;
		length += 1;
		return ret;
	}
	int add_mouse_move_scaled(LONG x, LONG y, LONG scr_width = 1280, LONG src_height = 720, DWORD delay = 15)
	{
		int ret = 0;
		tryextend(1);
		INPUT* in = &input[length];
		memset(in, 0, sizeof(INPUT));
		in[0].type = INPUT_MOUSE;
		in[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		in[0].mi.dx = (double)x * 65536 / scr_width;
		in[0].mi.dy = (double)y * 65536 / src_height;
		DWORD* de = &delay_ms[length];
		de[0] = delay;
		length += 1;
		return ret;
	}
	int add_mouse_move_relative_speeded(LONG dx, LONG dy, double len_per_frame, DWORD delay = 15)
	{
		double total_len = sqrt(dx * dx + dy * dy);
		double frames = total_len / len_per_frame;
		double dx_per_frame = dx / frames;
		double dy_per_frame = dy / frames;

		double dx_now = 0, dx_last = 0;
		double dy_now = 0, dy_last = 0;

		for (; ; dx_last = dx_now, dy_last = dy_now)
		{
			dx_now += dx_per_frame;
			dy_now += dy_per_frame;

			if (abs(dx_now) < abs(dx)) {
				LONG dxIn = dx_now - dx_last;
				LONG dyIn = dy_now - dy_last;

				add_mouse_move_relative(dxIn, dyIn, delay);
			}
			else
			{
				LONG dxIn = dx - dx_last;
				LONG dyIn = dy - dy_last;

				add_mouse_move_relative(dxIn, dyIn, delay);
				return 0;
			}
		}



	};
	//我们假设录制和载入时都是以没有按下任何键作为开始
	//void bin_load_keys(const char* path, DWORD deltat_ms = 15)
	//{
	//	FILE* fp;
	//	uint8_t keystate[256],keystatel[256];
	//	POINT mousecoord;
	//	if (path != 0)
	//		fopen_s(&fp, path, "rb+");
	//	if (fp == 0)
	//	{
	//		printf("fopen failed with path %s", path);
	//		system("pause");
	//		return;
	//	}
	//	prtio pio;
	//	auto ftl = ftell(fp);
	//	fread(&pio, sizeof(pio), 1, fp);
	//	if (ftl != ftell(fp))
	//	{
	//		pio.unpack(keystate, &mousecoord);
	//		for (size_t i = 0; i < 256; i++)
	//		{
	//			if(keystate[i]!=keystatel[i])
	//		}
	//	}




	//}
	void delete_key_press(size_t sub=2)
	{
		length -= sub;
	}


};


#ifdef namespace_bionukg
}
#endif //namespace_bionukg

#endif // !_bionukg_inputs_h

