#pragma once
#ifndef bionukg_charset_h
#define bionukg_charset_h
#include <stdio.h>
#include <Windows.h>
#include <unordered_set>
#include <string>
#include <io.h>
#include <fcntl.h>
#ifdef bionukg_namespace
namespace bionukg
{
#endif

    /*

    SetConsoleOutputCP(CP_UTF8);
     _setmode(_fileno(stdout), _O_U16TEXT);
     */

    // Define common character encodings
    enum CharSet_
    {
        CS_UTF8 = CP_UTF8,     // UTF-8 encoding
        CS_UTF16LE = 1200,     // UTF-16 little-endian encoding
        CS_UTF16BE = 1201,     // UTF-16 big-endian encoding
        CS_GB2312 = 54936,     // GB2312 encoding, mainly used for Simplified Chinese
        CS_GBK = 936,          // GBK encoding, mainly used for Simplified Chinese
        CS_BIG5 = 950,         // Big5 encoding, mainly used for Traditional Chinese
        CS_ISO8859_1 = 28591,  // ISO-8859-1 encoding, mainly used for European languages
        CS_WINDOWS1252 = 1252, // Windows-1252 encoding, mainly used for European languages
        CS_ASCII = 20127       // ASCII encoding, mainly used for English and Western European languages
    };

    class charsetString
    {
    public:
        CharSet_ charset;
        char *data;
        size_t size;
        charsetString(CharSet_ cs = CS_UTF8) : charset(cs), data(nullptr), size(0) {}
        charsetString(const charsetString &str)
        {
            charset = str.charset;
            size = str.size;
            data = new char[size];
            memcpy(data, str.data, size + 1);
        }

        charsetString(FILE *fp, int len, CharSet_ cs = CS_UTF8)
        {
            charset = cs;
            size = len + 2;
            data = new char[size];
            fread(data, 1, len, fp);
            data[len] = '\0';
            data[len + 1] = '\0';
        }

        // move constructor
        charsetString(charsetString &&str)
        {
            charset = str.charset;
            size = str.size;
            data = str.data;
            str.data = nullptr;
            str.size = 0;
        }
        charsetString(const char *str)
        {
            charset = CS_UTF8;
            size = strlen(str) + 1;
            data = new char[size];
            memcpy(data, str, size + 1);
        }
        charsetString(const wchar_t *str)
        {
            charset = CS_UTF16LE;
            size = wcslen(str) * 2 + 2;
            data = new char[size];
            memcpy(data, str, size);
        }

        ~charsetString()
        {
            if (data != nullptr)
            {
                delete[] data;
                data = nullptr;
            }
        }

        int to_wchar_t()
        {
            if (charset == CS_UTF16LE)
            {
                return 0;
            }
            int len = MultiByteToWideChar(charset, 0, data, -1, NULL, 0);
            wchar_t *wstr = (wchar_t *)new char[len * 2];
            MultiByteToWideChar(charset, 0, data, -1, wstr, len);
            delete[] data;
            data = (char *)wstr;
            charset = CS_UTF16LE;
            size = len * 2;
            return 0;
        }
        int to_charset(CharSet_ cs)
        {
            if (charset == cs)
            {
                return 0;
            }
            if (cs == CS_UTF16LE)
            {
                return to_wchar_t();
            }
            if (charset != CS_UTF16LE)
            {
                to_wchar_t();
            }
            int len = WideCharToMultiByte(cs, 0, (wchar_t *)data, -1, NULL, 0, NULL, NULL);
            char *str = new char[len];
            WideCharToMultiByte(cs, 0, (wchar_t *)data, -1, str, len, NULL, NULL);
            delete[] data;
            data = str;
            charset = cs;
            size = len;
            return 0;
        }
    };

    class HFONT_
    {
    public:
        HFONT hFont;
        HFONT_(const HFONT_ &hFont_) = delete;
        HFONT_ &operator=(const HFONT_ &hFont_) = delete;
        HFONT_(const wchar_t *font)
        {
            hFont = CreateFontW(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font);
        }
        ~HFONT_()
        {
            DeleteObject(hFont);
        }
    };

    class fontEnumMap
    {
    public:
        std::unordered_set<std::wstring> fontSet;
        fontEnumMap(int charset = DEFAULT_CHARSET, HDC hdc_in = NULL)
        {
            HDC hdc;
            if (hdc_in == NULL)
            {
                hdc = GetDC(NULL);
            }
            else
            {
                hdc = hdc_in;
            }
            LOGFONT lf = {0};
            lf.lfCharSet = charset;
            EnumFontFamiliesExW(
                hdc, &lf,
                [](const LOGFONT *lplf, const TEXTMETRIC *lptm, DWORD dwFontType, LPARAM lpData) -> int
                {
                    std::unordered_set<std::wstring> *fontSet =
                        reinterpret_cast<decltype(fontSet)>(lpData);
                    fontSet->insert(lplf->lfFaceName);
                    return 1;
                },
                reinterpret_cast<LPARAM>(&fontSet), 0);
            if (hdc_in == NULL)
                ReleaseDC(NULL, hdc);
        }

        bool checkFont(const wchar_t *font)
        {
            return fontSet.find(font) != fontSet.end();
        }

        HFONT_ try_getFont(const wchar_t *font)
        {
            if (checkFont(font))
            {
                return HFONT_(font);
            }
            throw "font not found";
        }
    };

    class FILEp
    {
    public:
        enum fmode
        {
            fm_utf8=_O_U8TEXT,
            fm_utf16le=_O_U16TEXT,
            fm_bin=_O_BINARY,
            fm_text=_O_TEXT
        };
        static int setCharset(FILE *fp, fmode cs)
        {
            return _setmode(_fileno(fp), cs);
        }
    };

#ifdef bionukg_namespace
}
#endif

#endif // bionukg_charset_h
