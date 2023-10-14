#pragma once

#ifndef bionukg_bignum_h
#define bionukg_bignum_h

#include <stdlib.h>
#include <stdint.h>
// #include<intrin.h>
#include <new>
#include <vector>
#include <concepts>
#include <tuple>
#include "./bionukg_meta_base.h"
#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	template <typename unit>
	concept unit_t = requires(unit a, unit b) {
		unit();
		unit(int(1));
		concept_condition(unit() == unit(0));
		concept_condition(unit() != unit(int(1)));
		concept_condition(unit() < unit(int(1)));
		concept_condition((unit(int(1)) > unit()));
		{
			unit::addcarry(uint8_t(0), a, b, &a)
		} -> std::same_as<uint8_t>;
		{
			unit::subborrow(uint8_t(0), a, b, &a)
		} -> std::same_as<uint8_t>;
		concept_condition(std::get<0>(unit::subborrow(1, unit(), unit())) == 1);
		{
			unit::mulx(a, b, &a)
		} -> std::same_as<unit>;
	};

	template <typename unit>
	class bigUint
	{
		constexpr static size_t getBase()
		{
			return 0;
		}

	public:
		tconstval size_t base = bigUint<unit>::getBase();
		std::vector<unit> num;

	public:
	};

	class DECchar
	{
	public:
		char n;

	public:
		constexpr DECchar() { n = '0'; }
		template <typename INType>
		constexpr DECchar(const INType &that) { n = '0' + that; }
		~DECchar() = default;

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
		constexpr bool operator!=(DECchar i)
		{
			return this->n != i.n;
		}
		constexpr bool operator==(DECchar i)
		{
			return this->n == i.n;
		};

		constexpr bool operator>(DECchar i)
		{
			return this->n > i.n;
		};

		constexpr bool operator<(DECchar i)
		{
			return this->n < i.n;
		};

		// void print(DECchar c)
		//{
		//	printf("%c", c.n);
		// }
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
		};
		constexpr static std::tuple<uint8_t, DECchar> addcarry(uint8_t c, DECchar a, DECchar b)
		{
			DECchar r;
			r.n = c + a.n + b.n - '0';
			if (r.n > '9')
			{
				r.n -= 10;
				return std::make_tuple(1, r);
			}
			else
			{
				return std::make_tuple(0, r);
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
		constexpr static std::tuple<uint8_t, DECchar> subborrow(uint8_t c, DECchar a, DECchar b)
		{
			DECchar r;
			r.n = a.n - c;
			if (r.n >= b.n)
			{
				r.n = r.n - b.n + '0';
				return std::make_tuple(0, r);
			}
			else
			{
				r.n = r.n - b.n + '0' + 10;
				return std::make_tuple(1, r);
			}
		}

		static DECchar inline __cdecl mulx(DECchar mul0, DECchar mul1, DECchar *highres)
		{
			uint8_t x = (mul0.n - '0') * (mul1.n - '0');
			highres->n = x / 10 + '0';
			DECchar r;
			r.n = x % 10 + '0';
			return r;
		}

		// tuple<lowpart, hightpart>
		constexpr static std::tuple<DECchar, DECchar> mulx(DECchar mul0, DECchar mul1)
		{
			uint8_t x = (mul0.n - '0') * (mul1.n - '0');
			DECchar highres;
			highres.n = x / 10 + '0';
			DECchar r;
			r.n = x % 10 + '0';
			return std::make_tuple(r, highres);
		};
	};

#ifdef namespace_bionukg
};

#endif // namespace_bionukg

#endif // !bionukg_bignum_h
