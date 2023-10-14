#pragma once
#ifndef bionukg_audiowavdx_h
#define bionukg_audiowavdx_h

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

#include "./bionukg_audiowav.h"

#ifdef _DEBUG
#define audiowavdx_err(errmsg)                                                                            \
    do                                                                                                    \
    {                                                                                                     \
        printf("error at file %s, line %d, function %s: %s\n", __FILE__, __LINE__, __FUNCTION__, errmsg); \
        throw errmsg;                                                                                     \
    } while (0)
#elif defined(bionukg_print_errmsg)
#define audiowavdx_err(errmsg) printf("error at file %s, line %d, function %s: %s\n", __FILE__, __LINE__, __FUNCTION__, errmsg)
#else
#define audiowavdx_err(errmsg)
#endif // _DEBUG
    class IDirectSound8_obj
    {
    public:
        IDirectSound8 *DS8;
        IDirectSound8_obj(HWND hwnd = GetDesktopWindow())
        {
            DS8 = NULL;
            if (FAILED(DirectSoundCreate8(NULL, &DS8, NULL)))
            {
                DS8 = NULL;
                audiowavdx_err("DirectSoundCreate8 failed\n");
                return;
            }
            // Set DirectSound cooperative level
            if (FAILED(DS8->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
            {
                DS8->Release();
                DS8 = NULL;
                audiowavdx_err("SetCooperativeLevel failed\n");
                return;
            }
        }
        ~IDirectSound8_obj()
        {
            if (DS8 != NULL)
            {
                DS8->Release();
                DS8 = NULL;
            }
        }
    };

    class IDirectSoundBuffer_obj
    {
    public:
        IDirectSoundBuffer *dsbuf;
        IDirectSound8_obj &DS8;
        enum init_type
        {
            init_default,
            init_secondary,
        };

        void init(init_type type = init_default, WAVEFORMATEX *wavFmt = nullptr)
        {
            dsbuf = NULL;
            DSBUFFERDESC dsBufDes;
            ZeroMemory(&dsBufDes, sizeof(DSBUFFERDESC));

            switch (type)
            {

            case init_secondary:
                dsBufDes.dwSize = sizeof(DSBUFFERDESC);
                dsBufDes.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS;
                dsBufDes.dwBufferBytes = wavFmt->nAvgBytesPerSec / 2; // Half a second of audio data
                dsBufDes.lpwfxFormat = wavFmt;
                break;
            case init_default:
            default:
                dsBufDes.dwSize = sizeof(DSBUFFERDESC);
                dsBufDes.dwFlags = DSBCAPS_PRIMARYBUFFER;
                dsBufDes.dwBufferBytes = 0;
                break;
            }
            if(FAILED(DS8.DS8->CreateSoundBuffer(&dsBufDes, &dsbuf, NULL)))
            {
                dsbuf = NULL;
                audiowavdx_err("CreateSoundBuffer failed\n");
                return;
            }
        }

        IDirectSoundBuffer_obj(IDirectSound8_obj &DS8_) : DS8(DS8_)
        {
            init();
        }

        void set_format(WORD bits_per_sample = 16, DWORD sample_rate = 48000, WORD channels = 2)
        {
            WAVEFORMATEX wavFmt;
            ZeroMemory(&wavFmt, sizeof(WAVEFORMATEX));
            wavFmt.wFormatTag = WAVE_FORMAT_PCM;
            wavFmt.nChannels = channels;
            wavFmt.nSamplesPerSec = sample_rate;
            wavFmt.wBitsPerSample = bits_per_sample;
            wavFmt.nBlockAlign = wavFmt.wBitsPerSample / 8 * wavFmt.nChannels;
            wavFmt.nAvgBytesPerSec = wavFmt.nSamplesPerSec * wavFmt.nBlockAlign;
            set_format(wavFmt);
        }
        void set_format(WAVEFORMATEX &wavFmt)
        {
            if (dsbuf == NULL)
            {
                audiowavdx_err("set_format failed\n");
                return;
            }
            if (FAILED(dsbuf->SetFormat(&wavFmt)))
            {
                audiowavdx_err("SetFormat failed\n");
                return;
            }
        }

        ~IDirectSoundBuffer_obj()
        {
            if (dsbuf != NULL)
            {
                dsbuf->Release();
                dsbuf = NULL;
            }
        }
    };



    class audio_out
    {

        IDirectSound8_obj DS8;

    public:
        audio_out()
        {
        }
    };

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // !bionukg_audiowavdx_h
