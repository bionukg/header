#pragma once

#ifndef bionukg_window_h
#define bionukg_window_h
#include<Windows.h>
#include<wingdi.h>
#include<iostream>
#include<vector>
#include<thread>

#include"./bionukg_inputsdx.h"

void change_resolution(int width, int height) {
    // Get the current screen resolution
    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm)) {
        std::cout << "Current screen resolution: " << dm.dmPelsWidth << "x" << dm.dmPelsHeight << std::endl;
    }

    // Change the screen resolution
    dm.dmPelsWidth = width;
    dm.dmPelsHeight = height;
    dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    LONG result = ChangeDisplaySettings(&dm, 0);
    if (result == DISP_CHANGE_SUCCESSFUL) {
        std::cout << "Screen resolution changed successfully" << std::endl;
    }
    else {
        std::cerr << "Failed to change screen resolution" << std::endl;
    }
}

void invisible_cursor() {

    BYTE color = 255;
    // Create a monochrome mask for the cursor
    HBITMAP mask = CreateBitmap(1, 1, 1, 1, &color);

    // Create an icon from the mask
    ICONINFO ii = { 0 };
    ii.fIcon = FALSE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = mask;
    ii.hbmColor = mask;
    HCURSOR cursor = CreateIconIndirect(&ii);

    // Set the cursor
    SetCursor(cursor);

    // Clean up
    DeleteObject(mask);
}

void nullcursor()
{
    // Replace these values with the actual data of your bitmap
    int width = 1;
    int height = 1;

    BYTE bitmapData[4] = { 0,0,0,255 };

    // Create a monochrome mask for the cursor
    HBITMAP mask = CreateBitmap(width, height, 1, 1, &bitmapData[3]);

    // Create a color bitmap for the cursor
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    HDC hdc = GetDC(NULL);
    HBITMAP color = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, bitmapData, &bmi, DIB_RGB_COLORS);
    ReleaseDC(NULL, hdc);

    // Create an icon from the bitmaps
    ICONINFO ii = { 0 };
    ii.fIcon = FALSE;
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = mask;
    ii.hbmColor = color;
    HCURSOR cursor = CreateIconIndirect(&ii);

    // Set the cursor
    SetCursor(cursor);

    // Clean up
    DeleteObject(mask);
    DeleteObject(color);

}

void set_cursor_as(uint32_t* picdata, int pic_wid, int pic_hei) {
    // Create a bitmap mask for the cursor
    HBITMAP hbmMask = CreateBitmap(pic_wid, pic_hei, 1, 1, NULL);

    // Create a bitmap for the cursor
    HBITMAP hbmColor = CreateBitmap(pic_wid, pic_hei, 1, 32, picdata);

    // Create an icon information structure
    ICONINFO ii;
    ii.fIcon = FALSE; // Cursor
    ii.xHotspot = 0;
    ii.yHotspot = 0;
    ii.hbmMask = hbmMask;
    ii.hbmColor = hbmColor;

    // Create the cursor from the icon information
    HCURSOR hCursor = CreateIconIndirect(&ii);

    // Set the cursor
    SetCursor(hCursor);

    // Clean up
    DeleteObject(hbmMask);
    DeleteObject(hbmColor);
}

void gray_map(uint32_t* buf) {
    for (uint32_t y = 0; y < 256; y++) {
        for (uint32_t x = 0; x < 256; x++) {
            buf[y * 256 + x] = (y << 24) | (x << 16) | (x << 8) | x;
        }
    }
}

void gray_map_cursor()
{
    std::vector<uint32_t> buf(256 * 256);
    gray_map(buf.data());
    set_cursor_as(buf.data(), 256, 256);
}

void set_window_on_top(HWND hwnd) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


static outputs ou;

class onceobj
{
public:
    onceobj(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        gray_map_cursor();
        //set_window_on_top(hwnd);
    }

    void rep(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LONG mx, my;
        ou.get_mouse(&mx, &my);
        mx &= 0xff;
        my &= 0xff;
        std::vector<uint32_t> buf(256 * 256);
        for (uint32_t y = 0; y < 256; y++) {
            for (uint32_t x = 0; x < 256; x++) {
                buf[y * 256 + x] = (y << 24) | (mx << 16) | (my << 8) | x;
            }
        }
        set_cursor_as(buf.data(), 256, 256);
        if (ou.is_down(BACKQUOTE_GRAVEACCENT_KC))
        {
            set_window_on_top(hwnd);
        }

    }
};



LRESULT CALLBACK minWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static onceobj once(hwnd, uMsg, wParam, lParam);
    //for (;;)
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_NULL:        
        once.rep(hwnd, uMsg, wParam, lParam);
        return 0;
    default:
        once.rep(hwnd, uMsg, wParam, lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void full_window(WNDPROC wproc = minWindowProc)
{

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = wproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"FullScreenWindowClass";
    RegisterClass(&wc);

    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);
    HWND hwnd = CreateWindow(L"FullScreenWindowClass", L"Full Screen Window", WS_POPUP, 0, 0, w, h, NULL, NULL, GetModuleHandle(NULL), NULL);
    ShowWindow(hwnd, SW_SHOW);


    for (;;) {
        ou.refresh();
        CallWindowProc(wproc, hwnd, 0, 0, 0);
        Sleep(15);
    }
}


#endif