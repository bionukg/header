#pragma once
#ifndef _WINDOWS_
#include <Windows.h>
#endif

#ifdef _bionukg_inputs_h
#error "do not include or define _bionukg_inputs_h, I will do this."
#endif // _bionukg_inputs_h

#ifndef _bionukg_inputsdx_h
#define _bionukg_inputsdx_h

#include <dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef namespace_bionukg
namespace bionukg {
#endif //namespace_bionukg
    //mouse device of dinput8
	class mouse_d
	{
    private:
        LPDIRECTINPUT8 di;
        HRESULT hr;
        LPDIRECTINPUTDEVICE8 mouse;

    public:
        DIMOUSESTATE mouseState = {};

        mouse_d(HWND hwnd = NULL)
		{
            if (hwnd == NULL)
            {
                hwnd = GetConsoleWindow();
            }

            // Create a DirectInput object
            hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL);

            // Create a mouse device object
            hr = di->CreateDevice(GUID_SysMouse, &mouse, NULL);

            // Set the data format to "mouse format"
            hr = mouse->SetDataFormat(&c_dfDIMouse);

            // Set the cooperative level
            hr = mouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

            // Acquire the mouse device
            hr = mouse->Acquire();


		}
        void refresh()
        {
            hr = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
        }
		~mouse_d()
		{
            // Release the mouse device
            mouse->Unacquire();

            // Release the DirectInput object
            di->Release();

		}
	};

    //keyboard device of dinput8
    class keyboard_d
    {
    private:
        LPDIRECTINPUT8 di;
        HRESULT hr;
        LPDIRECTINPUTDEVICE8 keyboard;
    public:
        char keyState[256] = {};
        keyboard_d(HWND hwnd=NULL)
        {
            if (hwnd==NULL)
            {
                hwnd = GetConsoleWindow();
            }

            // Create a DirectInput object
            hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, NULL);

            // Create a keyboard device object
            hr = di->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);

            // Set the data format to "keyboard format"
            hr = keyboard->SetDataFormat(&c_dfDIKeyboard);

            // Set the cooperative level
            hr = keyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

            // Acquire the keyboard device
            hr = keyboard->Acquire();
        }
        void refresh()
        {
            hr = keyboard->GetDeviceState(sizeof(keyState), (LPVOID)&keyState);
        }
    };
        
#ifdef namespace_bionukg
};
#endif //namespace_bionukg
#endif //!_bionukg_inputsdx_h
#include "./bionukg_inputs.h"