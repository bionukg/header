#pragma once

#ifndef bionukg_meta_base_h
#define bionukg_meta_base_h

#include<stdint.h>

#ifdef namespace_bionukg
namespace bionukg
{
#endif

    using i64 = int64_t;
    template <i64 N>
    struct integer
    {
        constexpr static inline const i64 value = N;
        using type = integer<N>;
        using template_type = integer<0>;
        using add_base = integer<0>;
        using mul_base = integer<1>;
    };
    using integer_template = integer<0>;

    template <i64 N, i64 D = 1>
    struct fraction
    {
        constexpr static inline const i64 numerator_ = N;
        constexpr static inline const i64 denominator_ = D;
        using numerator = integer<N>;
        using denominator = integer<D>;
        using type = fraction<N, D>;
        using template_type = fraction<0, 1>;
        using add_base = fraction<0, 1>;
        using mul_base = fraction<1, 1>;
    };
    using fraction_template = fraction<0, 1>;

    template <bool B>
    struct boolean_
    {
    };

    using true_ = boolean_<true>;
    using false_ = boolean_<false>;
    using boolean__template = boolean_<false>;

    template <>
    struct boolean_<true>
    {
        using type = true_;
        using template_type = boolean__template;
        constexpr static inline const bool value = true;
    };

    template <>
    struct boolean_<false>
    {
        using type = false_;
        using template_type = boolean__template;
        constexpr static inline const bool value = false;
    };

    template <int T>
    struct trit
    {
    };

    using true_trit = trit<1>;
    using false_trit = trit<-1>;
    using unknown_trit = trit<0>;
    using trit_template = trit<0>;

    template <>
    struct trit<1>
    {
        using type = true_trit;
        using template_type = trit_template;
        constexpr static inline const int8_t value = 1;
    };

    template <>
    struct trit<0>
    {
        using type = unknown_trit;
        using template_type = trit_template;
        constexpr static inline const int8_t value = 0;
    };

    template <>
    struct trit<-1>
    {
        using type = false_trit;
        using template_type = trit_template;
        constexpr inline static const int8_t value = -1;
    };

    struct void__t_;
    using void__t = void__t_;

    struct void__t_
    {
        using type = void__t;
        using template_type = void__t;
    };

    template <typename condition, typename True, typename False>
    struct if__impl : False
    {
        using type = False;
    };

    template <typename True, typename False>
    struct if__impl<boolean_<true>, True, False> : True
    {
        using type = True;
    };

    template <typename condition, typename True, typename False>
    struct if__ : if__impl<condition, True, False>
    {
        using type = typename if__impl<condition, True, False>::type;
    };

    // enable_if
    template <typename B, typename T = void__t>
    struct enable_if__
    {
        using type = T;
    };

    template <typename T>
    struct enable_if__<false_, T>
    {
        // using type = void__t;
    };
    // is_same_type
    template <typename T, typename U>
    struct is_same_type : boolean_<false>
    {
        // using type = boolean_<false>;
    };

    template <typename T>
    struct is_same_type<T, T> : boolean_<true>
    {
        // using type = boolean_<true>;
    };

    // is same template, use template_type to compare
    template <typename T, typename U>
    struct is_same_template : is_same_type<typename T::template_type, typename U::template_type>
    {
        using type = typename is_same_type<typename T::template_type, typename U::template_type>;
    };

    template <typename enable_type, typename actual_type>
    struct enable_if_same_type
    {
        using type = enable_if__<is_same_type<enable_type, actual_type>, actual_type>;
    };

    template <typename enable_type, typename actual_type>
    struct enable_if_same_template
    {
        using type = enable_if__<is_same_template<enable_type, actual_type>, actual_type>;
    };

    template <typename T>
    concept integer_t = is_same_template<T, integer_template>::value;

    template <typename T>
    concept nonzero_integer_t = (is_same_template<T, integer_template>::value) && (T::value != 0);

    template <typename T>
    concept zero_integer_t = is_same_type<T, integer<0>>::value;

    template <typename T>
    concept positive_integer_t = (is_same_template<T, integer_template>::value) && (T::value > 0);

    template <typename T>
    concept zero_or_positive_integer_t = (is_same_template<T, integer_template>::value) && (T::value >= 0);

    template <typename T>
    concept negative_integer_t = (is_same_template<T, integer_template>::value) && (T::value < 0);

    template <typename T>
    concept zero_or_negative_integer_t = (is_same_template<T, integer_template>::value) && (T::value <= 0);

    template <typename T>
    concept fraction_t = (is_same_template<T, fraction_template>::value && T::denominator_ != 0);

    template <typename T>
    concept positive_fraction_t = (is_same_template<T, fraction_template>::value) && (T::denominator_ != 0) && ((T::numerator_ > 0 && T::denominator_ > 0) || (T::numerator_ < 0 && T::denominator_ < 0));

    template <typename T>
    concept negative_fraction_t = (is_same_template<T, fraction_template>::value) && (T::denominator_ != 0) && ((T::numerator_ > 0 && T::denominator_ < 0) || (T::numerator_<0 && T::denominator_> 0));

    template <typename T>
    concept nonzero_fraction_t = (is_same_template<T, fraction_template>::value) && (T::denominator_ != 0) && (T::numerator_ != 0);

    template <typename T>
    concept boolean__t = is_same_template<T, boolean__template>::value;

    template <typename T>
    concept true_t = is_same_type<T, true_>::value;

    template <typename T>
    concept false_t = is_same_type<T, false_>::value;

    template <typename T>
    concept trit_t = is_same_template<T, trit_template>::value;

    template <typename T>
    concept true_trit_t = is_same_type<T, true_trit>::value;

    template <typename T>
    concept false_trit_t = is_same_type<T, false_trit>::value;

    template <typename T>
    concept unknown_trit_t = is_same_type<T, unknown_trit>::value;

    template <typename T>
    concept uint_any_t = is_same_type<T, uint8_t>::value || is_same_type<T, uint16_t>::value || is_same_type<T, uint32_t>::value || is_same_type<T, uint64_t>::value;

    template <typename T>
    concept sint_any_t = is_same_type<T, int8_t>::value || is_same_type<T, int16_t>::value || is_same_type<T, int32_t>::value || is_same_type<T, int64_t>::value;


#ifdef namespace_bionukg
};
#endif
#endif // !bionukg_meta_base_h_
