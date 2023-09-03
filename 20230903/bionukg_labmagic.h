#pragma once
#include <iostream>
#include <type_traits>
#include <intrin.h>
#include "./bionukg_meta_base.h"
#ifndef bionukg_labmagic_h
#define bionukg_labmagic_h

#ifdef namespace_bionukg
namespace bionukg
{
#endif

#ifndef labmagic_enable
	namespace bionukg_labmagic
	{
#endif
		//		namespace str_to_u32
		//		{
		//			template <size_t N>
		//			struct t4chars;
		//
		//			template <>
		//			struct t4chars<0>
		//			{
		//				static uint32_t c(const char *arr)
		//				{
		//					return 0;
		//				}
		//			};
		//
		//			template <size_t N>
		//			struct t4chars
		//			{
		//				static uint32_t c(const char *arr)
		//				{
		//					return (static_cast<uint8_t>(arr[N - 1]) << ((N - 1) * 8)) |
		//						   t4chars<N - 1>::c(arr);
		//				}
		//			};
		//
		//			uint32_t inline f4chars(const char *s)
		//			{
		//				return *(uint32_t *)s;
		//			};
		//
		//			uint32_t operator""_u4chars(const char *s, size_t len)
		//			{
		//				uint32_t ret = 0;
		//				len = len > 4 ? 4 : len;
		//				switch (len)
		//				{
		//				case 4:
		//					ret |= static_cast<uint8_t>(s[3]) << 24;
		//				case 3:
		//					ret |= static_cast<uint8_t>(s[2]) << 16;
		//				case 2:
		//					ret |= static_cast<uint8_t>(s[1]) << 8;
		//				case 1:
		//					ret |= static_cast<uint8_t>(s[0]);
		//				default:
		//				}
		//				return ret;
		//			};
		//
		//			namespace int_to_type
		//			{
		//				template <int N, bool isUnsigned = true>
		//				struct int_type
		//				{
		//					using std::conditional;
		//					using type = typename std::conditional <
		//								 isUnsigned,
		//						  std::conditional<
		//							  N == 8,
		//							  uint8_t,
		//							  std::conditional<
		//								  N == 16,
		//								  uint16_t,
		//								  std::conditional<
		//									  N == 32,
		//									  uint32_t,
		// #if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
		//									  std::conditional<
		//										  N == 64,
		//										  uint64_t,
		//										  void>
		// #else
		//								  void
		// #endif
		//									  >>,
		//							  std::conditional<
		//								  N == 8,
		//								  int8_t,
		//								  std::conditional<
		//									  N == 16,
		//									  int16_t,
		//									  std::conditional<
		//										  N == 32,
		//										  int32_t,
		// #if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
		//										  std::conditional<
		//											  N == 64,
		//											  int64_t,
		//											  void>
		// #else
		//									  void
		// #endif
		//										  >>>>::type;
		//					// if type is void, use static_assert to report error
		//					static_assert(!std::is_void<type>::value, "int_type<N> is not defined for N");
		//				};
		//				using std::ref
		//			};
		//		};
		//
#ifndef labmagic_enable
		namespace meta
		{
#endif

#ifdef labmagic_enable
#define identifier id_
#endif // labmagic_enable

			template <int t>
			struct identifier
			{
				constexpr static inline const int value = t;
				using type = identifier<t>;
				using template_type = identifier<0>;
			};
			using identifier_template = identifier<0>;
			namespace id
			{
				// using a = identifier<'a'>;
				using b = identifier<'b'>;
			}

			template <typename T>
			concept id_digit = is_same_template<T, identifier_template>::value && (T::value >= '0' && T::value <= '9');

			template <typename T>
			concept id_alpha = is_same_template<T, identifier_template>::value && ((T::value >= 'a' && T::value <= 'z') || (T::value >= 'A' && T::value <= 'Z'));

			template <char... c>
			struct string_t
			{
				constexpr static inline const char str[sizeof...(c)] = {c...};
			};

#ifndef labmagic_enable
		};
#endif

		uint8_t byteswap_(uint8_t num)
		{
			return num;
		};
		uint16_t byteswap_(uint16_t num)
		{
			return _byteswap_ushort(num);
		};
		uint32_t byteswap_(uint32_t num)
		{
			return _byteswap_ulong(num);
		};
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
		uint64_t byteswap_(uint64_t num)
		{
			return _byteswap_uint64(num);
		};
#endif

		template <uint_any_t uint_t>
		uint_t bitswap_(uint_t num)
		{
			uint_t mask;
			mask = 0x5555555555555555;
			num = (num & mask) << 1 | (num & ~mask) >> 1;
			mask = 0x3333333333333333;
			num = (num & mask) << 2 | (num & ~mask) >> 2;
			mask = 0x0F0F0F0F0F0F0F0F;
			num = (num & mask) << 4 | (num & ~mask) >> 4;
			return byteswap_(num);
		};

#ifndef labmagic_enable
	};
#endif

#ifdef namespace_bionukg
};
#endif // namespace_bionukg
#endif //_bionukg_labmagic_h
