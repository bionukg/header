#pragma once

#ifndef _bionukg_console_h
#define _bionukg_console_h

#ifndef _WINDOWS_
#include <Windows.h>
#endif
#ifdef namespace_bionukg
namespace bionukg {
#endif //namespace_bionukg

    class console
    {
    public:
        HANDLE txthd = 0;;
        CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
        char* txtbuffer = 0;
        //文字颜色
        void set_txt_color(bool R, bool G, bool B)
        {
            set_txt_color(R, G, B, 0);
        }
        void set_txt_color(bool R, bool G, bool B, bool I)
        {
            SetConsoleTextAttribute(txthd, (I == 0 ? 0 : 1) << 3 | (R == 0 ? 0 : 1) << 2 |
                (G == 0 ? 0 : 1) << 1 | (B == 0 ? 0 : 1));
        }
        //背景颜色
        
        void set_bgd_color(bool R, bool G, bool B)
        {
            set_bgd_color(R, G, B, 0);
        }
        void set_bgd_color(bool R, bool G, bool B, bool I)
        {
            SetConsoleTextAttribute(txthd, ((I == 0 ? 0 : 1) << 3 | (R == 0 ? 0 : 1) << 2 |
                (G == 0 ? 0 : 1) << 1 | (B == 0 ? 0 : 1)) << 4);
        }
        void cls(void)
        {
            SetConsoleCursorPosition(txthd, { 0,0 });
        }
        void ShowConsoleAttribute(void)
        {
            GetConsoleScreenBufferInfo(txthd, &csbi);
            printf("X:%3d Y:%3d ", csbi.dwSize.X, csbi.dwSize.Y);
            printf("X:%3d Y:%3d ", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
            printf("%x", csbi.wAttributes);
            printf("L:%3d R:%3d T:%3d B:%3d ",
                csbi.srWindow.Left, csbi.srWindow.Right, csbi.srWindow.Top, csbi.srWindow.Bottom);
            printf("X:%3d Y:%3d ", csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y);

        }





        console(HANDLE conshd) {
            txthd = conshd;
            txtbuffer = new char[256 * 256];
        }
        console(void)
        {
            txthd = GetStdHandle(STD_OUTPUT_HANDLE);
            txtbuffer = new char[256 * 256];
        }
        ~console(void)
        {
            if (txtbuffer != 0)
            {
                delete[] txtbuffer;
                txtbuffer = 0;
            }
            CloseHandle(txthd);
        }


    };






#ifdef namespace_bionukg
}
#endif //namespace_bionukg

#endif //!_bionukg_console_h