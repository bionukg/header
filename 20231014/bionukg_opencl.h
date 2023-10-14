#pragma once
#ifndef bionukg_opencl_h
#define bionukg_opencl_h

#include "C:\Program Files\Polyspace\R2020b\sys\cuda\win64\cuda\include\CL\opencl.h"
#include "./bionukg_dllloader.h"

#ifdef bionukg_namespace
namespace bionukg_namespace
{
#endif // bionukg_namespace

#include "./bionukg_dllloader_def.h"
    class opencl_lib_
    {
    private:
        class opencl_;
        class opencl_instance_
        {
        private:
            friend class opencl_;
            friend class opencl_lib_;
            HMODULE libm;
            // clBuildProgram
            cl_int (*clBuildProgram)(cl_program, cl_uint, const cl_device_id *, const char *, void(CL_CALLBACK *)(cl_program, void *), void *) = 0;
            // clCreateContext
            cl_context (*clCreateContext)(const cl_context_properties *, cl_uint, const cl_device_id *, void(CL_CALLBACK *)(const char *, const void *, size_t, void *), void *, cl_int *) = 0;
            // clCreateCommandQueue
            cl_command_queue (*clCreateCommandQueue)(cl_context, cl_device_id, cl_command_queue_properties, cl_int *) = 0;
            // clCreateCommandQueueWithProperties
            using cl_queue_properties = void;
            cl_command_queue (*clCreateCommandQueueWithProperties)(cl_context, cl_device_id, const cl_queue_properties *, cl_int *) = 0;
            // clCreateContextFromType
            cl_context (*clCreateContextFromType)(const cl_context_properties *, cl_device_type, void(CL_CALLBACK *)(const char *, const void *, size_t, void *), void *, cl_int *) = 0;
            // clCreateKernel
            cl_kernel (*clCreateKernel)(cl_program, const char *, cl_int *) = 0;
            // clCreateProgramWithSource
            cl_program (*clCreateProgramWithSource)(cl_context, cl_uint, const char **, const size_t *, cl_int *) = 0;
            // clCreateProgramWithBinary
            cl_program (*clCreateProgramWithBinary)(cl_context, cl_uint, const cl_device_id *, const size_t *, const unsigned char **, cl_int *, cl_int *) = 0;
            // clCreateBuffer
            cl_mem (*clCreateBuffer)(cl_context, cl_mem_flags, size_t, void *, cl_int *) = 0;
            // clFinish
            cl_int (*clFinish)(cl_command_queue) = 0;
            // clGetPlatformIDs
            cl_int (*clGetPlatformIDs)(cl_uint, cl_platform_id *, cl_uint *) = 0;
            // clGetDeviceIDs
            cl_int (*clGetDeviceIDs)(cl_platform_id, cl_device_type, cl_uint, cl_device_id *, cl_uint *) = 0;
            // clGetProgramBuildInfo
            cl_int (*clGetProgramBuildInfo)(cl_program, cl_device_id, cl_program_build_info, size_t, void *, size_t *) = 0;
            // clGetContextInfo
            cl_int (*clGetContextInfo)(cl_context, cl_context_info, size_t, void *, size_t *) = 0;
            // clGetDeviceInfo
            cl_int (*clGetDeviceInfo)(cl_device_id, cl_device_info, size_t, void *, size_t *) = 0;
            // clReleaseProgram
            cl_int (*clReleaseProgram)(cl_program) = 0;
            // clReleaseMemObject
            cl_int (*clReleaseMemObject)(cl_mem) = 0;
            // clReleaseCommandQueue
            cl_int (*clReleaseCommandQueue)(cl_command_queue) = 0;
            // clReleaseContext
            cl_int (*clReleaseContext)(cl_context) = 0;
            // clReleaseKernel
            cl_int (*clReleaseKernel)(cl_kernel) = 0;
            // clSetKernelArg
            cl_int (*clSetKernelArg)(cl_kernel, cl_uint, size_t, const void *) = 0;
            // clEnqueueNDRangeKernel
            cl_int (*clEnqueueNDRangeKernel)(cl_command_queue, cl_kernel, cl_uint, const size_t *, const size_t *, const size_t *, cl_uint, const cl_event *, cl_event *) = 0;
            // clEnqueueReadBuffer
            cl_int (*clEnqueueReadBuffer)(cl_command_queue, cl_mem, cl_bool, size_t, size_t, void *, cl_uint, const cl_event *, cl_event *) = 0;
            opencl_instance_()
            {
                libm = LoadLibraryA("opencl.dll");
                if (libm == 0)
                {
#ifdef _DEBUG
                    throw "LoadLibraryA(\"opencl.dll\"); failed";
#endif // _DEBUG
                    return;
                }
                fninit(clGetPlatformIDs);
                fninit(clGetDeviceIDs);
                fninit(clCreateContext);
                fninit(clCreateCommandQueue);
                fninit(clCreateCommandQueueWithProperties);
                fninit(clCreateKernel);
                fninit(clCreateProgramWithSource);
                fninit(clCreateProgramWithBinary);
                fninit(clBuildProgram);
                fninit(clGetProgramBuildInfo);
                fninit(clGetContextInfo);
                fninit(clGetDeviceInfo);
                fninit(clCreateBuffer);
                fninit(clReleaseProgram);
                fninit(clReleaseMemObject);
                fninit(clReleaseCommandQueue);
                fninit(clReleaseContext);
                fninit(clReleaseKernel);
                fninit(clSetKernelArg);
                fninit(clEnqueueNDRangeKernel);
                fninit(clEnqueueReadBuffer);
                fninit(clFinish);
                fninit(clCreateContextFromType);
            }
            ~opencl_instance_()
            {
                if (libm != 0)
					FreeLibrary(libm);
            }
        } opencl_instance;

    public:
        class opencl_
        {
        public:
            using instance = opencl_lib_::opencl_instance_;
        private:

            friend class opencl_lib_;
            opencl_(instance&ins) :
                clGetPlatformIDs(ins.clGetPlatformIDs),
                clGetDeviceIDs(ins.clGetDeviceIDs),
                clCreateContext(ins.clCreateContext),
                clCreateCommandQueue(ins.clCreateCommandQueue),
                clCreateCommandQueueWithProperties(ins.clCreateCommandQueueWithProperties),
                clCreateKernel(ins.clCreateKernel),
                clCreateProgramWithSource(ins.clCreateProgramWithSource),
                clCreateProgramWithBinary(ins.clCreateProgramWithBinary),
                clBuildProgram(ins.clBuildProgram),
                clGetProgramBuildInfo(ins.clGetProgramBuildInfo),
                clGetContextInfo(ins.clGetContextInfo),
                clGetDeviceInfo(ins.clGetDeviceInfo),
                clCreateBuffer(ins.clCreateBuffer),
                clReleaseProgram(ins.clReleaseProgram),
                clReleaseMemObject(ins.clReleaseMemObject),
                clReleaseCommandQueue(ins.clReleaseCommandQueue),
                clReleaseContext(ins.clReleaseContext),
                clReleaseKernel(ins.clReleaseKernel),
                clSetKernelArg(ins.clSetKernelArg),
                clEnqueueNDRangeKernel(ins.clEnqueueNDRangeKernel),
                clEnqueueReadBuffer(ins.clEnqueueReadBuffer),
                clFinish(ins.clFinish),
                clCreateContextFromType(ins.clCreateContextFromType)
            {};

        public:
            fnrdec(clGetPlatformIDs);
            fnrdec(clGetDeviceIDs);
            fnrdec(clCreateContext);
            fnrdec(clCreateCommandQueue);
            fnrdec(clCreateCommandQueueWithProperties);
            fnrdec(clCreateKernel);
            fnrdec(clCreateProgramWithSource);
            fnrdec(clCreateProgramWithBinary);
            fnrdec(clBuildProgram);
            fnrdec(clGetProgramBuildInfo);
            fnrdec(clGetContextInfo);
            fnrdec(clGetDeviceInfo);
            fnrdec(clCreateBuffer);
            fnrdec(clReleaseProgram);
            fnrdec(clReleaseMemObject);
            fnrdec(clReleaseCommandQueue);
            fnrdec(clReleaseContext);
            fnrdec(clReleaseKernel);
            fnrdec(clSetKernelArg);
            fnrdec(clEnqueueNDRangeKernel);
            fnrdec(clEnqueueReadBuffer);
            fnrdec(clFinish);
            fnrdec(clCreateContextFromType);
        } opencl = opencl_(opencl_instance);
    } opencl_lib;

    static const auto &opencl = opencl_lib.opencl;

#ifdef dynamic_link_opencl
#define pubfn(name) static const auto &name = opencl.name
    pubfn(clGetPlatformIDs);
    pubfn(clGetDeviceIDs);
    pubfn(clCreateContext);
    pubfn(clCreateCommandQueue);
    pubfn(clCreateCommandQueueWithProperties);
    pubfn(clCreateKernel);
    pubfn(clCreateProgramWithSource);
    pubfn(clCreateProgramWithBinary);
    pubfn(clBuildProgram);
    pubfn(clGetProgramBuildInfo);
    pubfn(clGetContextInfo);
    pubfn(clGetDeviceInfo);
    pubfn(clCreateBuffer);
    pubfn(clReleaseProgram);
    pubfn(clReleaseMemObject);
    pubfn(clReleaseCommandQueue);
    pubfn(clReleaseContext);
    pubfn(clReleaseKernel);
    pubfn(clSetKernelArg);
    pubfn(clEnqueueNDRangeKernel);
    pubfn(clEnqueueReadBuffer);
    pubfn(clFinish);
    pubfn(clCreateContextFromType);

#endif //dynamic_link_opencl


#include "./bionukg_dllloader_undef.h"
#ifdef bionukg_namespace
};
#endif // bionukg_namespace

#endif // !bionukg_opencl_h
