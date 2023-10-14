#pragma once

#ifndef bionukg_tsring_h
#define bionukg_tsring_h

// #include<iostream>
#include "./bionukg_meta_base.h"

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	enum token_t
	{
		number_,
		operator_,

	};

	template <char c>
	struct tCharacter_;

	template <char c>
	struct tCharacter_
	{
		tconstval char value = c;
		using type = tCharacter_<c>;
		using template_type = tCharacter_<0>;
	};
	using tCharacter_template = tCharacter_<0>;

	template <char... c>
	struct tString_;
	using tString_template = tString_<>;

	template <char... c>
	struct tString_
	{
		tconstval size_t len = sizeof...(c);
		tconstval char str[len + 1] = {c..., '\0'};
		using type = tString_<c...>;
		using template_type = tString_template;
	};

	template <typename T>
	concept tString_t = is_same_template<T, tString_template>::value;

	template <typename T>
	concept tCharacter_t = is_same_template<T, tCharacter_template>::value;

	template <tString_t T, size_t N>
	struct tString_at_
	{
		tconstval char value =
			if__<boolean_<(N < T::len)>,
				 tCharacter_<T::str[N]>,
				 tCharacter_<0>>::type::value;
	};

	template <typename T, int N = 0>
	struct tStringcpy;

	template <tString_t T, int N>
	struct tStringcpy<T, N>
	{
		using type = tStringcpy<T, N>;
		using template_type = tStringcpy<void__t, 0>;
		tconstval char (&str)[sizeof(T::str)] = T::str;
	};
	template <tString_t T, integer_t idx, char... c>
	struct tStringrev_imp_
	{
		using type = typename tStringrev_imp_<T, integer<idx::value - 1>, c..., tString_at_<T, idx::value>::value>::type;
	};
	template <tString_t T, char... c>
	struct tStringrev_imp_<T, integer<0>, c...>
	{
		using type = tString_<c..., tString_at_<T, 0>::value>;
	};
	template <tString_t T, size_t idx = T::len>
	struct tStringrev
	{
		using type = typename tStringrev_imp_<T, integer<T::len - 1>>::type;
	};

	template <tString_t T, integer_t begin, integer_t end, char... c>
	struct tStringcut_imp_
	{
		using type = typename tStringcut_imp_<T, integer<begin::value + 1>, end, c..., tString_at_<T, begin::value>::value>::type;
	};
	template <tString_t T, integer_t beginend, char... c>
	struct tStringcut_imp_<T, beginend, beginend, c...>
	{
		using type = tString_<c..., tString_at_<T, beginend::value>::value>;
	};
	template <tString_t T, size_t begin = 0, size_t end = T::len - 1>
	struct tStringcut_
	{
		using type = typename tStringcut_imp_<T, integer<begin>, integer<end>>::type;
	};

	template <tString_t T, size_t idx, char c, char now>
	struct tStringfind_imp_
	{
		using type = typename if__<boolean_<(idx < T::len)>,
								   typename tStringfind_imp_<T, idx + 1, c, tString_at_<T, idx + 1>::value>::type,
								   integer<-1>>::type;
	};
	template <tString_t T, size_t idx, char c>
	struct tStringfind_imp_<T, idx, c, c>
	{
		using type = integer<idx>;
	};

	template <tString_t T, char c>
	struct tStringfind_
	{
		using first = typename tStringfind_imp_<T, 0, c, tString_at_<T, 0>::value>::type;
	};

	template <tString_t T0, tString_t T1, int idx0, int idx1, char... c>
	struct tStringadd_imp_;

	template <tString_t T0, tString_t T1, int idx0, char... c>
	struct tStringadd_imp_<T0, T1, idx0, 0, c...>
	{
		using type = typename if__ < boolean_<(idx0 < T0::len)>,
			  tStringadd_imp_<T0, T1, idx0 + 1, 0, c..., tString_at_<T0, idx0>::value>,

			typename if__<boolean_<(T1::len > 0)>,
				   tStringadd_imp_<T0, T1, -1, 1, c..., tString_at_<T1, 0>::value>,
				   tString_<c...>>::type

		>::type::type;
	};
	template <tString_t T0, tString_t T1, int idx1, char... c>
	struct tStringadd_imp_<T0, T1, -1, idx1, c...>
	{
		using type = typename if__<boolean_<(idx1 < T1::len)>,
								   tStringadd_imp_<T0, T1, -1, idx1 + 1, c..., tString_at_<T1, idx1>::value>,
								   tString_<c...>>::type::type;
	};
	template <tString_t T0, tString_t T1 = tString_<>>
	struct tStringadd_
	{
		using t1 = T1;
		using self = tStringadd_<T0, T1>;
		using type = typename tStringadd_imp_<T0, T1, 0, 0>::type;
	};







#define strex(s, n) strex16(s, n)
#define strex1(s, n) (n < sizeof(#s) ? (#s)[n] : 0)
#define strex2(s, n) strex1(s, n), strex1(s, n + 1)
#define strex4(s, n) strex2(s, n), strex2(s, n + 2)
#define strex8(s, n) strex4(s, n), strex4(s, n + 4)
#define strex16(s, n) strex8(s, n), strex8(s, n + 8)
#define MacrotString_(s) tString_<strex16(s, 0)>
#define MacrotString_8(s) tString_<strex8(s, 0)>

#define MtString_(s, len) tString_<strex##len(s, 0)>

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif //! bionukg_tsring_h
