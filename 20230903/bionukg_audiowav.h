
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#ifndef PI
#define PI  acos(-1.0)
#endif
#ifndef _bionukg_audiowav_h
#define _bionukg_audiowav_h

class wav
{
#define wav_headerlength 38
private:
    uint8_t* header=0;
    uint32_t* filetag=0;//"RIFF"
    uint32_t* filedatasize=0;//+8==filesize
    uint32_t* format=0;//"WAVE"
    uint32_t* formatag=0;//"fmt "
    uint32_t* blocksize=0;//16/18/20/40//这里是18
    uint16_t* format_tag=0;//1
    uint16_t* soundtrack=0;//1/2声道
    uint32_t* sampling_rate=0;//通常11025000/22050000/44100000采样率
    uint32_t* bitrate=0;//=soundtrack*sampling_rate*bitdepth/8 数据速率
    uint16_t* dataunit=0;//=soundtrack*bitdepth/8 采样帧大小
    uint16_t* bitdepth=0;//4/8/12/16/24/32 位深度
    uint16_t* extrasize=0;//额外块
public:
    uint8_t* extra = 0;
    uint32_t datastart = 0;//"data"
    uint8_t* data = 0;
    uint32_t datasize = 0;
    uint8_t* file = 0;//用于播放
    uint32_t inline _4chars(const char str[5])
    {
        return *(uint32_t*)str;
    }

    wav(void)
    {
        header = new uint8_t[wav_headerlength];

        filetag = (uint32_t*)&header[0];
        *filetag = _4chars("RIFF");
        filedatasize = (uint32_t*)&header[0x04];
        format = (uint32_t*)&header[0x08];
        *format = _4chars("WAVE");
        formatag = (uint32_t*)&header[0x0c];
        *formatag = _4chars("fmt ");
        blocksize = (uint32_t*)&header[0x10];
        format_tag = (uint16_t*)&header[0x14];
        soundtrack = (uint16_t*)&header[0x16];
        sampling_rate = (uint32_t*)&header[0x18];
        bitrate = (uint32_t*)&header[0x1c];
        dataunit = (uint16_t*)&header[0x20];
        bitdepth = (uint16_t*)&header[0x22];
        extrasize = (uint16_t*)&header[0x24];
        datastart = _4chars("data");
        data = 0;
        datasize = 0;
    }
    ~wav(void)
    {
        delete[] header;
        if (data != 0)
        {
            delete[] data;
            data = 0;
        }
        if (extra != 0)
        {
            delete[] extra;
            extra = 0;
        }
        if (file != 0)
        {
            delete[] file;
            file = 0;
        }
            
    }
    wav(const char* path)
    {
        FILE* fp = fopen(path, "rb+");
        if (fp == 0)
        {
            std::cout << "unknown file" << path << "\n";
            system("pause");
            return;
        }
        header = new uint8_t[wav_headerlength];
        //std::cout << "header:" << 
        fread(header, 1, wav_headerlength - 2, fp);
        filetag = (uint32_t*)&header[0];
        //*filetag = _4chars("RIFF");
        filedatasize = (uint32_t*)&header[0x04];
        format = (uint32_t*)&header[0x08];
        //*format = _4chars("WAVE");
        formatag = (uint32_t*)&header[0x0c];
        //*formatag = _4chars("fmt ");
        blocksize = (uint32_t*)&header[0x10];
        extrasize = (uint16_t*)&header[0x24];
        switch (*blocksize)
        {
        case 16:
            *extrasize = 0;
            break;
        case 18:
            fread(extrasize, 1, 2, fp);

            extra = new uint8_t[*extrasize];
            //std::cout << "extra:" << 
            fread(extra, 1, *extrasize, fp);//<< "\n";

            break;
        default:
            std::cout << "unknown blocksize type "<<*blocksize;
            system("pause");
            delete[] header;

            return;
            break;
        }
        format_tag = (uint16_t*)&header[0x14];
        soundtrack = (uint16_t*)&header[0x16];
        sampling_rate = (uint32_t*)&header[0x18];
        bitrate = (uint32_t*)&header[0x1c];
        dataunit = (uint16_t*)&header[0x20];
        bitdepth = (uint16_t*)&header[0x22];


        fread(&datastart, 4, 1, fp);
        if (datastart - *(uint32_t*)&"data" != 0)
        {
            printf("data字段有误");
            system("pause");
        }
        datasize = *filedatasize - wav_headerlength + 4 - *extrasize;
        data = (uint8_t*)new int8_t[datasize];
        //std::cout << "data:" << 
        fread(data, 1, datasize, fp);// << "\n";
    }
    wav(wav& in)
    {
        header = new uint8_t[wav_headerlength];
        memcpy(header, in.header, wav_headerlength);

        filetag = (uint32_t*)&header[0];
        *filetag = _4chars("RIFF");
        filedatasize = (uint32_t*)&header[0x04];
        format = (uint32_t*)&header[0x08];
        *format = _4chars("WAVE");
        formatag = (uint32_t*)&header[0x0c];
        *formatag = _4chars("fmt ");
        blocksize = (uint32_t*)&header[0x10];
        format_tag = (uint16_t*)&header[0x14];
        soundtrack = (uint16_t*)&header[0x16];
        sampling_rate = (uint32_t*)&header[0x18];
        bitrate = (uint32_t*)&header[0x1c];
        dataunit = (uint16_t*)&header[0x20];
        bitdepth = (uint16_t*)&header[0x22];
        extrasize = (uint16_t*)&header[0x24];

        if (in.extra != 0)
        {
            extra = new uint8_t[*extrasize];
            memcpy(extra, in.extra, *extrasize);
        }
        datastart = in.datastart;
        datasize = in.datasize;
        data = new uint8_t[datasize];
        memcpy(data, in.data, datasize);

    }
    //sono,65536Hz,8bit
    void init_std_wav(void) 
    {
        init_wav(1, 0x10000, 8);
        return;
    }
    void init_wav(uint16_t _soundtrack, uint32_t _sampling_rate, uint16_t _bitdepth)
    {
        *blocksize = 18;
        *format_tag = 1;
        *soundtrack = _soundtrack;
        *sampling_rate = _sampling_rate;
        *bitdepth = _bitdepth;
        *dataunit = _soundtrack * _bitdepth / 8;
        *bitrate = *sampling_rate * *dataunit;
        *extrasize = 0;
        return;
    }
    private:
    int16_t inline double_int16(double x)
    {
        int32_t r = round(x);
        return r < INT16_MIN ? INT16_MIN : (r > INT16_MAX ? INT16_MAX : r);
    }
    public:

    wav* new_wav16_multi_n(void(*fn)(double*, uint32_t, double*),uint32_t n)
    {
        wav* ret = new wav();
        ret->init_wav(*this->soundtrack, *this->sampling_rate *n, *this->bitdepth);
        ret->resize(this->datasize * n);
        uint32_t retcnt = 0, incnt = 0;
        double* inbuf = new double[points_counts()];
        for (; incnt < this->points_counts(); incnt++)
        {
            inbuf[incnt] = this->getpoint16sono(incnt);
        }
        double* outbuf = new double[points_counts()*n];

        fn(inbuf, points_counts(), outbuf);

        for (; retcnt < this->points_counts() * n; retcnt++)
        {
            ret->putpoint16sono(double_int16(outbuf[retcnt]), retcnt);
        }
        delete[]inbuf;
        delete[]outbuf;
        return ret;
    }

    wav* new_wav16_up_sample(uint32_t rate)
    {
        wav* ret = new wav;
        ret->init_wav(*soundtrack, *sampling_rate * rate, *bitdepth);
        ret->resize(this->datasize * rate);
        for (uint32_t i = 0; i < this->points_counts(); i++)
        {
            ((int16_t*)(ret->data))[i* rate] = ((int16_t*)(this->data))[i];
        }
        return ret;
    }

    wav* new_wav16_down_sample(uint32_t rate)
    {
        wav* ret = new wav;
        ret->init_wav(*soundtrack, *sampling_rate / rate, *bitdepth);
        ret->resize(this->datasize / rate);
        for (uint32_t i = 0; i < ret->points_counts(); i++)
        {
            ((int16_t*)(ret->data))[i] = ((int16_t*)(this->data))[i * rate];
        }
        return ret;
    }

#ifdef _bionukg_signal_h_
    int16_t cast_double(double i)
    {
        int32_t r = (int32_t)round(i);
        return
        r < INT16_MIN ?
            INT16_MIN :
            (r > INT16_MAX ?
                INT16_MAX :
                int16_t(r));
    }
    wav* new_wav16_lowpass_filter_gained(double normalized_freq,double gain)
    {
        int fflen = 0x8000;
        FFT_sys fs(fflen);
        fs.set_low_pass(normalized_freq);
        fs.set_gain(gain);
        wav* ret = new wav();
        ret->init_wav(*soundtrack, *sampling_rate, *bitdepth);
        ret->resize(this->datasize);
        uint32_t inidx = 0, outidx = 0;
        for (inidx = 0; inidx < fflen; inidx++)
        {
            fs.io(this->getpoint16sono(inidx));
        }
        for (inidx = fflen; inidx < this->points_counts(); inidx++, outidx++)
        {
            ret->putpoint16sono(cast_double(fs.io(this->getpoint16sono(inidx))), outidx);
        }
        for (; outidx < this->points_counts(); outidx++)
        {
            ret->putpoint16sono(cast_double(fs.io(0)), outidx);
        }
        return ret;
    }
    wav* new_wav16_lowpass_filter(double normalized_freq)
    {
        int fflen = 0x8000;
        FFT_sys fs(fflen);
        fs.set_low_pass(normalized_freq);
        wav* ret = new wav();
        ret->init_wav(*soundtrack, *sampling_rate, *bitdepth);
        ret->resize(this->datasize);
        uint32_t inidx = 0, outidx = 0;
        for (inidx = 0; inidx < fflen; inidx++)
        {
            fs.io(this->getpoint16sono(inidx));
        }
        for (inidx = fflen; inidx < this->points_counts(); inidx++,outidx++)
        {
            ret->putpoint16sono(fs.io(this->getpoint16sono(inidx)), outidx);
        }
        for (; outidx < this->points_counts(); outidx++)
        {
            ret->putpoint16sono(fs.io(0), outidx);
        }
        return ret;
    } 

#endif
#ifdef __INTRIN_H_

    wav* new_wav16_random_noise_signal(void)
    {
        wav* ret = new wav();
        ret->init_wav(*soundtrack, *sampling_rate, *bitdepth);
        ret->resize(this->datasize);
        for (uint32_t i = 0, len = this->points_counts(); i < len; i++)
        {
            uint16_t t16;
            while (_rdrand16_step(&t16)==0) {};
            ret->putpoint16sono(t16, i);
        }
        return ret;
    }
#endif
    //void fill_wav32(int32_t(*f_t)(uint32_t), uint32_t sampleing_point_count)
    //{
    //    if (*bitdepth != 32)
    //    {
    //        printf("\n不是32位音频\n");
    //        system("pause");
    //    }
    //    data = (uint8_t*)new int32_t[sampleing_point_count];
    //    datasize = *bitdepth / 8;
    //    datasize *= sampleing_point_count;
    //    
    //    uint32_t i = 0;
    //    
    //    for (i = 0; i < sampleing_point_count; i++)
    //    {
    //        *(int32_t*)&data[i] = f_t(i);
    //    }
    //    return;
    //}
    //void fill_wav32_double(double(*f_t)(double), time_t t)
    //{
    //    if (*bitdepth != 32)
    //    {
    //        printf("\n不是32位音频\n");
    //        system("pause");
    //    }
    //    uint64_t sampleing_point_count = t * *sampling_rate;
    //    data = (uint8_t*)new int32_t[sampleing_point_count];
    //    datasize = *bitdepth / 8;
    //    datasize *= sampleing_point_count;
    //    double time = 0.0,value=0.0;
    //    uint32_t i = 0;
    //    for (i = 0; i < sampleing_point_count; i++)
    //    {
    //        time = ((double)i) / *sampling_rate;
    //        value = f_t(time)*INT32_MAX;
    //        *(int32_t*)&data[i]=(int32_t)value;
    //    }
    //    return;
    //}
    void resize(uint32_t newsize)
    {
        uint32_t oldsize = datasize;
        uint8_t* ndata = new uint8_t[newsize]{};
        if (data != 0)
        {
            memcpy(ndata, data, newsize < oldsize ? newsize : oldsize);
            delete[] data;
        }
        data = ndata;
        datasize = newsize;
    }
    void fill_wav8(int8_t(*f_t)(uint32_t), uint32_t sampleing_point_count)
    {
        if (*bitdepth != 8)
        {
            printf("\n不是8位音频\n");
            system("pause");
        }
        if (data != 0)
            delete[]data;
        data = (uint8_t*)new int8_t[sampleing_point_count];
        datasize = *bitdepth / 8;
        datasize *= sampleing_point_count;

        uint32_t i = 0;

        for (i = 0; i < sampleing_point_count; i++)
        {
            ((int8_t*)data)[i] = f_t(i);
        }
        return;
    }
    void fill_wav8t_double(double(*f_t)(double), time_t t)
    {
        if (*bitdepth != 8)
        {
            printf("\n不是8位音频\n");
            system("pause");
        }
        uint64_t sampleing_point_count = t * *sampling_rate;
        if (data != 0)
            delete[]data;
        data = (uint8_t*)new int8_t[sampleing_point_count];
        datasize = *bitdepth / 8;
        datasize *= sampleing_point_count;
        double time = 0.0, value = 0.0;

        uint32_t i = 0;

        for (i = 0; i < sampleing_point_count; i++)
        {
            time = ((double)i) / *sampling_rate;
            value = f_t(time) * INT8_MAX;
            *(int8_t*)&data[i] = (int8_t)value;
        }
        return;
    }
    void fill_wav16t_double(double(*f_t)(double), double t)
    {
        if (*bitdepth != 16)
        {
            printf("\n不是16位音频\n");
            system("pause");
        }
        uint64_t sampleing_point_count =(uint64_t) (t * *sampling_rate);
        if (data != 0)
            delete[]data;
        data = (uint8_t*)new int16_t[sampleing_point_count];
        datasize = *bitdepth / 8;
        datasize *= sampleing_point_count;
        double time = 0.0, value = 0.0;

        uint32_t i = 0;

        for (i = 0; i < sampleing_point_count; i++)
        {
            time = ((double)i) / *sampling_rate;
            value = f_t(time) * INT16_MAX;
            *(int16_t*)&data[2 * i] = (int16_t)value;
        }
        return;
    }
    void fill_wav16(int16_t(*f_t)(uint32_t), uint32_t sampleing_point_count)
    {
        if (*bitdepth != 16)
        {
            printf("\n不是16位音频\n");
            system("pause");
        }
        if (data != 0)
            delete[]data;
        data = (uint8_t*)new int16_t[sampleing_point_count];
        datasize = *bitdepth / 8;
        datasize *= sampleing_point_count;

        uint32_t i = 0;

        for (i = 0; i < sampleing_point_count; i++)
        {
            ((int16_t*)data)[i] = f_t(i);
        }
        return;

    }
    void fill_wav16(int16_t*f_t, uint32_t sampleing_point_count)
    {
        if (*bitdepth != 16)
        {
            printf("\n不是16位音频\n");
            system("pause");
        }
        if (data != 0)
            delete[]data;
        data = (uint8_t*)new int16_t[sampleing_point_count];
        datasize = *bitdepth / 8;
        datasize *= sampleing_point_count;

        uint32_t i = 0;

        for (i = 0; i < sampleing_point_count; i++)
        {
            ((int16_t*)data)[i] = f_t[i];
        }
        return;

    }
    //void fill_wavfloat_double(float(*f_t)(double), time_t t)
    //{
    //    if (*bitdepth != 32)
    //    {
    //        printf("\n不是32位浮点音频\n");
    //        system("pause");
    //    }
    //    uint64_t sampleing_point_count = t * *sampling_rate;
    //    data = (uint8_t*)new int8_t[sampleing_point_count];
    //    datasize = *bitdepth / 8;
    //    datasize *= sampleing_point_count;
    //    double time = 0.0, value = 0.0;
    //    uint32_t i = 0;
    //    for (i = 0; i < sampleing_point_count; i++)
    //    {
    //        time = ((double)i) / *sampling_rate;
    //        value = f_t(time) * INT8_MAX;
    //        *(int8_t*)&data[i] = (int8_t)value;
    //    }
    //    return;
    //}
    void save_as(const char* path)
    {
        int actual_header_length = wav_headerlength;
        FILE* fp = 0;
        fopen_s(&fp,path, "wb+");
        if (fp == 0)
        {
            printf("打开\"%s\"失败", path);
            system("pause");
            return;
        }
        switch (*blocksize)
        {
        case 16:
            actual_header_length -= 2;
            break;

        case 18:
            break;

        default:
            std::cout << "unknown blocksize type";
            system("pause");
            return;
            break;
        }
        *filedatasize = actual_header_length + datasize;//本来+8，但是多出来一个'data'
        fseek(fp, 0, 0);
        *extrasize = 0;

        fwrite(header, 1, actual_header_length, fp);
        fwrite("data", 1, 4, fp);
        fwrite(&datasize, sizeof(datasize), 1, fp);
        fwrite(data, 1, datasize, fp);
        fclose(fp);
        return;
    }
    uint32_t points_counts()
    {
        return datasize * 8 / *bitdepth;
    }
    int16_t getpoint16sono(uint32_t idx)
    {
        if (data != 0)
            return ((int16_t*)data)[idx];
        else       
            return 0;        
    }
    void putpoint16sono(int16_t input, uint32_t idx)
    {
        if (data != 0)
        ((int16_t*)data)[idx] = input;
    }

//激活这个功能，你需要
//#include<Windows.h>
//#include<Mmsystem.h>
//#pragma comment(lib,"winmm.lib")
#ifdef _PLAYSOUNDAPI_H_
#define _同步_播放音频 SND_SYNC
#define _异步_播放音频 SND_ASYNC
#define _内存_播放音频 SND_MEMORY
#define _循环_播放音频 SND_LOOP
#define _不停_播放音频 SND_NOSTOP
    //循环自动异步
    void play(bool 异步,bool 循环)
    {
        if (file != 0)
        {
            delete[] file;
        }
        file = new uint8_t[wav_headerlength - 18 + *blocksize + 4 + sizeof(datasize) + datasize];
        uint32_t i = 0, j = 0;
        for (i = 0,j=0; j < wav_headerlength - 18 + *blocksize; i++,j++)
        {
            file[i] = header[i];
        }
        for (j = 0; j < 4; i++, j++)
        {
            file[i] = ((uint8_t*)"data")[j];
        }
        for (j = 0; j < sizeof(datasize); i++, j++)
        {
            file[i] = ((uint8_t*) & datasize)[j];
        }
        for (j = 0; j < datasize; i++, j++)
        {
            file[i] = data[j];
        }
        DWORD arg = _内存_播放音频;
        
        if (循环 != 0)
        {
            arg |= _循环_播放音频 | _异步_播放音频;
        }
        else if (异步 != 0)
        {
            arg |= _异步_播放音频;
        }
        
        PlaySoundA((LPCSTR)file, NULL, arg);
    }
    void resource(void** res)
    {
        uint8_t** filep = (uint8_t**)res;
        *filep = (uint8_t*)malloc(wav_headerlength - 18 + *blocksize + 4 + sizeof(datasize) + datasize);
        uint8_t* file = *filep;
        uint32_t i = 0, j = 0;
        for (i = 0, j = 0; j < wav_headerlength - 18 + *blocksize; i++, j++)
        {
            file[i] = header[i];
        }
        for (j = 0; j < 4; i++, j++)
        {
            file[i] = ((uint8_t*)"data")[j];
        }
        for (j = 0; j < sizeof(datasize); i++, j++)
        {
            file[i] = ((uint8_t*)&datasize)[j];
        }
        for (j = 0; j < datasize; i++, j++)
        {
            file[i] = data[j];
        }

    }
    class wav_resource {
    void* res;
public:
    wav_resource(wav* w)
    {
        w->resource(&res);
    }
    ~wav_resource()
    {
        free(res);
    }
};
#endif

};


#endif
//参考文献(雾)https://www.cnblogs.com/wangguchangqing/p/5970516.html
// 
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
