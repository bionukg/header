#pragma once
//signal.h
#include<stdint.h>
#include"bionukg_datastruct.h"
//#include<complex.h>
#ifndef PI
#define PI  acos(-1.0)
#else
#error PI has been defined
#endif
#ifndef _bionukg_signal_h_
#define _bionukg_signal_h_

class complex_ {
private:
	
public:
	double r, i;//实部和虚部


	complex_() { r = 0; i = 0; }
	complex_(const complex_& another) { this->r = another.r; this->i = another.i; }
	complex_(double r, double i) { this->r = r; this->i = i; }
	complex_(double direction) { this->r = cos(direction); this->i = sin(direction); }

	void real(const double& x) { r = x; }
	void image(const double& x) { i = x; }
	//赋值
	void assign(const double& x, const double& y) { r = x; i = y; }
	void xchg(complex_& another)
	{
		complex_ tem(another);
		another = *this;
		*this = tem;
		return;
	}
	double real() { return r; }
	double image() { return i; }
	//复数模长
	double modulus(){
		return sqrt(i * i + r * r);
	}
	void show_ri(void)
	{
		printf("%3.3e + %3.3e i\n", r, i);
		//std::cout << r << '+' << i <<" i" << '\n';
	}
	complex_ operator + (const complex_& x)  const {
		return complex_(r + x.r, i + x.i);
	}
	complex_ operator - (const complex_& x)  const {
		return complex_(r - x.r, i - x.i);
	}
	complex_ operator * (const complex_& x)  const {
		return complex_(r * x.r - i * x.i, r * x.i + i * x.r);
	}
	complex_ operator * (const double& x)  const {
		return complex_(r * x ,i * x);
	}

	void operator /= (const double& x) {
		r /= x, i /= x;
	}
	void operator *= (const double& x) {

		this->r *= x;
		this->i *= x;
	}

	void operator *= (const complex_& x) {

		this->r = r * x.r - i * x.i;
		this->i = r * x.i + i * x.r;
	}
	void operator += (const complex_& x) {
		r += x.r, i += x.i;
	}
	void operator -= (const complex_& x) {
		r -= x.r, i -= x.i;
	}

	void operator = (const complex_& x) {
		r = x.r, i = x.i;
	}
	
	void operator =(const int& x)
	{
		r = x, i = 0;
	}
	void operator =(const double& x)
	{
		r = x, i = 0;
	}

	inline complex_ conj() {
		return complex_(r, -i);
	}
};

class dpu //数据处理单元
{
private:
protected:
	//int ocnt = 0;//记录输出绑定了多少输入端
	void (*efunc)(dpu* this_) = 0;//单元功能函数指针
public:
	double o = 0;//输出的数据
	const double* i[2] = { 0 };//输入数据的指针，只读
	void exec()
	{
		if (efunc != 0)
			efunc(this);
		return;
	}
	dpu() {};
	dpu(dpu* i0, dpu* i1)
	{
		i[0] = &i0->o;
		/*if (i0 != 0)
			i0->ocnt++;*/
		i[1] = &i1->o;
		/*if (i1 != 0)
			i1->ocnt++;*/
	};
	~dpu()
	{
		clear();
	}
	/// <summary>
	/// 修改单元功能
	/// </summary>
	void modify(void(*newf)(dpu*))
	{
		efunc = newf;
		return;
	}
	void copyf(dpu* that)
	{
		efunc = that->efunc;
		return;
	}
	void redirect(dpu* i0, dpu* i1)
	{
		/*dpu x;
		auto offset = &x.ocnt - (void*)&x.o;
		if (i[0] != 0)
			((dpu*)(i[0] + offset))->ocnt--;
		if (i[1] != 0)
			((dpu*)(i[1] + offset))->ocnt--;*/
		if (i0 != 0)
			i[0] = &i0->o;
		else
			i[0] = 0;
		/*if (i0 != 0)
			i0->ocnt++;*/
		if (i1 != 0)
			i[1] = &i1->o;
		else
			i[1] = 0;
		/*if (i1 != 0)
			i1->ocnt++;*/
	}
	void clear()
	{
		/*auto offset = (int*)&this->ocnt - (int*)&this->o;
		if (i[0] != 0)
		{
			((dpu*)(i[0] + offset))->ocnt--;
		}
		if (i[1] != 0)
		{
			((dpu*)(i[1] + offset))->ocnt--;
		}*/
		o = 0;
		//ocnt = 0;
		i[0] = 0 ;
		i[1] = 0;
		efunc = 0;
	}
	void operator =(const dpu& that)
	{
		memcpy(this, &that, sizeof(*this));
	}
};

void adder(dpu* dcu)
{
	dcu->o = *dcu->i[0] + *dcu->i[1];
	return;
}

void multiplier(dpu* dcu)
{
	dcu->o = *dcu->i[0] * *dcu->i[1];
	return;
}
//从右向左即延迟
void delayer(dpu* dcu)
{
	dcu->o = *dcu->i[0];
	return;

}
void constant(dpu* dcu)
{
	return;
}
class dpu_system:public list_<dpu>
{
public:
	void run(void)
	{
		for (int i=0;i<length;i++)
		{			
			list[i].exec();
		}
		return;
	}
	void add_dpu(dpu* dcu,int input1,int input2)
	{
		add(*dcu);
		list[length-1].redirect(&list[input1], &list[input2]);

	}
	void addf(void (*fn)(dpu*))
	{
		if (length + 2 > capacity)
			resize(capacity + 8);
		list[length].modify(fn);
		list[length].o = 0;
		length++;
	}
	void addf(void (*fn)(dpu* ), int input1, int input2)
	{
		if (length + 2 > capacity)
			resize(capacity + 8);
		list[length].modify(fn);
		list[length].redirect(&list[input1], &list[input2]);
		list[length].o=0;
		length++;
	}
	void redirect(int idx,int input1, int input2)
	{
		list[idx].redirect(&list[input1], &list[input2]);
		return;
	}
	dpu_system() {};
	dpu_system(size_t size) {};
	~dpu_system()
	{
		

	}
};

class FIR_sys
{
public:
	size_t length=0,idx_in=0,idx_out=0;
	dpu_system* sys = 0;
	double io(double input)
	{
		sys->list[idx_in].o = input;
		sys->run();
		return sys->list[idx_out].o;
	}
	void even_extention_init(double* factors, size_t length)
	{
		deleter();
#define even_extention_factors(i) (i<length0?factors[i]:factors[length0-i])
		size_t length0 = length;
		length *= 2;
		this->length = length;
		sys = new dpu_system();
		sys->resize(length * 3 + 4);
		for (size_t i = 0; i < length; i++)
		{
			sys->addf(constant);
		}
		for (size_t i = 0; i < length; i++)
		{
			sys->addf(multiplier);
		}
		for (size_t i = 0; i < length - 1; i++)
		{
			sys->addf(adder);
		}
		sys->addf(constant, 0, 0);
		for (size_t i = 0; i < length * 3 - 1; i++)
		{
			if (i < length) {
				if (i == 0)
					sys->list[length - i - 1].o = even_extention_factors(i);
				else
					sys->list[length - i - 1].o = even_extention_factors(i) / even_extention_factors(i - 1);
				sys->redirect(i, 0, 0);
			}
			else if (i < length * 2)
			{
				sys->redirect(i, i - length, i + 1);
			}
			else
			{
				sys->redirect(i, i - length + 1, i - 1);
			}
		}
		sys->redirect(2 * length, length, length + 1);//加法器末端

		sys->redirect(3 * length - 1, 0, 0);//输入器
		sys->redirect(2 * length - 1, length - 1, 3 * length - 1);//连接到输入端的乘法器

		idx_out = sys->length - 2;
		idx_in = sys->length - 1;

#undef even_extention_factors
	}
	void default_init(double* factors, size_t length)
	{
		deleter();
		this->length = length;
		sys = new dpu_system();
		sys->resize(length * 3 + 4);
		for (size_t i = 0; i < length; i++)
		{
			sys->addf(constant);
		}
		for (size_t i = 0; i < length; i++)
		{
			sys->addf(multiplier);
		}
		for (size_t i = 0; i < length - 1; i++)
		{
			sys->addf(adder);
		}
		sys->addf(constant, 0, 0);
		for (size_t i = 0; i < length * 3 - 1; i++)
		{
			if (i < length) {
				if (i == 0)
					sys->list[length - i - 1].o = factors[i];
				else
					sys->list[length - i - 1].o = factors[i] / factors[i-1];
				sys->redirect(i, 0, 0);
			}
			else if (i < length * 2)
			{
				sys->redirect(i, i - length, i + 1);
			}
			else
			{
				sys->redirect(i, i - length + 1, i - 1);
			}
		}
		sys->redirect(2 * length,  length , length + 1);//加法器末端

		sys->redirect(3 * length - 1, 0, 0);//输入器
		sys->redirect(2 * length - 1, length - 1, 3 * length - 1);//连接到输入端的乘法器

		idx_out = sys->length - 2;
		idx_in = sys->length - 1;
	};
	//自行保证缓冲区够长
	void get_factors(_Out_ double* factors)
	{
		factors[0] = sys->list[length-1].o;
		for (size_t i = 1; i < length; i++)
		{
			factors[i] = sys->list[length - i - 1].o * factors[i - 1];
		}
	}
	//输入单位冲激响应序列，自动构建
	FIR_sys(double* factors, size_t length)
	{
		default_init(factors,length);
	}
	FIR_sys() {};
	void deleter()
	{
		if (sys != 0)
		{
			delete sys;
		}
	}
	~FIR_sys()
	{
		deleter();
	}
private:

};


//离散傅里叶变换的原始定义，输入时域t，输出频域f
complex_* ft(complex_* t, complex_* f, const int& N)
{
	if (f == 0)
		return 0;
	complex_ omega;
	for (int k = 0; k < N; k++)
	{
		f[k] = complex_(0, 0);
		for (int n = 0; n < N; n++)
		{
			omega = complex_(-2 * PI * k * n / N);//以方向表示的模长为1的复数
			f[k] += t[n] * omega;
		}
	}
	return f;
}
//利用fft原理，但仅递归了一层
complex_* ft2(complex_* t, complex_* f, const int& N2)
{
	if (f == 0)
		return 0;
	complex_ omega;
	for (int k = 0; k < N2; k++)
	{
		f[k] = complex_(0, 0);
		f[k + N2] = complex_(0, 0);
		complex_ f1, f2;
		for (int n = 0; n < N2; n++)
		{
			omega = complex_(-2 * PI * k * n / N2);
			f1 += t[2 * n] * omega;
			f2 += t[2 * n + 1] * omega;
		}
		f2 = f2 * complex_(-1 * PI * k / N2);
		f[k] = f1 + f2;
		f[k + N2] = f1 - f2;
	}
	return f;
}
/// <summary>
/// 递归fft
/// </summary>
/// <returns>f是外部申请的空间</returns>
complex_* fft_recurse(_In_ complex_* in, _Out_ complex_* out, _In_ uint32_t N)
{
	if (out == 0||in==0)
		return 0;
	if (N == 1)
	{
		*out = *in;
		return out;
	}
	N /= 2;
	complex_* t1 = new complex_[N], * t2 = new complex_[N],
		* f1 = new complex_[N], * f2 = new complex_[N];

	for (int i = 0; i < N; i++)
	{
		t1[i] = in[2 * i];
		t2[i] = in[2 * i + 1];
	}
	fft_recurse(t1, f1, N);
	fft_recurse(t2, f2, N);

	complex_ omega;

	for (uint32_t k = 0; k < N; k++)
	{
		omega = complex_(-1 * PI * k / N);
		out[k] = f1[k] + f2[k] * omega;
		out[k + N] = f1[k] - f2[k] * omega;
	}
	delete[]t1, t2, f1, f2;
	return out;
}
/// <summary>
/// 递归fft，输入即输出
/// </summary>
/// <returns></returns>
complex_* fft_recurse(_Inout_ complex_* io,_In_ uint32_t N)
{
	if (io == 0)
		return 0;
	if (N == 1)
	{		
		return io;
	}
	N /= 2;
	complex_* io0 = new complex_[N], * io1 = new complex_[N];

	for (uint32_t i = 0; i < N; i++)//交错分配
	{
		io0[i] = io[2 * i];
		io1[i] = io[2 * i + 1];
	}
	fft_recurse(io0, N);//递归完成前一级的fft
	fft_recurse(io1, N);

	complex_ omega;

	for (uint32_t k = 0; k < N; k++)
	{
		omega = complex_(cos(-1 * PI * k / N), sin(-1 * PI * k / N));
		complex_ tem = io1[k] * omega;
		io[k] = io0[k] + tem;
		io[k + N] = io0[k] - tem;//一次性完成两个运算，由此得以简化
	}
	delete[]io0, io1;
	return io;
}
uint32_t inline bitrev(uint32_t num, uint8_t bitlen)
{
	uint32_t mask = 1, end = 1 << bitlen, ret = 0;
	while (mask!=end)
	{
		ret |= (num & mask) == 0 ? 0 : 1;
		ret <<= 1;
		mask <<= 1;
	}
	return ret>>1;
}
uint32_t inline bitrev2(uint32_t num, uint8_t bitlen)
{
	uint32_t mask = 1, end = 1 << bitlen, ret = 0;
	for(;;)
	{
		ret |= (num & mask) == 0 ? 0 : 1;		
		mask <<= 1;
		if (mask == end)
			return ret;
		ret <<= 1;
	}

}

/// <summary>
/// 蝴蝶变换，理论上不需要额外空间...实际上存了个omega
/// </summary>
/// <param name="io"></param>
/// <param name="N">变换的大小，是2的n次方</param>
/// <returns></returns>
complex_* _cdecl fft_butterfly(_Inout_ complex_* io, _In_ uint32_t N)
{
	if (N == 0)
		return io;
	//蝴蝶层数
	uint32_t n = 0;

	n = 31 - __lzcnt(N);
	/*while (N != 1)
	{
		n++;
		N >>= 1;
	}
	N = 1 << n;*/
	N = 1 << n;

	complex_* omegaN = new complex_[N];

	for (uint32_t i = 0; i < N; i++)
	{
		omegaN[i] = complex_(PI * i*2 / N);//生成全部omega...虽然理论上只用生成N/8个就足矣但是多生成也没关系
		uint32_t revi = bitrev(i, n);
		if (i < revi)
			io[i].xchg(io[revi]);//以及交换二者，把顺序排好
	}
	///
	uint32_t delta = 1;
	complex_ up, down;

	for (uint32_t i = 0; i < n; i++)//一层
	{
		/*
		delta = 1;
		//第一层蝴蝶的跨度是1，没有交错，每坨蝴蝶高度为2
		for (uint32_t j = 0; j < N; j += delta*2)
		{
			up = io[j] + io[j + delta];
			down = io[j] - io[j + delta];
			io[j] = up;
			io[j + delta] = down;
		}
		i++;
		if (i >= n)
			break;
		//第二层蝴蝶的跨度是2，每2只蝴蝶交错在一起，每坨蝴蝶高度为4
		delta <<= 1;//delta=2
		for (uint32_t j = 0; j < N; j += delta * 2)
		{
			up = io[j] + io[j + delta];
			down = io[j] - io[j + delta];
			io[j] = up;
			io[j + 2] = down;
			j++;
			complex_ tem = io[j + delta] * complex_(0, 1);//omegaN[N/4]==虚数i==complex_(0, 1)
			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j -= 1;
		}
		i++;
		if (i >= n)
			break;
		//第三层蝴蝶的跨度是4，每4只蝴蝶交错在一起，每坨蝴蝶高度为8
		delta <<= 1;//delta=4
		for (uint32_t j = 0; j < N; j += delta)
		{
			up = io[j] + io[j + delta] * omegaN[0];
			down = io[j] - io[j + delta] * omegaN[0];
			io[j] = up;
			io[j + delta] = down;
			j++;

			complex_ tem = io[j + delta] * omegaN[N / 8];//用临时变量减少运算

			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j++;

			down = io[j + delta] * omegaN[N / 8 * 2];//发现可以节约一个变量

			up = io[j] + down;
			down = io[j] - down;
			io[j] = up;
			io[j + delta] = down;
			j++;

			up = io[j + delta] * omegaN[N / 8 * 3];//可以再节约一次运算

			down = io[j] - up;
			up += io[j];
			io[j] = up;
			io[j + delta] = down;
			j++;//不-3而是+1；所以循环时j少加4，即少加一个delta

			//其实只需要一个变量：down没有储存信息的意义，不过这么写会一脸懵逼还是算了
			//up = io[j + delta] * omegaN[N / 8 * 3];
			//io[j + delta] = io[j] - up;
			//io[j] += up
		}
		i++;
		if (i >= n)
			break;
		*/
		//找到规律了

		for (uint32_t j = 0; j < N; j += delta)
		{
			for (uint32_t k = 0; k < delta; k++)
			{
				complex_ tem = io[j + delta] * omegaN[N / (2 * delta) * k];
				up = io[j] + tem;
				down = io[j] - tem;
				io[j] = up;
				io[j + delta] = down;
				j++;
			}
		}
		delta <<= 1;
	}
	//...完了？...让我试试.
	delete[] omegaN;
	return io;
} 
complex_* fft_butterfly_v1(_Inout_ complex_* io, _In_ uint32_t N)
{
	if (N == 0)
		return io;
	//蝴蝶层数
	uint32_t n = 0;
	while (N != 1)
	{
		n++;
		N >>= 1;
	}
	N = 1 << n;
	complex_* omegaN = new complex_[N];

	for (uint32_t i = 0; i < N; i++)
	{
		omegaN[i] = complex_(PI * i * 2 / N);//生成全部omega...虽然理论上只用生成N/4但是多生成也没关系罢
		uint32_t revi = bitrev(i, n);
		if (i < revi)
			io[i].xchg(io[revi]);//以及把顺序排好
	}
	///
	uint32_t delta = 1;
	complex_ up, down;

	for (uint32_t i = 0; i < n; )//一层
	{
		while (i < n) {
			for (uint32_t j = 0; j < N; j += delta)
			{
				for (uint32_t k = 0; k < delta; k++)
				{
					up = io[j + delta] * omegaN[N / (2 * delta) * k];
					io[j + delta] = io[j] - up;
					io[j] += up;
					j++;
				}
			}
			delta <<= 1;
			i++;
		}

	}

	
	delete[] omegaN;
	return io;
}
complex_* fft_butterfly_v2(_Inout_ complex_* io, _In_ uint32_t N)
{
	if (N == 0)
		return io;
	//蝴蝶层数
	uint32_t n = 0;
	while (N != 1)
	{
		n++;
		N >>= 1;
	}
	N = 1 << n;
	complex_* omegaN = new complex_[N/2];
	double valuepi = acos(-1.0);
//#define omegaN(i) complex_(valuepi * i * 2 / N)
	for (uint32_t i = 0; i < N/2; i++)
	{
		omegaN[i] = complex_(PI * i * 2 / N);//生成全部omega...实际时只用到了N/2
		uint32_t revi = bitrev(i, n);
		if (i < revi)
			io[i].xchg(io[revi]);//以及把顺序排好
		uint32_t i0 = i + N / 2;

		revi = bitrev(i0, n);
		if (i0 < revi)
			io[i0].xchg(io[revi]);//以及把顺序排好
	}
	///
	uint32_t delta = 1;
	complex_ up, down;

	for (uint32_t i = 0; i < n; )//一层
	{
		/*
		delta = 1;
		//第一层蝴蝶的跨度是1，没有交错，每坨蝴蝶高度为2
		for (uint32_t j = 0; j < N; j += delta*2)
		{
			up =  io[j + delta];
			io[j + delta] = io[j] - up;
			io[j] += up;
		}
		i++;
		if (i >= n)
			break;
		//第二层蝴蝶的跨度是2，每2只蝴蝶交错在一起，每坨蝴蝶高度为4
		delta <<= 1;//delta=2
		for (uint32_t j = 0; j < N; j += delta * 2)
		{
			up = io[j] + io[j + delta];
			down = io[j] - io[j + delta];
			io[j] = up;
			io[j + 2] = down;
			j++;
			complex_ tem = io[j + delta] * complex_(0, 1);//omegaN[N/4]==虚数i==complex_(0, 1)
			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j -= 1;
		}
		i++;
		if (i >= n)
			break;
		//第三层蝴蝶的跨度是4，每4只蝴蝶交错在一起，每坨蝴蝶高度为8
		delta <<= 1;//delta=4
		for (uint32_t j = 0; j < N; j += delta)
		{
			up = io[j] + io[j + delta] * omegaN[0];
			down = io[j] - io[j + delta] * omegaN[0];
			io[j] = up;
			io[j + delta] = down;
			j++;

			complex_ tem = io[j + delta] * omegaN[N / 8];//用临时变量减少运算

			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j++;

			down = io[j + delta] * omegaN[N / 8 * 2];//发现可以节约一个变量

			up = io[j] + down;
			down = io[j] - down;
			io[j] = up;
			io[j + delta] = down;
			j++;

			up = io[j + delta] * omegaN[N / 8 * 3];//可以再节约一次运算

			down = io[j] - up;
			up += io[j];
			io[j] = up;
			io[j + delta] = down;
			j++;//不-3而是+1；所以循环时j少加4，即少加一个delta

			//其实只需要一个变量：down没有储存信息的意义，不过这么写会一脸懵逼还是算了
			//up = io[j + delta] * omegaN[N / 8 * 3];
			//io[j + delta] = io[j] - up;
			//io[j] += up
		}
		i++;
		if (i >= n)
			break;
		*/
		//找到规律了？
		while (i < n) {
			for (uint32_t j = 0; j < N; j += delta)
			{
				for (uint32_t k = 0; k < delta; k++)
				{
					up = io[j + delta] * omegaN[N / (2 * delta) * k];
					io[j + delta] = io[j] - up;
					io[j] += up;
					j++;
				}
			}
			delta <<= 1;
			i++;
		}

	}










	delete[] omegaN;
#ifdef omegaN
#undef omegaN
#endif // omegaN

	return io;
}
complex_* fft_butterfly_v3(_Inout_ complex_* io, _In_ uint32_t N)
{
	if (N == 0)
		return io;
	//蝴蝶层数
	uint32_t n = 0;
	while (N != 1)
	{
		n++;
		N >>= 1;
	}
	N = 1 << n;
	complex_* omegaN = new complex_[N / 2];
	double valuepi = acos(-1.0);
	//#define omegaN(i) complex_(valuepi * i * 2 / N)
	for (uint32_t i = 0; i < N / 2; i++)
	{
		omegaN[i] = complex_(PI * i * 2 / N);//生成全部omega...实际时只用到了N/2
		uint32_t revi = bitrev2(i, n);
		if (i < revi)
			io[i].xchg(io[revi]);//以及把顺序排好
		uint32_t i0 = i + N / 2;
		revi = bitrev2(i0, n);
		if (i0 < revi)
			io[i0].xchg(io[revi]);//以及把顺序排好//
	}
	///
	uint32_t delta = 1;
	complex_ up, down;

	for (uint32_t i = 0; i < n; )//一层
	{
		/*
		delta = 1;
		//第一层蝴蝶的跨度是1，没有交错，每坨蝴蝶高度为2
		for (uint32_t j = 0; j < N; j += delta*2)
		{
			up =  io[j + delta];
			io[j + delta] = io[j] - up;
			io[j] += up;
		}
		i++;
		if (i >= n)
			break;
		//第二层蝴蝶的跨度是2，每2只蝴蝶交错在一起，每坨蝴蝶高度为4
		delta <<= 1;//delta=2
		for (uint32_t j = 0; j < N; j += delta * 2)
		{
			up = io[j] + io[j + delta];
			down = io[j] - io[j + delta];
			io[j] = up;
			io[j + 2] = down;
			j++;
			complex_ tem = io[j + delta] * complex_(0, 1);//omegaN[N/4]==虚数i==complex_(0, 1)
			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j -= 1;
		}
		i++;
		if (i >= n)
			break;
		//第三层蝴蝶的跨度是4，每4只蝴蝶交错在一起，每坨蝴蝶高度为8
		delta <<= 1;//delta=4
		for (uint32_t j = 0; j < N; j += delta)
		{
			up = io[j] + io[j + delta] * omegaN[0];
			down = io[j] - io[j + delta] * omegaN[0];
			io[j] = up;
			io[j + delta] = down;
			j++;

			complex_ tem = io[j + delta] * omegaN[N / 8];//用临时变量减少运算

			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j++;

			down = io[j + delta] * omegaN[N / 8 * 2];//发现可以节约一个变量

			up = io[j] + down;
			down = io[j] - down;
			io[j] = up;
			io[j + delta] = down;
			j++;

			up = io[j + delta] * omegaN[N / 8 * 3];//可以再节约一次运算

			down = io[j] - up;
			up += io[j];
			io[j] = up;
			io[j + delta] = down;
			j++;//不-3而是+1；所以循环时j少加4，即少加一个delta

			//其实只需要一个变量：down没有储存信息的意义，不过这么写会一脸懵逼还是算了
			//up = io[j + delta] * omegaN[N / 8 * 3];
			//io[j + delta] = io[j] - up;
			//io[j] += up
		}
		i++;
		if (i >= n)
			break;
		*/
		//找到规律了？
		while (i < n) {
			for (uint32_t j = 0; j < N; j += delta)
			{
				for (uint32_t k = 0; k < delta; k++)
				{
					up = io[j + delta] * omegaN[N / (2 * delta) * k];
					io[j + delta] = io[j] - up;
					io[j] += up;
					j++;
				}
			}
			delta <<= 1;
			i++;
		}

	}










	delete[] omegaN;
#ifdef omegaN
#undef omegaN
#endif // omegaN

	return io;
}
complex_* fft_butterfly_v4(_Inout_ complex_* io, _In_ uint32_t N)
{
	if (N == 0)
		return io;
	//蝴蝶层数
	uint32_t n = 0;
	while (N != 1)
	{
		n++;
		N >>= 1;
	}
	N = 1 << n;
	complex_* omegaN = new complex_[N / 2];
	double valuepi = acos(-1.0);
	//#define omegaN(i) complex_(valuepi * i * 2 / N)
	for (uint32_t i = 0; i < N / 2; i++)
	{
		omegaN[i] = complex_(PI * i * 2 / N);//生成全部omega...实际时只用到了N/2
		uint32_t revi = bitrev2(i, n);
		if (i < revi)
			io[i].xchg(io[revi]);//以及把顺序排好
		uint32_t i0 = i + N / 2;
		revi |= 1;
		if (i0 < revi)
			io[i0].xchg(io[revi]);//以及把顺序排好//
	}
	///
	uint32_t delta = 1;
	complex_ up, down;

	for (uint32_t i = 0; i < n; )//一层
	{
		/*
		delta = 1;
		//第一层蝴蝶的跨度是1，没有交错，每坨蝴蝶高度为2
		for (uint32_t j = 0; j < N; j += delta*2)
		{
			up =  io[j + delta];
			io[j + delta] = io[j] - up;
			io[j] += up;
		}
		i++;
		if (i >= n)
			break;
		//第二层蝴蝶的跨度是2，每2只蝴蝶交错在一起，每坨蝴蝶高度为4
		delta <<= 1;//delta=2
		for (uint32_t j = 0; j < N; j += delta * 2)
		{
			up = io[j] + io[j + delta];
			down = io[j] - io[j + delta];
			io[j] = up;
			io[j + 2] = down;
			j++;
			complex_ tem = io[j + delta] * complex_(0, 1);//omegaN[N/4]==虚数i==complex_(0, 1)
			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j -= 1;
		}
		i++;
		if (i >= n)
			break;
		//第三层蝴蝶的跨度是4，每4只蝴蝶交错在一起，每坨蝴蝶高度为8
		delta <<= 1;//delta=4
		for (uint32_t j = 0; j < N; j += delta)
		{
			up = io[j] + io[j + delta] * omegaN[0];
			down = io[j] - io[j + delta] * omegaN[0];
			io[j] = up;
			io[j + delta] = down;
			j++;

			complex_ tem = io[j + delta] * omegaN[N / 8];//用临时变量减少运算

			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j++;

			down = io[j + delta] * omegaN[N / 8 * 2];//发现可以节约一个变量

			up = io[j] + down;
			down = io[j] - down;
			io[j] = up;
			io[j + delta] = down;
			j++;

			up = io[j + delta] * omegaN[N / 8 * 3];//可以再节约一次运算

			down = io[j] - up;
			up += io[j];
			io[j] = up;
			io[j + delta] = down;
			j++;//不-3而是+1；所以循环时j少加4，即少加一个delta

			//其实只需要一个变量：down没有储存信息的意义，不过这么写会一脸懵逼还是算了
			//up = io[j + delta] * omegaN[N / 8 * 3];
			//io[j + delta] = io[j] - up;
			//io[j] += up
		}
		i++;
		if (i >= n)
			break;
		*/
		//找到规律了？
		while (i < n) {
			for (uint32_t j = 0; j < N; j += delta)
			{
				for (uint32_t k = 0; k < delta; k++)
				{
					up = io[j + delta] * omegaN[N / (2 * delta) * k];
					io[j + delta] = io[j] - up;
					io[j] += up;
					j++;
				}
			}
			delta <<= 1;
			i++;
		}

	}










	delete[] omegaN;
#ifdef omegaN
#undef omegaN
#endif // omegaN

	return io;
}
complex_* fft_butterfly_v5(_Inout_ complex_* io, _In_ uint32_t N)
{
	if (N == 0)
		return io;
	//蝴蝶层数
	uint32_t n = 0;
	while (N != 1)
	{
		n++;
		N >>= 1;
	}
	N = 1 << n;
	complex_* omegaN = new complex_[N / 2];
	double valuepi = acos(-1.0);
	//#define omegaN(i) complex_(valuepi * i * 2 / N)
	for (uint32_t i = 0; i < N / 2; i++)
	{
		omegaN[i] = complex_(valuepi * i * 2 / N);//生成全部omega...实际时只用到了N/2
		uint32_t revi = bitrev2(i, n);
		if (i < revi)
			io[i].xchg(io[revi]);//以及把顺序排好
		uint32_t i0 = i + N / 2;
		revi |= 1;
		if (i0 < revi)
			io[i0].xchg(io[revi]);//以及把顺序排好//
	}
	///
	uint32_t delta = 1;
	complex_ up, down;

	for (uint32_t i = 0; i < n; )//一层
	{
		/*
		delta = 1;
		//第一层蝴蝶的跨度是1，没有交错，每坨蝴蝶高度为2
		for (uint32_t j = 0; j < N; j += delta*2)
		{
			up =  io[j + delta];
			io[j + delta] = io[j] - up;
			io[j] += up;
		}
		i++;
		if (i >= n)
			break;
		//第二层蝴蝶的跨度是2，每2只蝴蝶交错在一起，每坨蝴蝶高度为4
		delta <<= 1;//delta=2
		for (uint32_t j = 0; j < N; j += delta * 2)
		{
			up = io[j] + io[j + delta];
			down = io[j] - io[j + delta];
			io[j] = up;
			io[j + 2] = down;
			j++;
			complex_ tem = io[j + delta] * complex_(0, 1);//omegaN[N/4]==虚数i==complex_(0, 1)
			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j -= 1;
		}
		i++;
		if (i >= n)
			break;
		//第三层蝴蝶的跨度是4，每4只蝴蝶交错在一起，每坨蝴蝶高度为8
		delta <<= 1;//delta=4
		for (uint32_t j = 0; j < N; j += delta)
		{
			up = io[j] + io[j + delta] * omegaN[0];
			down = io[j] - io[j + delta] * omegaN[0];
			io[j] = up;
			io[j + delta] = down;
			j++;

			complex_ tem = io[j + delta] * omegaN[N / 8];//用临时变量减少运算

			up = io[j] + tem;
			down = io[j] - tem;
			io[j] = up;
			io[j + delta] = down;
			j++;

			down = io[j + delta] * omegaN[N / 8 * 2];//发现可以节约一个变量

			up = io[j] + down;
			down = io[j] - down;
			io[j] = up;
			io[j + delta] = down;
			j++;

			up = io[j + delta] * omegaN[N / 8 * 3];//可以再节约一次运算

			down = io[j] - up;
			up += io[j];
			io[j] = up;
			io[j + delta] = down;
			j++;//不-3而是+1；所以循环时j少加4，即少加一个delta

			//其实只需要一个变量：down没有储存信息的意义，不过这么写会一脸懵逼还是算了
			//up = io[j + delta] * omegaN[N / 8 * 3];
			//io[j + delta] = io[j] - up;
			//io[j] += up
		}
		i++;
		if (i >= n)
			break;
		*/
		//找到规律了？
		while (i < n) {
#pragma loop(hint_parallel (8))
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
		}

	}










	delete[] omegaN;
#ifdef omegaN
#undef omegaN
#endif // omegaN

	return io;
}


#ifdef _INCLUDED_IMM
//include this to enable these function
//#include<immintrin.h>

int16_t rand16s(void)
{
	int16_t r;
	_rdseed16_step((uint16_t*)&r);
	return r;
}

int16_t rand16(void)
{
	int16_t r;
	if (0 == _rdrand16_step((uint16_t*)&r)) {
		_rdrand16_step((uint16_t*)&r);
	}
	return r;
}
int32_t rand32s(void)
{
	int32_t r;
	_rdseed32_step((uint32_t*)&r);
	return r;
}
int32_t rand32(void)
{
	int32_t r;
	_rdrand32_step((uint32_t*)&r);
	return r;
}
#if defined(_M_X64)
int64_t rand64s(void)
{
	int64_t r;
	_rdseed64_step((uint64_t*)&r);
	return r;
}
int64_t rand64(void)
{
	int64_t r;
	_rdrand64_step((uint64_t*)&r);
	return r;
}
#endif
#endif // _INCLUDED_IMM
//FFT窗
class FFT_sys
{
private:
	uint8_t n = 0;
	uint32_t N = 0;
	complex_* omegaN = 0;
	uint32_t* revi = 0;
	double valuepi = acos(-1.0);
	complex_* inbuffer[2] = { 0 };
	complex_* outbuffer[2] = { 0 };
/// <summary>
/// case N=4
/// idx 		     0 1 2 3 0 1 2 3
/// value			 a b c d e f g h
/// block1		|0 0 a b|c d e f|g h
/// block2		    |a b c d|e f g h|
/// 对N点fft，中间N/4点按原样取，两侧各N/4点线性加权
/// 因此，N不应少于8，额，理论上也许是这样
/// </summary>
public:
	uint32_t idx = 0;
	double* factors = 0;
public:
	//输出延迟N，每N/2个点进行一次FFT
	double inline io(const double& input)
	{
		inbuffer[0][idx ^ (N >> 1)] = input;
		inbuffer[1][idx] = input;
		double o0 = linear_mix();
		uint32_t lastidx = idx;
		idx++;

		if (idx == N / 2)
		{
			for (uint32_t i = 0; i < N; i++)
			{
				outbuffer[0][i] = inbuffer[0][i];
			}
			fft_ifft(outbuffer[0]);
		}
		else if (idx >= N)
		{
			idx = 0;
			for (uint32_t i = 0; i < N; i++)
			{
				outbuffer[1][i] = inbuffer[1][i];
			}
			fft_ifft(outbuffer[1]);
		}
		complex_ ret = outbuffer[1][lastidx] * o0 + outbuffer[0][lastidx ^ (N >> 1)] * (1.0-o0);
		return ret.r;
	}
	uint32_t inline getN()
	{
		return N;
	}
	void set_low_pass(double normalized_freq)
	{
		int min = N * normalized_freq / 2, max = N - 1 - min;
		for (int i = 0; i < N; i++)
		{
			if (i > min && i < max)
				factors[i] = 0;
			else
			{
				factors[i] = 1;
			}
		}
	}
	void set_gain(double gain)
	{
		for (int i = 0; i < N; i++)
		{
			factors[i] *= gain;			
		}
		return;
	}

	/// <summary>
	/// [查看字符画]
	///		   __ 
	///		  /  \
	///		_/    \_
    ///
	/// 归一化的线性混合：
	/// f(x)∈[0,1],x∈[0,1)
	/// 拐点在x=1/8、3/8、5/8、7/8处
	/// </summary>
	/// <returns></returns>
	double linear_mix()
	{
		double o0 = idx;
		o0 += 0.5;
		o0 /= N;
		if (o0 <= 0.125 || o0 >= 0.875)
		{
			o0 = 0;
		}
		else if (o0 >= 0.375 && o0 <= 0.625)
		{
			o0 = 1;
		}
		else if (o0 < 0.375)
		{
			o0 -= 0.125;
			o0 *= 4;
		}
		else
		{
			o0 = 0.875 - o0;
			o0 *= 4;
		}

		return o0;

	}
	complex_* fft_ifft(_Inout_ complex_* io)
	{
		uint32_t delta;
		complex_ up;

		//时域到频域
		for (uint32_t i = 0; i < N; i++)
		{
			if (i < revi[i])
				io[i].xchg(io[revi[i]]);//以及交换二者，把顺序排好
		}
		delta = 1;
		for (uint32_t i = 0; i < n; i++)//一层
		{
			for (uint32_t j = 0; j < N; j += delta)
			{
				for (uint32_t k = 0; k < delta; k++)
				{
					up = io[j + delta] * omegaN[N / (2 * delta) * k];
					io[j + delta] = io[j] - up;
					io[j] += up;
					j++;
				}
			}
			delta <<= 1;
		}

		//频域处理
		for (uint32_t i = 0; i < N; i++)
		{
			io[i] *= factors[i];
		}

		//频域到时域
		for (uint32_t i = 0; i < N; i++)
		{
			if (i < revi[i])
				io[i].xchg(io[revi[i]]);//以及交换二者，把顺序排好
		}
		delta = 1;
		for (uint32_t i = 0; i < n; i++)//一层
		{
			for (uint32_t j = 0; j < N; j += delta)
			{
				for (uint32_t k = 0; k < delta; k++)
				{
					up = io[j + delta] * omegaN[N / (2 * delta) * k].conj();
					io[j + delta] = io[j] - up;
					io[j] += up;
					j++;
				}
			}
			delta <<= 1;
		}

		//归一化处理
		for (uint32_t i = 0; i < N; i++)
		{
			io[i] /= (double)N;
		}

		return io;
	}


	//2的N次方
	FFT_sys(size_t N_points)
	{
		
		n = 31 - __lzcnt(N_points);
		N = 1 << n;
		omegaN = new complex_[N];
		revi = new uint32_t[N];
		factors = new double[N] ;
		inbuffer[0] = new complex_[N] ;
		inbuffer[1] = new complex_[N] ;
		outbuffer[0] = new complex_[N];
		outbuffer[1] = new complex_[N];

		for (uint32_t i = 0; i < N; i++)
		{
			omegaN[i] = complex_(PI * i * 2 / N);
			revi[i] = bitrev(i, n);
			factors[i] = 1;
			inbuffer[0][i] = complex_(0.0, 0.0);
			inbuffer[1][i] = complex_(0.0, 0.0);
			outbuffer[0][i] = complex_(0.0, 0.0);
			outbuffer[1][i] = complex_(0.0, 0.0);
		}
	}
	~FFT_sys()
	{
		if (omegaN != 0)
		{
			delete[] omegaN;
		}
		if (revi != 0)
		{
			delete[] revi;
		}
		if (factors != 0)
		{
			delete[] factors;
		}
		if (inbuffer[0] != 0)
		{
			delete[] inbuffer[0];
		}
		if (inbuffer[1] != 0)
		{
			delete[] inbuffer[1];
		}
		if (outbuffer[0] != 0)
		{
			delete[] outbuffer[0];
		}
		if (outbuffer[1] != 0)
		{
			delete[] outbuffer[1];
		}

	}
};


#endif // !_bionukg_signal_h_
