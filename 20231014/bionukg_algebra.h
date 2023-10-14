#pragma once

#ifndef bionukg_algebra_h
#include <stdint.h>
#include <intrin.h>
#include <type_traits>
#include <vector>
// #include"./bionukg_datastruct.h"

#define bionukg_algebra_h (0x00)

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	template <class unif>
	class int_inf;

	class DECchar
	{
	public:
		char n;
		DECchar() { n = '0'; }
		template <typename INType>
		DECchar(const INType &that) { n = '0' + that; }

		template <typename INType>
		void operator=(const INType &that)
		{
			n = that + '0';
			return;
		}
		void operator=(const char &that)
		{
			n = that;
		}
		void operator=(const DECchar &i)
		{
			this->n = i.n;
		}
		bool operator!=(DECchar i)
		{
			return this->n != i.n;
		}
		bool operator==(DECchar i)
		{
			return this->n == i.n;
		}
		void print(DECchar c)
		{
			printf("%c", c.n);
		}
		static uint8_t inline __cdecl addcarry(uint8_t c, DECchar a, DECchar b, DECchar *r)
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
		static uint8_t inline __cdecl subborrow(uint8_t c, DECchar a, DECchar b, DECchar *r)
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
		static DECchar inline __cdecl mulx(const DECchar &mul0, const DECchar &mul1, DECchar *highres)
		{
			uint8_t x = (mul0.n - '0') * (mul1.n - '0');
			highres->n = x / 10 + '0';
			DECchar r;
			r.n = x % 10 + '0';
			return r;
		}
	};

	template <int scale>
	class N_nary
	{
	public:
		// 仅用于2进制到10进制
		static_assert(2 <= scale && scale <= 10, "N_nary<n>: n should be a number between 2 and 10.");
		char n;
		N_nary() { n = '0'; }

		// N_nary(int that) { n = '0' + scale - 1; }
		N_nary(const N_nary &that)
		{
			this->n = that.n;
		}

		N_nary(const int &that) { n = '0' + (char)that; }

		template <typename INType>
		void operator=(const INType &that)
		{
			n = that + '0';
			return;
		}
		void operator=(const char &that)
		{
			n = that;
		}
		void operator=(const N_nary &i)
		{
			this->n = i.n;
		}
		bool operator!=(const N_nary &i) const
		{
			return this->n != i.n;
		}
		bool operator==(const N_nary &i) const
		{
			return this->n == i.n;
		}
		bool operator<(const N_nary &i) const
		{
			return this->n < i.n;
		}
		operator int()
		{
			return n - '0';
		}
		void print(N_nary c)
		{
			printf("%c", c.n);
		}
		static uint8_t inline __cdecl addcarry(uint8_t c, N_nary a, N_nary b, N_nary *r)
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
		static uint8_t inline __cdecl subborrow(uint8_t c, N_nary a, N_nary b, N_nary *r)
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
		static N_nary inline __cdecl mulx(N_nary src0, N_nary src1, N_nary *highbits)
		{
			uint8_t x = (src0.n - '0') * (src1.n - '0');
			highbits->n = x / scale + '0';
			N_nary r;
			r.n = x % scale + '0';
			return r;
		}

		/// @brief get the half of the number
		/// @param high high part of the number
		/// @param low  low part of the number
		/// @return if the number is odd
		static bool half(N_nary &high, N_nary &low)
		{
			bool ret = false;
			high.n -= '0';
			ret = (high.n % 2 != 0);
			high.n = high.n / 2 + '0';

			low.n = low.n - '0' + (ret ? scale : 0);
			ret = (low.n % 2 != 0);
			low.n = low.n / 2 + '0';
			return ret;
		}
		bool average(const N_nary &another)
		{
			n = n - '0' + another - '0';
			bool ret = n % 2;
			n = n / 2 + '0';
			return ret;
		}
		/// <summary>
		/// need: operator&lt;,add,sub,mul,half
		/// </summary>
		/// <returns></returns>
		static N_nary inline __cdecl udiv(const N_nary& HighDiv, const N_nary& LowDiv,
			const N_nary& Divisor, N_nary* Remain)
		{
			int div = (HighDiv.n - '0') * scale + LowDiv.n - '0';
			*Remain = N_nary(div % (Divisor.n - '0'));
			return N_nary(div / (Divisor.n - '0'));
		};
	};
	// friend operator<< for std::ostream of N_nary
	template <int T>
	std::ostream& operator<<(std::ostream& os, const N_nary<T>& n)
	{
		os << n.n;
		return os;
	};
	template <class unif>
	class virtual_array
	{

	public:
		unif *ptr;
		size_t len;
		virtual_array(unif *p, size_t range)
		{
			ptr = p;
			len = range;
		}
		unif operator[](size_t idx) const
		{
			return idx < len ? ptr[idx] : unif(0);
		}
		virtual_array()
		{
			ptr = 0;
			len = 0;
		};

	private:
	};

	template <class unif>
	class int_inf_p : public int_inf<unif>
	{
	public:
		virtual_array<unif> nums;
		int_inf_p(const int_inf<unif> *another)
		{
			this->is_negative = another->get_is_negative();
			this->length = another->get_length();
			this->used = another->get_used();
			this->nums = virtual_array<unif>(another->get_nums(), another->get_length());
		}
		int_inf_p(){};
	};

	template <class unif>
	class int_inf
	{
#define int_inf_stacklength (16 / sizeof(unif))
#ifdef _M_X64
		static_assert(int_inf_stacklength * sizeof(unif) > sizeof(uint64_t), "default size should be longer.use // to disable warning if you are sure");
#else
	static_assert(int_inf_stacklength * sizeof(unif) > sizeof(uint32_t), "default size should be longer.use // to disable warning if you are sure");

#endif // _M_X64

	protected:
		size_t length = 0;
		size_t used = 0;
		bool is_negative = false;
		unif *nums = stacks; // nums[0]是最低的位
		unif stacks[int_inf_stacklength] = {0};
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
		int_inf(int_inf &another)
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
		unif *get_nums(void) const { return nums; }
		bool get_is_negative(void) const { return is_negative; }
		size_t get_length(void) const { return length; }
		void try_extend(size_t ext)
		{
			if (this->length <= ext)
			{
				this->resize(ext);
			}
			return;
		}

		template <class INType>
		int_inf(const INType &num)
		{
			stdnew();
			*this = num;
		}
		int_inf &resize(size_t length)
		{
			if (length > int_inf_stacklength)
			{
				unif *temp = new unif[length];
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
				if (used > length)
				{
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

		// template <typename aunif>
		// int_inf<aunif>& dynamic_cast_as(int_inf<aunif>* typeptr)
		//{//不能仅按返回值重载...模板大概也一样
		//	int_inf<aunif>* ret = (int_inf<aunif>*)this;
		// 突然发现会搞乱stacks，所以算了
		//	return *ret;
		// }
		uint8_t castu8()
		{
			switch (sizeof(unif) * used)
			{
			case 0:
				return 0;
			default:
				return (*(uint8_t *)nums);
			}
		}
		uint16_t castu16()
		{
			switch (sizeof(unif) * used)
			{
			case 0:
				return 0;
			case 1:
				return (*(uint8_t *)nums);
			default:
				return (*(uint16_t *)nums);
			}
		}
		uint32_t castu32()
		{
			switch (sizeof(unif) * used)
			{
			case 0:
				return 0;
			case 1:
				return (*(uint8_t *)nums);
			case 2:
				return (*(uint16_t *)nums);
			case 3:
				return (*(uint32_t *)nums) & 0x00ffffff;
			default:
				return (*(uint32_t *)nums);
				break;
			}
		}
		operator uint32_t()
		{
			return *(uint32_t *)nums;
		}
		operator int32_t()
		{
			return is_negative ? (-(*(int32_t *)nums)) : *(int32_t *)nums;
		}
#ifdef _M_X64
		operator uint64_t()
		{
			return *(uint64_t *)nums;
		}
		operator int64_t()
		{
			return is_negative ? (-(*(int64_t *)nums)) : *(int64_t *)nums;
		}
		uint64_t castu64()
		{
			uint64_t ret = (*(uint64_t *)nums), mask = ~0;
			if (used * sizeof(unif) < 8)
			{
				mask >>= (8 - used * sizeof(unif)) * 8;
			}
			return ret & mask;
		}

#endif

		void superadd(unif c) // 在末尾（最高位）追加一个单元
		{
			if (this->length <= this->used + 4)
			{
				this->resize(this->used + 4);
			}
			this->nums[this->used] = c;
			this->used++;
		}
		void push_back(unif c)
		{
			superadd(c);
		}
		void operator=(char str[])
		{
			this->operator=((const char *)str);
		}
		void operator=(const char *str)
		{
			size_t i = 0;
			while (str[i] != 0)
			{
				i++;
			}
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
		void operator=(const int_inf<aunif> &num)
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
		template <typename INType>
		void operator=(const INType &num)
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
			used = sizeof(INType) / sizeof(unif);
			if (used == 0)
				used = 1;
			refresh_used_down();
		}
		int_inf &negative()
		{
			is_negative = !is_negative;
			return *this;
		}
		int_inf &negative(int_inf &num)
		{
			num.is_negative = !num.is_negative;
			return num;
		}
		bool operator_larger_abs(const int_inf &num)
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
		// sbb重载
		unsigned char inline __cdecl subborrow(uint8_t c, uint8_t a, uint8_t b, uint8_t *r)
		{
			return _subborrow_u8(c, a, b, r);
		}
		unsigned char inline __cdecl subborrow(uint8_t c, uint16_t a, uint16_t b, uint16_t *r)
		{
			return _subborrow_u16(c, a, b, r);
		}
		unsigned char inline __cdecl subborrow(uint8_t c, uint32_t a, uint32_t b, uint32_t *r)
		{
			return _subborrow_u32(c, a, b, r);
		}
#if defined(_M_X64)
		unsigned char inline __cdecl subborrow(uint8_t c, uint64_t a, uint64_t b, uint64_t *r)
		{
			return _subborrow_u64(c, a, b, r);
		}
#endif // _M_X64
		template <class subT>
		uint8_t inline __cdecl subborrow(uint8_t c, subT a, subT b, subT *r)
		{
			return a.subborrow(c, a, b, r);
		}
		// adc重载
		unsigned char inline __cdecl addcarry(uint8_t c, uint8_t a, uint8_t b, uint8_t *r)
		{
			return _addcarry_u8(c, a, b, r);
		}
		unsigned char inline __cdecl addcarry(uint8_t c, uint16_t a, uint16_t b, uint16_t *r)
		{
			return _addcarry_u16(c, a, b, r);
		}
		unsigned char inline __cdecl addcarry(uint8_t c, uint32_t a, uint32_t b, uint32_t *r)
		{
			return _addcarry_u32(c, a, b, r);
		}
#if defined(_M_X64)
		unsigned char inline __cdecl addcarry(uint8_t c, uint64_t a, uint64_t b, uint64_t *r)
		{
			return _addcarry_u64(c, a, b, r);
		}
#endif // _M_X64
		template <class subT>
		uint8_t inline __cdecl addcarry(uint8_t c, subT a, subT b, subT *r)
		{
			return a.addcarry(c, a, b, r);
		}

	public:
		inline int_inf &add_abs(const int_inf &that)
		{
			size_t maxused = this->used >= that.used ? this->used : that.used;
			size_t thatused = that.used;
			int_inf_p<unif> thatp = int_inf_p<unif>(&that);
			if (this->length <= maxused + 4)
			{
				this->resize(maxused + 4);
			}
			uint8_t carry = 0;
			for (size_t i = 0; i < thatused || carry != 0; i++)
			{
				carry = addcarry(carry, this->nums[i], thatp.nums[i], &this->nums[i]);
			}
			refresh_used_down(maxused + 2);
			return *this;
		}
		inline int_inf &add_abs_shl(const int_inf &that, size_t offset)
		{
			size_t maxused = this->used >= that.used + offset ? this->used : that.used + offset;
			size_t thatused = that.used + offset;
			int_inf_p<unif> thatp = int_inf_p<unif>(&that);
			if (this->length <= maxused + 4)
			{
				this->resize(maxused + 4);
			}
			uint8_t carry = 0;
			for (size_t i = 0; i < thatused || carry != 0; i++)
			{
				carry = addcarry(carry, this->nums[i], thatp.nums[i - offset], &this->nums[i]);
			}
			refresh_used_down(maxused + 2);
			return *this;
		}

		int_inf &add(const int_inf &that, const int_inf &another);
		inline int_inf &addu32(const int_inf &num);
		inline int_inf &addu64(const int_inf &num);
		int_inf &sub(const int_inf &num);

	public:
		inline int_inf &sub_abs(const int_inf &that) // this abs should be bigger than that
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
		inline int_inf &sub_rev_abs(const int_inf &that)
		{ // that abs should be bigger than this, otherwise UB
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
		inline int_inf &and_abs(const int_inf &that)
		{
			for (size_t i = 0; i < this.used && i < that.used; i++)
			{
				this->nums[i] &= that.nums[i];
			}
			refresh_used_down();
			return *this;
		}
		inline int_inf &orr_abs(const int_inf &that)
		{
			for (size_t i = 0; i < this.used && i < that.used; i++)
			{
				this->nums[i] |= that.nums[i];
			}
			refresh_used_down();
			return *this;
		}
		inline int_inf &xor_abs(const int_inf &that)
		{
			for (size_t i = 0; i < this.used && i < that.used; i++)
			{
				this->nums[i] ^= that.nums[i];
			}
			refresh_used_down();
			return *this;
		}
		inline int_inf &not_abs()
		{
			for (size_t i = 0; i < this.used; i++)
			{
				this->nums[i] = ~this->nums[i];
			}
			refresh_used_down();
			return *this;
		}

	private:
		// mulx重载
		uint8_t mulx(uint8_t src0, uint8_t src1, uint8_t *highbits)
		{

			uint16_t r = (uint16_t)src0 * src1;
			*highbits = (r >> 8);
			return r;
		}
		uint16_t mulx(uint16_t src0, uint16_t src1, uint16_t *highbits)
		{

			uint32_t r = (uint32_t)src0 * src1;
			*highbits = (r >> 16);
			return r;
		}
		uint32_t mulx(uint32_t src0, uint32_t src1, uint32_t *highbits)
		{
			return _mulx_u32(src0, src1, highbits);
		}
#if defined(_M_X64)
		uint64_t mulx(uint64_t src0, uint64_t src1, uint64_t *highbits)
		{
			return _mulx_u64(src0, src1, highbits);
		}

		template <class mulT>
		mulT mulx(mulT src0, mulT src1, mulT *highbits)
		{
			return src0.mulx(src0, src1, highbits);
		}

#endif
	public:
		inline int_inf &mul_abs_unit(const int_inf &that, unif m2)
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
			this->used = that.used + 1;
			this->refresh_used_down();
			return *this;
		}
		int_inf &mul_abs(const int_inf &that, const int_inf &another)
		{
			this->resize(that.used + another.used).setzero();
			int_inf step;
			for (size_t i = 0; i < another.used; i++)
			{
				step.mul_abs_unit(that, another.nums[i]);
				this->add_abs_shl(step, i);
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
		template <typename printT>
		void print(printT c)
		{
			c.print(c);
		}

#endif
		void show(char separator = ' ')
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
		// avlint<sizeof(unif)> cast_avlint()
		//{
		//	avlint<sizeof(unif)> ret;
		//	ret.set_byte_length(used * sizeof(unif));
		//	size_t retLen = ret.length();
		//	for(size_t i=0;i<used;i++)
		//	{
		//		ret.data[retLen - i - 1] |= nums[i];
		//	}
		//	return ret;
		// }
	};

	int_inf<N_nary<10>> operator""_dec(size_t num)
	{
		int_inf<N_nary<10>> ret;

		while (num > 0)
		{
			int rem = num % 10;
			num = num / 10;
			ret.push_back(N_nary<10>(rem));
		}
		return ret;
	}
	// 略微快速的乘法。真的有用吗？
	template <typename INType>
	void inline product(const INType &x, const INType &y, INType (&xy)[2])
	{
		// auto nm=[(a+b)(c+d)-(ac)-(bd)]
		//(ab)*(cd)==
		//  ac00
		//+ 00bd
		//+ 0nm0
		INType halflen = (sizeof(INType) * 4);
		INType mask = (INType(1) << halflen) - 1;
		INType ly = y & mask, hy = (y >> halflen) & mask; // a/b
		INType lx = x & mask, hx = (x >> halflen) & mask; // c/d
		INType hproduct = hy * hx, lproduct = ly * lx;	  // ac/bd
		INType sumy = ly + hy, sumx = lx + hx;			  // a+b/c+d
		INType midproduct = (INType)((sumy & mask) * (sumx & mask));
		INType productsub = hproduct + lproduct; //(a+b)(c+d)-(ac)-(bd),overflow exception
		INType temproduct = midproduct - productsub;
		// overflow handle
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
	// 半自动宏模板 	示例：MACRO_int_inf_add(64, n2, n3);
	// 是我草率了。我忘了函数可以重载。详见addcarry
	// 这玩意留着做宏用法的参考吧，虽然删了也没事。
#define MACRO_int_inf_add(bits, this_, that)                                                                                            \
	do                                                                                                                                  \
	{                                                                                                                                   \
		static_assert(sizeof(*(this_.get_nums())) == sizeof(__int##bits), "size error: sizeof(unif)!=sizeof(unsigned __int" #bits ")"); \
		auto maxused = this_.get_used() > that.get_used() ? this_.get_used() : that.get_used();                                         \
		this_.try_extend(maxused + 4);                                                                                                  \
		that.try_extend(maxused + 4);                                                                                                   \
		auto i = maxused;                                                                                                               \
		unsigned char carry = 0;                                                                                                        \
		auto this_nums = this_.get_nums();                                                                                              \
		auto thatnums = that.get_nums();                                                                                                \
		for (i = 0; i < maxused; i++)                                                                                                   \
		{                                                                                                                               \
			carry = _addcarry_u##bits(carry, thisnums[i], thatnums[i], &thisnums[i]);                                                   \
		}                                                                                                                               \
		if (carry != 0)                                                                                                                 \
		{                                                                                                                               \
			thisnums[maxused] = 1;                                                                                                      \
		};                                                                                                                              \
		that.refresh_used();                                                                                                            \
	} while (false)

	template <typename unit>
	class uint_infinite_t
	{
	public:
		static constexpr size_t buffer_size()
		{
			return sizeof(size_t) / sizeof(unit);
		}
		unit buffer[buffer_size()];
		unit *data = buffer;
		// data[0] is the lowest unit
		size_t capacity = sizeof(size_t) / sizeof(unit);
		size_t used = 0;
		uint_infinite_t(){};
		uint_infinite_t(unit val)
		{
			data[0] = val;
			used = 1;
		}
		uint_infinite_t(const uint_infinite_t &another)
		{
			if (another.used > buffer_size())
			{
				data = new unit[another.used];
				capacity = another.used;
			}
			else
			{
				data = buffer;
				capacity = buffer_size();
			}
			used = another.used;
			memcpy(data, another.data, used * sizeof(unit));
		}

		// the move constructor
		uint_infinite_t(uint_infinite_t &&another) noexcept
		{
			this->move_assigned(std::move(another));
		}

		~uint_infinite_t()
		{
			if (data != buffer)
			{
				delete[] data;
			}
		}

		// use copy constructor
		uint_infinite_t<unit> copy() const
		{
			return uint_infinite_t<unit>(*this);
		}

		// use
		uint_infinite_t<unit> move()
		{
			return uint_infinite_t<unit>(std::move(*this));
		}

		/// @brief move another to this
		void move_assigned(uint_infinite_t<unit> &&another) noexcept
		{
			if (data != buffer)
			{
				delete[] data;
			}
			this->capacity = another.capacity;
			this->used = another.used;

			if (another.data == another.buffer)
			{
				this->data = this->buffer;
				memcpy(this->buffer, another.buffer, another.used * sizeof(unit));
			}
			else
			{
				this->data = another.data;
				another.data = another.buffer;
			}
			another.capacity = another.buffer_size();
			another.used = 0;
		}

		/// @brief refit the capacity of the data
		/// @param newcapacity the expected minimum capacity.
		/// @param - if used is larger than newcapacity, some data will be lost.
		/// @param - newused is always no more than newcapacity
		/// @return the actual capacity
		size_t refit_capacity(size_t newcapacity)
		{
			if (used > newcapacity)
			{
				used = newcapacity;
			}
			// case 1: newcapacity is smaller than buffer_size
			if (newcapacity <= buffer_size())
			{
				// case 1.1: data is buffer
				if (data == buffer)
				{
					// do nothing
				}
				// case 1.2: data is not buffer, copy data to buffer
				else
				{
					memcpy(buffer, data, used * sizeof(unit));
					delete[] data;
					data = buffer;
					capacity = buffer_size();
				}
			}
			// case 2: newcapacity is larger than buffer_size
			// if capacity is enough, do nothing
			else if (newcapacity <= capacity)
			{
				// do nothing
			}
			// if capacity is not enough, extend the capacity
			else
			{
				// always: new data
				unit *newdata = new unit[newcapacity];
				memcpy(newdata, data, used * sizeof(unit));
				// case 2.1: data is buffer
				if (data == buffer)
				{
					// do nothing
				}
				// case 2.2: data is not buffer
				else
				{
					delete[] data;
				}
				// update data and capacity
				data = newdata;
				capacity = newcapacity;
			}
			return capacity;
		}

		/// @brief used will be down fitted to the first non-zero unit, or 0 if all units are 0
		void down_fit_used()
		{
			size_t i;
			if (used == 0)
				return;
			i = used - 1;
			for (;;)
			{
				if (data[i] != unit())
				{
					used = i + 1;
					return;
				}
				if (i == 0)
				{
					used = 0;
					return;
				}
				i--;
			}
		}

		/// @brief reverse the used data from low~high to high~low
		void unit_swap()
		{
			size_t i = 0, j = used - 1;
			while (i < j)
			{
				std::swap(data[i], data[j]);
				i++, j--;
			}
		}

		// operator = , copy
		void operator=(const uint_infinite_t<unit> &another)
		{
			if (another.used > buffer_size())
			{
				if (data != buffer)
				{
					delete[] data;
				}
				data = new unit[another.used];
				capacity = another.used;
			}
			else
			{
				if (data != buffer)
				{
					delete[] data;
					data = buffer;
				}
				capacity = buffer_size();
			}
			used = another.used;
			memcpy(data, another.data, used * sizeof(unit));
		}

		// operator = , move
		void operator=(uint_infinite_t<unit> &&another)
		{
			this->move_assigned(std::move(another));
		}

		/// @brief the number will be multiplied by the base,
		/// which is determined by the type of the unit
		/// @param distance the number of units to be shifted
		/// @return void
		uint_infinite_t<unit> shift_up(size_t distance = 1)
		{
			if (distance == 0)
				return *this;
			if (used + distance > capacity)
			{
				// extend the capacity
				unit *newdata = new unit[used + distance]{};
				memcpy(newdata + distance, data, used * sizeof(unit));
				if (data != buffer)
				{
					delete[] data;
				}
				data = newdata;
			}
			else
			{
				memmove(data + distance, data, used * sizeof(unit));
			}
			// set the new units to unit()
			std::fill(data, data + distance, unit());
			used += distance;
			return *this;
		}

		/// @brief the number will be divided by the base
		/// @param distance the number of units to be shifted
		/// @return units that are shifted out
		uint_infinite_t<unit> shift_down(size_t distance = 1)
		{
			if (distance == 0)
				return uint_infinite_t<unit>();
			if (distance >= used)
			{
				return move();
			}
			else
			{
				uint_infinite_t<unit> ret;
				ret.used = distance;
				ret.refit_capacity(ret.used);
				memcpy(ret.data, data, ret.used * sizeof(unit));
				memmove(data, data + distance, (used - distance) * sizeof(unit));
				used -= distance;
				return ret;
			}
		}

		// the addcarry functions of basic uint types, overloaded
		static uint8_t inline addcarry(uint8_t c, uint8_t a, uint8_t b, uint8_t *r)
		{
			return _addcarry_u8(c, a, b, r);
		}
		static uint8_t inline subborrow(uint8_t c, uint8_t a, uint8_t b, uint8_t *r)
		{
			return _subborrow_u8(c, a, b, r);
		}
		static uint8_t inline addcarry(uint8_t c, uint16_t a, uint16_t b, uint16_t *r)
		{
			return _addcarry_u16(c, a, b, r);
		}
		static uint8_t inline subborrow(uint8_t c, uint16_t a, uint16_t b, uint16_t *r)
		{
			return _subborrow_u16(c, a, b, r);
		}
		static uint8_t inline addcarry(uint8_t c, uint32_t a, uint32_t b, uint32_t *r)
		{
			return _addcarry_u32(c, a, b, r);
		}
		static uint8_t inline subborrow(uint8_t c, uint32_t a, uint32_t b, uint32_t *r)
		{
			return _subborrow_u32(c, a, b, r);
		}
#if defined(_M_X64)
		static uint8_t inline addcarry(uint8_t c, uint64_t a, uint64_t b, uint64_t *r)
		{
			return _addcarry_u64(c, a, b, r);
		}
		static uint8_t inline subborrow(uint8_t c, uint64_t a, uint64_t b, uint64_t *r)
		{
			return _subborrow_u64(c, a, b, r);
		}
#endif // _M_X64
		template <class adcT>
		static uint8_t inline addcarry(uint8_t c, adcT a, adcT b, adcT *r)
		{
			return adcT::addcarry(c, a, b, r);
		}
		template <class sbbT>
		static uint8_t inline subborrow(uint8_t c, sbbT a, sbbT b, sbbT *r)
		{
			return sbbT::subborrow(c, a, b, r);
		}

		unit get_unit(size_t index) const
		{
			if (index >= used)
				return unit();
			return data[index];
		}

		void operator+=(const uint_infinite_t<unit> &another)
		{
			auto maxused = used > another.used ? used : another.used;
			if (capacity <= maxused)
			{
				refit_capacity(maxused);
			}
			uint8_t carry = 0;
			size_t i;
			for (i = 0; i < maxused; i++)
			{
				carry = addcarry(carry, get_unit(i), another.get_unit(i), &data[i]);
			}
			if (carry != 0)
			{
				addcarry(carry, unit(), unit(), &data[i]);
				used = i + 1;
			}
			else
			{
				used = i;
			}
		}

		/// @brief a-b , a should be larger than b
		/// @param a the minuend
		/// @param b the subtrahend
		/// @return the difference
		static uint_infinite_t<unit> sub(const uint_infinite_t<unit> &a, const uint_infinite_t<unit> &b)
		{
			uint_infinite_t<unit> ret(a);
			uint8_t borrow = 0;
			auto maxused = std::max(a.used, b.used);
			for (size_t i = 0; i < maxused; i++)
			{
				borrow = subborrow(borrow, ret.get_unit(i), b.get_unit(i), &ret.data[i]);
			}
			ret.down_fit_used();
			return ret;
		}

		/// @brief a+b
		/// @param a the augend
		/// @param b the addend
		/// @return the sum
		static uint_infinite_t<unit> add(const uint_infinite_t<unit> &a, const uint_infinite_t<unit> &b)
		{
			uint_infinite_t<unit> ret;
			auto maxused = std::max(a.used, b.used);
			ret.refit_capacity(maxused + 1);
			uint8_t carry = 0;
			size_t i;
			for (i = 0; i < maxused; i++)
			{
				carry = addcarry(carry, a.get_unit(i), b.get_unit(i), &ret.data[i]);
			}
			if (carry != 0)
			{
				addcarry(carry, unit(), unit(), &ret.data[i]);
				ret.used = i + 1;
			}
			else
			{
				ret.used = i;
			}
			return ret;
		}

		/// @brief minus another number
		/// @param another the number to be subtracted
		/// @return the sign of the result, true if negative
		bool operator-=(const uint_infinite_t<unit> &another)
		{
			bool is_negative = *this < another;
			if (is_negative)
			{
				this->move_assigned(sub(another, *this));
			}
			else
			{
				this->move_assigned(sub(*this, another));
			}
			return is_negative;
		}

		bool operator==(const uint_infinite_t<unit> &another) const
		{
			if (used == another.used)
			{
				for (size_t i = 0; i < used; i++)
				{
					if (data[i] == another.data[i])
					{
						continue;
					}
					else
					{
						return false;
					}
				}
				return true;
			}
			else
			{
				return false;
			}
		}

		bool operator!=(const uint_infinite_t<unit> &another) const
		{
			if (used != another.used)
			{
				return true;
			}
			for (size_t i = 0; i < used; i++)
			{
				if (data[i] != another.data[i])
				{
					return true;
				}
			}
			return false;
		}

		int comparator(const uint_infinite_t<unit> &another) const
		{
			if (used > another.used)
			{
				return 1;
			}
			else if (used < another.used)
			{
				return -1;
			}
			else
			{
				for (size_t i = used - 1;;)
				{
					if (data[i] > another.data[i])
					{
						return 1;
					}
					else if (data[i] < another.data[i])
					{
						return -1;
					}
					if (i == 0)
					{
						break;
					}
					i--;
				}
				return 0;
			}
		}

		bool comparator(const uint_infinite_t<unit> &another, const bool islarge, const bool isequal) const
		{
			if (used > another.used)
			{
				return islarge;
			}
			else if (used < another.used)
			{
				return !islarge;
			}
			else
			{
				for (size_t i = used - 1;;)
				{
					if (data[i] > another.data[i])
					{
						return islarge;
					}
					else if (data[i] < another.data[i])
					{
						return !islarge;
					}
					if (i == 0)
					{
						break;
					}
					i--;
				}
				return isequal;
			}
		}

		bool operator<(const uint_infinite_t<unit> &another) const
		{
			return comparator(another, false, false);
		}

		bool operator<=(const uint_infinite_t<unit> &another) const
		{
			return comparator(another, false, true);
		}

		bool operator>(const uint_infinite_t<unit> &another) const
		{
			return comparator(another, true, false);
		}

		bool operator>=(const uint_infinite_t<unit> &another) const
		{
			return comparator(another, true, true);
		}

		uint_infinite_t<unit> operator+(const uint_infinite_t<unit> &another) const
		{
			return add(*this, another);
		}

		uint_infinite_t<unit> operator-(const uint_infinite_t<unit> &another) const
		{
			return sub(*this, another);
		}

		// the mulx and udiv functions of basic uint types, overloaded
		static uint8_t inline mulx(uint8_t src0, uint8_t src1, uint8_t *highbits)
		{
			uint16_t r = (uint16_t)src0 * src1;
			*highbits = (r >> 8);
			return r;
		}
		static uint8_t inline udiv(uint8_t highpart, uint8_t lowpart, uint8_t divisor, uint8_t *remainder)
		{
			uint16_t dividend = (uint16_t)highpart << 8 | lowpart;
			*remainder = dividend % divisor;
			return dividend / divisor;
		}
		static uint16_t inline mulx(uint16_t src0, uint16_t src1, uint16_t *highbits)
		{
			uint32_t r = (uint32_t)src0 * src1;
			*highbits = (r >> 16);
			return r;
		}
		static uint16_t inline udiv(uint16_t highpart, uint16_t lowpart, uint16_t divisor, uint16_t *remainder)
		{
			uint32_t dividend = (uint32_t)highpart << 16 | lowpart;
			*remainder = dividend % divisor;
			return dividend / divisor;
		}
		static uint32_t inline mulx(uint32_t src0, uint32_t src1, uint32_t *highbits)
		{
			return _mulx_u32(src0, src1, highbits);
		}
		static uint32_t inline udiv(uint32_t highpart, uint32_t lowpart, uint32_t divisor, uint32_t *remainder)
		{
			return _udiv64((uint64_t)highpart << 32 | lowpart, divisor, remainder);
		}
#if defined(_M_X64)
		static uint64_t inline mulx(uint64_t src0, uint64_t src1, uint64_t *highbits)
		{
			return _mulx_u64(src0, src1, highbits);
		}
		static uint64_t inline udiv(uint64_t highpart, uint64_t lowpart, uint64_t divisor, uint64_t *remainder)
		{
			return _udiv128(highpart, lowpart, divisor, remainder);
		}
#endif // _M_X64
	   /// @return the low part of the product
		template <class mulT>
		static mulT mulx(mulT src0, mulT src1, mulT *highbits)
		{
			return mulT::mulx(src0, src1, highbits);
		}

		/// @brief unsigned division, the divisor should not be 0, and highpart should be less than divisor
		/// @return the quotient
		template <class divT>
		static divT udiv(divT highpart, divT lowpart, divT divisor, divT *remainder)
		{
			return divT::udiv(highpart, lowpart, divisor, remainder);
		}

		/// @brief divide the number by another number,
		/// @return the quotient
		static unit udiv_unit(const uint_infinite_t<unit> &denominator, const uint_infinite_t<unit> &divisor, uint_infinite_t<unit> *remainder)
		{
// denominator should between divisor and divisor*base(divisor shifted up 1 )
// check and throw exception
#ifdef _DEBUG
			if (denominator < divisor)
			{
				throw std::invalid_argument("denominator should be larger than divisor");
			}
			uint_infinite_t<unit> up = divisor;
			up.shift_up(1);
			if (denominator >= up)
			{
				throw std::invalid_argument("denominator should be less than divisor*base");
			}
#endif
			unit unit_max;
			subborrow(1, unit(), unit(), &unit_max);

			// use binary search to find the quotient, the min is 1, for denominator is larger than divisor
			unit quotient = unit(1);
			unit low = unit(1), high = unit_max;
			while (low < high)
			{
				// mid = (low+high)/2
				// use origin addcarry
				unit mid;
				uint8_t carry = addcarry(0, low, high, &mid);
				// then use origin udiv
				unit remainder_unit;
				mid = udiv(mid, unit(carry == 0 ? 0 : 1), unit(2), &remainder_unit);

				// mid*divisor
				uint_infinite_t<unit> middivisor = divisor * mid;

				switch (middivisor.comparator(denominator))
				{
				case -1: // middivisor < denominator
					// low = mid + unit(1);
					// use origin addcarry
					addcarry(0, mid, unit(1), &low);
					break;
				case 0: // middivisor == denominator
					quotient = mid;
					goto end;
				case 1: // middivisor > denominator
					// high = mid - unit(1);
					// use origin subborrow
					subborrow(0, mid, unit(1), &high);
					break;
				default:
					break;
				}
			}
		end:
			// remainder = denominator - quotient*divisor
			*remainder = denominator - divisor * quotient;
			return quotient;
		}

		/// @brief multiply the number by a unit
		/// @param another the multiplier unit
		/// @return the product
		uint_infinite_t<unit> operator*(const unit &another) const
		{
			uint_infinite_t<unit> part[2];
			part[0].refit_capacity(used);	  // low units
			part[1].refit_capacity(used + 1); // high units
			part[1].data[0] = unit();
			for (size_t i = 0; i < used; i++)
			{
				part[0].data[i] = mulx(data[i], another, &part[1].data[i + 1]);
			}
			// then add the high units
			part[0].used = used;
			part[1].used = used + 1;
			return part[0] + part[1];
		}

		/// @brief multiply the number by another number
		/// @param another the multiplier
		/// @return the product
		uint_infinite_t<unit> operator*(const uint_infinite_t<unit> &another) const
		{
			uint_infinite_t<unit> ret;
			ret.refit_capacity(used + another.used);
			for (size_t i = 0; i < used; i++)
			{
				uint_infinite_t<unit> part = another * data[i];
				part.down_fit_used();
				part.shift_up(i);
				ret += part;
			}
			return ret;
		}

		/// @brief divide the number by a unit
		/// @param another the divisor unit
		/// @return the quotient
		uint_infinite_t<unit> operator/(const unit &another) const
		{
			// todo
		}

		// safe check
		bool safe_check()
		{
			// capacity should be no less than used and buffer_size
			if (capacity < used || capacity < buffer_size())
				return false;
			// if data is buffer, capacity should be buffer_size
			if (data == buffer && capacity != buffer_size())
				return false;
			// if data is not buffer, capacity should be larger than buffer_size
			if (data != buffer && capacity < buffer_size())
				return false;
			// data should not be null
			if (data == nullptr)
				return false;
			return true;
		}
	};

	// the firend operator<< for std::ostream of uint_infinite_t
	template <typename unit>
	std::ostream& operator<<(std::ostream& os, const uint_infinite_t<unit>& num)
	{
		if (num.used == 0)
		{
			os << "0";
			return os;
		}
		size_t i = num.used - 1;
		for (;;)
		{
			os << num.data[i];
			if (i == 0)
				break;
			i--;
		}
		return os;
	};

	uint_infinite_t<N_nary<10>> operator""_10based(size_t num)
	{
		uint_infinite_t<N_nary<10>> ret;
		while (num > 0)
		{
			int rem = num % 10;
			num = num / 10;
			ret.shift_up();
			ret.data[0] = N_nary<10>(rem);
		}
		ret.unit_swap();
		return ret;
	}


	template <typename unit>
	class bigUnsignedInteger
	{
	public:
		//a vector to store the numbers
		std::vector<unit> data;

		constexpr static const uint64_t base = getBase();

		//constructor
		bigUnsignedInteger() {};
		~bigUnsignedInteger() {};
		bigUnsignedInteger(unit num)
		{
			data.push_back(num);
		};



	private:
		//get the base of the number
		constexpr static uint64_t getBase()
		{
			unit num0 = unit();
			unit num1 = unit(1);
			decltype(getBase()) base = 0;
			for (;;)
			{
				auto car = unit::addcarry(0, num0, num1, &num0);
				if (car != 0)
					return base;
				base++;
			}
		};
	};


#ifdef namespace_bionukg
};
#endif // namespace_bionukg
#endif
