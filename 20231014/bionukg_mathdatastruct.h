#ifndef bionukg_mathdatastruct_h
#define bionukg_mathdatastruct_h
#include <stdint.h>
#include <cmath>
#include <iostream>
#if defined(_M_X64) || defined(_MSC_VER)
#include <intrin.h>
#else

uint32_t _lzcnt_u32(uint32_t val)
{
    uint32_t ret;
    __asm__("lzcnt %1, %0"
            : "=r"(ret)
            : "r"(val));
    return ret;
}
uint64_t _lzcnt_u64(uint64_t val)
{
    uint64_t ret;
    __asm__("lzcnt %1, %0"
            : "=r"(ret)
            : "r"(val));
    return ret;
}

#endif
#ifdef namespace_bionukg
namespace bionukg
{
#endif

    class float64_
    {
    public:
        using float_type = double;
        using uint_type = uint64_t;
        using int_type = int64_t;
        constexpr static const inline size_t mantissa_bits = 52;
        constexpr static const inline size_t exponent_bits = 11;
        constexpr static const inline size_t total_bits = 64;
        constexpr static const inline uint_type exponent_offset = (uint_type(1) << (exponent_bits - 1)) - 1;
        constexpr static const inline uint_type exponent_max = (uint_type(1) << exponent_bits) - 1;

        union
        {
            float_type float_val;
            uint_type uint_val;
            struct
            {
                uint_type mantissa : mantissa_bits;
                uint_type exponent : exponent_bits;
                uint_type sign : 1;
            };
        };
        constexpr float64_() : uint_val(0){};
        constexpr float64_(float_type val) : float_val(val){};
        constexpr float64_(uint_type val) : uint_val(val){};
        constexpr operator float_type() const
        {
            return float_val;
        }
    };
    class float32_
    {
    public:
        using float_type = float;
        using uint_type = uint32_t;
        using int_type = int32_t;
        constexpr static const inline size_t mantissa_bits = 23;
        constexpr static const inline size_t exponent_bits = 8;
        constexpr static const inline size_t total_bits = 32;
        constexpr static const inline uint_type exponent_offset = (uint_type(1) << (exponent_bits - 1)) - 1;
        constexpr static const inline uint_type exponent_max = (uint_type(1) << exponent_bits) - 1;
        union
        {
            float_type float_val;
            uint_type uint_val;
            struct
            {
                uint_type mantissa : mantissa_bits;
                uint_type exponent : exponent_bits;
                uint_type sign : 1;
            };
        };
        constexpr float32_() : uint_val(0){};
        constexpr float32_(float_type val) : float_val(val){};
        constexpr float32_(uint_type val) : uint_val(val){};
        constexpr operator float_type() const
        {
            return float_val;
        }
        static constexpr uint_type cast_(float_type val)
        {

            uint_type mantissa;
            uint_type exponent;
            uint_type sign;

            sign = val < 0 ? 1 : 0;
            val = val < 0 ? -val : val;
            exponent = exponent_offset;
            
            if (val >= 1)
            {
                while (val >= 2)
                {
                    val /= 2;
                    exponent++;
                }
            }
            else
            {
                while (val < 1)
                {
                    val *= 2;
                    exponent--;
                }
            }
            mantissa = uint_type(val * (1 << mantissa_bits));

            return (sign << (exponent_bits + mantissa_bits)) | (exponent << mantissa_bits) | mantissa;
        }
        static constexpr float_type cast_(uint_type val)
        {
            uint_type mantissa = val & ((uint_type(1) << mantissa_bits) - 1);
            mantissa |= uint_type(1) << mantissa_bits;
            uint_type exponent = (val >> mantissa_bits) & ((uint_type(1) << exponent_bits) - 1);
            uint_type sign = (val >> (exponent_bits + mantissa_bits)) & 1;
            float_type ret = float_type(mantissa) / (uint_type(1) << mantissa_bits);
            if (exponent >= exponent_offset)
            {
                uint_type exp = exponent - exponent_offset;
                while (exp > 0)
                {
                    ret *= 2;
                    exp--;
                }
            }
            else
            {
                uint_type exp = exponent_offset - exponent;
                while (exp > 0)
                {
                    ret /= 2;
                    exp--;
                }
            }
            return sign ? -ret : ret;
        }
    };

    template <size_t N = 128>
    class uint_
    {
    public:
        using base_uint = typename uint_<N / 2>::base_uint;
        constexpr static const inline size_t base_bit = sizeof(base_uint) * 8;
        constexpr static const inline size_t num_count = N / base_bit;
        friend class uint_<N / 2>;
        friend class uint_<N * 2>;
        union
        {
            struct
            {
                uint_<N / 2> low;
                uint_<N / 2> high;
            };
            base_uint num[num_count]{};
        };

    public:
        constexpr uint_<N>() : low(), high(){};
        constexpr uint_<N>(const uint_<N> &that) : low(that.low), high(that.high){};
        constexpr uint_<N>(const uint_<N / 2> &l) : low(l), high() {}
        constexpr uint_<N>(const uint_<N / 2> &lowpart, const uint_<N / 2> &highpart)
            : low(lowpart), high(highpart) {}

    private:
        constexpr uint_<N>(const base_uint *nums, uint_<2 * N> *) : low(nums, this), high(nums + num_count / 2, this){};

    public:
        constexpr uint_<N>(const base_uint (&nums)[num_count]) : low(nums, this), high(nums + num_count / 2, this){};

        template <typename T>
        constexpr uint_<N>(T val_l, T val_h) : low(val_l), high(val_h){};
        constexpr uint_<N>(size_t l) : low(l), high(0){};
        void operator=(const uint_<N> &that)
        {
            low = that.low;
            high = that.high;
        }

        size_t cast_size_t() const
        {
            constexpr size_t mask = N > sizeof(size_t) * 8 ? ~size_t(0) : (size_t(1) << N) - 1;
            return (*(size_t *)this) & mask;
        }

        constexpr static uint_<N> mask_bits1(size_t bit_count, size_t shift_left = 0)
        {
            base_uint(num_)[num_count] = {};
            size_t total_count = bit_count + shift_left;
            size_t blocks_1 = total_count / base_bit;
            size_t blocks_0 = shift_left / base_bit;
            size_t bits_1 = total_count % base_bit;
            size_t bits_0 = shift_left % base_bit;

            for (size_t i = blocks_0; i < blocks_1; i++)
            {
                num_[i] = ~base_uint(0);
            }
            // set 1 in the last block
            if (bits_1 != 0)
            {
                num_[blocks_1] = (base_uint(1) << bits_1) - 1;
            }
            // set 0 in the first block
            if (bits_0 != 0)
            {
                num_[blocks_0] &= ~((base_uint(1) << bits_0) - 1);
            }
            return uint_<N>(num_);
        };

        /*	template<size_t N0>
            constexpr uint_<N0> cast() const
            {

            }*/

        // arithmetic operators
        void operator+=(const uint_<N> &that)
        {
            addcarry(that, 0);
        }
        void operator-=(const uint_<N> &that)
        {
            subborrow(that, 0);
        }
        void operator&=(const uint_<N> &that)
        {
            low &= that.low;
            high &= that.high;
        }
        void operator|=(const uint_<N> &that)
        {
            low |= that.low;
            high |= that.high;
        }
        void operator^=(const uint_<N> &that)
        {
            low ^= that.low;
            high ^= that.high;
        }

        constexpr void operator<<=(int64_t shl)
        {
            if (shl < 0)
                operator_shiftRightEqual((uint64_t)(-shl));
            else
                operator_shiftLeftEqual((uint64_t)shl);
        }
        constexpr void operator>>=(int64_t shr)
        {
            if (shr < 0)
                operator_shiftLeftEqual((uint64_t)(-shr));
            else
                operator_shiftRightEqual((uint64_t)shr);
        }
        constexpr void shiftLeftEqual_Block(uint64_t shlb)
        {
            for (size_t i = num_count; i > 0; i--)
            // range 0~num_count-1.
            {
                // in range shlb~num_count-1, set num[i0] = num[i0-shlb]
                if (i > shlb)
                {
                    num[i - 1] = num[i - 1 - shlb];
                }
                else
                { // in range 0~shlb-1, set 0
                    num[i - 1] = 0;
                }
            }
        }
        constexpr void shiftRightEqual_Block(uint64_t shrb)
        {
            for (size_t i = 0; i < num_count; i++)
            // range 0~num_count-1.
            {
                // in range 0~num_count-1-shrb, set num[i0] = num[i0+shrb]
                if (i + shrb < num_count)
                {
                    num[i] = num[i + shrb];
                }
                else
                { // in range num_count-shrb~num_count-1, set 0
                    num[i] = 0;
                }
            }
        }
        constexpr void shiftLeftEqual_Short(uint64_t shls)
        {
            high <<= shls;
            high |= (low >> (N / 2 - shls));
            low <<= shls;
        }
        constexpr void shiftRightEqual_Short(uint64_t shrs)
        {
            low >>= shrs;
            low |= (high << (N / 2 - shrs));
            high >>= shrs;
        }

        constexpr void operator_shiftLeftEqual(uint64_t shl)
        {
            if (shl >= base_bit)
            {
                shiftLeftEqual_Block(shl / base_bit);
                shl %= base_bit;
            }
            shiftLeftEqual_Short(shl);
        }
        constexpr void operator_shiftRightEqual(uint64_t shr)
        {
            if (shr >= base_bit)
            {
                shiftRightEqual_Block(shr / base_bit);
                shr %= base_bit;
            }
            shiftRightEqual_Short(shr);
        }

        template <size_t N1>
        constexpr void operator<<=(const uint_<N1> &that)
        {
            this->operator<<=(that.cast_size_t());
        }
        template <size_t N1>
        constexpr void operator>>=(const uint_<N1> &that)
        {
            this->operator>>=(that.cast_size_t());
        }
        constexpr uint_<N> operator+(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret += that;
            return ret;
        }
        constexpr uint_<N> operator-(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret -= that;
            return ret;
        }
        constexpr uint_<N> operator&(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret &= that;
            return ret;
        }
        constexpr uint_<N> operator|(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret |= that;
            return ret;
        }
        constexpr uint_<N> operator^(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret ^= that;
            return ret;
        }
        constexpr uint_<N> operator>>(int64_t shr) const
        {
            uint_<N> ret = *this;
            ret >>= shr;
            return ret;
        }
        constexpr uint_<N> operator<<(int64_t shl) const
        {
            uint_<N> ret = *this;
            ret <<= shl;
            return ret;
        }
        // operator~
        constexpr uint_<N> operator~() const
        {
            return uint_<N>(~(this->low), ~(this->high));
        }

        // comparison operators
        bool operator==(const uint_<N> &that) const
        {
            return low == that.low && high == that.high;
        }
        bool operator!=(const uint_<N> &that) const
        {
            return low != that.low || high != that.high;
        }
        bool operator<(const uint_<N> &that) const
        {
            return high < that.high || (high == that.high && low < that.low);
        }
        bool operator>(const uint_<N> &that) const
        {
            return high > that.high || (high == that.high && low > that.low);
        }
        bool operator<=(const uint_<N> &that) const
        {
            return high < that.high || (high == that.high && low <= that.low);
        }
        bool operator>=(const uint_<N> &that) const
        {
            return high > that.high || (high == that.high && low >= that.low);
        }

        // advanced arithmetic operators
        uint8_t addcarry(const uint_<N> &that, uint8_t carry_in)
        {
            return high.addcarry(that.high, low.addcarry(that.low, carry_in));
        }
        uint8_t adc(const uint_<N> &that, uint8_t carry_in)
        {
            return addcarry(that, carry_in);
        }
        uint8_t subborrow(const uint_<N> &that, uint8_t borrow_in)
        {
            return high.subborrow(that.high, low.subborrow(that.low, borrow_in));
        }
        uint8_t sbb(const uint_<N> &that, uint8_t borrow_in)
        {
            return subborrow(that, borrow_in);
        }
        bool getlowbit() const
        {
            return low.getlowbit();
        }
        bool gethighbit() const
        {
            return high.gethighbit();
        }
        void setlowbit(bool bit)
        {
            low.setlowbit(bit);
        }
        void sethighbit(bool bit)
        {
            high.sethighbit(bit);
        }
        void setbit(size_t idx, bool bit)
        {
            if (idx > N / 2)
            {
                setbit(idx - N / 2, bit);
            }
            else
            {
                setbit(idx, bit);
            }
        }
        bool getbit(size_t idx) const
        {
            if (idx > N / 2)
            {
                return getbit(idx - N / 2);
            }
            else
            {
                return getbit(idx);
            }
        }

        void shl1()
        {
            high <<= 1;
            high.setlowbit(low.gethighbit());
            low <<= 1;
        }
        void shr1()
        {
            low >>= 1;
            low.sethighbit(high.getlowbit());
            high >>= 1;
        }
        size_t lzcnt() const
        {
            size_t ret = this->high.lzcnt();
            return ret * 2 == N ? ret + this->low.lzcnt() : ret;
        }
        static size_t lzcnt(uint_<N> val)
        {
            return val.lzcnt();
        }
        static uint_<N> mul(const uint_<N> &_Multiplier, const uint_<N> &_Multiplicand,
                            uint_<N> *_HighProduct = nullptr)
        {
            // there are 4 parts: low*low, low*high, high*low, high*high
            uint_<N / 2> _LowLowProduct, _MidLowProduct, _MidHighProduct, _HighHighProduct, buffer;
            uint8_t MidHighCarry = 0, HighCarry = 0;
            // low*low
            _LowLowProduct = uint_<N / 2>::mul(_Multiplier.low, _Multiplicand.low, &_MidLowProduct);
            //_MidHighProduct += uint_<N/2>::mul(_Multiplier.high, _Multiplicand.low, &_MidHighProduct);
            // high*low
            MidHighCarry = _MidLowProduct.addcarry(
                uint_<N / 2>::mul(_Multiplier.high, _Multiplicand.low, &_MidHighProduct), 0);
            HighCarry = _MidHighProduct.addcarry(0, MidHighCarry);
            //_MidHighProduct += uint_<N/2>::mul(_Multiplier.low, _Multiplicand.high, &buffer);
            // low*high
            MidHighCarry = _MidLowProduct.addcarry(
                uint_<N / 2>::mul(_Multiplier.low, _Multiplicand.high, &buffer), 0);
            HighCarry += _MidHighProduct.addcarry(buffer, MidHighCarry);

            // high*high
            HighCarry += _MidHighProduct.addcarry(
                uint_<N / 2>::mul(_Multiplier.high, _Multiplicand.high, &_HighHighProduct), 0);
            _HighHighProduct += HighCarry;
            if (_HighProduct != nullptr)
            {
                _HighProduct->low = _MidHighProduct;
                _HighProduct->high = _HighHighProduct;
            }
            return uint_<N>(_LowLowProduct, _MidLowProduct);
        }

        static uint_<N> div(
            const uint_<N> &_HighDividend, const uint_<N> &_LowDividend,
            const uint_<N> &_Divisor, uint_<N> &_Remainder)
        {
            // assert(_Divisor != 0);
            // assert(_HighDividend < _Divisor);
            // use uint_<N/2>::div
            uint_<N> _Quotient;
            uint_<2 *N> _Dividend = uint_<2 * N>(_LowDividend, _HighDividend);
            uint_<2 *N> _Divisor2 = uint_<2 * N>(0, _Divisor);
            // size_t lz0 = _Divisor.lzcnt(), lz1 = _Dividend.lzcnt();

            for (size_t i = 0; i < N; i++)
            {
                _Quotient.shl1();
                _Divisor2.shr1();
                if (_Dividend >= _Divisor2)
                {
                    _Dividend -= _Divisor2;
                    _Quotient.setlowbit(true);
                }
            }
            _Remainder = _Dividend.low;
            return _Quotient;
        }

        void operator*=(const uint_<N> &that)
        {
            *this = mul(*this, that);
        }
        uint_<N * 2> operator*(const uint_<N> &that) const
        {
            uint_<N * 2> ret;
            ret.low = uint_<N>::mul(*this, that, &(ret.high));
            return ret;
        }

        void operator/=(const uint_<N> &that)
        {
            uint_<N> rem, quo;
            quo = uint_<N>::div(0, *this, that, rem);
            *this = quo;
        }

        uint_<N> operator/(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret /= that;
            return ret;
        }

        void operator%=(const uint_<N> &that)
        {
            uint_<N> rem, quo;
            quo = uint_<N>::div(0, *this, that, rem);
            *this = rem;
        }

        uint_<N> operator%(const uint_<N> &that) const
        {
            uint_<N> ret = *this;
            ret %= that;
            return ret;
        }

        uint8_t inc(uint8_t carry = 1)
        {
            return addcarry(0, carry);
        }
        uint8_t dec(uint8_t borrow = 1)
        {
            return subborrow(0, borrow);
        }

        // for std::cout<<
        friend std::ostream &operator<<(std::ostream &os, const uint_<N> &that)
        {
            os << that.high << that.low;
            return os;
        }
    };
// msvc
#if defined(_M_X64) || defined(_MSC_VER)
    template <>
    class uint_<64>
    {
    public:
        friend class uint_<128>;
        friend class uint_<32>;
        constexpr static const inline size_t base_bit = 64;
        constexpr static const inline size_t num_count = 1;
        using base_uint = uint64_t;
        union
        {
            uint64_t num;
            struct
            {
                uint32_t low, high;
            };
        };
        constexpr uint_<64>() : num(0){};
        constexpr uint_<64>(uint64_t n) : num(n){};
        constexpr uint_<64>(const uint_<64> &that) : num(that.num){};

    private:
        constexpr uint_<64>(const base_uint *nums, uint_<128> *) : num(nums[0]){};

    public:
        constexpr uint_<64>(const base_uint (&nums)[num_count]) : num(nums[0]){};

        size_t cast_size_t() const { return size_t(num); }

        constexpr void operator=(const uint_<64> &that) { num = that.num; }
        // arithmetic operators
        constexpr void operator+=(const uint_<64> &that) { this->num += that.num; }
        constexpr void operator-=(const uint_<64> &that) { this->num -= that.num; }
        constexpr void operator*=(const uint_<64> &that) { this->num *= that.num; }
        constexpr void operator/=(const uint_<64> &that) { this->num /= that.num; }
        constexpr void operator%=(const uint_<64> &that) { this->num %= that.num; }
        constexpr void operator&=(const uint_<64> &that) { this->num &= that.num; }
        constexpr void operator|=(const uint_<64> &that) { this->num |= that.num; }
        constexpr void operator^=(const uint_<64> &that) { this->num ^= that.num; }
        constexpr void operator<<=(int64_t shl) { this->num <<= shl; }
        constexpr void operator>>=(int64_t shr) { this->num >>= shr; }

        // advanced arithmetic operators
        uint8_t addcarry(const uint_<64> &that, uint8_t carry_in)
        {
            return _addcarry_u64(carry_in, this->num, that.num, (unsigned long long *)&(this->num));
        }
        uint8_t adc(const uint_<64> &that, uint8_t carry_in)
        {
            return addcarry(that, carry_in);
        }
        uint8_t subborrow(const uint_<64> &that, uint8_t borrow_in)
        {
            return _subborrow_u64(borrow_in, this->num, that.num, (unsigned long long *)&(this->num));
        }
        uint8_t sbb(const uint_<64> &that, uint8_t borrow_in)
        {
            return subborrow(that, borrow_in);
        }
        static uint_<64> mul(const uint_<64> &_Multiplier, const uint_<64> &_Multiplicand, uint_<64> *_HighProduct)
        {
            return uint_<64>(_umul128(_Multiplier.num, _Multiplicand.num, (unsigned long long *)&(_HighProduct->num)));
        }
        static uint_<64> div(const uint_<64> &_HighDividend, const uint_<64> &_LowDividend, const uint_<64> &_Divisor, uint_<64> &_Remainder)
        {
            return uint_<64>(_udiv128(_HighDividend.num, _LowDividend.num, _Divisor.num, &(_Remainder.num)));
        }
        uint8_t inc(uint8_t carry = 1) { return addcarry(0, carry); }
        uint8_t dec(uint8_t borrow = 1) { return subborrow(0, borrow); }
        void shl1() { this->num <<= 1; }
        void shr1() { this->num >>= 1; }

        void setlowbit(bool bit)
        {
            num >>= 1;
            num <<= 1;
            num |= bit ? 1 : 0;
        }
        bool getlowbit() const { return (num & 1) != 0; }
        void sethighbit(bool bit)
        {
            num <<= 1;
            num >>= 1;
            num |= bit ? (uint64_t(1) << 63) : 0;
        }
        bool gethighbit() const { return (num >> 63) != 0; }

        size_t lzcnt() const
        {
            return _lzcnt_u64(this->num);
        }

        void setbit(size_t idx, bool bit)
        {
            num &= ~(uint64_t(1) << idx);
            num |= (uint64_t(bit ? 1 : 0) << idx);
        }
        bool getbit(size_t idx) const
        {
            return ((num >> idx) & 1) != 0;
        }

        constexpr uint_<64> operator+(const uint_<64> &that) const
        {
            return uint_<64>(this->num + that.num);
        };
        constexpr uint_<64> operator-(const uint_<64> &that) const
        {
            return uint_<64>(this->num - that.num);
        };
        uint_<128> operator*(const uint_<64> &that) const
        {
            uint_<128> ret;
            ret.low.num = _umul128(this->num, that.num, &(ret.high.num));
            return ret;
        };
        constexpr uint_<64> operator/(const uint_<64> &that) const
        {
            return uint_<64>(this->num / that.num);
        };
        constexpr uint_<64> operator|(const uint_<64> &that) const
        {
            return uint_<64>(this->num | that.num);
        };
        constexpr uint_<64> operator&(const uint_<64> &that) const
        {
            return uint_<64>(this->num & that.num);
        };
        constexpr uint_<64> operator^(const uint_<64> &that) const
        {
            return uint_<64>(this->num ^ that.num);
        };
        constexpr uint_<64> operator~() const
        {
            return uint_<64>(~(this->num));
        }
        constexpr uint_<64> operator<<(int64_t shl) const
        {
            return uint_<64>((this->num) << shl);
        }
        constexpr uint_<64> operator>>(int64_t shr) const
        {
            return uint_<64>((this->num) >> shr);
        }

        // compare operators
        bool operator==(const uint_<64> &that) const { return this->num == that.num; }
        bool operator!=(const uint_<64> &that) const { return this->num != that.num; }
        bool operator>(const uint_<64> &that) const { return this->num > that.num; }
        bool operator<(const uint_<64> &that) const { return this->num < that.num; }
        bool operator>=(const uint_<64> &that) const { return this->num >= that.num; }
        bool operator<=(const uint_<64> &that) const { return this->num <= that.num; }

        // for std::cout<<
        friend std::ostream &operator<<(std::ostream &os, const uint_<64> &that)
        {
            // sets the fill character to use when padding results to the field width.
            os.fill('0');
            // sets the field width to be used on output operations.
            os.width(16);
            os << std::hex << that.num;
            return os;
        }
    };
#endif
    template <>
    class uint_<8>
    {
    public:
        constexpr static const inline size_t base_bit = 8;
        constexpr static const inline size_t num_count = 1;
        using base_uint = uint8_t;
        friend class uint_<16>;
        union
        {
            uint8_t num;
            struct
            {
                uint8_t low : 4;
                uint8_t high : 4;
            };
        };
        constexpr uint_<8>() : num(0) {}
        template <typename T>
        constexpr uint_<8>(T n) : num(uint8_t(n)) {}
        constexpr uint_<8>(const uint_<8> &that) : num(that.num) {}

    private:
        constexpr uint_<8>(const base_uint *nums, uint_<16> *) : num(nums[0]){};

    public:
        constexpr uint_<8>(const base_uint (&nums)[num_count]) : num(nums[0]){};

        constexpr void operator=(const uint_<8> &that) { this->num = that.num; }

        // arithmetic operators
        constexpr void operator+=(const uint_<8> &that) { this->num += that.num; }
        constexpr void operator-=(const uint_<8> &that) { this->num -= that.num; }
        constexpr void operator*=(const uint_<8> &that) { this->num *= that.num; }
        constexpr void operator/=(const uint_<8> &that) { this->num /= that.num; }
        constexpr void operator%=(const uint_<8> &that) { this->num %= that.num; }
        constexpr void operator&=(const uint_<8> &that) { this->num &= that.num; }
        constexpr void operator|=(const uint_<8> &that) { this->num |= that.num; }
        constexpr void operator^=(const uint_<8> &that) { this->num ^= that.num; }
        constexpr void operator<<=(int shl) { this->num <<= shl; }
        constexpr void operator>>=(int shr) { this->num >>= shr; }

        // advanced arithmetic operators
        uint8_t addcarry(const uint_<8> &that, uint8_t carry_in)
        {
            uint16_t sum = (uint16_t)this->num + (uint16_t)that.num + carry_in;
            this->num = (uint8_t)(sum & 0xff);
            return (uint8_t)((sum >> 8) & 1);
        }
        uint8_t adc(const uint_<8> &that, uint8_t carry_in)
        {
            return addcarry(that, carry_in);
        }
        uint8_t subborrow(const uint_<8> &that, uint8_t borrow_in)
        {
            uint16_t diff = (uint16_t)this->num - (uint16_t)that.num - borrow_in;
            this->num = (uint8_t)(diff & 0xff);
            return (uint8_t)((diff >> 8) & 1);
        }
        uint8_t sbb(const uint_<8> &that, uint8_t borrow_in)
        {
            return subborrow(that, borrow_in);
        }
        constexpr static uint_<8> mul(const uint_<8> &_Multiplier, const uint_<8> &_Multiplicand, uint_<8> *_HighProduct = nullptr)
        {
            uint16_t product = (uint16_t)_Multiplier.num * (uint16_t)_Multiplicand.num;
            if (_HighProduct != nullptr)
                _HighProduct->num = (uint8_t)(product >> 8);
            return uint_<8>((uint8_t)product);
        }
        constexpr static uint_<8> div(const uint_<8> &_HighDividend, const uint_<8> &_LowDividend, const uint_<8> &_Divisor, uint_<8> &_Remainder)
        {
            uint16_t dividend = (uint16_t)_HighDividend.num << 8 | (uint16_t)_LowDividend.num;
            uint8_t quotient = dividend / _Divisor.num;
            _Remainder.num = dividend % _Divisor.num;
            return uint_<8>(quotient);
        }
        uint8_t inc(uint8_t carry = 1) { return addcarry(0, carry); }
        uint8_t dec(uint8_t borrow = 1) { return subborrow(0, borrow); }
        void shl1() { this->num <<= 1; }
        void shr1() { this->num >>= 1; }

        void setlowbit(bool bit)
        {
            num >>= 1;
            num <<= 1;
            num |= bit ? 1 : 0;
        }
        bool getlowbit() const { return (num & 1) != 0; }
        void sethighbit(bool bit)
        {
            num <<= 1;
            num >>= 1;
            num |= bit ? 0x80 : 0;
        }
        bool gethighbit() const { return (num >> 7) != 0; }

        size_t lzcnt() const
        {
            size_t ret = _lzcnt_u32(this->num);
            return ret - 24;
        }

        void setbit(size_t idx, bool bit)
        {
            num &= ~(uint8_t(1) << idx);
            num |= (uint8_t(bit ? 1 : 0) << idx);
        }

        bool getbit(size_t idx) const
        {
            return ((num >> idx) & 1) != 0;
        }

        constexpr uint_<8> operator+(const uint_<8> &that) const
        {
            return uint_<8>(this->num + that.num);
        };
        constexpr uint_<8> operator-(const uint_<8> &that) const
        {
            return uint_<8>(this->num - that.num);
        };
        constexpr uint_<16> operator*(const uint_<8> &that) const
        {
            uint_<16> ret;
            ret.low = uint_<8>::mul(*this, that, &(ret.high));
            return ret;
        };
        constexpr uint_<8> operator/(const uint_<8> &that) const
        {
            return uint_<8>(this->num / that.num);
        };
        constexpr uint_<8> operator|(const uint_<8> &that) const
        {
            return uint_<8>(this->num | that.num);
        };
        constexpr uint_<8> operator&(const uint_<8> &that) const
        {
            return uint_<8>(this->num & that.num);
        };
        constexpr uint_<8> operator^(const uint_<8> &that) const
        {
            return uint_<8>(this->num ^ that.num);
        };
        constexpr uint_<8> operator<<(int shift) const
        {
            return uint_<8>(this->num << shift);
        }
        constexpr uint_<8> operator>>(int shift) const
        {
            return uint_<8>(this->num >> shift);
        }
        constexpr uint_<8> operator~() const
        {
            return uint_<8>(~(this->num));
        }

        // compare operators
        bool operator==(const uint_<8> &that) const { return this->num == that.num; }
        bool operator!=(const uint_<8> &that) const { return this->num != that.num; }
        bool operator>(const uint_<8> &that) const { return this->num > that.num; }
        bool operator<(const uint_<8> &that) const { return this->num < that.num; }
        bool operator>=(const uint_<8> &that) const { return this->num >= that.num; }
        bool operator<=(const uint_<8> &that) const { return this->num <= that.num; }

        // for std::cout<<
        friend std::ostream &operator<<(std::ostream &os, const uint_<8> &that)
        {
            // sets the fill character to use when padding results to the field width.
            os.fill('0');
            // sets the field width to be used on output operations.
            os.width(2);
            os << std::hex << (uint16_t)that.num;
            return os;
        }
    };

    template <size_t N = 128, size_t bit_start = 0, size_t bit_end = N - 1>
    class uint_bitfield
    {
        uint_<N> &val;
        using uint_type = uint_<N>;
        constexpr static const inline size_t bit_N = N;

    public:
        static_assert(bit_start < bit_end, "bit_start must be less than bit_end");
        constexpr static const inline size_t bit_width = bit_end - bit_start;
        constexpr static const inline uint_<N> mask = uint_<N>::mask_bits1(bit_width, bit_start);
        constexpr static const inline uint_<N> mask_inv = ~mask;
        uint_bitfield() = delete;
        uint_bitfield(uint_<N> &val) : val(val) {}
        uint_bitfield(const uint_bitfield &that) = delete;

        operator uint_<N>() const
        {
            return (val & mask) >> bit_start;
        }

        uint_<N> cast() const
        {
            return uint_<N>(*this);
        }

        uint_<N> &shiftAndMask_inv(uint_<N> that)
        {
            that <<= bit_start;
            that |= mask_inv;
            return that;
        }
        uint_<N> &shiftAndMask(uint_<N> that)
        {
            that <<= bit_start;
            that &= mask;
            return that;
        }

        template <size_t N0>
        void operator=(const uint_<N0> &that)
        {
            val &= mask_inv;
            val |= shiftAndMask(that);
        }

        void operator|=(uint_<N> that)
        {
            val |= shiftAndMask(that);
        }
        void operator&=(uint_<N> that)
        {
            val &= shiftAndMask_inv(that);
        }
        void operator^=(uint_<N> that)
        {
            val ^= shiftAndMask(that);
        }
        void operator<<=(size_t shift)
        {
            *this = uint_<N>(*this) << shift;
        }
        void operator>>=(size_t shift)
        {
            *this = uint_<N>(*this) >> shift;
        }
        void operator+=(const uint_<N> &that)
        {
            *this = uint_<N>(*this) + that;
        }
        void operator-=(const uint_<N> &that)
        {
            *this = uint_<N>(*this) - that;
        }

        uint_<N> operator|(const uint_<N> &that) const
        {
            auto ret = uint_<N>(*this);
            ret |= that;
            return ret;
        }
        uint_<N> operator&(const uint_<N> &that) const
        {
            auto ret = uint_<N>(*this);
            ret &= that;
            return ret;
        }
        uint_<N> operator^(const uint_<N> &that) const
        {
            auto ret = uint_<N>(*this);
            ret ^= that;
            return ret;
        }
        uint_<N> operator<<(size_t shift) const
        {
            auto ret = uint_<N>(*this);
            ret <<= shift;
            return ret;
        }
        uint_<N> operator>>(size_t shift) const
        {
            auto ret = uint_<N>(*this);
            ret >>= shift;
            return ret;
        }
        uint_<N> operator+(const uint_<N> &that) const
        {
            auto ret = uint_<N>(*this);
            ret += that;
            return ret;
        }
        uint_<N> operator-(const uint_<N> &that) const
        {
            auto ret = uint_<N>(*this);
            ret -= that;
            return ret;
        }
    };

    class floatx32_
    {
    public:
        using float_type = float;
        using uint_type = uint_<32>;
        using int_type = int32_t;
        constexpr static const inline size_t mantissa_bits = 23;
        constexpr static const inline size_t exponent_bits = 8;
        constexpr static const inline size_t total_bits = 32;
        constexpr static const inline uint_type exponent_offset = uint_type::mask_bits1(exponent_bits - 1);
        constexpr static const inline uint_type exponent_max = uint_type::mask_bits1(exponent_bits);

        // float_type float_val;

    private:
        constexpr static const inline size_t mantissa_ends = mantissa_bits;
        constexpr static const inline size_t exponent_ends = mantissa_bits + exponent_bits;
        constexpr static const inline size_t sign_ends = total_bits;

    public:
        uint_type uint_val;
        uint_bitfield<total_bits, 0, mantissa_ends> mantissa = decltype(mantissa)(uint_val);
        uint_bitfield<total_bits, mantissa_ends, exponent_ends> exponent = decltype(exponent)(uint_val);
        uint_bitfield<total_bits, exponent_ends, sign_ends> sign = decltype(sign)(uint_val);

        floatx32_() : uint_val(0) {}
        floatx32_(float_type f) : uint_val(*(uint_type *)&f) {}
        floatx32_(uint_type u) : uint_val(u) {}
        floatx32_(const floatx32_ &that) : uint_val(that.uint_val){};
        operator float_type() const
        {
            return *(float *)&uint_val;
        }
        float32_ &fthis = *(float32_ *)this;
    };
    template <typename float_union = float64_>
    class float_
    {
    public:
        float_union val;
        using val_t = decltype(val);
        using float_type = typename val_t::float_type;
        using uint_type = typename val_t::uint_type;
        using int_type = typename val_t::int_type;
        float_() {}
        float_(float_type fval) : val(fval){};
        float_(const float_ &that) : val(that.val){};
        float_(const float_union &that) : val(that){};
        void operator=(const float_ &that)
        {
            this->val.uint_val = that.val.uint_val;
        }

        uint_type get_exponent() const
        {
            return val.exponent;
        }
        void set_exponent(uint_type exp)
        {
            this->val.exponent = exp;
        }

        uint_type get_exponent_abs() const
        {
            return val.exponent >= val_t::exponent_offset
                       ? val.exponent - val_t::exponent_offset
                       : val_t::exponent_offset - val.exponent;
        }
        bool get_exponent_sign() const
        {
            return val.exponent >= val_t::exponent_offset;
        }

        uint_type get_mantissa() const
        {
            return uint_type(this->val.mantissa) | ((uint_type)(1) << val_t::mantissa_bits);
        }
        void set_mantissa(uint_type mant)
        {
            this->val.mantissa = mant & (((uint_type)(1) << val_t::mantissa_bits) - 1);
        }
        bool get_sign() const
        {
            return uint_type(this->val.sign) != 0;
        }
        void set_sign(bool sign)
        {
            this->val.sign = uint_type(sign ? 1 : 0);
        }

        bool issame(const float_ &that) const
        {
            return this->val.uint_val == that.val.uint_val;
        }

        static float_ nan()
        {
            float_ ret;
            ret.val.exponent = 0;
            ret.val.exponent = ~ret.val.exponent;
            ret.val.mantissa = 0;
            ret.val.mantissa = ~ret.val.mantissa;
            return ret;
        }
        bool is_nan() const
        {
            return uint_type(0) == (~uint_type(this->val.exponent)) && uint_type(0) != uint_type(this->val.mantissa);
        }
        static float_ inf_positive()
        {
            float_ ret;
            ret.val.exponent = 0;
            ret.val.exponent = ~ret.val.exponent;
            ret.val.mantissa = 0;
            ret.val.sign = 0;
            return ret;
        }
        bool is_inf_positive() const
        {
            return this->issame(inf_positive());
        }
        static float_ inf_negative()
        {
            float_ ret;
            ret.val.exponent = 0;
            ret.val.exponent = ~ret.val.exponent;
            ret.val.mantissa = 0;
            ret.val.sign = 1;
            return ret;
        }
        bool is_inf_negative() const
        {
            return this->issame(inf_negative());
        }
        bool is_inf() const
        {
            return uint_type(0) == (~uint_type(this->val.exponent)) && uint_type(0) == uint_type(this->val.mantissa);
        }
        static float_ zero_positive()
        {
            float_ ret;
            ret.val.exponent = 0;
            ret.val.mantissa = 0;
            ret.val.sign = 0;
            return ret;
        }
        bool is_zero_positive() const
        {
            return this->issame(zero_positive());
        }
        static float_ zero_negative()
        {
            float_ ret;
            ret.val.exponent = 0;
            ret.val.mantissa = 0;
            ret.val.sign = 1;
            return ret;
        }
        bool is_zero_negative() const
        {
            return this->issame(zero_negative());
        }

        bool is_denormalized() const
        {
            return uint_type(0) == uint_type(this->val.exponent) && uint_type(0) != uint_type(this->val.mantissa);
        }

        template <typename T>
        size_t lzcnt(T val)
        {
            return val.lzcnt();
        }

        template <>
        size_t lzcnt(uint64_t val)
        {
            return _lzcnt_u64(val);
        }
        template <>
        size_t lzcnt(uint32_t val)
        {
            return _lzcnt_u32(val);
        }

        // compare operators
        bool operator==(const float_ &that) const
        {
            return this->val.uint_val == that.val.uint_val;
        }
        bool operator!=(const float_ &that) const
        {
            return this->val.uint_val != that.val.uint_val;
        }
        bool operator_larger_abs(const float_ &that) const
        {
            auto thisexp = this->get_exponent();
            auto thatexp = that.get_exponent();
            if (thisexp > thatexp)
                return true;
            else if (thisexp < thatexp)
                return false;
            else
                return this->get_mantissa() > that.get_mantissa();
        }
        bool operator>(const float_ &that) const
        {
            const bool thissign = this->get_sign();
            const bool thatsign = that.get_sign();
            // positive > negative
            if (!thissign && thatsign)
                return true;
            // negative < positive
            if (thissign && !thatsign)
                return false;

            // same sign
            bool abs_greater = this->operator_larger_abs(that);

            if (thissign)
                return !abs_greater;
            else
                return abs_greater;
        }

        bool operator<(const float_ &that) const
        {
            return that > *this;
        }
        bool operator>=(const float_ &that) const
        {
            return !(*this < that);
        }
        bool operator<=(const float_ &that) const
        {
            return !(*this > that);
        }
        // negative operator
        float_ operator-() const
        {
            float_ ret = *this;
            ret.set_sign(!ret.get_sign());
            return ret;
        }
        void operator-=(const float_ &that_)
        {
            if (that_.get_sign() != this->get_sign())
            {
                *this += (-that_);
                return;
            }

            if (this->operator_larger_abs(that_))
            {
                this->operator_absSubEqual_largeSubSmall(that_);
            }
            else
            {
                auto that = that_;
                that.operator_absSubEqual_largeSubSmall(*this);
                *this = -that;
            }
        }

        void operator_absSubEqual_largeSubSmall(const float_ &that_)
        {
            auto that = that_;
            // not support nan,+-inf
            // not support denormalized number
            // throw exceptions
            if (this->is_nan() || that.is_nan() || this->is_inf() || that.is_inf() || this->is_denormalized() || that.is_denormalized())
            {
                printf("not support nan,+-inf,denormalized number\n");
                throw "not support nan,+-inf,denormalized number";
            }
            auto exp_this = this->get_exponent();
            auto exp_that = that.get_exponent();
            // abs of this is larger than abs of that
            // so exp_this >= exp_that
            auto exp_delta = exp_this - exp_that;
            // if that is too small, no need to sub
            if (exp_delta >= val_t::mantissa_bits + 1)
            {
                return;
            }
            // if exp_delta==0, mantissa_this>=mantissa_that
            if (exp_delta == 0)
            {
                auto this_mant = this->get_mantissa();
                auto that_mant = that.get_mantissa();
                // this_mant>=that_mant
                auto mant_delta = this_mant - that_mant;
                if (mant_delta == 0)
                {
                    this->set_exponent(0);
                    this->set_mantissa(0);
                    return;
                }
                else
                {
                    this->set_mantissa(mant_delta);
                    return;
                }
            }
        }
        void operator+=(const float_ &that_)
        {
            auto that = that_;
            // not support nan,+-inf
            // not support denormalized number
            // throw exceptions
            if (this->is_nan() || that.is_nan() || this->is_inf() || that.is_inf() || this->is_denormalized() || that.is_denormalized())
            {
                printf("not support nan,+-inf,denormalized number\n");
                throw "not support nan,+-inf,denormalized number";
            }
            auto exp_this = this->get_exponent();
            auto exp_that = that.get_exponent();
            auto exp_final = exp_this;
            // compare and convert
            auto mant_this = this->get_mantissa();
            auto mant_that = that.get_mantissa();
            decltype(mant_that) mant_lost;
            auto delta_exp = exp_this - exp_that;
            int mant_lost_type = 0; // 0: start with 0; 1: start with 1, the rest is 0; 2: start with 1, the rest is not 0
            if (exp_this < exp_that)
            {
                exp_final = exp_that;
                mant_lost = mant_this;
                delta_exp = exp_that - exp_this;
                mant_this >>= delta_exp;
                mant_lost -= mant_this << delta_exp;
            }
            else
            {
                exp_final = exp_this;
                mant_lost = mant_that;
                // delta_exp = exp_this - exp_that;
                mant_that >>= delta_exp;
                mant_lost -= mant_that << delta_exp;
            }

            if (delta_exp != 0)
            {
                auto mant_lost_1stbit = mant_lost >> (delta_exp - 1);
                if (mant_lost_1stbit == 0)
                {
                    mant_lost_type = 0;
                }
                else if (mant_lost_1stbit == 1)
                {
                    mant_lost -= mant_lost_1stbit << (delta_exp - 1);
                    if (mant_lost == 0)
                    {
                        mant_lost_type = 1;
                    }
                    else
                    {
                        mant_lost_type = 2;
                    }
                }
                else
                {
                    printf("error");
                    throw "error";
                }
            }

            auto mant_final = mant_this;
            bool sign_final = this->get_sign();
            // add
            if (this->get_sign() == that.get_sign())
            {
                mant_final = mant_this + mant_that;
                switch (mant_lost_type)
                {
                case 0:
                    break;
                case 1: // mant_lost_type==1, to closest even
                    if ((mant_final & 1) == 1)
                    {
                        mant_final += 1;
                    }
                    break;
                case 2:
                    mant_final += 1;
                    break;
                default:
                    printf("error");
                    throw "error";
                }
            }
            else
            {
                if (mant_this > mant_that)
                {
                    mant_final = mant_this - mant_that;
                }
                else
                {
                    sign_final = !sign_final;
                    mant_final = mant_that - mant_this;
                }
                switch (mant_lost_type)
                {
                case 0:
                    break;
                case 1: // mant_lost_type==1, to closest even
                    if ((mant_final & 1) == 1)
                    {
                        mant_final -= 1;
                    }
                    break;
                case 2:
                    mant_final -= 1;
                    break;
                }
            }
            auto lz = this->lzcnt(mant_final);
            // check zero
            if (mant_final == decltype(mant_final)(0))
            {
                exp_final = 0;
                sign_final = false;
            }
            // check overflow: mant_final>=2^53, so lz==10
            else if (lz == val_t::exponent_bits - 1)
            {
                // mant_final >>= 1;
                // to nearest even
                auto lowbit = (mant_final & 1) != 0;
                mant_final >>= 1;
                if (lowbit && (mant_final & 1) != 0)
                    mant_final += 1;
                exp_final += 1;
            }
            // check underflow: mant_final<2^53, so lz>=11
            else if (lz >= val_t::exponent_bits)
            {
                mant_final <<= (lz - val_t::exponent_bits);
                exp_final -= decltype(exp_final)(lz - val_t::exponent_bits);
            }
            else
            {
                printf("error:unknown,not overflow or underflow\n");
                throw "error:unknown, not overflow or underflow\n";
            }
            // not check exp overflow or underflow
            this->set_exponent(exp_final);
            this->set_mantissa(mant_final);
            this->set_sign(sign_final);
        }
        float_ operator+(const float_ &that) const
        {
            auto ret = *this;
            ret += that;
            return ret;
        }
        float_ operator-(const float_ &that) const
        {
            auto ret = *this;
            ret -= that;
            return ret;
        }
        // convert to double

        operator float_type() const
        {
            return float_type(val);
        }

        template <typename From, typename To>
        static To convert_float(const From &from, To *to_ptr = nullptr)
        {
            constexpr auto from_mantissa_bits = From::mantissa_bits;
            constexpr auto from_exponent_bits = From::exponent_bits;
            constexpr auto from_exponent_offset = From::exponent_offset;
            constexpr auto from_exponent_max = From::exponent_max;
            constexpr auto to_mantissa_bits = To::mantissa_bits;
            constexpr auto to_exponent_bits = To::exponent_bits;
            constexpr auto to_exponent_offset = To::exponent_offset;
            constexpr auto to_exponent_max = To::exponent_max;
            To to_;
            To &to =
                to_ptr == nullptr
                    ? to_
                    : *to_ptr;
            constexpr bool to_uint_larger = sizeof(typename To::uint_type) > sizeof(typename From::uint_type);
            using to_uint = typename To::uint_type;
            using l_uint = typename std::conditional<to_uint_larger, typename To::uint_type, typename From::uint_type>::type;

            // Calculate the new exponent for the To float
            // const auto to_exponent = from_exponent + (to_exponent_offset - from_exponent_offset);
            // type is unsigned, so no need to use sign and abs
            const bool to_exponent_signed = from.exponent + to_exponent_offset < from_exponent_offset;
            const auto to_exponent_abs =
                to_exponent_signed
                    ? from_exponent_offset - from.exponent - to_exponent_offset
                    : from.exponent + to_exponent_offset - from_exponent_offset;

            // If the To float exponent is out of range, return infinity with the correct sign
            if (to_exponent_abs > to_exponent_max)
            {
                to.sign = to_uint(from.sign);
                to.exponent = to_exponent_max;
                to.mantissa = 0;
                return to;
            }
            // If the To float exponent is underflow
            else if (to_exponent_signed)
            {
                if (to_exponent_abs > to_mantissa_bits)
                { // to under denormalized, return zero with the correct sign
                    to.sign = from.sign;
                    to.exponent = 0;
                    to.mantissa = 0;
                    return to;
                }
                else //(to_exponent_abs <= to_mantissa_bits)
                // so (to_mantissa_bits - to_exponent_abs)is always positive
                { // to denormalized, return denormalized with the correct sign
                    to.sign = from.sign;
                    to.exponent = 0;
                    to.mantissa = from.mantissa >> (to_mantissa_bits - to_exponent_abs);
                    return to;
                }
            }
            else //(to_exponent_abs <= to_exponent_max
                 //&& to_exponent_signed == false)
            {    // Set the sign, exponent, and mantissa of the To float
                to.sign = from.sign;
                to.exponent = to_exponent_abs;
                // shift mantissa
                constexpr bool shift_left = to_mantissa_bits > from_mantissa_bits;
                constexpr auto shift_abs =
                    shift_left
                        ? to_mantissa_bits - from_mantissa_bits
                        : from_mantissa_bits - to_mantissa_bits;

                to.mantissa = to_uint(shift_left
                                          ? l_uint(from.mantissa) << shift_abs
                                          : l_uint(from.mantissa) >> shift_abs);

                return to;
            }
        }

        template <typename To>
        float_<To> convert_float(To *to_ptr = nullptr)
        {
            return float_<To>(this->convert_float(this->val, to_ptr));
        }
    };

    void float_test_bench()
    {
        // check in range -1.0f~1.0f, if the +,-,*,/ is correct
        uint32_t cnt = 0;
        for (float f0 = 1e-5f; f0 <= 1.0f; f0 = nextafterf(f0, 2.0f))
        {
            cnt = 0;
            for (float f1 = 1e-5f; f1 <= 1.0f; f1 = nextafterf(f1, 2.0f))
            {
                float_<float32_> f0_(f0);
                float_<float32_> f1_(f1);
                float_<float32_> fadd = f0_ + f1_;
                float_<float32_> fsub = f0_ - f1_;
                // float_<float32_> fmul=f0_*f1_;
                // float_<float32_> fdiv=f0_/f1_;
                float f2 = f0 + f1;
                float f3 = f0 - f1;
                // float f4=f0*f1;
                // float f5=f0/f1;
                if (f2 != fadd.val.float_val)
                {
                    printf("error: %f+%f=%f, but %f+%f=%f\n", f0, f1, f2, f0, f1, fadd.val.float_val);
                    throw "error";
                }
                if (f3 != fsub.val.float_val)
                {
                    printf("error: %f-%f=%f, but %f-%f=%f\n", f0, f1, f3, f0, f1, fsub.val.float_val);
                    throw "error";
                }
                // if(f4!=fmul.val.float_val){
                //	printf("error: %f*%f=%f, but %f*%f=%f\n",f0,f1,f4,f0,f1,fmul.val.float_val);
                //	throw "error";
                // }
                // if(f5!=fdiv.val.float_val){
                //	printf("error: %f/%f=%f, but %f/%f=%f\n",f0,f1,f5,f0,f1,fdiv.val.float_val);
                //	throw "error";
                // }
                cnt++;
            }

            printf("%e float test pass, count=%d\n", f0, cnt);
        }
    }

#ifdef namespace_bionukg
}
#endif
#endif // !bionukg_mathdatastruct_h
