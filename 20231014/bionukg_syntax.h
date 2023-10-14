#pragma once

#ifndef bionukg_syntax_h
#define bionukg_syntax_h
#include "./bionukg_tString.h"

#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

	enum tErrmsg
	{
		invaild_args
	};

	enum tBaseType
	{
		bin = 2,
		oct = 8,
		dec = 10,
		hex = 16
	};

	template<tErrmsg msg,typename ...Ts>
	struct errmsg
	{
		using type = errmsg<msg>;
		tconstval auto val = msg;
	};

	constexpr int8_t asnum_imp(char c)
	{
		if (c >= '0' && c <= '9')
			return c - '0';
		if (c >= 'a' && c <= 'z')
			return c - 'a' + 10;
		if (c >= 'A' && c <= 'Z')
			return c - 'A' + 10;
		return -1;
	};
	constexpr int8_t asnum(char c, tBaseType base = hex)
	{
		auto ret = asnum_imp(c);
		if (ret < base)
			return ret;
		return -1;
	}
	
	struct numexpr
	{
		size_t strlen;
		tBaseType base;
	};

	constexpr numexpr expr_num(const char*str,size_t N)
	{
		return { 1,(tBaseType)1 };
	};


	template<char c, tBaseType base = hex>
	struct char2num
	{
		tconstval int8_t val = asnum(c, base);
		using template_type = char2num<0>;
		using type = typename if__ <boolean_<(val == -1)>,
			errmsg<invaild_args, char2num<c,base>>,
			integer<val>
		>::type;
	};



	template<tString_t T, tBaseType base, size_t idx>
	struct praseNumber_imp_
	{
		tconstval auto c = asnum(tString_at_<T, idx>::value, base);
		using val = typename if__<boolean_<(c != -1)>,
			integer<c + (base * praseNumber_imp_<T, base, idx - 1>::val::value) >,
			errmsg<invaild_args, praseNumber_imp_<T,base,idx>>
		>::type;
	};
	template<tString_t T, tBaseType base>
	struct praseNumber_imp_<T, base, 0>
	{
		tconstval auto c = asnum(tString_at_<T, 0>::value, base);
		using val = typename if__<boolean_<(c != -1)>,
			integer<c>,
			errmsg<invaild_args, praseNumber_imp_<T, base, 0>>
		>::type;

	};

	template<tString_t T>
	struct praseNumber_
	{
		useval nexpr = expr_num(T::str,T::len);
	}; 

	using i = praseNumber_imp_<tString_<'2', '3', '4'>, tBaseType(5), 2>::val;
	useval ex = praseNumber_< tString_<'2', '3', '4'>>::nexpr;

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif //! bionukg_syntax_h