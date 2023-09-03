#pragma once

#ifndef bionukg_metatemplate_h
#define bionukg_metatemplate_h

#include <stdint.h>
#include "./bionukg_meta_base.h"

#ifdef namespace_bionukg
namespace bionukg
{
#endif

    // template <integer_t N, integer_t D = integer<1>>
    // struct fraction
    //{
    //     constexpr static inline const i64 numerator_ = N::value;
    //     constexpr static inline const i64 denominator_ = D::value;
    //     using numerator = N;
    //     using denominator = D;
    //     using type = fraction<N::value, D::value>;
    //     using template_type = fraction_template;
    // };

    // logical operators
    template <typename L0, typename L1 = L0>
    struct logical__
    {
        using type = logical__<void__t, void__t>;
        using template_type = logical__<void__t, void__t>;
        using operator_type = logical__<void__t, void__t>;
    };

    template <boolean_t L0, boolean_t L1>
    struct logical__<L0, L1>
    {
        using type = logical__<L0, L1>;
        using template_type = logical__<void__t, void__t>;
        using operator_type = logical__<boolean_template, boolean_template>;

        using and_ = boolean<L0::value && L1::value>;
        using or_ = boolean<L0::value || L1::value>;
        using not_ = boolean<!L0::value>;
        using xor_ = boolean<L0::value ^ L1::value>;
        using nand_ = boolean<!(L0::value && L1::value)>;
        using nor_ = boolean<!(L0::value || L1::value)>;
        using xnor_ = boolean<!(L0::value ^ L1::value)>;
    };

    template <trit_t L0, trit_t L1>
    struct logical__<L0, L1>
    {
        using type = logical__<L0, L1>;
        using template_type = logical__<void__t, void__t>;
        using operator_type = logical__<trit_template, trit_template>;

        using and_ = trit<L0::value <= L1::value ? L0::value : L1::value>;
        using or_ = trit<L0::value >= L1::value ? L0::value : L1::value>;
        using not_ = trit<-L0::value>;
        using xor_ = trit<-(L0::value *L1::value)>;
        using nand_ = trit<-(L0::value <= L1::value ? L0::value : L1::value)>;
        using nor_ = trit<-(L0::value >= L1::value ? L0::value : L1::value)>;
        using xnor_ = trit<L0::value * L1::value>;
    };

    template <integer_t L0, integer_t L1>
    struct logical__<L0, L1>
    {
        using type = logical__<L0, L1>;
        using template_type = logical__<void__t, void__t>;
        using operator_type = logical__<integer_template, integer_template>;

        using and_ = integer<L0::value & L1::value>;
        using or_ = integer<L0::value | L1::value>;
        using not_ = integer<~L0::value>;
        using xor_ = integer<L0::value ^ L1::value>;
        using nand_ = integer<~(L0::value &L1::value)>;
        using nor_ = integer<~(L0::value | L1::value)>;
        using xnor_ = integer<~(L0::value ^ L1::value)>;
    };

    template <typename L0, typename L1>
    struct and__ : logical__<L0, L1>::and_
    {
        using type = typename logical__<L0, L1>::and_;
    };

    template <typename L0, typename L1>
    struct or__ : logical__<L0, L1>::or_
    {
        using type = typename logical__<L0, L1>::or_;
    };

    template <typename L0>
    struct not__ : logical__<L0, L0>::not_
    {
        using type = typename logical__<L0, L0>::not_;
    };

    template <typename L0, typename L1>
    struct xor__ : logical__<L0, L1>::xor_
    {
        using type = typename logical__<L0, L1>::xor_;
    };

    template <typename L0, typename L1>
    struct nand__ : logical__<L0, L1>::nand_
    {
        using type = typename logical__<L0, L1>::nand_;
    };

    template <typename L0, typename L1>
    struct nor__ : logical__<L0, L1>::nor_
    {
        using type = typename logical__<L0, L1>::nor_;
    };

    template <typename L0, typename L1>
    struct xnor__ : logical__<L0, L1>::xnor_
    {
        using type = typename logical__<L0, L1>::xnor_;
    };

    // logical test
    namespace logical__test
    {
        logical__<true_, true_>::and_ and_test_tt;
        logical__<true_, false_>::and_ and_test_tf;
        logical__<false_, true_>::and_ and_test_ft;
        logical__<false_, false_>::and_ and_test_ff;

        logical__<true_, true_>::or_ or_test_tt;
        logical__<true_, false_>::or_ or_test_tf;
        logical__<false_, true_>::or_ or_test_ft;
        logical__<false_, false_>::or_ or_test_ff;

        logical__<true_>::not_ not_test_t;
        logical__<false_>::not_ not_test_f;

        logical__<true_, true_>::xor_ xor_test_tt;
        logical__<true_, false_>::xor_ xor_test_tf;
        logical__<false_, true_>::xor_ xor_test_ft;
        logical__<false_, false_>::xor_ xor_test_ff;

        logical__<true_, true_>::nand_ nand_test_tt;
        logical__<true_, false_>::nand_ nand_test_tf;
        logical__<false_, true_>::nand_ nand_test_ft;
        logical__<false_, false_>::nand_ nand_test_ff;

        logical__<true_, true_>::nor_ nor_test_tt;
        logical__<true_, false_>::nor_ nor_test_tf;
        logical__<false_, true_>::nor_ nor_test_ft;
        logical__<false_, false_>::nor_ nor_test_ff;

        logical__<true_, true_>::xnor_ xnor_test_tt;
        logical__<true_, false_>::xnor_ xnor_test_tf;
        logical__<false_, true_>::xnor_ xnor_test_ft;
        logical__<false_, false_>::xnor_ xnor_test_ff;

        logical__<true_trit, unknown_trit>::and_ and_test_tu_trit;
        logical__<false_trit, true_trit>::and_ and_test_ft_trit;
        logical__<false_trit, unknown_trit>::and_ and_test_fu_trit;
        logical__<false_trit, false_trit>::and_ and_test_uf_trit;

        logical__<true_trit, unknown_trit>::or_ or_test_tu_trit;
        logical__<false_trit, true_trit>::or_ or_test_ft_trit;
        logical__<false_trit, unknown_trit>::or_ or_test_fu_trit;
        logical__<false_trit, false_trit>::or_ or_test_uf_trit;

        logical__<true_trit>::not_ not_test_t_trit;
        logical__<false_trit>::not_ not_test_f_trit;
        logical__<unknown_trit>::not_ not_test_u_trit;

        logical__<true_trit, unknown_trit>::xor_ xor_test_tu_trit;
        logical__<false_trit, true_trit>::xor_ xor_test_ft_trit;
        logical__<false_trit, unknown_trit>::xor_ xor_test_fu_trit;
        logical__<false_trit, false_trit>::xor_ xor_test_uf_trit;

        logical__<true_trit, unknown_trit>::nand_ nand_test_tu_trit;
        logical__<false_trit, true_trit>::nand_ nand_test_ft_trit;
        logical__<false_trit, unknown_trit>::nand_ nand_test_fu_trit;
        logical__<false_trit, false_trit>::nand_ nand_test_uf_trit;

        logical__<true_trit, unknown_trit>::nor_ nor_test_tu_trit;
        logical__<false_trit, true_trit>::nor_ nor_test_ft_trit;
        logical__<false_trit, unknown_trit>::nor_ nor_test_fu_trit;
        logical__<false_trit, false_trit>::nor_ nor_test_uf_trit;

        logical__<true_trit, unknown_trit>::xnor_ xnor_test_tu_trit;
        logical__<false_trit, true_trit>::xnor_ xnor_test_ft_trit;
        logical__<false_trit, unknown_trit>::xnor_ xnor_test_fu_trit;
        logical__<false_trit, false_trit>::xnor_ xnor_test_uf_trit;

        logical__<integer<5>, integer<7>>::and_ and_test_57_int;
        logical__<integer<5>, integer<7>>::or_ or_test_57_int;
        logical__<integer<5>>::not_ not_test_5_int;
        logical__<integer<5>, integer<7>>::xor_ xor_test_57_int;
        logical__<integer<5>, integer<7>>::nand_ nand_test_57_int;
        logical__<integer<5>, integer<7>>::nor_ nor_test_57_int;
        logical__<integer<5>, integer<7>>::xnor_ xnor_test_57_int;

        and__<false_, true_>::type and_test_tf_type;
        and__<false_, false_>::type and_test_ff_type;
        and__<true_, false_>::type and_test_ft_type;
        and__<true_, true_>::type and_test_tt_type;
        and__<unknown_trit, true_trit>::type and_test_ut_type;
        and__<unknown_trit, false_trit>::type and_test_uf_type;
        and__<true_trit, unknown_trit>::type and_test_tu_type;

    }

    // arithmetic operators
    template <typename A0, typename A1 = A0>
    struct arithmetic__
    {
        using type = arithmetic__<void__t, void__t>;
        using template_type = arithmetic__<void__t, void__t>;
        using operator_type = arithmetic__<void__t, void__t>;
    };

    template <integer_t A0, integer_t A1>
    struct arithmetic__<A0, A1>
    {
        using type = arithmetic__<A0, A1>;
        using template_type = arithmetic__<void__t, void__t>;
        using operator_type = arithmetic__<integer_template, integer_template>;

        using add_ = integer<A0::value + A1::value>;
        using sub_ = integer<A0::value - A1::value>;
        using mul_ = integer<A0::value * A1::value>;
        using div_ = integer<A0::value / A1::value>;
        using mod_ = integer<A0::value % A1::value>;
        using abs_ = integer < A0::value<0 ? -A0::value : A0::value>;
    };

    template <integer_t A0>
    struct arithmetic__<A0, integer<0>>
    {
        using type = arithmetic__<A0, integer<0>>;
        using template_type = arithmetic__<void__t, void__t>;
        using operator_type = arithmetic__<integer_template, integer_template>;

        using add_ = A0;
        using sub_ = A0;
        using mul_ = integer<0>;
        using div_ = void__t;
        using mod_ = void__t;
        using abs_ = integer < A0::value<0 ? -A0::value : A0::value>;
    };

    template <typename A0, typename A1>
    struct add__ : arithmetic__<A0, A1>::add_
    {
        using type = typename arithmetic__<A0, A1>::add_;
    };

    template <typename A0, typename A1>
    struct sub__ : arithmetic__<A0, A1>::sub_
    {
        using type = typename arithmetic__<A0, A1>::sub_;
    };

    template <typename A0, typename A1>
    struct mul__ : arithmetic__<A0, A1>::mul_
    {
        using type = typename arithmetic__<A0, A1>::mul_;
    };

    template <typename A0, typename A1>
    struct div__ : arithmetic__<A0, A1>::div_
    {
        using type = typename arithmetic__<A0, A1>::div_;
    };

    template <typename A0, typename A1>
    struct mod__ : arithmetic__<A0, A1>::mod_
    {
        using type = typename arithmetic__<A0, A1>::mod_;
    };

    template <typename A0>
    struct abs__ : arithmetic__<A0, A0>::abs_
    {
        using type = typename arithmetic__<A0, A0>::abs_;
    };

    // arithmetic test
    namespace arithmetic__test
    {
        arithmetic__<integer<5>, integer<7>>::add_ add_test_57_int;
        arithmetic__<integer<5>, integer<7>>::sub_ sub_test_57_int;
        arithmetic__<integer<5>, integer<7>>::mul_ mul_test_57_int;
        arithmetic__<integer<5>, integer<7>>::div_ div_test_57_int;
        arithmetic__<integer<5>, integer<7>>::mod_ mod_test_57_int;
        arithmetic__<integer<-5>>::abs_ abs_test_5_int;

        add__<integer<5>, integer<7>>::type add_test_57_int_type;
        sub__<integer<5>, integer<7>>::type sub_test_57_int_type;
        mul__<integer<5>, integer<7>>::type mul_test_57_int_type;
        div__<integer<5>, integer<7>>::type div_test_57_int_type;
        mod__<integer<5>, integer<7>>::type mod_test_57_int_type;
        abs__<integer<-5>>::type abs_test_5_int_type;
    };

    // comparison operators
    template <typename C0, typename C1>
    struct comparison__
    {
        using type = comparison__<void__t, void__t>;
        using template_type = comparison__<void__t, void__t>;
        using operator_type = comparison__<void__t, void__t>;
    };

    template <integer_t C0, integer_t C1>
    struct comparison__<C0, C1>
    {
        using type = comparison__<C0, C1>;
        using template_type = comparison__<void__t, void__t>;
        using operator_type = comparison__<boolean_template, boolean_template>;

        using equal_ = boolean<(C0::value == C1::value)>;
        using not_equal_ = boolean<(C0::value != C1::value)>;
        using greater_ = boolean<(C0::value > C1::value)>;
        using less_ = boolean<(C0::value < C1::value)>;
        using greater_equal_ = boolean<(C0::value >= C1::value)>;
        using less_equal_ = boolean<(C0::value <= C1::value)>;
    };

    template <typename C0, typename C1>
    struct equal__ : comparison__<C0, C1>::equal_
    {
        using type = typename comparison__<C0, C1>::equal_;
    };

    template <typename C0, typename C1>
    struct not_equal__ : comparison__<C0, C1>::not_equal_
    {
        using type = typename comparison__<C0, C1>::not_equal_;
    };

    template <typename C0, typename C1>
    struct greater__ : comparison__<C0, C1>::greater_
    {
        using type = typename comparison__<C0, C1>::greater_;
    };

    template <typename C0, typename C1>
    struct less__ : comparison__<C0, C1>::less_
    {
        using type = typename comparison__<C0, C1>::less_;
    };

    template <typename C0, typename C1>
    struct greater_equal__ : comparison__<C0, C1>::greater_equal_
    {
        using type = typename comparison__<C0, C1>::greater_equal_;
    };

    template <typename C0, typename C1>
    struct less_equal__ : comparison__<C0, C1>::less_equal_
    {
        using type = typename comparison__<C0, C1>::less_equal_;
    };

    // comparison test
    namespace comparison__test
    {
        comparison__<integer<1>, integer<2>>::equal_ equal_test_57_int;
        comparison__<integer<5>, integer<5>>::not_equal_ not_equal_test_57_int;
        comparison__<integer<5>, integer<7>>::greater_ greater_test_57_int;
        comparison__<integer<4>, integer<7>>::less_ less_test_57_int;
        comparison__<integer<6>, integer<9>>::greater_equal_ greater_equal_test_57_int;
        comparison__<integer<5>, integer<9>>::less_equal_ less_equal_test_57_int;

        equal__<integer<6>, integer<2>>::type equal_test_57_int_type;
        not_equal__<integer<5>, integer<4>>::type not_equal_test_57_int_type;
        greater__<integer<5>, integer<5>>::type greater_test_57_int_type;
        less__<integer<8>, integer<7>>::type less_test_57_int_type;
        greater_equal__<integer<6>, integer<6>>::type greater_equal_test_57_int_type;
        less_equal__<integer<5>, integer<4>>::type less_equal_test_57_int_type;
    };

    // GCD: greatest common divisor

    template <typename G0, typename G1>
    struct gcd__impl
    {
        using type = gcd__impl<void__t, void__t>;
        using template_type = gcd__impl<void__t, void__t>;
        using operator_type = gcd__impl<void__t, void__t>;
    };

    template <integer_t G0, integer_t G1>
    struct gcd__impl<G0, G1>
    {
        using sub_ = sub__<G0, G1>::type;
        using abssub_ = abs__<sub_>::type;

        using type = gcd__impl<G1, abssub_>::type;
        using template_type = gcd__impl<void__t, void__t>;
        using operator_type = gcd__impl<integer_template, integer_template>;
    };

    template <integer_t G0>
    struct gcd__impl<G0, integer<0>>
    {
        using type = G0;
        using template_type = gcd__impl<void__t, void__t>;
        using operator_type = gcd__impl<integer_template, integer_template>;
    };

    template <typename G0, typename G1>
    struct gcd__
    {
        using type = gcd__<void__t, void__t>;
        using template_type = gcd__<void__t, void__t>;
        using operator_type = gcd__<void__t, void__t>;
    };

    template <integer_t G0, integer_t G1>
    struct gcd__<G0, G1>
    {
        using type = gcd__impl<G0, G1>::type;
        using template_type = gcd__<void__t, void__t>;
        using operator_type = gcd__<integer_template, integer_template>;
    };

    // handle 0
    template <integer_t G0>
    struct gcd__<G0, integer<0>>
    {
        using type = void__t;
        using template_type = gcd__<void__t, void__t>;
        using operator_type = gcd__<integer_template, integer_template>;
    };

    template <integer_t G1>
    struct gcd__<integer<0>, G1>
    {
        using type = void__t;
        using template_type = gcd__<void__t, void__t>;
        using operator_type = gcd__<integer_template, integer_template>;
    };

    // LCM: least common multiple
    template <typename L0, typename L1>
    struct lcm__
    {
        using type = lcm__<void__t, void__t>;
        using template_type = lcm__<void__t, void__t>;
        using operator_type = lcm__<void__t, void__t>;
    };

    template <integer_t L0, integer_t L1>
    struct lcm__<L0, L1>
    {
        using type = div__<typename mul__<L0, L1>::type, typename gcd__<L0, L1>::type>::type;
        using template_type = lcm__<void__t, void__t>;
        using operator_type = lcm__<integer_template, integer_template>;
    };

    // handle 0
    template <integer_t L0>
    struct lcm__<L0, integer<0>>
    {
        using type = void__t;
        using template_type = lcm__<void__t, void__t>;
        using operator_type = lcm__<integer_template, integer_template>;
    };

    template <integer_t L1>
    struct lcm__<integer<0>, L1>
    {
        using type = void__t;
        using template_type = lcm__<void__t, void__t>;
        using operator_type = lcm__<integer_template, integer_template>;
    };

    // test gcd and lcm
    namespace gcd_lcm__test
    {
        gcd__<integer<-7>, integer<-21>>::type gcd_test_int;
        lcm__<integer<-7>, integer<-21>>::type lcm_test_int;
    };

    // reduce fraction
    template <typename numerator, typename denominator>
    struct reducted__impl
    {
        using type = reducted__impl<void__t, void__t>;
        using template_type = reducted__impl<void__t, void__t>;
        using operator_type = reducted__impl<void__t, void__t>;
    };
    template <integer_t numerator, nonzero_integer_t denominator>
    struct reducted__impl<numerator, denominator>
    {
        using gcd = typename gcd__<typename numerator, typename denominator>::type;
        // using gcd = integer<gcd_s::value>;
        using type_numerator = integer<(numerator::value / gcd::value)>;
        using type_denominator = integer<(denominator::value / gcd::value)>;
        ////  process the sign
        using type_numerator_s = integer<(type_denominator::value < 0 ? -type_numerator::value : type_numerator::value)>;
        using type_denominator_s = abs__<type_denominator>::type;
        using type = fraction<type_numerator_s::value, type_denominator_s::value>;

        using template_type = reducted__impl<void__t, void__t>;
        using operator_type = reducted__impl<integer<0>, integer<1>>;
    };

    template <integer_t denominator>
    struct reducted__impl<integer<0>, denominator>
    {
        using type = fraction<0, 1>;
        using template_type = reducted__impl<void__t, void__t>;
        using operator_type = reducted__impl<integer<0>, integer<1>>;
    };

    template <integer_t numerator>
    struct reducted__impl<numerator, integer<0>>
    {
        using type = void__t;
        using template_type = reducted__impl<void__t, void__t>;
        using operator_type = reducted__impl<integer<0>, integer<1>>;
    };

    template <>
    struct reducted__impl<integer<0>, integer<0>>
    {
        using type = void__t;
        using template_type = reducted__impl<void__t, void__t>;
        using operator_type = reducted__impl<integer<0>, integer<1>>;
    };

    template <typename F>
    struct reducted__
    {
        using type = reducted__<void__t>;
        using template_type = reducted__<void__t>;
        using operator_type = reducted__<void__t>;
    };

    template <fraction_t F>
    struct reducted__<F>
    {
        using type = reducted__impl<typename F::numerator, typename F::denominator>::type;
        using template_type = reducted__<void__t>;
        using operator_type = reducted__<fraction_template>;
    };

    // test reduce fraction
    namespace reduce_fraction__test
    {
        using redu_0 = reducted__impl<integer<-7>, integer<-21>>;

        redu_0::gcd redu_0_gcd;
        redu_0::type_numerator redu_tn;
        redu_0::type_denominator redu_td;
        redu_0::type_numerator_s redu_tns;
        redu_0::type_denominator_s redu_tds;
        redu_0::type redu_0_type;
        redu_0::template_type redu_0_template_type;
        redu_0::operator_type redu_0_operator_type;

    };

    template <fraction_t F0, nonzero_fraction_t F1>
    struct arithmetic__<F0, F1>
    {
        using type = arithmetic__<F0, F1>;
        using template_type = arithmetic__<void__t, void__t>;
        using operator_type = arithmetic__<fraction_template, fraction_template>;

        using lcm_denominator = lcm__<typename F0::denominator, typename F1::denominator>::type;

        using F0_numerator = integer<(F0::numerator::value * (lcm_denominator::value / F0::denominator::value))>;
        using F1_numerator = integer<(F1::numerator::value * (lcm_denominator::value / F1::denominator::value))>;

        using add_numerator = integer<(F0_numerator::value + F1_numerator::value)>;
        using sub_numerator = integer<(F0_numerator::value - F1_numerator::value)>;
        using mul_numerator = integer<(F0::numerator::value * F1::numerator::value)>;
        using mul_denominator = integer<(F0::denominator::value * F1::denominator::value)>;
        using div_numerator = integer<(F0::numerator::value * F1::denominator::value)>;
        using div_denominator = integer<(F0::denominator::value * F1::numerator::value)>;

        using add_unreducted = fraction<add_numerator::value, lcm_denominator::value>;
        using sub_unreducted = fraction<sub_numerator::value, lcm_denominator::value>;
        using mul_unreducted = fraction<mul_numerator::value, mul_denominator::value>;
        using div_unreducted = fraction<div_numerator::value, div_denominator::value>;

        using add_ = typename reducted__<add_unreducted>::type;
        using sub_ = typename reducted__<sub_unreducted>::type;
        using mul_ = typename reducted__<mul_unreducted>::type;
        using div_ = typename reducted__<div_unreducted>::type;

        using as_integer = integer<(F0::numerator::value / F0::denominator::value)>;

        // devide to an integer quotient and a fraction remainder
        using quot_ = integer<div_unreducted::numerator::value / div_unreducted::denominator::value>;
        using rem_n_unreducted = fraction<div_unreducted::numerator::value % div_unreducted::denominator::value, div_unreducted::denominator::value>;
        using rem_ = arithmetic__<rem_n_unreducted, F1>::mul_;

        using abs_ = void__t;
    };

    template <typename F0, typename F1>
    struct quot__
    {
        using type = arithmetic__<F0, F1>::quot_;
        using template_type = quot__<void__t, void__t>;
        using operator_type = quot__<typename F0::template_type, typename F1::template_type>;
    };

    template <typename F0, typename F1>
    struct rem__
    {
        using type = arithmetic__<F0, F1>::rem_;
        using template_type = rem__<void__t, void__t>;
        using operator_type = rem__<typename F0::template_type, typename F1::template_type>;
    };

    template <fraction_t F0>
    struct arithmetic__<F0, F0>
    {
        using type = arithmetic__<F0, F0>;
        using template_type = arithmetic__<void__t, void__t>;
        using operator_type = arithmetic__<fraction_template, fraction_template>;

        using add_ = typename reducted__<fraction<F0::numerator::value * 2, F0::denominator::value>>::type;
        using sub_ = fraction<0, 1>;
        using mul_ = typename reducted__<fraction<F0::numerator::value * F0::numerator::value, F0::denominator::value * F0::denominator::value>>::type;
        using div_ = fraction<1, 1>;
        using as_integer = integer<1>;
        using quot_ = integer<1>;
        using rem_ = fraction<0, 1>;
        using abs_ = fraction<(F0::numerator::value < 0 ? -F0::numerator::value : F0::numerator::value),
                              (F0::denominator::value < 0 ? -F0::denominator::value : F0::denominator::value)>;
    };
    namespace arithmetic__test
    {
        using f0 = fraction<-3, 4>;
        using f1 = fraction<1, 6>;
        using ar0 = arithmetic__<fraction<-3, 4>, fraction<1, 6>>;

        ar0::add_ ar0_add;
        ar0::sub_ ar0_sub;
        ar0::mul_ ar0_mul;
        ar0::div_ ar0_div;
        ar0::as_integer ar0_as_integer;
        ar0::abs_ ar0_abs;
        ar0::quot_ ar0_quot;
        ar0::rem_ ar0_rem;

        add__<f0, f1>::type add_test_f0f1_type;
        sub__<f0, f1>::type sub_test_f0f1_type;
        mul__<f0, f1>::type mul_test_f0f1_type;
        div__<f0, f1>::type div_test_f0f1_type;
        abs__<f0>::type abs_test_f0_type;
        quot__<f0, f1>::type quot_test_f0f1_type;
        rem__<f0, f1>::type rem_test_f0f1_type;
    };

    template <typename base, typename exponent, typename result>
    struct power__impl
    {
    };
    // power impl integer**integer
    template <integer_t base, positive_integer_t exponent, integer_t result>
    struct power__impl<base, exponent, result>
    {
        using next_exponent = integer<exponent::value - 1>;
        using next_result = integer<result::value * base::value>;
        using type = power__impl<base, next_exponent, next_result>::type;
        using template_type = power__impl<void__t, void__t, void__t>;
        using operator_type = power__impl<integer_template, integer_template, integer_template>;
    };

    template <integer_t base, integer_t result>
    struct power__impl<base, integer<0>, result>
    {
        using type = result;
        using template_type = power__impl<void__t, void__t, void__t>;
        using operator_type = power__impl<integer_template, integer_template, integer_template>;
    };

    template <integer_t base, negative_integer_t exponent, fraction_t result>
    struct power__impl<base, exponent, result>
    {
        using next_exponent = integer<-exponent::value>;
        using next_result = fraction<1, 1>;
        using next_base = fraction<1, base::value>;
        using type = power__impl<next_base, next_exponent, next_result>::type;
        using template_type = power__impl<void__t, void__t, void__t>;
        using operator_type = power__impl<integer_template, integer_template, fraction_template>;
    };

    // power impl fraction**integer
    template <fraction_t base, positive_integer_t exponent, fraction_t result>
    struct power__impl<base, exponent, result>
    {
        using next_exponent = integer<exponent::value - 1>;
        using next_result = fraction<result::numerator_ * base::numerator_,
                                     result::denominator_ * base::denominator_>;
        using type = power__impl<base, next_exponent, next_result>::type;
        using template_type = power__impl<void__t, void__t, void__t>;
        using operator_type = power__impl<fraction_template, integer_template, fraction_template>;
    };

    template <fraction_t base, fraction_t result>
    struct power__impl<base, integer<0>, result>
    {
        using type = reducted__<result>::type;
        using template_type = power__impl<void__t, void__t, void__t>;
        using operator_type = power__impl<fraction_template, integer_template, fraction_template>;
    };

    template <fraction_t base, negative_integer_t exponent, fraction_t result>
    struct power__impl<base, exponent, result>
    {
        using next_exponent = integer<-exponent::value>;
        using next_result = result;
        using next_base = fraction<base::denominator_, base::numerator_>;
        using type = power__impl<next_base, next_exponent, next_result>::type;
        using template_type = power__impl<void__t, void__t, void__t>;
        using operator_type = power__impl<fraction_template, integer_template, fraction_template>;
    };

    // power
    template <typename base, typename exponent>
    struct power__
    {
        using type = power__impl<base,
                                 exponent,
                                 typename base::mul_base>::type;
        using template_type = power__<void__t, void__t>;
        using operator_type = power__<typename base::template_type, typename exponent::template_type>;
    };

    // test power
    namespace power__test
    {
        power__<integer<4>, integer<3>>::type power_test_int;
        power__<fraction<2, 4>, integer<3>>::type power_test_frac;
    };

    // exponential express
    template <typename base, typename exponent>
    struct exp__
    {
        using type = exp__<base, exponent>;
        using template_type = exp__<void__t, void__t>;
        using operator_type = exp__<typename base::template_type, typename exponent::template_type>;
    };

    template <integer_t base, zero_or_positive_integer_t exponent>
    struct exp__<base, exponent>
    {
        using type = power__<base, exponent>::type;
        using template_type = exp__<void__t, void__t>;
        using operator_type = exp__<integer_template, integer_template>;
    };

    template <fraction_t base, zero_or_positive_integer_t exponent>
    struct exp__<base, exponent>
    {
        using type = power__<base, exponent>::type;
        using template_type = exp__<void__t, void__t>;
        using operator_type = exp__<fraction_template, integer_template>;
    };

    template <integer_t base, negative_integer_t exponent>
    struct exp__<base, exponent>
    {
        using type = power__<fraction<1,base::value>, integer<-exponent::value>>::type;
        using template_type = exp__<void__t, void__t>;
        using operator_type = exp__<integer_template, integer_template>;
    };

    template <nonzero_fraction_t base, negative_integer_t exponent>
    struct exp__<base, exponent>
    {
        using type = power__<fraction<base::denominator_, base::numerator_>, integer<-exponent::value>>::type;
        using template_type = exp__<void__t, void__t>;
        using operator_type = exp__<integer_template, integer_template>;
    };

    // test exp
    namespace exp__test
    {
        exp__<integer<2>, integer<-1>>::type test0;
        exp__<decltype(test0), integer<-2>>::type test1;
    };

#ifdef namespace_bionukg
};
#endif // namespace_bionukg

#endif // !bionukg_metatemplate_h
