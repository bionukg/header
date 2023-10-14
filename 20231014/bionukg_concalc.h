#pragma once
#ifndef bionukg_concalc_h
#define bionukg_concalc_h

#if (!defined(_WIN32_WINNT))
#define _WIN32_WINNT 0x600
#elif (_WIN32_WINNT >= 0x600)

#else
#error "_WIN32_WINNT is not 0x600"
#endif
// no min max macro
#define NOMINMAX
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// for IDXGraphicsAnalysis in debug mode
#ifdef _DEBUG
#include <DXGItype.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <dxgi1_3.h>
#if (NTDDI_VERSION < NTDDI_WINBLUE)
// warning for NTDDI_VERSION
#pragma message("warning: NTDDI_VERSION is not NTDDI_WINBLUE or later, IDXGraphicsAnalysis is not available")
#pragma push_macro("NTDDI_VERSION")
#undef NTDDI_VERSION
#define NTDDI_VERSION NTDDI_WINBLUE
#define NTDDI_VERSION_PUSHED
#endif
#include <DXProgrammableCapture.h>
#if defined(NTDDI_VERSION_PUSHED)
#undef NTDDI_VERSION_PUSHED
#pragma pop_macro("NTDDI_VERSION")
#endif

#include <dxgidebug.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxva2.lib")
#endif

#ifdef namespace_bionukg
namespace bionukg
{
#endif

#ifdef _DEBUG
    static class dxgi_debugger
    {
        IDXGraphicsAnalysis *dbg = 0;

    public:
        dxgi_debugger()
        {
            auto res = DXGIGetDebugInterface1(0, __uuidof(dbg), (void **)&dbg);
            if (FAILED(res))
            {
                // some error message
                printf("failed at \"dxgi_debugger\": DXGIGetDebugInterface1 returned %08X\n", res);
                printf("to enable dxgi debug, in Visual Studio, go to \"Debug\"->\"Graphics\"->\"Start Graphics Debugging\"\n");
                printf("if there is no need to debug GPU calls, this warning can be ignored\n");
                return;
            }
            dbg->BeginCapture();
        }
        ~dxgi_debugger()
        {
            if (dbg != 0)
                dbg->EndCapture();
        }
    } dxgi_debugger_obj;

#endif

    class hlsl_code
    {
        const char *code = nullptr;
        size_t code_size = 0;

    public:
        hlsl_code() = default;
        typedef struct
        {
            uint32_t width;
            uint32_t height;
            uint32_t kernel_width;
            uint32_t kernel_height;
            uint32_t kernel_center_x;
            uint32_t kernel_center_y;
            uint32_t kernel_stride_x;
            uint32_t kernel_stride_y;
            uint32_t kernel_padding_x;
            uint32_t kernel_padding_y;
            uint32_t kernel_dilation_x;
            uint32_t kernel_dilation_y;
        } conv_param;

    private:
        // 卷积代码,参数未使用，仅用于提示hlsl代码参数形式
        const char c4x4fb[1709] = {"\
                ByteAddressBuffer conv_core : register(t0);\n\
                ByteAddressBuffer conv_data : register(t1);\n\
                ByteAddressBuffer conv_param : register(t2);\n\
                RWByteAddressBuffer conv_result : register(u0);\n\
                \n\
                [numthreads(1, 1, 1)]\n\
                void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
                {\n\
                    uint y = DTid.y;\n\
                    uint x = DTid.x;\n\
                    uint width = conv_param.Load(0);\n\
                    float sum = 0;\n\
                    #define load_float4(place,idx) float4(asfloat(place.Load(idx)),asfloat(place.Load(idx+4)),asfloat(place.Load(idx+8)),asfloat(place.Load(idx+12)))\n\
                    #define index_4BYTE(x,y) ((x+y*width)*4)\n\
                    sum += dot(load_float4(conv_core, 0), load_float4(conv_data, index_4BYTE(x,y)));\n\
                    sum += dot(load_float4(conv_core, 16), load_float4(conv_data, index_4BYTE(x,y+1)));\n\
                    sum += dot(load_float4(conv_core, 32), load_float4(conv_data, index_4BYTE(x,y+2)));\n\
                    sum += dot(load_float4(conv_core, 48), load_float4(conv_data, index_4BYTE(x,y+3)));\n\
                    /*result*/\n\
                    conv_result.Store(index_4BYTE(x,y), asuint(sum));\n\
                }\n"};
        const char c3x3fb[1601] = {"\
                /*3x3 convolution*/\n\
                ByteAddressBuffer conv_core : register(t0);\n\
                ByteAddressBuffer conv_data : register(t1);\n\
                ByteAddressBuffer conv_param : register(t2);\n\
                RWByteAddressBuffer conv_result : register(u0);\n\
                \n\
                [numthreads(1, 1, 1)]\n\
                void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
                {\n\
                    uint y = DTid.y;\n\
                    uint x = DTid.x;\n\
                    uint width = conv_param.Load(0);\n\
                    float sum = 0;\n\
                    #define load_float3(place,idx) float3(asfloat(place.Load(idx)),asfloat(place.Load(idx+4)),asfloat(place.Load(idx+8)))\n\
                    #define index_4BYTE(x,y) ((x+y*width)*4)\n\
                    sum += dot(load_float3(conv_core, 0), load_float3(conv_data, index_4BYTE(x,y)));\n\
                    sum += dot(load_float3(conv_core, 12), load_float3(conv_data, index_4BYTE(x,y+1)));\n\
                    sum += dot(load_float3(conv_core, 24), load_float3(conv_data, index_4BYTE(x,y+2)));\n\
                    /*result*/\n\
                    conv_result.Store(index_4BYTE(x,y), asuint(sum));\n\
                }\n"};

    public:
        const char cellauto[2227] = {"\
                ByteAddressBuffer data : register(t0);\n\
                /*the input cell is stored in uint*/\n\
                /*the result is stored like the input data*/\n\
                ByteAddressBuffer param : register(t1);\n\
                RWByteAddressBuffer result : register(u0);\n\
                \n\
                [numthreads(1, 1, 1)]\n\
                void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
                {\n\
                    uint y = DTid.y;\n\
                    uint x = DTid.x;\n\
                    uint width = asuint(param.Load(0));\n\
                    uint3 cell[3];\n\
                    cell[0]= uint3( 1&asuint(data.Load(((x-1)+(y-1)*width)*4)),\n\
                                    1&asuint(data.Load(((x)+(y-1)*width)*4)),\n\
                                    1&asuint(data.Load(((x+1)+(y-1)*width)*4)));\n\
                    cell[1]= uint3( 1&asuint(data.Load(((x-1)+(y)*width)*4)),\n\
                                    1&asuint(data.Load(((x)+(y)*width)*4)),\n\
                                    1&asuint(data.Load(((x+1)+(y)*width)*4)));\n\
                    cell[2]= uint3( 1&asuint(data.Load(((x-1)+(y+1)*width)*4)),\n\
                                    1&asuint(data.Load(((x)+(y+1)*width)*4)),\n\
                                    1&asuint(data.Load(((x+1)+(y+1)*width)*4)));\n\
                    uint cell_center = cell[1].y;\n\
                    uint cell_sum = cell[0].x + cell[0].y + cell[0].z + cell[1].x + cell[1].z + cell[2].x + cell[2].y + cell[2].z;\n\
                    /*the rule is, less than 2 or more than 3, clear; 3, set; 2, keep*/\n\
                    /*when the 2rd bit is 1,and the 1st bit is 1, set*/\n\
                    cell_center |= (cell_sum == 3) ? 1 : 0;\n\
                    /*when the 3rd bit is 1,or the 2nd bit is 0, clear*/\n\
                    cell_center &= (((cell_sum & 0x4) != 0) || ((cell_sum & 0x2) == 0)) ? 0 : 1;\n\
                    /*otherwise, keep*/\n\
                    cell_center |= cell_center << 7;\n\
                    cell_center |= cell_center << 8;\n\
                    cell_center |= cell_center << 8; \n\
                    //uint rest=0x00ffffff;\n\
                    result.Store((x+y*width)*4, asuint(cell_center));\n\
                    return;\n\
                }\n"};
        // 输入输出都用UAV的版本
        const char cellauto_u[1972] = {"\
            RWByteAddressBuffer data : register(u0);\n\
            RWByteAddressBuffer result : register(u1);\n\
            ByteAddressBuffer param : register(t0);\n\
            \n\
            [numthreads(1, 1, 1)]\n\
            void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
            {\n\
                uint y = DTid.y;\n\
                uint x = DTid.x;\n\
                uint width = asuint(param.Load(0));\n\
                uint3 cell[3];\n\
                cell[0]= uint3( 1&asuint(data.Load(((x-1)+(y-1)*width)*4)),\n\
                                1&asuint(data.Load(((x)+(y-1)*width)*4)),\n\
                                1&asuint(data.Load(((x+1)+(y-1)*width)*4)));\n\
                cell[1]= uint3( 1&asuint(data.Load(((x-1)+(y)*width)*4)),\n\
                                1&asuint(data.Load(((x)+(y)*width)*4)),\n\
                                1&asuint(data.Load(((x+1)+(y)*width)*4)));\n\
                cell[2]= uint3( 1&asuint(data.Load(((x-1)+(y+1)*width)*4)),\n\
                                1&asuint(data.Load(((x)+(y+1)*width)*4)),\n\
                                1&asuint(data.Load(((x+1)+(y+1)*width)*4)));\n\
                uint cell_center = cell[1].y;\n\
                uint cell_sum = cell[0].x + cell[0].y + cell[0].z + cell[1].x + cell[1].z + cell[2].x + cell[2].y + cell[2].z;\n\
                /*the rule is, less than 2 or more than 3, clear; 3, set; 2, keep*/\n\
                /*when the 2rd bit is 1,and the 1st bit is 1, set*/\n\
                cell_center |= (cell_sum == 3) ? 1 : 0;\n\
                /*when the 3rd bit is 1,or the 2nd bit is 0, clear*/\n\
                cell_center &= (((cell_sum & 0x4) != 0) || ((cell_sum & 0x2) == 0)) ? 0 : 1;\n\
                /*otherwise, keep*/\n\
                cell_center |= cell_center << 7;\
                cell_center |= cell_center << 8;\
                cell_center |= cell_center << 8; \n\
                //uint rest=0x00ffffff;\n\
                result.Store((x+y*width)*4, asuint(cell_center));\n\
                return;\n\
            }"};
        // 用结构体的版本
        const char cellauto_s[1517] = {"\
            RWStructuredBuffer<uint> data : register(u0);\n\
            RWStructuredBuffer<uint> result : register(u1);\n\
            ByteAddressBuffer param : register(t0);\n\
            \n\
            [numthreads(1, 1, 1)]\n\
            void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
            {\n\
                uint y = DTid.y;\n\
                uint x = DTid.x;\n\
                uint width = asuint(param.Load(0));\n\
                uint3 cell[3];\n\
                cell[0]= uint3( 1&data[(x-1)+(y-1)*width],\n\
                                1&data[(x)+(y-1)*width],\n\
                                1&data[(x+1)+(y-1)*width]);\n\
                cell[1]= uint3( 1&data[(x-1)+(y)*width],\n\
                                1&data[(x)+(y)*width],\n\
                                1&data[(x+1)+(y)*width]);\n\
                cell[2]= uint3( 1&data[(x-1)+(y+1)*width],\n\
                                1&data[(x)+(y+1)*width],\n\
                                1&data[(x+1)+(y+1)*width]);\n\
                uint cell_center = cell[1].y;\n\
                uint cell_sum = cell[0].x + cell[0].y + cell[0].z + cell[1].x + cell[1].z + cell[2].x + cell[2].y + cell[2].z;\n\
                cell_center |= (cell_sum == 3) ? 1 : 0;\n\
                cell_center &= (((cell_sum & 0x4) != 0) || ((cell_sum & 0x2) == 0)) ? 0 : 1;\n\
                cell_center |= cell_center << 7;\n\
                cell_center |= cell_center << 8;\n\
                cell_center |= cell_center << 8; \n\
                result[x+y*width] = cell_center;\n\
                return;\n\
            }"};
        // example render?
        const char example[257] = "\
            RWStructuredBuffer<uint> result : register(u0);\n\
            \n\
            [numthreads(1, 1, 1)]\n\
            void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
            {\n\
                result[DTid.x]=DTid.x;\n\
                return;\n\
            };";
        // k-means
        const char kmeans[1500] = {"\
struct Point//\n\
{ //\n\
    float x, y;//\n\
};//\n\
ByteAddressBuffer points : register(t0);//\n\
RWByteAddressBuffer group : register(u0);//\n\
RWByteAddressBuffer centroids : register(u1);//\n\
#define centCount %d //\n\
#define pointXY(index) float2(asfloat(points.Load(index*8)), asfloat(points.Load(index*8+4)))//\n\
#define centXY(index) float2(asfloat(centroids.Load(index*8)), asfloat(centroids.Load(index*8+4)))//\n\
[numthreads(1, 1, 1)]//\n\
void CSMain(uint3 DTid : SV_DispatchThreadID)//\n\
{//\n\
    #define maxfloat (1000000)//\n\
    //asfloat is like reinterpret_cast<float> in C++//\n\
    // Find the closest centroid.//\n\
    float minDist = maxfloat;//\n\
    uint minIndex = 0;//\n\
    [unroll]//\n\
    for (uint i = 0; i < centCount; i++)//\n\
    {//\n\
        float dist = distance(pointXY(DTid.x), centXY(i));//\n\
        //[branch]//\n\
        [flatten]//\n\
        if (dist < minDist)//\n\
        {//\n\
            minDist = dist;//\n\
            minIndex = i;//\n\
        }//\n\
    }//\n\
    if(minIndex<3)//\n\
    minIndex=0xff0000>>(minIndex*8);//\n\
    else if(minIndex<6)//\n\
    {//\n\
    minIndex=0xff0000>>((minIndex-3)*8);//\n\
    minIndex=~minIndex;//\n\
    }//\n\
    group.Store(DTid.x*4,minIndex);//\n\
}//\n"};

        const char kmeanstest[1500] = {"\
ByteAddressBuffer points : register(t0);//\n\
RWByteAddressBuffer group : register(u0);//\n\
RWByteAddressBuffer centroids : register(u1);//\n\
#define centCount %d //\n\
[numthreads(1, 1, 1)]//\n\
void CSMain(uint3 DTid : SV_DispatchThreadID)//\n\
{//\n\
//just store x and y into group, only 8bit is used//\n\
    float x=asfloat(points.Load(DTid.x*8));//\n\
    float y=asfloat(points.Load(DTid.x*8+4));//\n\
    uint x_i=x;//\n\
    uint y_i=y;//\n\
    group.Store(DTid.x*4,x_i|(y_i<<8));//\n\
}//\n"};

        char codebuf[4096];

        uint32_t code_convolution_4x4_basic(float *conv_core = 0, float *conv_data = 0, conv_param *conv_param = 0)
        {
            code = c4x4fb;
            code_size = sizeof(c4x4fb);
            return sizeof(c4x4fb);
        }
        uint32_t code_cellauto(uint32_t *data = 0, uint32_t *param = 0)
        {
            code = cellauto;
            code_size = sizeof(cellauto);
            return sizeof(cellauto);
        }
        uint32_t code_cellauto_u(uint32_t *data = 0, uint32_t *result = 0, uint32_t *param = 0)
        {
            code = cellauto_u;
            code_size = sizeof(cellauto_u);
            return sizeof(cellauto_u);
        }
        uint32_t code_cellauto_struct(uint32_t *data = 0, uint32_t *result = 0, uint32_t *param = 0)
        {
            code = cellauto_s;
            code_size = sizeof(cellauto_s);
            return sizeof(cellauto_s);
        }
        uint32_t code_example(uint32_t *data = 0, uint32_t *result = 0, uint32_t *param = 0)
        {
            code = example;
            code_size = sizeof(example);
            return sizeof(example);
        }

        uint32_t code_fft()
        {
            /*
                for (uint32_t i = 0; i < n;)
                {
                    for (uint32_t j = 0; j < N; j += delta)
                    {
                        for (uint32_t k = 0; k < delta; k++)
                        {
                            up = io[j + delta] * omegaN[N / 2 / delta * k];
                            io[j + delta] = io[j] - up;
                            io[j] += up;
                            j++;
                        }
                    }
                    delta <<= 1;
                    i++;
                }*/
            const char fft[] =
                {"\
struct complex\n\
{\n\
    float re;\n\
    float im;\n\
};\n\
#define addeq(comp0,comp1) comp0.re+=comp1.re;comp0.im+=comp1.im;\n\
#define subeq(comp0,comp1) comp0.re-=comp1.re;comp0.im-=comp1.im;\n\
#define muleq_r(comp,real) comp.re*=real;comp.im*=real;\n\
RWStructuredBuffer<complex> io : register(u0);\n\
StructuredBuffer<complex> omegaN : register(t0);\n\
StructuredBuffer<uint> args : register(t1);\n\
\n\
[numthreads(1, 1, 1)]\n\
void CSMain(uint3 DTid : SV_DispatchThreadID)\n\
{\n\
uint N=args[0];\n\
uint delta=args[1];\n\
uint mask=args[2];\n\
uint x=DTid.x;\n\
uint x_h=(x&(~mask))<<1;\n\
uint x_l=x&mask;\n\
uint j=x_h|x_l;\n\
//up = io[j+delta] * omegaN[N/2/delta*x_l];\n\
complex up;\n\
up.re=io[j+delta].re*omegaN[N/2/delta*x_l].re-io[j+delta].im*omegaN[N/2/delta*x_l].im;\n\
up.im=io[j+delta].re*omegaN[N/2/delta*x_l].im+io[j+delta].im*omegaN[N/2/delta*x_l].re;\n\
//io[j+delta] = io[j]-up;\n\
io[j+delta].re=io[j].re-up.re;\n\
io[j+delta].im=io[j].im-up.im;\n\
//io[j] += up;\n\
io[j].re+=up.re;\n\
io[j].im+=up.im;\n\
return;\n\
};"};
            memcpy(codebuf, fft, sizeof(fft));
            code = codebuf;
            code_size = sizeof(fft);
            return sizeof(fft);
        }
        uint32_t code_kmeans(_In_ uint32_t centroids_count = 3, uint32_t *points = 0, uint32_t *centroids = 0)
        {
            // auto s = sprintf(codebuf, kmeans, centroids_count);
            auto s = sprintf_s(codebuf, kmeans, centroids_count);
            code = codebuf;
            code_size = s;
            return sizeof(codebuf);
        }
        uint32_t code_kmeanstest(_In_ uint32_t centroids_count = 3, uint32_t *points = 0, uint32_t *centroids = 0)
        {
            // auto s = sprintf(codebuf, kmeanstest, centroids_count);
            auto s = sprintf_s(codebuf, kmeanstest, centroids_count);
            code = codebuf;
            code_size = s;
            return sizeof(codebuf);
        }

        const char *get_code() const
        {
            return code;
        }
        auto get_code_size() const
        {
            return code_size;
        }
    };

    class compute_GPU
    {
    private:
        ID3D11Device *device = nullptr;
        ID3DBlob *csBlob = nullptr;
        ID3D11ComputeShader *computeShader = nullptr;
#define MAX_SRV_buffer 128
        ID3D11Buffer *SRV_buffer[MAX_SRV_buffer] = {nullptr};
        ID3D11ShaderResourceView *SRV[MAX_SRV_buffer] = {nullptr};
#define MAX_UAV_buffer 8
        ID3D11Buffer *UAV_buffer[MAX_UAV_buffer] = {nullptr};
        ID3D11Buffer *buffer_result[MAX_UAV_buffer] = {nullptr};
        ID3D11UnorderedAccessView *UAV[MAX_UAV_buffer] = {nullptr};

        uint8_t UAV_buffer_used = 0;
        uint8_t SRV_buffer_used = 0;

        const D3D_FEATURE_LEVEL lvl[4] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
                                          D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};
        UINT createDeviceFlags = 0
#ifdef _DEBUG
                                 | D3D11_CREATE_DEVICE_DEBUG
#endif
            ;
        // 1.创建设备
        HRESULT init()
        {
            HRESULT hr = D3D11CreateDevice(nullptr,
                                           D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, lvl,
                                           _countof(lvl), D3D11_SDK_VERSION, &device, nullptr, nullptr);
            if (hr == E_INVALIDARG)
            {
                // DirectX 11.0 Runtime doesn't recognize D3D_FEATURE_LEVEL_11_1 as a valid value
                hr = D3D11CreateDevice(nullptr,
                                       D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &lvl[1],
                                       _countof(lvl) - 1, D3D11_SDK_VERSION, &device, nullptr, nullptr);
            }
            if (FAILED(hr))
            {
                printf("Failed creating Direct3D 11 device %08X\n", hr);
            }
            return hr;
        }
        // 8.释放资源
        HRESULT release()
        {
            for (int i = 0; i < SRV_buffer_used; i++)
            {
                SRV_buffer[i]->Release();
                SRV[i]->Release();
            }
            for (int i = 0; i < UAV_buffer_used; i++)
            {
                UAV_buffer[i]->Release();
                UAV[i]->Release();
                buffer_result[i]->Release();
            }
            computeShader->Release();
            return S_OK;
        }
        // if strlength==0 ,means input is a file name, otherwise is the length for ascii hlsl code
        HRESULT CompileComputeShaderFromFileOrString(_In_ LPCWSTR srcFileStr, _In_ LPCSTR entryPoint,
                                                     _Outptr_ ID3DBlob **blob, _In_ size_t length = 0,
                                                     _In_ const D3D_SHADER_MACRO *defines = nullptr)
        {
            if (!srcFileStr || !entryPoint || !device || !blob)
                return E_INVALIDARG;

            *blob = nullptr;

            UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
            flags |= D3DCOMPILE_DEBUG;
#else
        flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

            // We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
            LPCSTR profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

            const D3D_SHADER_MACRO defines_eg[] =
                {
                    "EXAMPLE_DEFINE", "1",
                    NULL, NULL};
            if (defines == nullptr) // if no defines are passed in, use the example ones (they do nothing, but show how to pass in defines)
                defines = defines_eg;
            ID3DBlob *shaderBlob = nullptr;
            ID3DBlob *errorBlob = nullptr;
            HRESULT hr;
            if (length == 0)
                hr = D3DCompileFromFile(srcFileStr, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                        entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);
            else
                hr = D3DCompile(srcFileStr, length, 0, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);

            if (FAILED(hr))
            {
                if (errorBlob)
                {
#ifdef _DEBUG
                    printf("%s\n", (char *)errorBlob->GetBufferPointer());
                    system("pause");
                    // OutputDebugStringA((char *)errorBlob->GetBufferPointer());
#endif
                    errorBlob->Release();
                }

                if (shaderBlob)
                    shaderBlob->Release();

                return hr;
            }

            *blob = shaderBlob;

            return hr;
        }

    public:
        // 2.设置着色器
        HRESULT set_code_segment(LPCWSTR srcFile = nullptr, LPCSTR entryPoint = nullptr,
                                 _In_ size_t strlength = 0, const D3D_SHADER_MACRO *defines = nullptr)
        {
            HRESULT hr = S_OK;
            // Verify compute shader is supported
            if (device->GetFeatureLevel() < D3D_FEATURE_LEVEL_11_0)
            {
                D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hwopts = {0};
                hr = device->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hwopts, sizeof(hwopts));
                if (!hwopts.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
                {
                    device->Release();
#ifdef _DEBUG
                    printf("DirectCompute is not supported by this device\n");
#endif
                    return hr;
                }
            }
            LPCWSTR srcFile_eg = L"comp.hlsl";
            LPCSTR entryPoint_eg = "CSMain";
            if (srcFile == nullptr)
                srcFile = srcFile_eg;
            if (entryPoint == nullptr)
                entryPoint = entryPoint_eg;
            hr = CompileComputeShaderFromFileOrString(srcFile, entryPoint,
                                                      &csBlob, strlength, defines);
            if (FAILED(hr))
            {
                device->Release();
                printf("Failed compiling shader %08X\nmaybe check filename or entry point", hr);
                system("pause");

                return hr;
            }
            // Create shader
            hr = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &computeShader);
            if (FAILED(hr))
            {
                csBlob->Release();
                device->Release();
                printf("Failed creating shader %08X\n", hr);
                system("pause");

                return hr;
            }
            return hr;
        }
        HRESULT set_code_segment(const hlsl_code &hlsl)
        {
            return set_code_segment((LPCWSTR)(hlsl.get_code()), "CSMain", hlsl.get_code_size());
        }
        // 3.创建视图
        HRESULT set_input_data_segment_u32(uint32_t **data_SRV_t, const int *data_SRV_t_size, const size_t ptrs_count)
        {
            HRESULT hr = S_OK;
            // create views, 128 is the max number of SRVs
            for (int i = 0; data_SRV_t[i] != nullptr &&
                            SRV_buffer_used < MAX_SRV_buffer && i < ptrs_count;
                 i++, SRV_buffer_used++)
            {
                int bufSize = data_SRV_t_size[i];
                CD3D11_BUFFER_DESC desc0(
                    bufSize * sizeof(uint32_t),
                    D3D11_BIND_SHADER_RESOURCE);
                uint32_t *bufin0 = data_SRV_t[i];
                D3D11_SUBRESOURCE_DATA InitData0 = {bufin0, 0, 0};
                hr = device->CreateBuffer(&desc0, &InitData0, &SRV_buffer[SRV_buffer_used]);
                if (FAILED(hr))
                {
                    printf("Failed creating buffer %08X at the %dth\n", hr, i);
                    return hr;
                }

                CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(
                    D3D11_SRV_DIMENSION_BUFFEREX,
                    DXGI_FORMAT_R32_UINT,
                    0,
                    bufSize,
                    D3D11_BUFFEREX_SRV_FLAG_RAW);
                hr = device->CreateShaderResourceView(SRV_buffer[SRV_buffer_used], &srvDesc, &SRV[SRV_buffer_used]);
                if (FAILED(hr))
                {
                    printf("Failed creating SRV view %08X at the %dth\n", hr, i);
                    return hr;
                }
            }
            return hr;
        }
        HRESULT set_output_data_segment_u32(uint32_t **data_UAV_u, const int *data_UAV_u_size, const size_t ptrs_count)
        {
            // warning: due to unknown reason, data_UAV_u_size*4 makes the actually expected output

            HRESULT hr = S_OK;
            // create views, 8 is the max number of UAVs
            for (int i = 0; /*data_UAV_u[i] != nullptr &&*/
                 UAV_buffer_used < MAX_UAV_buffer && i < ptrs_count;
                 i++, UAV_buffer_used++)
            {

                /*warning*/
                uint32_t bufSize = data_UAV_u_size[i];
                /*warning*/

                uint32_t *bufout = data_UAV_u[i];
                CD3D11_BUFFER_DESC desc_out(
                    bufSize * sizeof(uint32_t),
                    D3D11_BIND_UNORDERED_ACCESS,
                    D3D11_USAGE_DEFAULT,
                    0,
                    D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
                    sizeof(uint32_t));

                D3D11_SUBRESOURCE_DATA InitData_out = {bufout, 0, 0};
                if (bufout != NULL)
                    hr = device->CreateBuffer(&desc_out, &InitData_out, &UAV_buffer[UAV_buffer_used]);
                else
                    hr = device->CreateBuffer(&desc_out, 0, &UAV_buffer[UAV_buffer_used]);

                if (FAILED(hr))
                {
                    printf("Failed creating UAV buffer %08X at the %dth\n", hr, i);
                    return hr;
                }

                CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(
                    D3D11_UAV_DIMENSION_BUFFER,
                    DXGI_FORMAT_UNKNOWN,
                    0,
                    bufSize,
                    0 /*D3D11_BUFFER_UAV_FLAG_RAW*/);
                hr = device->CreateUnorderedAccessView(UAV_buffer[UAV_buffer_used], &uavDesc, &UAV[UAV_buffer_used]);
                if (FAILED(hr))
                {
                    printf("Failed creating UAV view %08X at %d\n", hr, UAV_buffer_used);
                    return hr;
                }
                // creaat the result buffer, to store the result from UAV

                CD3D11_BUFFER_DESC desc_result(
                    bufSize * sizeof(uint32_t),
                    0,
                    D3D11_USAGE_STAGING,
                    D3D11_CPU_ACCESS_READ,
                    D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
                    sizeof(uint32_t));
                hr = device->CreateBuffer(&desc_result, nullptr, &buffer_result[UAV_buffer_used]);
                if (FAILED(hr))
                {
                    printf("Failed creating UAV buffer result %08X at %d\n", hr, UAV_buffer_used);
                    return hr;
                }
            }
            return hr;
        }
        HRESULT set_input_data_segment_u32(const void *data_SRV_t, const int data_SRV_t_size)
        {
            return set_input_data_segment_u32((uint32_t **)&(data_SRV_t), &data_SRV_t_size, 1);
        }
        HRESULT set_output_data_segment_u32(void *data_UAV_u, const int data_UAV_u_size)
        {
            return set_output_data_segment_u32((uint32_t **)&data_UAV_u, &data_UAV_u_size, 1);
        }
        // 4.准备好数据后，加载数据
        HRESULT load_data()
        {
            ID3D11DeviceContext *ImmediateContext = nullptr;
            device->GetImmediateContext(&ImmediateContext);
            ImmediateContext->CSSetShader(computeShader, nullptr, 0);
            ImmediateContext->CSSetShaderResources(0, SRV_buffer_used, SRV);
            ImmediateContext->CSSetUnorderedAccessViews(0, UAV_buffer_used, UAV, nullptr);
            return S_OK;
        }
        // 根据指定的顺序而不是默认顺序加载数据
        HRESULT load_data_reordered(const uint8_t *SRV_order, const uint8_t *UAV_order)
        {
            ID3D11DeviceContext *ImmediateContext = nullptr;
            device->GetImmediateContext(&ImmediateContext);
            ImmediateContext->CSSetShader(computeShader, nullptr, 0);
            ID3D11ShaderResourceView *SRV_reordered[MAX_SRV_buffer];
            ID3D11UnorderedAccessView *UAV_reordered[MAX_UAV_buffer];
            for (int i = 0; i < SRV_buffer_used; i++)
            {
                SRV_reordered[i] = SRV[SRV_order[i]];
            }
            for (int i = 0; i < UAV_buffer_used; i++)
            {
                UAV_reordered[i] = UAV[UAV_order[i]];
            }
            ImmediateContext->CSSetShaderResources(0, SRV_buffer_used, SRV_reordered);
            ImmediateContext->CSSetUnorderedAccessViews(0, UAV_buffer_used, UAV_reordered, nullptr);
            return S_OK;
        }
        uint8_t get_SRV_used()
        {
            return SRV_buffer_used;
        }
        uint8_t get_UAV_used()
        {
            return UAV_buffer_used;
        }
        // 5.执行
        HRESULT run(UINT x_d, UINT y_d = 1, UINT z_d = 1)
        {
            ID3D11DeviceContext *ImmediateContext = nullptr;
            device->GetImmediateContext(&ImmediateContext);

            ImmediateContext->Dispatch(x_d, y_d, z_d);
            // dispatch(x_d,y_d,z_d) sets the arranges of thread groups, they run in undefined order
            // which means 0<=SV_GroupID.x< x_d, 0<=SV_GroupID.y< y_d, 0<=SV_GroupID.z< z_d
            //  D3D11_CS_DISPATCH_MAX_THREAD_GROUPS_PER_DIMENSION = 65535
            //   [numthreads(x_n, y_n, z_n)] sets by shader,
            //  0<=SV_GroupThreadID.x< x_n, 0<=SV_GroupThreadID.y< y_n, 0<=SV_GroupThreadID.z< z_n
            //  SV_dispatchThreadID.x = SV_GroupID.x * x_n + SV_GroupThreadID.x
            //  so as SV_dispatchThreadID.y and SV_dispatchThreadID.z
            //  SV_GroupIndex = SV_GroupID.z * y_d * x_d + SV_GroupID.y * x_d + SV_GroupID.x
            return S_OK;
        }
        // 6.解除数据绑定
        HRESULT unload_data()
        {
            ID3D11DeviceContext *ImmediateContext = nullptr;
            device->GetImmediateContext(&ImmediateContext);
            ID3D11ShaderResourceView *nullSRV[MAX_SRV_buffer] = {nullptr};
            ImmediateContext->CSSetShaderResources(0, MAX_SRV_buffer, nullSRV);
            ID3D11UnorderedAccessView *nullUAV[MAX_UAV_buffer] = {nullptr};
            ImmediateContext->CSSetUnorderedAccessViews(0, MAX_UAV_buffer, nullUAV, nullptr);

            ImmediateContext->CSSetShader(nullptr, nullptr, 0);

            ImmediateContext->Flush();
            return S_OK;
        }
        // 7.获取算好的数据
        HRESULT get_data(
            uint32_t **data_out,           // the pointers to store the result
            const uint32_t *data_out_size, // the size of data_out pointers,in DWORDs
            const uint8_t *data_out_idx,   // the index of UAV_buffer, vaule should range from 0 to UAV_buffer_used-1
            const size_t counts)
        { // ref:https://stackoverflow.com/questions/32049639/directx-11-compute-shader-writing-to-an-output-resource
            ID3D11DeviceContext *ImmediateContext = nullptr;
            device->GetImmediateContext(&ImmediateContext);
            HRESULT hr = S_OK;
            for (int i = 0; i < counts; i++)
            {
                ImmediateContext->CopyResource(buffer_result[data_out_idx[i]], UAV_buffer[data_out_idx[i]]);
                D3D11_MAPPED_SUBRESOURCE MappedResource;
                hr = ImmediateContext->Map(buffer_result[data_out_idx[i]],
                                           0, D3D11_MAP_READ, 0, &MappedResource);
                memcpy(data_out[i], MappedResource.pData, data_out_size[i] * sizeof(uint32_t));
                ImmediateContext->Unmap(buffer_result[data_out_idx[i]], 0);
            }
            return S_OK;
        }
        HRESULT get_data(void *data_out, const uint32_t data_out_size, const uint8_t data_out_idx = 0)
        {
            return get_data((uint32_t **)&data_out, &data_out_size, &data_out_idx, 1);
        }
        compute_GPU()
        {
            init();
        }
        ~compute_GPU()
        {
            release();
        }
    };

    class GPU_DX11_cs
    {
    public:
#define HLSL_CORE(...) #__VA_ARGS__
        static inline class device_
        {
        public:
            ID3D11Device *operator->()
#ifdef bionukg_nothrow
                noexcept
#endif
            {
                if(initated==false)
				{
					init();
					initated=true;
				}
#ifdef _DEBUG
                if (device == nullptr)
                {
                    printf("device is nullptr\n");
#ifndef bionukg_nothrow
                    throw;
#endif
                }
#endif
                return device;
            }

        private:
            friend class GPU_DX11_cs;
            ID3D11Device *device = nullptr;
            ID3D11DeviceContext *context = nullptr;
            bool initated = false;

            void init()
            {
                const D3D_FEATURE_LEVEL lvl[4] = {D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
                                                  D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0};
                UINT createDeviceFlags = 0
#ifdef _DEBUG
                                         | D3D11_CREATE_DEVICE_DEBUG
#endif
                    ;
                // 1.创建设备
                HRESULT hr = D3D11CreateDevice(nullptr,
                                               D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, lvl,
                                               _countof(lvl), D3D11_SDK_VERSION, &device, nullptr, nullptr);
                if (hr == E_INVALIDARG)
                {
                    // DirectX 11.0 Runtime doesn't recognize D3D_FEATURE_LEVEL_11_1 as a valid value
                    hr = D3D11CreateDevice(nullptr,
                                           D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &lvl[1],
                                           _countof(lvl) - 1, D3D11_SDK_VERSION, &device, nullptr, nullptr);
                }
                if (FAILED(hr))
                {
                    printf("Failed creating Direct3D 11 device %08X\n", hr);
                }
                device->GetImmediateContext(&context);
            }
            ~device_()
            {
                context->Release();
                device->Release();
            }
        } device = device_();
        class ComputeShader_
        {
        private:
            friend class compute_GPU;
            ID3D11ComputeShader *computeShader = nullptr;

        public:
            ComputeShader_(){};                                  // empty constructor
            ComputeShader_(const ComputeShader_ &copy) = delete; // no copy constructor
            ComputeShader_(ComputeShader_ &&move) noexcept       // move constructor
            {
                computeShader = move.computeShader;
                move.computeShader = nullptr;
            }
            ComputeShader_(const char *hlsl_code, size_t code_len, const char *entryPoint = "main")
            {
                init(hlsl_code, code_len, entryPoint);
            }
            void init(const char *hlsl_code, size_t code_len, const char *entryPoint)
            {
                HRESULT hr = S_OK;
                // Verify compute shader is supported
                auto featureLevel = device->GetFeatureLevel();
                if (featureLevel < D3D_FEATURE_LEVEL_11_0)
                {
                    D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS hwopts = {0};
                    hr = device->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &hwopts, sizeof(hwopts));
                    if (!hwopts.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x)
                    {
                        return;
                    }
                }
                // Create compute shader
                UINT flags = D3DCOMPILE_ENABLE_STRICTNESS
#if defined(DEBUG) || defined(_DEBUG)
                             | D3DCOMPILE_DEBUG
#else
                         | D3DCOMPILE_OPTIMIZATION_LEVEL3
#endif
                    ;
                ID3DBlob *csBlob = nullptr;
                ID3DBlob *errorBlob = nullptr;
                hr = D3DCompile(hlsl_code, code_len, 0, nullptr, nullptr, entryPoint, "cs_5_0", flags, 0, &csBlob, &errorBlob);

                if (FAILED(hr))
                {
                    if (errorBlob != nullptr)
                    {
                        printf("%s\n", (char *)errorBlob->GetBufferPointer());
                        errorBlob->Release();
                    }

                    if (csBlob != nullptr)
                        csBlob->Release();
                    printf("Failed compiling shader %08X\n", hr);
                    return;
                }
                // Create compute shader

                hr = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &computeShader);
                if (FAILED(hr))
                {
                    csBlob->Release();
                    printf("Failed creating shader %08X\n", hr);
                    return;
                }
                csBlob->Release();

                return;
            }
            // if strlength==0 ,means input is a file name, otherwise is the length for ascii hlsl code
            ~ComputeShader_()
            {
                if (computeShader != nullptr)
                {
                    computeShader->Release();
                    computeShader = nullptr;
                }
            };
            ID3D11ComputeShader *operator->()
#ifdef bionukg_nothrow
                noexcept
#endif
            {
#ifdef _DEBUG
                if (computeShader == nullptr)
                {
                    printf("computeShader is nullptr\n");
#ifndef bionukg_nothrow
                    throw;
#endif
                }
#endif
                return computeShader;
            }

            _Check_return_ ComputeShader_ make(const char *hlsl_code, size_t code_len, const char *entryPoint = "main")
            {
                return ComputeShader_(hlsl_code, code_len, entryPoint);
            }

            _Check_return_ ComputeShader_ make(const char *hlsl_code, const char *entryPoint = "main")
            {
                return ComputeShader_(hlsl_code, strlen(hlsl_code), entryPoint);
            }

            // uint core_function(float2 p)
            std::string svg_hlsl_build(const char *core_fpoint, int outwidth)
            {
                std::string svg_hlsl;
                const char *fmt =
                    ""
                    "RWStructuredBuffer<uint> output : register(u0);\n"
                    "uint core_function(float2 p)"
                    "{\n"
                    "%s"
                    "}\n"
                    "float2 position_transform(float2 p,uint wid)\n"
                    "{\n"
                    // transform the pixel position from [0,0]~[wid,wid] to [-1,-1]~[1,1]
                    "    float2 p2 = (p*2+1) / wid - 1;"
                    "    return p2;\n"
                    "}\n"
                    "[numthreads(1, 1, 1)]\n"
                    "void main(uint3 DTid : SV_DispatchThreadID)\n"
                    "{\n"
                    "    uint wid =%d;\n"
                    // get the pixel position
                    "    float2 p = float2(DTid.x,DTid.y);\n"
                    // get the pixel value
                    "    uint v = core_function(position_transform(p,wid));\n"
                    // write the pixel value, z is always 0
                    "    output[DTid.x + DTid.y * wid] = v;\n"
                    "}\n";
                svg_hlsl.resize(strlen(fmt) + strlen(core_fpoint) + 4);
                sprintf_s(&svg_hlsl[0], svg_hlsl.size(), fmt, core_fpoint, outwidth);

                return svg_hlsl;
            }

            std::string svg_hlsl_build_oversample(const char *core_fpoint, int outwidth, int oversample)
            {
                std::string svg_hlsl;
                const char *fmt =
                    ""
                    "RWStructuredBuffer<uint> output : register(u0);\n"
                    "uint core_function(float2 p)"
                    "{\n"
                    "%s"
                    "}\n"

                    "[numthreads(1, 1, 1)]\n"
                    "void main(uint3 DTid : SV_DispatchThreadID)\n"
                    "{\n"
                    "    uint wid =%d;\n"
                    "    uint oversample = %d;\n"
                    "    uint4 rgba = uint4(0,0,0,0);\n"
                    "    float x_left=float(DTid.x)/wid*2-1;\n"
                    "    float y_top=float(DTid.y)/wid*2-1;\n"
                    "    float delta=2.0/wid/oversample;\n"
                    "    for(int i=0;i<oversample;i++)\n"
                    "    {\n"
                    "        float x_center=x_left+delta*(i+0.5);\n"
                    "        for(int j=0;j<oversample;j++)\n"
                    "        {\n"
                    "            float y_center=y_top+delta*(j+0.5);\n"
                    "            uint v = core_function(float2(x_center,y_center));\n"
                    // add to rgba
                    "            rgba += uint4(v&0xFF,(v>>8)&0xFF,(v>>16)&0xFF,(v>>24)&0xFF);\n"
                    "        }\n"
                    "    }\n"
                    // divide by oversample^2
                    "    rgba /= oversample*oversample;\n"
                    // write the pixel value, z is always 0
                    "    output[DTid.x + DTid.y * wid] = rgba.x + (rgba.y<<8) + (rgba.z<<16) + (rgba.w<<24);\n"
                    "}\n";
                svg_hlsl.resize(strlen(fmt) + strlen(core_fpoint) + 4);
                sprintf_s(&svg_hlsl[0], svg_hlsl.size(), fmt, core_fpoint, outwidth, oversample);

                return svg_hlsl;
            }
        };
        class ShaderResourceView_
        {
            static inline int srv_cnt = 0;
            ID3D11Buffer *SRV_buffer = nullptr;
            ID3D11ShaderResourceView *SRV = nullptr;

        public:
            constexpr static const inline int max_srv_cnt = 128;

            ShaderResourceView_(){};                                       // empty constructor
            ShaderResourceView_(const ShaderResourceView_ &copy) = delete; // no copy constructor
            ShaderResourceView_(ShaderResourceView_ &&move) noexcept       // move constructor
            {
                SRV_buffer = move.SRV_buffer;
                SRV = move.SRV;
                move.SRV_buffer = nullptr;
                move.SRV = nullptr;
            }
            ShaderResourceView_(const void *data_SRV_t, const int data_SRV_t_size)
            {
                if (srv_cnt >= max_srv_cnt)
                {
                    printf("too many SRV created, try delete some\n");
#ifndef bionukg_nothrow
                    throw;
#endif
                    return;
                }
                srv_cnt++;
                HRESULT hr = S_OK;
                CD3D11_BUFFER_DESC desc0(
                    data_SRV_t_size * sizeof(uint32_t),
                    D3D11_BIND_SHADER_RESOURCE);
                uint32_t *bufin0 = (uint32_t *)data_SRV_t;
                D3D11_SUBRESOURCE_DATA InitData0 = {bufin0, 0, 0};
                hr = device->CreateBuffer(&desc0, &InitData0, &SRV_buffer);
                if (FAILED(hr))
                {
                    printf("Failed creating buffer %08X\n", hr);
                    return;
                }

                CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(
                    D3D11_SRV_DIMENSION_BUFFEREX,
                    DXGI_FORMAT_R32_UINT,
                    0,
                    data_SRV_t_size,
                    D3D11_BUFFEREX_SRV_FLAG_RAW);
                hr = device->CreateShaderResourceView(SRV_buffer, &srvDesc, &SRV);
                if (FAILED(hr))
                {
                    printf("Failed creating SRV view %08X\n", hr);
                    return;
                }
            }
            ~ShaderResourceView_()
            {
                if (SRV_buffer != nullptr)
                {
                    SRV_buffer->Release();
                    SRV_buffer = nullptr;
                }
                if (SRV != nullptr)
                {
                    SRV->Release();
                    SRV = nullptr;
                }
                srv_cnt--;
            };
            ID3D11ShaderResourceView *operator->()
#ifdef bionukg_nothrow
                noexcept
#endif

            {
#ifdef _DEBUG
                if (SRV == nullptr)
                {
                    printf("SRV is nullptr\n");
#ifndef bionukg_nothrow
                    throw;
#endif
                }
#endif
                return SRV;
            }
            _Check_return_ ShaderResourceView_ make(const void *data_SRV_t, const int data_SRV_t_size)
            {
                return ShaderResourceView_(data_SRV_t, data_SRV_t_size);
            }
            template <typename T, int size>
            _Check_return_ ShaderResourceView_ make(const T (&data_SRV_t)[size])
            {
                static_assert(sizeof(T) == sizeof(uint32_t), "size of T and uint32_t should be the same");
                return ShaderResourceView_(data_SRV_t, size);
            }
        };
        using ReadOnlyBuffer_ = ShaderResourceView_;
        class UnorderedAccessView_
        {
            static inline int uav_cnt = 0;
            ID3D11Buffer *UAV_buffer = nullptr;
            ID3D11UnorderedAccessView *UAV = nullptr;
            ID3D11Buffer *buffer_result = nullptr;

        public:
            constexpr static const inline int max_uav_cnt = 8;

            UnorderedAccessView_(){};                                        // empty constructor
            UnorderedAccessView_(const UnorderedAccessView_ &copy) = delete; // no copy constructor
            UnorderedAccessView_(UnorderedAccessView_ &&move) noexcept       // move constructor
            {
                UAV_buffer = move.UAV_buffer;
                UAV = move.UAV;
                move.UAV_buffer = nullptr;
                move.UAV = nullptr;
            }
            UnorderedAccessView_(void *data_UAV, const int data_UAV_size_)
            {
#ifdef _DEBUG
                if (uav_cnt >= max_uav_cnt)
                {
                    printf("too many UAV created, try delete some\n");
#ifndef bionukg_nothrow
                    throw;
#endif
                }
#endif
                uav_cnt++;
                HRESULT hr = S_OK;
                size_t data_UAV_size = (size_t)data_UAV_size_ * 4;
                CD3D11_BUFFER_DESC desc_out(
                    (UINT)(sizeof(uint32_t) * data_UAV_size),
                    D3D11_BIND_UNORDERED_ACCESS,
                    D3D11_USAGE_DEFAULT,
                    0,
                    D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
                    sizeof(uint32_t));

                D3D11_SUBRESOURCE_DATA InitData_out = {data_UAV, 0, 0};
                if (data_UAV != NULL)
                    hr = device->CreateBuffer(&desc_out, &InitData_out, &UAV_buffer);
                else
                    hr = device->CreateBuffer(&desc_out, 0, &UAV_buffer);

                if (FAILED(hr))
                {
                    printf("Failed creating UAV buffer %08X\n", hr);
                    return;
                }

                CD3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc(
                    D3D11_UAV_DIMENSION_BUFFER,
                    DXGI_FORMAT_UNKNOWN,
                    0,
                    (UINT)data_UAV_size,
                    0);
                hr = device->CreateUnorderedAccessView(UAV_buffer, &uavDesc, &UAV);
                if (FAILED(hr))
                {
                    printf("Failed creating UAV view %08X\n", hr);
                    return;
                }

                // create the result buffer, to store the result from UAV

                CD3D11_BUFFER_DESC desc_result(
                    (UINT)(data_UAV_size * sizeof(uint32_t)),
                    0,
                    D3D11_USAGE_STAGING,
                    D3D11_CPU_ACCESS_READ,
                    D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
                    sizeof(uint32_t));
                hr = device->CreateBuffer(&desc_result, nullptr, &buffer_result);
                if (FAILED(hr))
                {
                    printf("Failed creating UAV buffer result %08X\n", hr);
                    return;
                }

                return;
            };

            ~UnorderedAccessView_()
            {
                if (UAV_buffer != nullptr)
                {
                    UAV_buffer->Release();
                    UAV_buffer = nullptr;
                }
                if (UAV != nullptr)
                {
                    UAV->Release();
                    UAV = nullptr;
                }
                if (buffer_result != nullptr)
                {
                    buffer_result->Release();
                    buffer_result = nullptr;
                }
                uav_cnt--;
            };
            ID3D11UnorderedAccessView *operator->()
#ifdef bionukg_nothrow
                noexcept
#endif
            {
#ifdef _DEBUG
                if (UAV == nullptr)
                {
                    printf("UAV is nullptr\n");
#ifndef bionukg_nothrow
                    throw;
#endif
                }
#endif
                return UAV;
            }
            void copy_result()
            {
                device.context->CopyResource(buffer_result, UAV_buffer);
                return;
            }
            ID3D11Buffer *get_buffer_result()
            {
                return buffer_result;
            }
            _Check_return_ UnorderedAccessView_ make(void *data_UAV, const int data_UAV_size)
            {
                return UnorderedAccessView_(data_UAV, data_UAV_size);
            }
            template <typename T, int size>
            _Check_return_ UnorderedAccessView_ make(T (&data_UAV)[size])
            {
                static_assert(sizeof(T) == sizeof(uint32_t), "size of T and uint32_t should be the same");
                return UnorderedAccessView_(data_UAV, size);
            }
        };
        using ReadWriteBuffer_ = UnorderedAccessView_;

        template <int SRV_count, int UAV_count>
        static void load(ComputeShader_ &shader, ShaderResourceView_ (&SRV)[SRV_count], UnorderedAccessView_ (&UAV)[UAV_count])
        {
            // Load the compute shader
            device.context->CSSetShader(shader.operator->(), nullptr, 0);

            { // Load the shader resource views
                ID3D11ShaderResourceView *srvs[SRV_count];
                for (int i = 0; i < SRV_count; i++)
                {
                    srvs[i] = SRV[i].operator->();
                }
                device.context->CSSetShaderResources(0, SRV_count, srvs);
            }

            { // Load the unordered access views
                ID3D11UnorderedAccessView *uavs[UAV_count];
                for (int i = 0; i < UAV_count; i++)
                {
                    uavs[i] = UAV[i].operator->();
                }
                device.context->CSSetUnorderedAccessViews(0, UAV_count, uavs, nullptr);
            }
        }

        static void load(ComputeShader_ &shader, ShaderResourceView_ *SRV, int SRV_count, UnorderedAccessView_ *UAV, int UAV_count)
        {
            // Load the compute shader
            device.context->CSSetShader(shader.operator->(), nullptr, 0);

            { // Load the shader resource views
                std::vector<ID3D11ShaderResourceView *> srvs(SRV_count);
                for (int i = 0; i < SRV_count; i++)
                {
                    srvs[i] = SRV[i].operator->();
                }
                device.context->CSSetShaderResources(0, SRV_count, srvs.data());
            }

            { // Load the unordered access views
                std::vector<ID3D11UnorderedAccessView *> uavs(UAV_count);
                for (int i = 0; i < UAV_count; i++)
                {
                    uavs[i] = UAV[i].operator->();
                }
                device.context->CSSetUnorderedAccessViews(0, UAV_count, uavs.data(), nullptr);
            }
        }

        static void run(UINT x_d, UINT y_d = 1, UINT z_d = 1)
        {
            device.context->Dispatch(x_d, y_d, z_d);
        }
        static void unload()
        {
            {
                ID3D11ShaderResourceView *nullSRV[ShaderResourceView_::max_srv_cnt] = {nullptr};
                device.context->CSSetShaderResources(0, ShaderResourceView_::max_srv_cnt, nullSRV);
            }
            {
                ID3D11UnorderedAccessView *nullUAV[UnorderedAccessView_::max_uav_cnt] = {nullptr};
                device.context->CSSetUnorderedAccessViews(0, UnorderedAccessView_::max_uav_cnt, nullUAV, nullptr);
            }

            device.context->CSSetShader(nullptr, nullptr, 0);
        }

        static void get_data(void *data_out, const uint32_t data_out_size, UnorderedAccessView_ &UAV)
        {
            UAV.copy_result();

            D3D11_MAPPED_SUBRESOURCE MappedResource;
            HRESULT hr = device.context->Map(UAV.get_buffer_result(),
                                             0, D3D11_MAP_READ, 0, &MappedResource);
            memcpy(data_out, MappedResource.pData, data_out_size * sizeof(uint32_t));
            device.context->Unmap(UAV.get_buffer_result(), 0);
        }

        static ComputeShader_ cs()
        {
            return ComputeShader_();
        };
        static ShaderResourceView_ srv()
        {
            return ShaderResourceView_();
        };
        static UnorderedAccessView_ uav()
        {
            return UnorderedAccessView_();
        };
    };

    extern "C"
    {
        _declspec(dllexport) int64_t create_compute_GPU()
        {
            return (int64_t) new compute_GPU();
        }
        _declspec(dllexport) void destroy_compute_GPU(int64_t del)
        {
            delete (compute_GPU *)del;
        }
        _declspec(dllexport) HRESULT compute_GPU__set_code_segment__hlsl_code(int64_t obj, const hlsl_code *hlsl)
        {
            return ((compute_GPU *)obj)->set_code_segment(*hlsl);
        }
        _declspec(dllexport) HRESULT compute_GPU__set_code_segment__file(int64_t obj, const wchar_t *srcFile, const char *entryPoint)
        {
            return ((compute_GPU *)obj)->set_code_segment(srcFile, entryPoint);
        }
        _declspec(dllexport) HRESULT compute_GPU__set_code_segment__string(int64_t obj, const char *src, const char *entryPoint)
        {
            return ((compute_GPU *)obj)->set_code_segment((LPCWSTR)src, entryPoint, strlen(src));
        }
        _declspec(dllexport) HRESULT compute_GPU__set_input_data_segment_u32__ptrs(int64_t obj, uint32_t **data_SRV_t, const int *data_SRV_t_size, const size_t ptrs_count)
        {
            return ((compute_GPU *)obj)->set_input_data_segment_u32(data_SRV_t, data_SRV_t_size, ptrs_count);
        }
        _declspec(dllexport) HRESULT compute_GPU__set_input_data_segment_u32__ptr(int64_t obj, const uint32_t *data_SRV_t, const int data_SRV_t_size)
        {
            return ((compute_GPU *)obj)->set_input_data_segment_u32((uint32_t **)&data_SRV_t, &data_SRV_t_size, 1);
        }
        _declspec(dllexport) HRESULT compute_GPU__set_output_data_segment_u32__ptrs(int64_t obj, uint32_t **data_UAV_u, const int *data_UAV_u_size, const size_t ptrs_count)
        {
            return ((compute_GPU *)obj)->set_output_data_segment_u32(data_UAV_u, data_UAV_u_size, ptrs_count);
        }
        _declspec(dllexport) HRESULT compute_GPU__set_output_data_segment_u32__ptr(int64_t obj, const uint32_t *data_UAV_u, const int data_UAV_u_size)
        {
            return ((compute_GPU *)obj)->set_output_data_segment_u32((uint32_t **)&data_UAV_u, &data_UAV_u_size, 1);
        }
        _declspec(dllexport) HRESULT compute_GPU__load_data(int64_t obj)
        {
            return ((compute_GPU *)obj)->load_data();
        }
        _declspec(dllexport) HRESULT compute_GPU__load_data_reordered(int64_t obj, const uint8_t *SRV_order, const uint8_t *UAV_order)
        {
            return ((compute_GPU *)obj)->load_data_reordered(SRV_order, UAV_order);
        }
        _declspec(dllexport) HRESULT compute_GPU__run_x(int64_t obj, UINT x_d)
        {
            return ((compute_GPU *)obj)->run(x_d);
        }
        _declspec(dllexport) HRESULT compute_GPU__run_xy(int64_t obj, UINT x_d, UINT y_d)
        {
            return ((compute_GPU *)obj)->run(x_d, y_d);
        }
        _declspec(dllexport) HRESULT compute_GPU__run_xyz(int64_t obj, UINT x_d, UINT y_d, UINT z_d)
        {
            return ((compute_GPU *)obj)->run(x_d, y_d, z_d);
        }
        _declspec(dllexport) HRESULT compute_GPU__unload_data(int64_t obj)
        {
            return ((compute_GPU *)obj)->unload_data();
        }
        _declspec(dllexport) HRESULT compute_GPU__get_data__ptrs(int64_t obj, uint32_t **data_out, const uint32_t *data_out_size, const uint8_t *data_out_idx, const size_t counts)
        {
            return ((compute_GPU *)obj)->get_data(data_out, data_out_size, data_out_idx, counts);
        }
        _declspec(dllexport) HRESULT compute_GPU__get_data__ptr(int64_t obj, uint32_t *data_out, const uint32_t data_out_size, const uint8_t data_out_idx)
        {
            return ((compute_GPU *)obj)->get_data((uint32_t **)&data_out, &data_out_size, &data_out_idx, 1);
        }
    }

    namespace HLSL_type
    {

        class float2
        {
        public:
            float f[2];

        public:
            float2(float x = 0, float y = 0) { f[0] = x, f[1] = y; }
            float2(const float2 &b) { f[0] = b.f[0], f[1] = b.f[1]; }

        public:
            void operator=(const float2 &b) { f[0] = b.f[0], f[1] = b.f[1]; };
            float2 operator+(const float2 &b) const { return float2(f[0] + b.f[0], f[1] + b.f[1]); };
            float2 operator-(const float2 &b) const { return float2(f[0] - b.f[0], f[1] - b.f[1]); };
            float2 operator*(const float2 &b) const { return float2(f[0] * b.f[0], f[1] * b.f[1]); };
            float2 operator/(const float2 &b) const { return float2(f[0] / b.f[0], f[1] / b.f[1]); };
            float2 operator+(const float &b) const { return float2(f[0] + b, f[1] + b); };
            float2 operator-(const float &b) const { return float2(f[0] - b, f[1] - b); };
            float2 operator*(const float &b) const { return float2(f[0] * b, f[1] * b); };
            float2 operator/(const float &b) const { return float2(f[0] / b, f[1] / b); };
            float2 operator-() const { return float2(-f[0], -f[1]); };
            void operator+=(const float2 &b) { f[0] += b.f[0], f[1] += b.f[1]; };
            void operator-=(const float2 &b) { f[0] -= b.f[0], f[1] -= b.f[1]; };
            void operator*=(const float2 &b) { f[0] *= b.f[0], f[1] *= b.f[1]; };
            void operator/=(const float2 &b) { f[0] /= b.f[0], f[1] /= b.f[1]; };
            void operator+=(const float &b) { f[0] += b, f[1] += b; };
            void operator-=(const float &b) { f[0] -= b, f[1] -= b; };
            void operator*=(const float &b) { f[0] *= b, f[1] *= b; };
            void operator/=(const float &b) { f[0] /= b, f[1] /= b; };
            bool operator==(const float2 &b) const { return f[0] == b.f[0] && f[1] == b.f[1]; };
            bool operator!=(const float2 &b) const { return f[0] != b.f[0] || f[1] != b.f[1]; };

        public:
            float x() const { return f[0]; };
            float y() const { return f[1]; };
            float2 xx() const { return float2(f[0], f[0]); };
            float2 xy() const { return float2(f[0], f[1]); };
            float2 yx() const { return float2(f[1], f[0]); };
            float2 yy() const { return float2(f[1], f[1]); };
        };
        class float3
        {
        public:
            float f[3];

        public:
            float3(float x = 0, float y = 0, float z = 0) { f[0] = x, f[1] = y, f[2] = z; }
            float3(float2 xy, float z = 0) { f[0] = xy.x(), f[1] = xy.y(), f[2] = z; }
            float3(float x, float2 yz) { f[0] = x, f[1] = yz.x(), f[2] = yz.y(); }
            float3(const float3 &b) { f[0] = b.f[0], f[1] = b.f[1], f[2] = b.f[2]; }

        public:
            void operator=(const float3 &b) { f[0] = b.f[0], f[1] = b.f[1], f[2] = b.f[2]; };
            float3 operator+(const float3 &b) const { return float3(f[0] + b.f[0], f[1] + b.f[1], f[2] + b.f[2]); };
            float3 operator-(const float3 &b) const { return float3(f[0] - b.f[0], f[1] - b.f[1], f[2] - b.f[2]); };
            float3 operator*(const float3 &b) const { return float3(f[0] * b.f[0], f[1] * b.f[1], f[2] * b.f[2]); };
            float3 operator/(const float3 &b) const { return float3(f[0] / b.f[0], f[1] / b.f[1], f[2] / b.f[2]); };
            float3 operator+(const float &b) const { return float3(f[0] + b, f[1] + b, f[2] + b); };
            float3 operator-(const float &b) const { return float3(f[0] - b, f[1] - b, f[2] - b); };
            float3 operator*(const float &b) const { return float3(f[0] * b, f[1] * b, f[2] * b); };
            float3 operator/(const float &b) const { return float3(f[0] / b, f[1] / b, f[2] / b); };
            float3 operator-() const { return float3(-f[0], -f[1], -f[2]); };
            void operator+=(const float3 &b) { f[0] += b.f[0], f[1] += b.f[1], f[2] += b.f[2]; };
            void operator-=(const float3 &b) { f[0] -= b.f[0], f[1] -= b.f[1], f[2] -= b.f[2]; };
            void operator*=(const float3 &b) { f[0] *= b.f[0], f[1] *= b.f[1], f[2] *= b.f[2]; };
            void operator/=(const float3 &b) { f[0] /= b.f[0], f[1] /= b.f[1], f[2] /= b.f[2]; };
            void operator+=(const float &b) { f[0] += b, f[1] += b, f[2] += b; };
            void operator-=(const float &b) { f[0] -= b, f[1] -= b, f[2] -= b; };
            void operator*=(const float &b) { f[0] *= b, f[1] *= b, f[2] *= b; };
            void operator/=(const float &b) { f[0] /= b, f[1] /= b, f[2] /= b; };
            bool operator==(const float3 &b) const { return f[0] == b.f[0] && f[1] == b.f[1] && f[2] == b.f[2]; };
            bool operator!=(const float3 &b) const { return f[0] != b.f[0] || f[1] != b.f[1] || f[2] != b.f[2]; };

        public:
            float x() const { return f[0]; };
            float y() const { return f[1]; };
            float z() const { return f[2]; };
            float2 xx() const { return float2(f[0], f[0]); };
            float2 xy() const { return float2(f[0], f[1]); };
            float2 xz() const { return float2(f[0], f[2]); };
            float2 yx() const { return float2(f[1], f[0]); };
            float2 yy() const { return float2(f[1], f[1]); };
            float2 yz() const { return float2(f[1], f[2]); };
            float2 zx() const { return float2(f[2], f[0]); };
            float2 zy() const { return float2(f[2], f[1]); };
            float2 zz() const { return float2(f[2], f[2]); };
            float3 xxx() const { return float3(f[0], f[0], f[0]); };
            float3 xxy() const { return float3(f[0], f[0], f[1]); };
            float3 xxz() const { return float3(f[0], f[0], f[2]); };
            float3 xyx() const { return float3(f[0], f[1], f[0]); };
            float3 xyy() const { return float3(f[0], f[1], f[1]); };
            float3 xyz() const { return float3(f[0], f[1], f[2]); };
            float3 xzx() const { return float3(f[0], f[2], f[0]); };
            float3 xzy() const { return float3(f[0], f[2], f[1]); };
            float3 xzz() const { return float3(f[0], f[2], f[2]); };
            float3 yxx() const { return float3(f[1], f[0], f[0]); };
            float3 yxy() const { return float3(f[1], f[0], f[1]); };
            float3 yxz() const { return float3(f[1], f[0], f[2]); };
            float3 yyx() const { return float3(f[1], f[1], f[0]); };
            float3 yyy() const { return float3(f[1], f[1], f[1]); };
            float3 yyz() const { return float3(f[1], f[1], f[2]); };
            float3 yzx() const { return float3(f[1], f[2], f[0]); };
            float3 yzy() const { return float3(f[1], f[2], f[1]); };
            float3 yzz() const { return float3(f[1], f[2], f[2]); };
            float3 zxx() const { return float3(f[2], f[0], f[0]); };
            float3 zxy() const { return float3(f[2], f[0], f[1]); };
            float3 zxz() const { return float3(f[2], f[0], f[2]); };
            float3 zyx() const { return float3(f[2], f[1], f[0]); };
            float3 zyy() const { return float3(f[2], f[1], f[1]); };
            float3 zyz() const { return float3(f[2], f[1], f[2]); };
            float3 zzx() const { return float3(f[2], f[2], f[0]); };
            float3 zzy() const { return float3(f[2], f[2], f[1]); };
            float3 zzz() const { return float3(f[2], f[2], f[2]); };
        };
        class float4
        {
        private:
            float f[4];

        public:
            float4(float x = 0, float y = 0, float z = 0, float w = 0) { f[0] = x, f[1] = y, f[2] = z, f[3] = w; }
            float4(float2 xy, float z = 0, float w = 0) { f[0] = xy.x(), f[1] = xy.y(), f[2] = z, f[3] = w; }
            float4(float x, float2 yz, float w = 0) { f[0] = x, f[1] = yz.x(), f[2] = yz.y(), f[3] = w; }
            float4(float x, float y, float2 zw) { f[0] = x, f[1] = y, f[2] = zw.x(), f[3] = zw.y(); }
            float4(float2 xy, float2 zw) { f[0] = xy.x(), f[1] = xy.y(), f[2] = zw.x(), f[3] = zw.y(); }
            float4(float3 xyz, float w = 0) { f[0] = xyz.x(), f[1] = xyz.y(), f[2] = xyz.z(), f[3] = w; }
            float4(float x, float3 yzw) { f[0] = x, f[1] = yzw.x(), f[2] = yzw.y(), f[3] = yzw.z(); }
            float4(const float4 &b) { f[0] = b.f[0], f[1] = b.f[1], f[2] = b.f[2], f[3] = b.f[3]; }

        public:
            void operator=(const float4 &b) { f[0] = b.f[0], f[1] = b.f[1], f[2] = b.f[2], f[3] = b.f[3]; };
            float4 operator+(const float4 &b) const { return float4(f[0] + b.f[0], f[1] + b.f[1], f[2] + b.f[2], f[3] + b.f[3]); };
            float4 operator-(const float4 &b) const { return float4(f[0] - b.f[0], f[1] - b.f[1], f[2] - b.f[2], f[3] - b.f[3]); };
            float4 operator*(const float4 &b) const { return float4(f[0] * b.f[0], f[1] * b.f[1], f[2] * b.f[2], f[3] * b.f[3]); };
            float4 operator/(const float4 &b) const { return float4(f[0] / b.f[0], f[1] / b.f[1], f[2] / b.f[2], f[3] / b.f[3]); };
            float4 operator+(const float &b) const { return float4(f[0] + b, f[1] + b, f[2] + b, f[3] + b); };
            float4 operator-(const float &b) const { return float4(f[0] - b, f[1] - b, f[2] - b, f[3] - b); };
            float4 operator*(const float &b) const { return float4(f[0] * b, f[1] * b, f[2] * b, f[3] * b); };
            float4 operator/(const float &b) const { return float4(f[0] / b, f[1] / b, f[2] / b, f[3] / b); };
            float4 operator-() const { return float4(-f[0], -f[1], -f[2], -f[3]); };
            void operator+=(const float4 &b) { f[0] += b.f[0], f[1] += b.f[1], f[2] += b.f[2], f[3] += b.f[3]; };
            void operator-=(const float4 &b) { f[0] -= b.f[0], f[1] -= b.f[1], f[2] -= b.f[2], f[3] -= b.f[3]; };
            void operator*=(const float4 &b) { f[0] *= b.f[0], f[1] *= b.f[1], f[2] *= b.f[2], f[3] *= b.f[3]; };
            void operator/=(const float4 &b) { f[0] /= b.f[0], f[1] /= b.f[1], f[2] /= b.f[2], f[3] /= b.f[3]; };
            void operator+=(const float &b) { f[0] += b, f[1] += b, f[2] += b, f[3] += b; };
            void operator-=(const float &b) { f[0] -= b, f[1] -= b, f[2] -= b, f[3] -= b; };
            void operator*=(const float &b) { f[0] *= b, f[1] *= b, f[2] *= b, f[3] *= b; };
            void operator/=(const float &b) { f[0] /= b, f[1] /= b, f[2] /= b, f[3] /= b; };
            bool operator==(const float4 &b) const { return f[0] == b.f[0] && f[1] == b.f[1] && f[2] == b.f[2] && f[3] == b.f[3]; };
            bool operator!=(const float4 &b) const { return f[0] != b.f[0] || f[1] != b.f[1] || f[2] != b.f[2] || f[3] != b.f[3]; };

        public:
            float x() const { return f[0]; };
            float y() const { return f[1]; };
            float z() const { return f[2]; };
            float w() const { return f[3]; };
            float2 xx() const { return float2(f[0], f[0]); };
            float2 xy() const { return float2(f[0], f[1]); };
            float2 xz() const { return float2(f[0], f[2]); };
            float2 xw() const { return float2(f[0], f[3]); };
            float2 yx() const { return float2(f[1], f[0]); };
            float2 yy() const { return float2(f[1], f[1]); };
            float2 yz() const { return float2(f[1], f[2]); };
            float2 yw() const { return float2(f[1], f[3]); };
            float2 zx() const { return float2(f[2], f[0]); };
            float2 zy() const { return float2(f[2], f[1]); };
            float2 zz() const { return float2(f[2], f[2]); };
            float2 zw() const { return float2(f[2], f[3]); };
            float2 wx() const { return float2(f[3], f[0]); };
            float2 wy() const { return float2(f[3], f[1]); };
            float2 wz() const { return float2(f[3], f[2]); };
            float2 ww() const { return float2(f[3], f[3]); };
            float3 xxx() const { return float3(f[0], f[0], f[0]); };
            float3 xxy() const { return float3(f[0], f[0], f[1]); };
            float3 xxz() const { return float3(f[0], f[0], f[2]); };
            float3 xxw() const { return float3(f[0], f[0], f[3]); };
            float3 xyx() const { return float3(f[0], f[1], f[0]); };
            float3 xyy() const { return float3(f[0], f[1], f[1]); };
            float3 xyz() const { return float3(f[0], f[1], f[2]); };
            float3 xyw() const { return float3(f[0], f[1], f[3]); };
            float3 xzx() const { return float3(f[0], f[2], f[0]); };
            float3 xzy() const { return float3(f[0], f[2], f[1]); };
            float3 xzz() const { return float3(f[0], f[2], f[2]); };
            float3 xzw() const { return float3(f[0], f[2], f[3]); };
            float3 xwx() const { return float3(f[0], f[3], f[0]); };
            float3 xwy() const { return float3(f[0], f[3], f[1]); };
            float3 xwz() const { return float3(f[0], f[3], f[2]); };
            float3 xww() const { return float3(f[0], f[3], f[3]); };
            float3 yxx() const { return float3(f[1], f[0], f[0]); };
            float3 yxy() const { return float3(f[1], f[0], f[1]); };
            float3 yxz() const { return float3(f[1], f[0], f[2]); };
            float3 yxw() const { return float3(f[1], f[0], f[3]); };
            float3 yyx() const { return float3(f[1], f[1], f[0]); };
            float3 yyy() const { return float3(f[1], f[1], f[1]); };
            float3 yyz() const { return float3(f[1], f[1], f[2]); };
            float3 yyw() const { return float3(f[1], f[1], f[3]); };
            float3 yzx() const { return float3(f[1], f[2], f[0]); };
            float3 yzy() const { return float3(f[1], f[2], f[1]); };
            float3 yzz() const { return float3(f[1], f[2], f[2]); };
            float3 yzw() const { return float3(f[1], f[2], f[3]); };
            float3 ywx() const { return float3(f[1], f[3], f[0]); };
            float3 ywy() const { return float3(f[1], f[3], f[1]); };
            float3 ywz() const { return float3(f[1], f[3], f[2]); };
            float3 yww() const { return float3(f[1], f[3], f[3]); };
            float3 zxx() const { return float3(f[2], f[0], f[0]); };
            float3 zxy() const { return float3(f[2], f[0], f[1]); };
            float3 zxz() const { return float3(f[2], f[0], f[2]); };
            float3 zxw() const { return float3(f[2], f[0], f[3]); };
            float3 zyx() const { return float3(f[2], f[1], f[0]); };
            float3 zyy() const { return float3(f[2], f[1], f[1]); };
            float3 zyz() const { return float3(f[2], f[1], f[2]); };
            float3 zyw() const { return float3(f[2], f[1], f[3]); };
            float3 zzx() const { return float3(f[2], f[2], f[0]); };
            float3 zzy() const { return float3(f[2], f[2], f[1]); };
            float3 zzz() const { return float3(f[2], f[2], f[2]); };
            float3 zzw() const { return float3(f[2], f[2], f[3]); };
            float3 zwx() const { return float3(f[2], f[3], f[0]); };
            float3 zwy() const { return float3(f[2], f[3], f[1]); };
            float3 zwz() const { return float3(f[2], f[3], f[2]); };
            float3 zww() const { return float3(f[2], f[3], f[3]); };
            float3 wxx() const { return float3(f[3], f[0], f[0]); };
            float3 wxy() const { return float3(f[3], f[0], f[1]); };
            float3 wxz() const { return float3(f[3], f[0], f[2]); };
            float3 wxw() const { return float3(f[3], f[0], f[3]); };
            float3 wyx() const { return float3(f[3], f[1], f[0]); };
            float3 wyy() const { return float3(f[3], f[1], f[1]); };
            float3 wyz() const { return float3(f[3], f[1], f[2]); };
            float3 wyw() const { return float3(f[3], f[1], f[3]); };
            float3 wzx() const { return float3(f[3], f[2], f[0]); };
            float3 wzy() const { return float3(f[3], f[2], f[1]); };
            float3 wzz() const { return float3(f[3], f[2], f[2]); };
            float3 wzw() const { return float3(f[3], f[2], f[3]); };
            float3 wwx() const { return float3(f[3], f[3], f[0]); };
            float3 wwy() const { return float3(f[3], f[3], f[1]); };
            float3 wwz() const { return float3(f[3], f[3], f[2]); };
            float3 www() const { return float3(f[3], f[3], f[3]); };
            // the code to generate the above code
            // char symbol[4] = {'x', 'y', 'z', 'w'};
            // for (int i = 0; i < 4; i++)
            //{
            //     for (int j = 0; j < 4; j++)
            //     {
            //         for (int k = 0; k < 4; k++)
            //         {
            //             printf("float3 %c%c%c() const { return float3(f[%d], f[%d], f[%d]); };\n", symbol[i], symbol[j], symbol[k], i, j, k);
            //         }
            //     }
            // }
            float4 xxxx() const { return float4(f[0], f[0], f[0], f[0]); };
            float4 xxxy() const { return float4(f[0], f[0], f[0], f[1]); };
            float4 xxxz() const { return float4(f[0], f[0], f[0], f[2]); };
            float4 xxxw() const { return float4(f[0], f[0], f[0], f[3]); };
            float4 xxyx() const { return float4(f[0], f[0], f[1], f[0]); };
            float4 xxyy() const { return float4(f[0], f[0], f[1], f[1]); };
            float4 xxyz() const { return float4(f[0], f[0], f[1], f[2]); };
            float4 xxyw() const { return float4(f[0], f[0], f[1], f[3]); };
            float4 xxzx() const { return float4(f[0], f[0], f[2], f[0]); };
            float4 xxzy() const { return float4(f[0], f[0], f[2], f[1]); };
            float4 xxzz() const { return float4(f[0], f[0], f[2], f[2]); };
            float4 xxzw() const { return float4(f[0], f[0], f[2], f[3]); };
            float4 xxwx() const { return float4(f[0], f[0], f[3], f[0]); };
            float4 xxwy() const { return float4(f[0], f[0], f[3], f[1]); };
            float4 xxwz() const { return float4(f[0], f[0], f[3], f[2]); };
            float4 xxww() const { return float4(f[0], f[0], f[3], f[3]); };
            float4 xyxx() const { return float4(f[0], f[1], f[0], f[0]); };
            float4 xyxy() const { return float4(f[0], f[1], f[0], f[1]); };
            float4 xyxz() const { return float4(f[0], f[1], f[0], f[2]); };
            float4 xyxw() const { return float4(f[0], f[1], f[0], f[3]); };
            float4 xyyx() const { return float4(f[0], f[1], f[1], f[0]); };
            float4 xyyy() const { return float4(f[0], f[1], f[1], f[1]); };
            float4 xyyz() const { return float4(f[0], f[1], f[1], f[2]); };
            float4 xyyw() const { return float4(f[0], f[1], f[1], f[3]); };
            float4 xyzx() const { return float4(f[0], f[1], f[2], f[0]); };
            float4 xyzy() const { return float4(f[0], f[1], f[2], f[1]); };
            float4 xyzz() const { return float4(f[0], f[1], f[2], f[2]); };
            float4 xyzw() const { return float4(f[0], f[1], f[2], f[3]); };
            float4 xywx() const { return float4(f[0], f[1], f[3], f[0]); };
            float4 xywy() const { return float4(f[0], f[1], f[3], f[1]); };
            float4 xywz() const { return float4(f[0], f[1], f[3], f[2]); };
            float4 xyww() const { return float4(f[0], f[1], f[3], f[3]); };
            float4 xzxx() const { return float4(f[0], f[2], f[0], f[0]); };
            float4 xzxy() const { return float4(f[0], f[2], f[0], f[1]); };
            float4 xzxz() const { return float4(f[0], f[2], f[0], f[2]); };
            float4 xzxw() const { return float4(f[0], f[2], f[0], f[3]); };
            float4 xzyx() const { return float4(f[0], f[2], f[1], f[0]); };
            float4 xzyy() const { return float4(f[0], f[2], f[1], f[1]); };
            float4 xzyz() const { return float4(f[0], f[2], f[1], f[2]); };
            float4 xzyw() const { return float4(f[0], f[2], f[1], f[3]); };
            float4 xzzx() const { return float4(f[0], f[2], f[2], f[0]); };
            float4 xzzy() const { return float4(f[0], f[2], f[2], f[1]); };
            float4 xzzz() const { return float4(f[0], f[2], f[2], f[2]); };
            float4 xzzw() const { return float4(f[0], f[2], f[2], f[3]); };
            float4 xzwx() const { return float4(f[0], f[2], f[3], f[0]); };
            float4 xzwy() const { return float4(f[0], f[2], f[3], f[1]); };
            float4 xzwz() const { return float4(f[0], f[2], f[3], f[2]); };
            float4 xzww() const { return float4(f[0], f[2], f[3], f[3]); };
            float4 xwxx() const { return float4(f[0], f[3], f[0], f[0]); };
            float4 xwxy() const { return float4(f[0], f[3], f[0], f[1]); };
            float4 xwxz() const { return float4(f[0], f[3], f[0], f[2]); };
            float4 xwxw() const { return float4(f[0], f[3], f[0], f[3]); };
            float4 xwyx() const { return float4(f[0], f[3], f[1], f[0]); };
            float4 xwyy() const { return float4(f[0], f[3], f[1], f[1]); };
            float4 xwyz() const { return float4(f[0], f[3], f[1], f[2]); };
            float4 xwyw() const { return float4(f[0], f[3], f[1], f[3]); };
            float4 xwzx() const { return float4(f[0], f[3], f[2], f[0]); };
            float4 xwzy() const { return float4(f[0], f[3], f[2], f[1]); };
            float4 xwzz() const { return float4(f[0], f[3], f[2], f[2]); };
            float4 xwzw() const { return float4(f[0], f[3], f[2], f[3]); };
            float4 xwwx() const { return float4(f[0], f[3], f[3], f[0]); };
            float4 xwwy() const { return float4(f[0], f[3], f[3], f[1]); };
            float4 xwwz() const { return float4(f[0], f[3], f[3], f[2]); };
            float4 xwww() const { return float4(f[0], f[3], f[3], f[3]); };
            float4 yxxx() const { return float4(f[1], f[0], f[0], f[0]); };
            float4 yxxy() const { return float4(f[1], f[0], f[0], f[1]); };
            float4 yxxz() const { return float4(f[1], f[0], f[0], f[2]); };
            float4 yxxw() const { return float4(f[1], f[0], f[0], f[3]); };
            float4 yxyx() const { return float4(f[1], f[0], f[1], f[0]); };
            float4 yxyy() const { return float4(f[1], f[0], f[1], f[1]); };
            float4 yxyz() const { return float4(f[1], f[0], f[1], f[2]); };
            float4 yxyw() const { return float4(f[1], f[0], f[1], f[3]); };
            float4 yxzx() const { return float4(f[1], f[0], f[2], f[0]); };
            float4 yxzy() const { return float4(f[1], f[0], f[2], f[1]); };
            float4 yxzz() const { return float4(f[1], f[0], f[2], f[2]); };
            float4 yxzw() const { return float4(f[1], f[0], f[2], f[3]); };
            float4 yxwx() const { return float4(f[1], f[0], f[3], f[0]); };
            float4 yxwy() const { return float4(f[1], f[0], f[3], f[1]); };
            float4 yxwz() const { return float4(f[1], f[0], f[3], f[2]); };
            float4 yxww() const { return float4(f[1], f[0], f[3], f[3]); };
            float4 yyxx() const { return float4(f[1], f[1], f[0], f[0]); };
            float4 yyxy() const { return float4(f[1], f[1], f[0], f[1]); };
            float4 yyxz() const { return float4(f[1], f[1], f[0], f[2]); };
            float4 yyxw() const { return float4(f[1], f[1], f[0], f[3]); };
            float4 yyyx() const { return float4(f[1], f[1], f[1], f[0]); };
            float4 yyyy() const { return float4(f[1], f[1], f[1], f[1]); };
            float4 yyyz() const { return float4(f[1], f[1], f[1], f[2]); };
            float4 yyyw() const { return float4(f[1], f[1], f[1], f[3]); };
            float4 yyzx() const { return float4(f[1], f[1], f[2], f[0]); };
            float4 yyzy() const { return float4(f[1], f[1], f[2], f[1]); };
            float4 yyzz() const { return float4(f[1], f[1], f[2], f[2]); };
            float4 yyzw() const { return float4(f[1], f[1], f[2], f[3]); };
            float4 yywx() const { return float4(f[1], f[1], f[3], f[0]); };
            float4 yywy() const { return float4(f[1], f[1], f[3], f[1]); };
            float4 yywz() const { return float4(f[1], f[1], f[3], f[2]); };
            float4 yyww() const { return float4(f[1], f[1], f[3], f[3]); };
            float4 yzxx() const { return float4(f[1], f[2], f[0], f[0]); };
            float4 yzxy() const { return float4(f[1], f[2], f[0], f[1]); };
            float4 yzxz() const { return float4(f[1], f[2], f[0], f[2]); };
            float4 yzxw() const { return float4(f[1], f[2], f[0], f[3]); };
            float4 yzyx() const { return float4(f[1], f[2], f[1], f[0]); };
            float4 yzyy() const { return float4(f[1], f[2], f[1], f[1]); };
            float4 yzyz() const { return float4(f[1], f[2], f[1], f[2]); };
            float4 yzyw() const { return float4(f[1], f[2], f[1], f[3]); };
            float4 yzzx() const { return float4(f[1], f[2], f[2], f[0]); };
            float4 yzzy() const { return float4(f[1], f[2], f[2], f[1]); };
            float4 yzzz() const { return float4(f[1], f[2], f[2], f[2]); };
            float4 yzzw() const { return float4(f[1], f[2], f[2], f[3]); };
            float4 yzwx() const { return float4(f[1], f[2], f[3], f[0]); };
            float4 yzwy() const { return float4(f[1], f[2], f[3], f[1]); };
            float4 yzwz() const { return float4(f[1], f[2], f[3], f[2]); };
            float4 yzww() const { return float4(f[1], f[2], f[3], f[3]); };
            float4 ywxx() const { return float4(f[1], f[3], f[0], f[0]); };
            float4 ywxy() const { return float4(f[1], f[3], f[0], f[1]); };
            float4 ywxz() const { return float4(f[1], f[3], f[0], f[2]); };
            float4 ywxw() const { return float4(f[1], f[3], f[0], f[3]); };
            float4 ywyx() const { return float4(f[1], f[3], f[1], f[0]); };
            float4 ywyy() const { return float4(f[1], f[3], f[1], f[1]); };
            float4 ywyz() const { return float4(f[1], f[3], f[1], f[2]); };
            float4 ywyw() const { return float4(f[1], f[3], f[1], f[3]); };
            float4 ywzx() const { return float4(f[1], f[3], f[2], f[0]); };
            float4 ywzy() const { return float4(f[1], f[3], f[2], f[1]); };
            float4 ywzz() const { return float4(f[1], f[3], f[2], f[2]); };
            float4 ywzw() const { return float4(f[1], f[3], f[2], f[3]); };
            float4 ywwx() const { return float4(f[1], f[3], f[3], f[0]); };
            float4 ywwy() const { return float4(f[1], f[3], f[3], f[1]); };
            float4 ywwz() const { return float4(f[1], f[3], f[3], f[2]); };
            float4 ywww() const { return float4(f[1], f[3], f[3], f[3]); };
            float4 zxxx() const { return float4(f[2], f[0], f[0], f[0]); };
            float4 zxxy() const { return float4(f[2], f[0], f[0], f[1]); };
            float4 zxxz() const { return float4(f[2], f[0], f[0], f[2]); };
            float4 zxxw() const { return float4(f[2], f[0], f[0], f[3]); };
            float4 zxyx() const { return float4(f[2], f[0], f[1], f[0]); };
            float4 zxyy() const { return float4(f[2], f[0], f[1], f[1]); };
            float4 zxyz() const { return float4(f[2], f[0], f[1], f[2]); };
            float4 zxyw() const { return float4(f[2], f[0], f[1], f[3]); };
            float4 zxzx() const { return float4(f[2], f[0], f[2], f[0]); };
            float4 zxzy() const { return float4(f[2], f[0], f[2], f[1]); };
            float4 zxzz() const { return float4(f[2], f[0], f[2], f[2]); };
            float4 zxzw() const { return float4(f[2], f[0], f[2], f[3]); };
            float4 zxwx() const { return float4(f[2], f[0], f[3], f[0]); };
            float4 zxwy() const { return float4(f[2], f[0], f[3], f[1]); };
            float4 zxwz() const { return float4(f[2], f[0], f[3], f[2]); };
            float4 zxww() const { return float4(f[2], f[0], f[3], f[3]); };
            float4 zyxx() const { return float4(f[2], f[1], f[0], f[0]); };
            float4 zyxy() const { return float4(f[2], f[1], f[0], f[1]); };
            float4 zyxz() const { return float4(f[2], f[1], f[0], f[2]); };
            float4 zyxw() const { return float4(f[2], f[1], f[0], f[3]); };
            float4 zyyx() const { return float4(f[2], f[1], f[1], f[0]); };
            float4 zyyy() const { return float4(f[2], f[1], f[1], f[1]); };
            float4 zyyz() const { return float4(f[2], f[1], f[1], f[2]); };
            float4 zyyw() const { return float4(f[2], f[1], f[1], f[3]); };
            float4 zyzx() const { return float4(f[2], f[1], f[2], f[0]); };
            float4 zyzy() const { return float4(f[2], f[1], f[2], f[1]); };
            float4 zyzz() const { return float4(f[2], f[1], f[2], f[2]); };
            float4 zyzw() const { return float4(f[2], f[1], f[2], f[3]); };
            float4 zywx() const { return float4(f[2], f[1], f[3], f[0]); };
            float4 zywy() const { return float4(f[2], f[1], f[3], f[1]); };
            float4 zywz() const { return float4(f[2], f[1], f[3], f[2]); };
            float4 zyww() const { return float4(f[2], f[1], f[3], f[3]); };
            float4 zzxx() const { return float4(f[2], f[2], f[0], f[0]); };
            float4 zzxy() const { return float4(f[2], f[2], f[0], f[1]); };
            float4 zzxz() const { return float4(f[2], f[2], f[0], f[2]); };
            float4 zzxw() const { return float4(f[2], f[2], f[0], f[3]); };
            float4 zzyx() const { return float4(f[2], f[2], f[1], f[0]); };
            float4 zzyy() const { return float4(f[2], f[2], f[1], f[1]); };
            float4 zzyz() const { return float4(f[2], f[2], f[1], f[2]); };
            float4 zzyw() const { return float4(f[2], f[2], f[1], f[3]); };
            float4 zzzx() const { return float4(f[2], f[2], f[2], f[0]); };
            float4 zzzy() const { return float4(f[2], f[2], f[2], f[1]); };
            float4 zzzz() const { return float4(f[2], f[2], f[2], f[2]); };
            float4 zzzw() const { return float4(f[2], f[2], f[2], f[3]); };
            float4 zzwx() const { return float4(f[2], f[2], f[3], f[0]); };
            float4 zzwy() const { return float4(f[2], f[2], f[3], f[1]); };
            float4 zzwz() const { return float4(f[2], f[2], f[3], f[2]); };
            float4 zzww() const { return float4(f[2], f[2], f[3], f[3]); };
            float4 zwxx() const { return float4(f[2], f[3], f[0], f[0]); };
            float4 zwxy() const { return float4(f[2], f[3], f[0], f[1]); };
            float4 zwxz() const { return float4(f[2], f[3], f[0], f[2]); };
            float4 zwxw() const { return float4(f[2], f[3], f[0], f[3]); };
            float4 zwyx() const { return float4(f[2], f[3], f[1], f[0]); };
            float4 zwyy() const { return float4(f[2], f[3], f[1], f[1]); };
            float4 zwyz() const { return float4(f[2], f[3], f[1], f[2]); };
            float4 zwyw() const { return float4(f[2], f[3], f[1], f[3]); };
            float4 zwzx() const { return float4(f[2], f[3], f[2], f[0]); };
            float4 zwzy() const { return float4(f[2], f[3], f[2], f[1]); };
            float4 zwzz() const { return float4(f[2], f[3], f[2], f[2]); };
            float4 zwzw() const { return float4(f[2], f[3], f[2], f[3]); };
            float4 zwwx() const { return float4(f[2], f[3], f[3], f[0]); };
            float4 zwwy() const { return float4(f[2], f[3], f[3], f[1]); };
            float4 zwwz() const { return float4(f[2], f[3], f[3], f[2]); };
            float4 zwww() const { return float4(f[2], f[3], f[3], f[3]); };
            float4 wxxx() const { return float4(f[3], f[0], f[0], f[0]); };
            float4 wxxy() const { return float4(f[3], f[0], f[0], f[1]); };
            float4 wxxz() const { return float4(f[3], f[0], f[0], f[2]); };
            float4 wxxw() const { return float4(f[3], f[0], f[0], f[3]); };
            float4 wxyx() const { return float4(f[3], f[0], f[1], f[0]); };
            float4 wxyy() const { return float4(f[3], f[0], f[1], f[1]); };
            float4 wxyz() const { return float4(f[3], f[0], f[1], f[2]); };
            float4 wxyw() const { return float4(f[3], f[0], f[1], f[3]); };
            float4 wxzx() const { return float4(f[3], f[0], f[2], f[0]); };
            float4 wxzy() const { return float4(f[3], f[0], f[2], f[1]); };
            float4 wxzz() const { return float4(f[3], f[0], f[2], f[2]); };
            float4 wxzw() const { return float4(f[3], f[0], f[2], f[3]); };
            float4 wxwx() const { return float4(f[3], f[0], f[3], f[0]); };
            float4 wxwy() const { return float4(f[3], f[0], f[3], f[1]); };
            float4 wxwz() const { return float4(f[3], f[0], f[3], f[2]); };
            float4 wxww() const { return float4(f[3], f[0], f[3], f[3]); };
            float4 wyxx() const { return float4(f[3], f[1], f[0], f[0]); };
            float4 wyxy() const { return float4(f[3], f[1], f[0], f[1]); };
            float4 wyxz() const { return float4(f[3], f[1], f[0], f[2]); };
            float4 wyxw() const { return float4(f[3], f[1], f[0], f[3]); };
            float4 wyyx() const { return float4(f[3], f[1], f[1], f[0]); };
            float4 wyyy() const { return float4(f[3], f[1], f[1], f[1]); };
            float4 wyyz() const { return float4(f[3], f[1], f[1], f[2]); };
            float4 wyyw() const { return float4(f[3], f[1], f[1], f[3]); };
            float4 wyzx() const { return float4(f[3], f[1], f[2], f[0]); };
            float4 wyzy() const { return float4(f[3], f[1], f[2], f[1]); };
            float4 wyzz() const { return float4(f[3], f[1], f[2], f[2]); };
            float4 wyzw() const { return float4(f[3], f[1], f[2], f[3]); };
            float4 wywx() const { return float4(f[3], f[1], f[3], f[0]); };
            float4 wywy() const { return float4(f[3], f[1], f[3], f[1]); };
            float4 wywz() const { return float4(f[3], f[1], f[3], f[2]); };
            float4 wyww() const { return float4(f[3], f[1], f[3], f[3]); };
            float4 wzxx() const { return float4(f[3], f[2], f[0], f[0]); };
            float4 wzxy() const { return float4(f[3], f[2], f[0], f[1]); };
            float4 wzxz() const { return float4(f[3], f[2], f[0], f[2]); };
            float4 wzxw() const { return float4(f[3], f[2], f[0], f[3]); };
            float4 wzyx() const { return float4(f[3], f[2], f[1], f[0]); };
            float4 wzyy() const { return float4(f[3], f[2], f[1], f[1]); };
            float4 wzyz() const { return float4(f[3], f[2], f[1], f[2]); };
            float4 wzyw() const { return float4(f[3], f[2], f[1], f[3]); };
            float4 wzzx() const { return float4(f[3], f[2], f[2], f[0]); };
            float4 wzzy() const { return float4(f[3], f[2], f[2], f[1]); };
            float4 wzzz() const { return float4(f[3], f[2], f[2], f[2]); };
            float4 wzzw() const { return float4(f[3], f[2], f[2], f[3]); };
            float4 wzwx() const { return float4(f[3], f[2], f[3], f[0]); };
            float4 wzwy() const { return float4(f[3], f[2], f[3], f[1]); };
            float4 wzwz() const { return float4(f[3], f[2], f[3], f[2]); };
            float4 wzww() const { return float4(f[3], f[2], f[3], f[3]); };
            float4 wwxx() const { return float4(f[3], f[3], f[0], f[0]); };
            float4 wwxy() const { return float4(f[3], f[3], f[0], f[1]); };
            float4 wwxz() const { return float4(f[3], f[3], f[0], f[2]); };
            float4 wwxw() const { return float4(f[3], f[3], f[0], f[3]); };
            float4 wwyx() const { return float4(f[3], f[3], f[1], f[0]); };
            float4 wwyy() const { return float4(f[3], f[3], f[1], f[1]); };
            float4 wwyz() const { return float4(f[3], f[3], f[1], f[2]); };
            float4 wwyw() const { return float4(f[3], f[3], f[1], f[3]); };
            float4 wwzx() const { return float4(f[3], f[3], f[2], f[0]); };
            float4 wwzy() const { return float4(f[3], f[3], f[2], f[1]); };
            float4 wwzz() const { return float4(f[3], f[3], f[2], f[2]); };
            float4 wwzw() const { return float4(f[3], f[3], f[2], f[3]); };
            float4 wwwx() const { return float4(f[3], f[3], f[3], f[0]); };
            float4 wwwy() const { return float4(f[3], f[3], f[3], f[1]); };
            float4 wwwz() const { return float4(f[3], f[3], f[3], f[2]); };
            float4 wwww() const { return float4(f[3], f[3], f[3], f[3]); };
        };
        float dot(const float2 &a, const float2 &b) { return a.x() * b.x() + a.y() * b.y(); }
        float dot(const float3 &a, const float3 &b) { return a.x() * b.x() + a.y() * b.y() + a.z() * b.z(); }
        float dot(const float4 &a, const float4 &b) { return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w(); }
        float3 cross(const float3 &a, const float3 &b) { return float3(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x()); }
        template <typename floatN>
        float length(const floatN &a) { return sqrtf(dot(a, a)); }
        template <typename floatN>
        float distance(const floatN &a, const floatN &b) { return length(a - b); }
        template <typename floatN>
        float dst(const floatN &a, const floatN &b) { return distance(a, b); }
        template <typename floatN>
        floatN normalize(const floatN &a) { return a / length(a); }
        template <typename floatN>
        floatN nrm(const floatN &a) { return normalize(a); }
        template <typename floatN>
        floatN reflect(const floatN &a, const floatN &n) { return a - 2.0f * dot(a, n) * n; }
        template <typename floatN>
        floatN refract(const floatN &a, const floatN &n, float eta)
        {
            float NdotI = dot(a, n);
            float k = 1.0f - eta * eta * (1.0f - NdotI * NdotI);
            if (k < 0.0f)
                return floatN(0.0f);
            else
                return eta * a - (eta * NdotI + sqrtf(k)) * n;
        }
        template <typename floatN>
        floatN faceforward(const floatN &a, const floatN &b, const floatN &c) { return dot(b, c) < 0.0f ? a : -a; }
        template <typename floatN>
        floatN lerp(const floatN &a, const floatN &b, float t) { return a * (1.0f - t) + b * t; }
        template <typename floatN>
        floatN mix(const floatN &a, const floatN &b, float t) { return lerp(a, b, t); }
        float minimum(const float &a, const float &b) { return a < b ? a : b; }
        float2 minimum(const float2 &a, const float2 &b) { return float2(minimum(a.x(), b.x()), minimum(a.y(), b.y())); }
        float3 minimum(const float3 &a, const float3 &b) { return float3(minimum(a.x(), b.x()), minimum(a.y(), b.y()), minimum(a.z(), b.z())); }
        float4 minimum(const float4 &a, const float4 &b) { return float4(minimum(a.x(), b.x()), minimum(a.y(), b.y()), minimum(a.z(), b.z()), minimum(a.w(), b.w())); }
        float maximum(const float &a, const float &b) { return a > b ? a : b; }
        float2 maximum(const float2 &a, const float2 &b) { return float2(maximum(a.x(), b.x()), maximum(a.y(), b.y())); }
        float3 maximum(const float3 &a, const float3 &b) { return float3(maximum(a.x(), b.x()), maximum(a.y(), b.y()), maximum(a.z(), b.z())); }
        float4 maximum(const float4 &a, const float4 &b) { return float4(maximum(a.x(), b.x()), maximum(a.y(), b.y()), maximum(a.z(), b.z()), maximum(a.w(), b.w())); }
        template <typename floatN>
        floatN clamp(const floatN &x, const floatN &min, const floatN &max) { return minimum(maximum(x, min), max); }

        float2 allOne(float2 type) { return float2(1.0f, 1.0f); }
        float3 allOne(float3 type) { return float3(1.0f, 1.0f, 1.0f); }
        float4 allOne(float4 type) { return float4(1.0f, 1.0f, 1.0f, 1.0f); }

        template <typename floatN>
        floatN saturate(const floatN &x) { return clamp(x, floatN(), allOne(floatN())); }
    };
#ifdef namespace_bionukg
};
#endif

#endif // bionukg_concalc_h