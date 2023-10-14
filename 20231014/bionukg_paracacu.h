#pragma once
#define _SILENCE_AMP_DEPRECATION_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>          
#include <process.h>
#include<amp.h>


typedef uint32_t icount;
unsigned __stdcall paraft(void* arg);

typedef struct _pararg {
    icount i;
    icount i_less_than;
    void (*fun)(void*, icount);
    void* arg;
    /*void showinfo(void)
    {
        printf("%3d\t%3d\t%p\t%p\n", i, i_less_than, fun, arg);
    }*/
}pararg;
/// <summary>
/// CPU多线程，与for循环功能相同（函数f应当线程安全）
/// </summary>
/// <param name="int_i_equ   "> 即int i=int_i_equ </param>
/// <param name="i_less_than "> 即i小于i_less_than </param>
/// <param name="f           "> 循环体中的函数(指针) </param>
/// <param name="arglists    "> 函数的参数（指针)  </param>
/// <param name="thread_num  "> 使用的线程数 </param>
/// <param name="waitime     "> 最大等待时间，可以填INFINITE </param>
void para_for_cpu(int int_i_equ, int i_less_than,
    void(*f)(void*, icount), void* arglists, int thread_num,DWORD waitime)
    //i_less_than-int_i_equ>thread_num 最好这样
{
    if (waitime < 5000)
    {
        waitime = 5000;
        printf("wait longer,otherwise this function is needless\n");
    }

    if (thread_num > MAXIMUM_WAIT_OBJECTS)
        thread_num = MAXIMUM_WAIT_OBJECTS;
    if (thread_num < 2)
        thread_num = 2;
    HANDLE* phd = new HANDLE[thread_num + 4];
    unsigned* tid = new unsigned[thread_num];
    pararg* parg = new pararg[thread_num];

    for (int tnum = 0; tnum < thread_num; tnum++)
    {
        parg[tnum].arg = arglists;
        parg[tnum].fun = f;
        parg[tnum].i = int_i_equ;
        parg[tnum].i_less_than = int_i_equ;
        parg[tnum].i += ((uint64_t)i_less_than - int_i_equ) * tnum / thread_num;
        parg[tnum].i_less_than += ((uint64_t)i_less_than - int_i_equ) * (tnum + 1) / thread_num;

        //printf("bef:%p  ", &parg[tnum]);
        //parg[tnum].showinfo();
        phd[tnum] = (HANDLE)_beginthreadex(0, 0, paraft, (void*)&parg[tnum], CREATE_SUSPENDED, &tid[tnum]);
        if (phd[tnum] == 0)
        {
            system("pause");
            return;
        }
    }
    for (int tnum = 0; tnum < thread_num; tnum++)
    {
        ResumeThread(phd[tnum]);
    }
    if (WaitForMultipleObjects(thread_num, phd, 1, waitime) == WAIT_TIMEOUT)
    {
        printf("time out %dms,try longer wait\n",waitime);
    }
    delete[] phd;
    delete[] tid;
    delete[] parg;
    return;
}
/// <summary>
/// 其中每个线程的函数入口。
/// </summary>
/// <param name="arg  ">   其实是一个pararg结构的指针</param>
/// <returns>没啥好返回的，就返回for循环的最大值吧</returns>
unsigned __stdcall paraft(void* arg)
{
    if (((pararg*)arg)->i < 0 || ((pararg*)arg)->i_less_than < 0) {
        system("pause");
    }
    //printf("aft:%p  %3d\t%3d\t%p\t%p\n", arg, ((pararg*)arg)->i, ((pararg*)arg)->i_less_than, ((pararg*)arg)->fun, ((pararg*)arg)->arg);

    for (; ((pararg*)arg)->i < ((pararg*)arg)->i_less_than; ((pararg*)arg)->i++)
    {
        ((pararg*)arg)->fun(((pararg*)arg)->arg, ((pararg*)arg)->i);
    }
    return (unsigned)((pararg*)arg)->i_less_than;
}




template <typename tin,typename tout,typename tfun>
class para_for_gpu
{
    /// <summary>
    /// 找到源代码自己填！这个暂时没想到怎么模板化
    /// </summary>
    /// <param name="arg"></param>
    /// <param name="size"></param>
    //void CppAmpMethod(void**arg, icount size) {
    //    //int aCPP[] = { 1, 2, 3, 4, 5 };
    //    //int bCPP[] = { 6, 7, 8, 9, 10 };
    //    //int sumCPP[size] = { 0 };

    //    // Create C++ AMP objects.
    //    array_view<tin, 1> a(size, arg[0]);//自己配置！输入端的内容不应被修改
    //    array_view<tout, 1> b(size, (arg[1]);//自己配置！输出端的内容初值无效
    //    //array_view<float, 1> sum(size, sumCPP);

    //    b.discard_data();//tout成员清空数据

    //    parallel_for_each(//显卡循环体
    //        // Define the compute domain, which is the set of threads that are created.
    //        sum.extent,
    //        // Define the code to run on each thread on the accelerator.
    //        [=](index<1> idx) restrict(amp)
    //        {
    //            //for循环内的函数，自己写
    //            
    //        }
    //    );



        // Print the results. The expected output is "7, 9, 11, 13, 15".
        /*for (int i = 0; i < size; i++) {
            std::cout << sum[i] << "\n";
        }
    }*/


public:
    void example(int size,int**arg)
    {

        int aCPP[] = { 1, 2, 3, 4, 5 };
        int bCPP[] = { 6, 7, 8, 9, 10 };
        int sumCPP[5] = { 0 };

        // Create C++ AMP objects.
        array_view<float, 1> a(5, aCPP);//自己配置！输入端的内容不应被修改
        array_view<float, 1> b(5, bCPP);//自己配置！输出端的内容初值无效
        array_view<float, 1> sum(5, sumCPP);

        b.discard_data();//tout成员清空数据

        parallel_for_each(//显卡循环体
            // Define the compute domain, which is the set of threads that are created.
            sum.extent,
            // Define the code to run on each thread on the accelerator.
            [=](index<1> idx) restrict(amp)
            {
                //for循环内的函数，自己写

            }
        );



        //Print the results.The expected output is "7, 9, 11, 13, 15".
            for (int i = 0; i < size; i++) {
                std::cout << sum[i] << "\n";
            }

    }
    para_for_gpu();
    ~para_for_gpu();

private:

};

