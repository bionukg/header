// load a dll
#pragma once

#ifndef bionukg_dllloader_h
#define bionukg_dllloader_h
#include<Windows.h>
#include <libloaderapi.h>

class dll
{
private:
    HMODULE libm = 0;
    uint8_t fncnt = 0;
    char dllname[255] = {};
    FARPROC fnptrs[256] = {};
    char* fnnames[256] = {};

public:
    dll() {};
    dll(const char* path)
    {
        strcpy_s(dllname, path);
        libm = LoadLibraryA(dllname);
    };
    FARPROC get_fnptr(const char* fnname)
    {
        if (fncnt != 0xff)
        {      
            auto len = strlen(fnname)+1;
            fnnames[fncnt] = new char[len];
            strcpy_s(fnnames[fncnt],len, fnname);
            fnptrs[fncnt] = GetProcAddress(libm, fnnames[fncnt]);
            return fnptrs[fncnt++];
        }
        return GetProcAddress(libm, fnname);
    }

    ~dll()
    {
        FreeLibrary(libm);
        for (uint8_t i = 0; i < fncnt; i++)
        {
            delete[] fnnames[i];
        }
    }
};

#endif