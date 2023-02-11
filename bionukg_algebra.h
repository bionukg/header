#pragma once


#ifndef bionukg_algebra_h
#include<intrin.h>



#define bionukg_algebra_h (0x00)

template <class unif>
class int_inf;



class DECchar
{	
public:
	char n;
	DECchar() { n = '0'; }
	template<typename INType>
	DECchar(const INType& that) { n = '0' + that; }


	template<typename INType>
	void operator=(const INType& that)
	{
		n = that + '0';
		return;
	}
	void operator=(const char& that)
	{
		n = that;
	}
	void operator=(const DECchar& i)
	{
		this->n = i.n;
	}
	bool operator!=(DECchar i)
	{
		return this->n != i .n;
	}
	bool operator==(DECchar i)
	{
		return this->n == i.n;
	}
	void print(DECchar c)
	{
		printf("%c", c.n);
	}
	uint8_t inline __cdecl addcarry(uint8_t c, DECchar a, DECchar b, DECchar* r)
	{
		r->n = c + a.n + b.n - '0';
		if (r->n > '9')
		{
			r->n -= 10;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	uint8_t inline __cdecl subborrow(uint8_t c, DECchar a, DECchar b, DECchar* r)
	{
		r->n = a.n - c;
		if (r->n >= b.n)
		{
			r->n = r->n - b.n + '0';
			return 0;
		}
		else
		{
			r->n = r->n - b.n + '0' + 10;
			return 1;
		}
	}
	DECchar inline __cdecl mulx(DECchar src0, DECchar src1, DECchar* highbits)
	{
		uint8_t x = (src0.n - '0') * (src1.n - '0');
		highbits->n = x / 10 + '0';
		DECchar r;
		r.n = x % 10 + '0';
		return r;
	}

};
template <int scale>
class N_nary
{
public:
	//仅用于2进制到10进制
	static_assert(2 <= scale && scale <= 10, "N_nary<n>: n should be a number between 2 and 10.");
	char n;
	N_nary() { n = '0'; }
	template<typename INType>
	N_nary(const INType& that) { n = '0' + that; }


	template<typename INType>
	void operator=(const INType& that)
	{
		n = that + '0';
		return;
	}
	void operator=(const char& that)
	{
		n = that;
	}
	void operator=(const N_nary& i)
	{
		this->n = i.n;
	}
	bool operator!=(N_nary i)
	{
		return this->n != i.n;
	}
	bool operator==(N_nary i)
	{
		return this->n == i.n;
	}
	void print(N_nary c)
	{
		printf("%c", c.n);
	}
	uint8_t inline __cdecl addcarry(uint8_t c, N_nary a, N_nary b, N_nary* r)
	{
		r->n = c + a.n + b.n - '0';
		if (r->n >= '0' + scale)
		{
			r->n -= scale;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	uint8_t inline __cdecl subborrow(uint8_t c, N_nary a, N_nary b, N_nary* r)
	{
		r->n = a.n - c;
		if (r->n >= b.n)
		{
			r->n = r->n - b.n + '0';
			return 0;
		}
		else
		{
			r->n = r->n - b.n + '0' + scale;
			return 1;
		}
	}
	N_nary inline __cdecl mulx(N_nary src0, N_nary src1, N_nary* highbits)
	{
		uint8_t x = (src0.n - '0') * (src1.n - '0');
		highbits->n = x / scale + '0';
		N_nary r;
		r.n = x % scale + '0';
		return r;
	}

};



template <class unif>
class virtual_array
{

public:
	unif* ptr;
	size_t len;
	virtual_array(unif* p, size_t range)
	{
		ptr = p;
		len = range;
	}
	unif operator[](size_t idx)const
	{
		return idx < len ? ptr[idx] : unif(0);
	}
	virtual_array() { ptr = 0; len = 0; };
private:

};


template <class unif>
class int_inf_p :public int_inf<unif>
{
public:
	virtual_array<unif> nums;
	int_inf_p(const int_inf<unif>* another)
	{
		this->is_negative = another->get_is_negative();
		this->length = another->get_length();
		this->used = another->get_used();
		this->nums = virtual_array<unif>(another->get_nums(), another->get_length());
	}
	int_inf_p() {};
};





template <class unif>
class int_inf
{
#define int_inf_stacklength (16/sizeof(unif))
#ifdef _M_X64
	static_assert(int_inf_stacklength * sizeof(unif) > sizeof(uint64_t), "default size should be longer.use // to disable warning if you are sure");
#else
	static_assert(int_inf_stacklength * sizeof(unif) > sizeof(uint32_t), "default size should be longer.use // to disable warning if you are sure");

#endif // _M_X64

protected:
	size_t length = 0;
	size_t used = 0;
	bool is_negative = false;
	unif* nums = stacks;//nums[0]是最低的位
	unif stacks[int_inf_stacklength] = { 0 };
	void stdnew()
	{
		nums = stacks;
		length = int_inf_stacklength;
		used = 0;
	}
public:
	void refresh_used()
	{
		for (size_t i = length; i != 0; i--)
		{
			if (nums[i - 1] != unif(0))
			{
				used = i;
				return;
			}
		}
		used = 0;
		return;
	}
	void refresh_used_down(size_t u = 0)
	{
		if (u > used)
			used = u;
		for (size_t i = used; i != 0; i--)
		{
			if (nums[i - 1] != unif(0))
			{
				used = i;
				return;
			}
		}
		used = 0;
		return;
	}

	void release_space()
	{
		refresh_used();
		
	}
	int_inf(int_inf& another)
	{
		is_negative = another.is_negative;
		length = another.length;
		used = another.used;
		nums = new unif[length]{};
		memcpy(nums, another.nums, length);
	}
	int_inf()
	{
		stdnew();
	}
	size_t get_used(void) const
	{ 
		return used;
	}
	unif* get_nums(void) const{ return nums; }
	bool get_is_negative(void)const { return is_negative; }
	size_t get_length(void) const { return length; }
	void try_extend(size_t ext)
	{
		if (this->length <= ext) { this->resize(ext); }
		return;
	}

	template <class INType>
	int_inf(const INType& num)
	{
		stdnew();
		*this = num;
	}
	int_inf& resize(size_t length)
	{
		if (length > int_inf_stacklength) {
			unif* temp = new unif[length];
			memcpy(temp, nums, length * sizeof(unif));
			if (nums != stacks)
				delete[] nums;
			nums = temp;
			this->length = length;
			if (used > length)
				used = length;
		}
		else if (nums != stacks)
		{
			memcpy(stacks, nums, length * sizeof(unif));
			this->length = int_inf_stacklength;
			delete[] nums;
			nums = stacks;
		}
		else
		{
			if (used > length) {
				used = length;
				for (size_t i = used; i < int_inf_stacklength; i++)
					stacks[i] = 0;
			}
		}
		return *this;
	}
	~int_inf()
	{
		if (nums != 0 && nums != stacks)
			delete[] nums;
	}

	//template <typename aunif>
	//int_inf<aunif>& dynamic_cast_as(int_inf<aunif>* typeptr)
	//{//不能仅按返回值重载...模板大概也一样
	//	int_inf<aunif>* ret = (int_inf<aunif>*)this;
	//突然发现会搞乱stacks，所以算了
	//	return *ret;
	//}
	uint8_t castu8()
	{
		switch (sizeof(unif) * used)
		{
		case 0:
			return 0;
		default:
			return (*(uint8_t*)nums);
		}
	}
	uint16_t castu16()
	{
		switch (sizeof(unif) * used)
		{
		case 0:
			return 0;
		case 1:
			return (*(uint8_t*)nums);
		default:
			return (*(uint16_t*)nums);
		}
	}
	uint32_t castu32()
	{
		switch (sizeof(unif) * used)
		{
		case 0:
			return 0;
		case 1:
			return (*(uint8_t*)nums);
		case 2:
			return (*(uint16_t*)nums);
		case 3:
			return (*(uint32_t*)nums) & 0x00ffffff;
		default:
			return (*(uint32_t*)nums);
			break;
		}
	}
	operator uint32_t()
	{
		return*(uint32_t*)nums;
	}
	operator int32_t()
	{
		return is_negative ? (-(*(int32_t*)nums)) : *(int32_t*)nums;
	}
#ifdef _M_X64
	operator uint64_t()
	{
		return*(uint64_t*)nums;
	}
	operator int64_t()
	{
		return is_negative ? (-(*(int64_t*)nums)) : *(int64_t*)nums;
	}
	uint64_t castu64()
	{
		uint64_t ret = (*(uint64_t*)nums), mask = ~0;
		if (used * sizeof(unif) < 8)
		{
			mask >>= (8 - used*sizeof(unif))*8;
		}
		return ret & mask;
	}

#endif
	
	void superadd(unif c)//在末尾（最高位）追加一个单元
	{
		if (this->length <= this->used + 4)
		{
			this->resize(this->used + 4);
		}
		this->nums[this->used] = c;
		this->used++;
	}
	void operator=(char str[])
	{
		this->operator=((const char*)str);
	}
	void operator=(const char* str)
	{
		size_t i = 0;
		while (str[i] != 0) { i++; }
		this->used = i;		
		this->resize(i + 4);
		if (i == 0)
			return;
		i--;
		for (size_t x = 0;;)
		{
			nums[x] = str[i]; 
			if (i == 0)
				return;
			i--, x++;
		}
	}
	template <typename aunif>
	void operator=(const int_inf<aunif>& num)
	{
		this->is_negative = num.get_is_negative();
		if (sizeof(aunif) >= sizeof(unif))
		{
			auto m = sizeof(aunif) / sizeof(unif);
			length = num.get_length() * m;
			used = num.get_used() * m;
			refresh_used_down();
		}
		else
		{
			auto m = sizeof(unif) / sizeof(aunif);
			length = num.get_length() / m + (num.get_length() % m == 0 ? 0 : 1);
			used = num.get_used() / m + (num.get_used() % m == 0 ? 0 : 1);
		}
		if (nums != stacks)
			delete[] nums;
		if (length <= int_inf_stacklength)
		{
			nums = stacks;
			length = int_inf_stacklength;
		}
		else
			nums = new unif[length]{};
		memmove(nums, num.get_nums(), num.get_length() * sizeof(aunif));
	}
	template<typename INType>
	void operator=(const INType& num)
	{
		static_assert(sizeof(unif) * int_inf_stacklength >= sizeof(INType), "set greater int_inf_stacksize");
		setzero();
		memmove(&this->stacks[0], &num, sizeof(INType));
		if (this->nums != this->stacks)
		{
			delete[] this->nums;
			this->nums = this->stacks;
		}
		is_negative = false;
		used = sizeof(INType)/ sizeof(unif);
		if (used == 0)
			used = 1;
		refresh_used_down();
	}
	int_inf& negative()
	{
		is_negative = !is_negative;
		return *this;
	}
	int_inf& negative(int_inf& num)
	{
		num.is_negative = !num.is_negative;
		return num;
	}
	bool operator_larger_abs(const int_inf& num)
	{
		if (used > num.used)
			return true;
		else if (used < num.used)
			return false;
		return this->nums[used - 1] > num.nums[used - 1] ? true : false;
	}
	void setzero()
	{
		for (size_t i = 0; i < length; i++)
		{
			this->nums[i] = 0;
		}
		return;
	}
private:
	//sbb重载
	unsigned char inline __cdecl subborrow(uint8_t c, uint8_t a, uint8_t b, uint8_t* r)
		{
			return _subborrow_u8(c, a, b, r);
		}
	unsigned char inline __cdecl subborrow(uint8_t c, uint16_t a, uint16_t b, uint16_t* r)
		{
			return _subborrow_u16(c, a, b, r);
		}
	unsigned char inline __cdecl subborrow(uint8_t c, uint32_t a, uint32_t b, uint32_t* r)
		{
			return _subborrow_u32(c, a, b, r);
		}
#if defined(_M_X64)
	unsigned char inline __cdecl subborrow(uint8_t c, uint64_t a, uint64_t b, uint64_t* r)
		{
			return _subborrow_u64(c, a, b, r);
		}
#endif // _M_X64
	template<class subT>
	uint8_t inline __cdecl subborrow(uint8_t c, subT a, subT b, subT* r)
	{
		return a.subborrow(c, a, b, r);
	}
	//adc重载
	unsigned char inline __cdecl addcarry(uint8_t c, uint8_t a, uint8_t b, uint8_t* r)
		{
			return _addcarry_u8(c, a, b, r);
		}
	unsigned char inline __cdecl addcarry(uint8_t c, uint16_t a, uint16_t b, uint16_t* r)
		{
			return _addcarry_u16(c, a, b, r);
		}
	unsigned char inline __cdecl addcarry(uint8_t c, uint32_t a, uint32_t b, uint32_t* r)
		{
			return _addcarry_u32(c, a, b, r);
		}
#if defined(_M_X64)
	unsigned char inline __cdecl addcarry(uint8_t c, uint64_t a, uint64_t b, uint64_t* r)
		{
			return _addcarry_u64(c, a, b, r);
		}
#endif // _M_X64
	template<class subT>
	uint8_t inline __cdecl addcarry(uint8_t c, subT a, subT b, subT* r)
		{
			return a.addcarry(c, a, b, r);
		}

public:
	inline int_inf& add_abs(const int_inf& that)
	{
		size_t maxused = this->used >= that.used ? this->used : that.used;
		size_t thatused = that.used;
		int_inf_p<unif> thatp = int_inf_p<unif>(&that);
		if (this->length <= maxused + 4)
		{
			this->resize(maxused + 4);
		}
		uint8_t carry = 0;
		for (size_t i = 0; i < thatused||carry!=0; i++)
		{
			carry = addcarry(carry, this->nums[i], thatp.nums[i], &this->nums[i]);
		}
		refresh_used_down(maxused+2);
		return *this;
	}
	inline int_inf& add_abs_shl(const int_inf& that, size_t offset)
	{
		size_t maxused = this->used >= that.used+offset ? this->used : that.used+offset;
		size_t thatused = that.used + offset;
		int_inf_p<unif> thatp = int_inf_p<unif>(&that);
		if (this->length <= maxused + 4)
		{
			this->resize(maxused + 4);
		}
		uint8_t carry = 0;
		for (size_t i = 0; i < thatused || carry != 0; i++)
		{
			carry = addcarry(carry, this->nums[i], thatp.nums[i-offset], &this->nums[i]);
		}
		refresh_used_down(maxused+2);
		return *this;
	}

	int_inf& add(const int_inf& that, const int_inf& another);
	inline int_inf& addu32(const int_inf& num);
	inline int_inf& addu64(const int_inf& num);
	int_inf& sub(const int_inf& num);


public:
	inline int_inf& sub_abs(const int_inf& that)//this abs should be bigger than that
	{
		size_t maxused = this->used;
		size_t thatused = that.used;
		int_inf_p<unif> thatp = int_inf_p<unif>(&that);
		if (this->length <= maxused + 4)
		{
			this->resize(maxused + 4);
		}
		uint8_t borrow = 0;
		for (size_t i = 0; i < thatused || borrow != 0; i++)
		{
			borrow = subborrow(borrow, this->nums[i], thatp.nums[i], &this->nums[i]);
		}

		refresh_used_down();
		return *this;
	}
	inline int_inf& sub_rev_abs(const int_inf& that)
	{	//that abs should be bigger than this, otherwise UB	
		auto thatused = that.get_used();
		
		this->resize(thatused);	
		uint8_t borrow = 0;
		for (size_t i = 0; i < thatused || borrow != 0; i++)
		{
			borrow = subborrow(borrow, that.nums[i], this->nums[i], &this->nums[i]);
		}
		this->used = that.get_used();
		refresh_used_down();
		return *this;
	}
	inline int_inf& and_abs(const int_inf& that)
	{
		for (size_t i = 0; i < this.used && i < that.used; i++)
		{
			this->nums[i] &= that.nums[i];
		}
		refresh_used_down();
		return *this;
	}
	inline int_inf& orr_abs(const int_inf& that)
	{
		for (size_t i = 0; i < this.used && i < that.used; i++)
		{
			this->nums[i] |= that.nums[i];
		}
		refresh_used_down();
		return *this;
	}
	inline int_inf& xor_abs(const int_inf& that)
	{
		for (size_t i = 0; i < this.used && i < that.used; i++)
		{
			this->nums[i] ^= that.nums[i];
		}
		refresh_used_down();
		return *this;
	}
	inline int_inf& not_abs()
	{
		for (size_t i = 0; i < this.used ; i++)
		{
			this->nums[i] = ~this->nums[i];
		}
		refresh_used_down();
		return *this;
	}	
private:
	//mulx重载
	uint8_t  mulx(uint8_t src0, uint8_t src1, uint8_t* highbits)
	{

		uint16_t r = (uint16_t)src0 * src1;
		*highbits = (r>>8);
		return r;
	}
	uint16_t  mulx(uint16_t src0, uint16_t src1, uint16_t* highbits)
	{

		uint32_t r = (uint32_t)src0 * src1;
		*highbits = (r >> 16);
		return r;
	}
	uint32_t  mulx(uint32_t src0, uint32_t src1, uint32_t* highbits)
	{
		return _mulx_u32(src0, src1, highbits);
	}
#if defined(_M_X64)
	uint64_t  mulx(uint64_t src0, uint64_t src1, uint64_t* highbits)
	{
		return _mulx_u64(src0, src1, highbits);
	}

	template<class mulT>
	mulT mulx(mulT src0, mulT src1, mulT* highbits)
	{
		return src0.mulx(src0, src1, highbits);
	}

#endif
public:
	inline int_inf& mul_abs_unit(const int_inf& that,unif m2)
	{
		size_t maxused = that.used + 1;		
		if (this->length <= maxused + 4)
		{
			this->resize(maxused + 4);
		}
		setzero();
		unif tl, th = 0, t;
		uint8_t carry = 0;
		for (size_t i = 0; i < that.used; i++)
		{
			t = th;
			tl = mulx(that.nums[i], m2, &th);
			carry = addcarry(carry, t, tl, &this->nums[i]);			
		}
		addcarry(carry, th, unif(0), &this->nums[that.used]);
		this->used = that.used+1;
		this->refresh_used_down();
		return *this;
	}
	int_inf& mul_abs(const int_inf& that, const int_inf& another)
	{
		this->resize(that.used + another.used).setzero();
		int_inf step;
		for (size_t i = 0; i < another.used; i++)
		{
			step.mul_abs_unit(that, another.nums[i]);
			this->add_abs_shl(step,i);
		}
		return *this;
	}
	void print(uint8_t c)
	{
		printf("%02X", c);
	}
	void print(uint16_t c)
	{
		printf("%04X", c);
	}
	void print(uint32_t c)
	{
		printf("%08X", c);
	}
#if defined(_M_X64)
	void print(uint64_t c)
	{
		printf("%016X", c);
	}
	template<typename printT>
	void print(printT c)
	{
		c.print(c);
	}

#endif
	void show(char separator=' ')
	{
		if (used == 0)
		{
			printf("0 ");
			return;
		}
		size_t i = used - 1;
		for (;;)
		{
			print(nums[i]);
			if (separator != 0)
				printf("%c", separator);
			if (i == 0)
				break;
			i--;
		}
		printf("\n");
		return;
	};


};


//略微快速的乘法。真的有用吗？
template<typename INType>
void inline product(const INType& x, const INType& y, INType xy[2])
{
	//(ab)*(cd)==(ac)<<2+(bd)+[(a+b)(c+d)-(ac)-(bd)]<<1
	INType halflen = (sizeof(INType) * 4);
	INType mask = (1 << halflen) - 1;
	INType ly = y & mask, hy = (y >> halflen) & mask;//a/b
	INType lx = x & mask, hx = (x >> halflen) & mask;//c/d
	INType hproduct = hy * hx, lproduct = ly * lx;//ac/bd
	INType sumy = ly + hy, sumx = lx + hx;//a+b/c+d
	INType midproduct = (INType)((sumy & mask) * (sumx & mask));
	INType productsub = hproduct + lproduct;//(a+b)(c+d)-(ac)-(bd),overflow exception
	INType temproduct = midproduct - productsub;
	//overflow handle
	INType mproduct = (temproduct >> halflen) & mask;

	switch ((sumx >> halflen << 1) | (sumy >> halflen))
	{
	case 0b01:
		mproduct += sumx;
		break;
	case 0b10:
		mproduct += sumy;
		break;
	case 0b11:
		mproduct += sumx + sumy - (1 << halflen);
		break;
	default:
		break;
	}
	mproduct -= (midproduct < productsub || productsub < lproduct ? (1 << halflen) : 0);
	xy[0] = lproduct + ((temproduct & mask) << halflen);
	xy[1] = hproduct + mproduct + (xy[0] < lproduct ? 1 : 0);
	return;
}
#if !defined(_M_IX86) && !defined(_M_X64)
#error These functions might be specific to X86 and X64 targets
#endif
//半自动宏模板 	示例：MACRO_int_inf_add(64, n2, n3);
//是我草率了。我忘了函数可以重载。详见addcarry
//这玩意留着做宏用法的参考吧，虽然删了也没事。
#define MACRO_int_inf_add(bits,this,that) do{\
	static_assert(sizeof(*(this.get_nums())) == sizeof(__int##bits), "size error: sizeof(unif)!=sizeof(unsigned __int"#bits")");\
	auto maxused = this.get_used() > that.get_used() ? this.get_used() : that.get_used();\
	this.try_extend(maxused + 4);\
	that.try_extend(maxused + 4);\
	auto i=maxused;\
	unsigned char carry = 0;\
	auto thisnums = this.get_nums();\
	auto thatnums = that.get_nums();\
	for (i = 0; i < maxused; i++){carry = _addcarry_u##bits(carry, thisnums[i], thatnums[i], &thisnums[i]);}\
	if (carry != 0){thisnums[maxused] = 1;};\
	that.refresh_used();\
	}while(false)

#endif 







