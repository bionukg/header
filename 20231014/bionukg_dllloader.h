// load a dll
#pragma once

#ifndef bionukg_dllloader_h
#define bionukg_dllloader_h
#include <Windows.h>
#include <libloaderapi.h>
#ifdef bionukg_namespace
namespace bionukg
{
#endif // bionukg_namespace
#include "./bionukg_dllloader_def.h"

    class dll
    {
    private:
        HMODULE libm = 0;
        int fncnt = 0;
        char dllname[255] = {};
        FARPROC fnptrs[256] = {};
        char *fnnames[256] = {};

    public:
        dll(){};
        dll(const char *path)
        {
            strcpy_s(dllname, path);
            libm = LoadLibraryA(dllname);
        };
        FARPROC get_fnptr(const char *fnname)
        {
            if (fncnt != 0xff)
            {
                auto len = strlen(fnname) + 1;
                fnnames[fncnt] = new char[len];
                strcpy_s(fnnames[fncnt], len, fnname);
                fnptrs[fncnt] = GetProcAddress(libm, fnnames[fncnt]);
                return fnptrs[fncnt++];
            }
            return GetProcAddress(libm, fnname);
        }

        ~dll()
        {
            FreeLibrary(libm);
            for (int i = 0; i < fncnt; i++)
            {
                delete[] fnnames[i];
            }
        }
    };

    auto &msvcrt_dll()
    {
        static class _
        {
            typedef struct _iobuf
            {
                void *_Placeholder;
            } *FILE;
            typedef void *va_list;
        public:
            const HMODULE libm = LoadLibraryA("msvcrt.dll");

            int (*const printf)(const char *_Format, ...) = (decltype(printf))GetProcAddress(libm, "printf");
            int (*const sprintf)(char *_Buffer, const char *_Format, ...) = (decltype(sprintf))GetProcAddress(libm, "sprintf");
            int (*const fprintf)(FILE *_File, const char *_Format, ...) = (decltype(fprintf))GetProcAddress(libm, "fprintf");
            int (*const vfprintf)(FILE *_File, const char *_Format, va_list _ArgList) = (decltype(vfprintf))GetProcAddress(libm, "vfprintf");
            int (*const vprintf)(const char *_Format, va_list _ArgList) = (decltype(vprintf))GetProcAddress(libm, "vprintf");
            int (*const vsprintf)(char *_Buffer, const char *_Format, va_list _ArgList) = (decltype(vsprintf))GetProcAddress(libm, "vsprintf");
            int (*const vsnprintf)(char *_Buffer, size_t _BufferCount, const char *_Format, va_list _ArgList) = (decltype(vsnprintf))GetProcAddress(libm, "vsnprintf");
            int (*const scanf)(const char *_Format, ...) = (decltype(scanf))GetProcAddress(libm, "scanf");
            int (*const sscanf)(const char *_Buffer, const char *_Format, ...) = (decltype(sscanf))GetProcAddress(libm, "sscanf");
            int (*const fscanf)(FILE *_File, const char *_Format, ...) = (decltype(fscanf))GetProcAddress(libm, "fscanf");
            int (*const vfscanf)(FILE *_File, const char *_Format, va_list _ArgList) = (decltype(vfscanf))GetProcAddress(libm, "vfscanf");
            int (*const vscanf)(const char *_Format, va_list _ArgList) = (decltype(vscanf))GetProcAddress(libm, "vscanf");
            int (*const vsscanf)(const char *_Buffer, const char *_Format, va_list _ArgList) = (decltype(vsscanf))GetProcAddress(libm, "vsscanf");

            void *(*const malloc)(size_t _Size) = (decltype(malloc))GetProcAddress(libm, "malloc");
            void (*const free)(void *_Memory) = (decltype(free))GetProcAddress(libm, "free");

            void *(*const memcpy)(void *_Dst, const void *_Src, size_t _Size) = (decltype(memcpy))GetProcAddress(libm, "memcpy");
            void *(*const memmove)(void *_Dst, const void *_Src, size_t _Size) = (decltype(memmove))GetProcAddress(libm, "memmove");
            void *(*const memset)(void *_Dst, int _Val, size_t _Size) = (decltype(memset))GetProcAddress(libm, "memset");

            // fread/fwrite/fseek/ftell/fclose/fopen
            size_t (*const fread)(void *_DstBuf, size_t _ElementSize, size_t _Count, FILE *_File) = (decltype(fread))GetProcAddress(libm, "fread");
            size_t (*const fwrite)(const void *_Str, size_t _Size, size_t _Count, FILE *_File) = (decltype(fwrite))GetProcAddress(libm, "fwrite");
            int (*const fseek)(FILE *_File, long _Offset, int _Origin) = (decltype(fseek))GetProcAddress(libm, "fseek");
            long (*const ftell)(FILE *_File) = (decltype(ftell))GetProcAddress(libm, "ftell");
            int (*const fclose)(FILE *_File) = (decltype(fclose))GetProcAddress(libm, "fclose");
            FILE *(*const fopen)(const char *_Filename, const char *_Mode) = (decltype(fopen))GetProcAddress(libm, "fopen");

        } msvcrt_inst;

        return msvcrt_inst;
    }
#define msvcrt_ msvcrt_dll()

#include "./bionukg_dllloader_undef.h"

#ifdef bionukg_namespace
};
#endif // bionukg_namespace

#endif