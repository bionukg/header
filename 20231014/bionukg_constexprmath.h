#pragma once
#ifndef bionukg_constexprmath_h
#define bionukg_constexprmath_h
// many math functions in cmath are not constexpr, so we need to make our own
// basic constexpr math operations:
// for integer: +, -, *, /, %, <<,>>, &, |, ^, ~, !, <, >, <=, >=, ==, !=
// for floating point: +, -, *, /, <, >, <=, >=, ==, !=

#ifdef namespace_bionukg
namespace bionukg
{
#endif
#ifdef namespace_bionukg_constexprmath
    namespace constexprmath
    {
#define constexpr_decl(function_name, return_type, ...) \
    constexpr return_type function_name##__(__VA_ARGS__) noexcept
#define constexpr_name(function_name) function_name##__

#else
#define constexpr_decl(return_type, function_name, ...) \
    constexpr return_type function_name##_constexpr(__VA_ARGS__) noexcept
#define constexpr_name(function_name) function_name##_constexpr
#endif

#include "./bionukg_mathdatastruct.h"
        // abs
        template <typename T>
        constexpr_decl(T, abs, T x)
        {
            return x < 0 ? -x : x;
        }

        // min max
        template <typename T>
        constexpr_decl(T, min, T x, T y)
        {
            return x < y ? x : y;
        }
        template <typename T>
        constexpr_decl(T, max, T x, T y)
        {
            return x > y ? x : y;
        }

        template <typename float_type = double>
        constexpr_decl(float_type, sqrt, float_type x)
        {
            if (x <= 0)
                return 0; // error code for negative input

            float_type curr = x, prev = 0;
            while (curr != prev)
            {
                prev = curr;
                curr = (curr + x / curr) / 2;
            }
            return curr;
        }
        float sqrt_float32_(float x) noexcept
        {
            if (x <= 0)
                return 0; // error code for negative input
            float curr, prev;

            {
                float32_ f32 = x;
                f32.exponent = uint32_t((int32_t(f32.exponent) - f32.exponent_offset) / 2 + f32.exponent_offset);
                curr = f32.float_val;
                prev = 0;
            }

            while (curr != prev)
            {
                prev = curr;
                curr = (curr + x / curr) /2;
                //printf("curr:%3.3e,\tprev:%3.3e,\tdelta:%3.3e\n", curr, prev, curr - prev);
            }
            //printf("\n");

            return curr;
        }

        // pow by integer
        template <typename float_type, typename uint_type = size_t>
        constexpr_decl(float_type, pow, float_type x, size_t n)
        {
            float_type result = 1;
            for (size_t i = 0; i < n; ++i)
            {
                result *= x;
            }
            return result;
        }

        // sum of a series
        constexpr_decl(double, sum, double (*f)(size_t), size_t end, size_t start = 0)
        {
            double result = 0.0;
            for (size_t i = start; i < end; ++i)
            {
                result += f(i);
            }
            return result;
        }

        constexpr double sum_byhalf(size_t start, size_t end, double (*f)(size_t))
        {
            size_t half = (start + end) / 2;
            return start == end ? f(start) : sum_byhalf(start, half, f) + sum_byhalf(half + 1, end, f);
        };

        // factorial
        template <size_t N = 21>
        constexpr_decl(auto, factorial)
        {
            static_assert(N <= 21, "factorial overflow");
            struct
            {
                size_t factorials[N];
            } ret;
            ret.factorials[0] = 1;
            for (size_t i = 1; i < N; ++i)
            {
                ret.factorials[i] = ret.factorials[i - 1] * i;
            }
            return ret;
        }

        enum PI_formula
        {
            PI_formula_Leibniz,   // bad:(pi/4) = 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + ...
            PI_formula_Newton,    // good:(pi/6) = 1/2 + 1/2*1/3 + 1/2*1/3*2/4 + 1/2*1/3*2/4*3/5 + ...
            PI_formula_Fouier,    // bad:(pi^2/8) = 1/1^2 + 1/3^2 + 1/5^2 + 1/7^2 + ...
            PI_formula_Riemann,   // not bad:(pi^4/90) = 1/1^4 + 1/2^4 + 1/3^4 + 1/4^4 + ...
            PI_formula_Ramanujan, //(1/pi) = (2*sqrt2)/9801*
            // sum((4n)!/(n!)^4* (26390n+1103)/396^(4n),n=0,inf)
        };

        template <typename float_type = double, int Nrep = 65536, PI_formula formula = PI_formula::PI_formula_Newton>
        constexpr_decl(float_type, pi, bool directaddmode = true)
        {
            float_type pi = 0.0, lastpi = 0.0;
            float_type add = 0.5;
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            switch (formula)
            {
            case PI_formula::PI_formula_Leibniz: // bad
                //(pi/4) = 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + ...
                for (size_t i = 0; i < N; ++i)
                {
                    lastpi = pi;
                    pi += (i & 1 ? -1.0 : 1.0) / (2 * i + 1);
                    if (pi == lastpi)
                        break;
                }
                return pi * 4;
            case PI_formula::PI_formula_Newton: // good 23
                //(pi/6) = sum(adder(i),i=1,N)
                // adder(i) = prod((2 * n + 1)/(8 * n), n = 1, i) / ((2 * n + 1) *(2 * n + 1)* 2)
                add = 0.5;
                pi += add;
                if (directaddmode)
                {
                    for (size_t i = 1; i < N; ++i)
                    {
                        lastpi = pi;
                        float_type n = float_type(i);
                        float_type _n2_1 = 2 * n + 1;
                        add *= (_n2_1) / (n * 8);
                        _n2_1 *= _n2_1;
                        pi += add / _n2_1;

                        if (pi == lastpi)
                            break;
                    }
                    return pi * 6;
                }
                else
                {
                    auto adder = [](size_t N) -> float_type
                    {
                        float_type add = 0.5;
                        float_type _n2_1 = 1;
                        for (size_t i = 1; i < N; ++i)
                        {
                            float_type n = float_type(i);
                            _n2_1 = 2 * n + 1;
                            add *= (_n2_1) / (n * 8);
                        }
                        _n2_1 *= _n2_1;
                        return add / _n2_1;
                    };
                    // 32 is enough
                    auto N32 = N > 32 ? 32 : N;
                    return sum_byhalf(1, N32, adder) * 6;
                }
            case PI_formula::PI_formula_Fouier: // bad
                //(pi^2/8) = 1/1^2 + 1/3^2 + 1/5^2 + 1/7^2 + ...
                for (size_t i = 0; i < N; ++i)
                {
                    lastpi = pi;
                    float_type n = float_type(i);
                    n = 2 * n + 1;
                    pi += 1.0 / (n * n);
                    if (pi == lastpi)
                        break;
                }
                return constexpr_name(sqrt)(pi * 8);
            case PI_formula::PI_formula_Riemann: // not bad
                //(pi^4/90) = 1/1^4 + 1/2^4 + 1/3^4 + 1/4^4 + ...
                // i=9742 end;
                if (directaddmode)
                {
                    for (size_t i = 1; i < N; ++i)
                    {
                        lastpi = pi;
                        float_type n = float_type(i);
                        n *= n;
                        n *= n;
                        pi += 1.0 / n;
                        if (pi == lastpi)
                            break;
                    }
                    return constexpr_name(sqrt)(
                        constexpr_name(sqrt)(
                            pi * 90));
                }
                else
                {
                    auto N65536 = N > 65536 ? 65536 : N;
                    auto adder_riemann = [](size_t i) -> float_type
                    {
                        float_type n = float_type(i);
                        n *= n;
                        n *= n;
                        return 1.0 / n;
                    };

                    return constexpr_name(sqrt)(
                        constexpr_name(sqrt)(
                            sum_byhalf(1, N65536, adder_riemann) * 90));
                }
            case PI_formula::PI_formula_Ramanujan:
                //(1/pi) = (2*sqrt2)/9801*
                // sum((4n)!/(n!)^4* (26390n+1103)/396^(4n),n=0,inf)
                constexpr float_type Ramanujan_constant = 2 * constexpr_name(sqrt)(2) / 9801;
                for (size_t i = 0; i < N; ++i)
                {
                    lastpi = pi;
                    float_type n = (float_type)i;

                    float_type fa_4n = (float_type)constexpr_name(factorial)().factorials[4 * i];
                    float_type fa_n4 = (float_type)constexpr_name(factorial)().factorials[i];
                    fa_n4 *= fa_n4;
                    fa_n4 *= fa_n4;
                    float_type fn = 26390 * n + 1103;
                    float_type fd = constexpr_name(pow)<float_type>(float_type(396), 4 * i);
                    pi += fa_4n / fa_n4 * fn / fd;
                    if (pi == lastpi)
                        break;
                }
                pi *= Ramanujan_constant;
                return 1.0 / pi;
            }
            return 0;
        }

        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, e, bool directaddmode = true)
        { // e = sum(1/n!,n=0,inf)
            float_type e = 0.0, laste = 0.0;
            float_type add = 1.0;
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            if (directaddmode)
            {
                for (size_t i = 0; i < N; ++i)
                {
                    laste = e;
                    e += add;
                    add /= i + 1;
                    if (e == laste)
                        break;
                }
                return e;
            }
            else
            {
                auto N65536 = N > 65536 ? 65536 : N;
                auto adder_e = [](size_t i) -> float_type
                {
                    float_type add = 1.0;
                    for (size_t j = 1; j <= i; ++j)
                    {
                        add /= j;
                    }
                    return add;
                };

                return sum_byhalf(0, N65536 - 1, adder_e);
            }
            return e;
        }

        // exp(x) x= 0~1
        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, exp_0_to_1, float_type _0_to_1)
        { // e^x = sum(x^n/n!,n=0,inf)
            float_type e = float_type(0), laste = float_type(0);
            float_type add = float_type(1);
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            size_t i;
            // 0<exp_0_to_1<1
            if (_0_to_1 < (float_type)0 || _0_to_1 > (float_type)1)
                return (float_type)0;

            for (i = 0; i < N; ++i)
            {
                laste = e;
                e += add;
                add *= _0_to_1 / (i + 1);
                if (e == laste)
                    break;
            }

            return e;
        }

        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, exp, float_type x)
        { // e^(2*x) = (e^x)*(e^x)
            size_t cnt = 0;
            bool x_neg = false;
            if (x < float_type(0))
            {
                x_neg = true;
                x = -x;
            }
            while (x > float_type(1))
            {
                x /= 2;
                ++cnt;
            }
            float_type exp = constexpr_name(exp_0_to_1)<Nrep>(x);
            if (x_neg)
            {
                exp = 1 / exp;
            }
            for (size_t i = 0; i < cnt; i++)
            {
                exp *= exp;
            }
            return exp;
        };

        // ln(x+1) x= 0.75~1.5
        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, ln_xadd1, float_type x_add1)
        { // ln(x+1) = sum((-1)^n*x^(n+1)/(n+1),n=0,inf)=x-x^2/2+x^3/3-x^4/4+...

            // do not accept too large x or small x
            //-0.25 <= 1+x <= 0.5
            // so 0.75 <= x <= 1.5

            if (x_add1 < float_type(-0.25) || x_add1 > float_type(0.5))
                return 0;

            float_type ln = 0.0, lastln = 0.0;
            float_type add = x_add1;
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            for (size_t i = 0; i < N; ++i)
            {
                lastln = ln;
                ln += add / (i + 1);
                add *= -x_add1;
                if (ln == lastln)
                    break;
            }
            return ln;
        }

        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, ln, float_type x)
        { // ln(2*x) = ln(x)+ln(2)
            auto ln_x = [](float_type x) -> float_type
            {
                return constexpr_name(ln_xadd1)<Nrep>(x - 1);
            };

            constexpr float_type sqrt2 = constexpr_name(sqrt)(float_type(2));
            constexpr float_type ln2 = ln_x(sqrt2) * 2;
            size_t cnt = 0;
            // error
            if (x <= float_type(0))
                return 0;
            else if (x < float_type(0.75))
            {
                cnt = 0;
                while (x < float_type(0.75))
                {
                    x *= 2;
                    ++cnt;
                }
                return ln_x(x) - ln2 * cnt;
            }
            else
            {
                cnt = 0;
                while (x > float_type(1.5))
                {
                    x /= 2;
                    ++cnt;
                }
                return ln_x(x) + ln2 * cnt;
            }
        };

        // pow by float
        template <typename float_type>
        constexpr_decl(float_type, pow, float_type x, float_type y)
        {
            if (y == 0)
                return 1;
            else if (y == 1)
                return x;
            else if (x == 0)
                return 0;
            else if (x == 1)
                return 1;
            else if (y == 0.5)
                return constexpr_name(sqrt)(x);
            else // x^y = e^(y*ln(x))
                return constexpr_name(exp)(y * constexpr_name(ln)(x));
        }

        // log by float
        template <typename float_type>
        constexpr_decl(float_type, log, float_type x, float_type y)
        {
            if (x == 0 || y == 0)
                return 0;
            else if (x == 1)
                return 0;
            else if (y == 1)
                return 0;
            else if (x == y)
                return 1;
            else // log(x,y) = ln(y)/ln(x)
                return constexpr_name(ln)(y) / constexpr_name(ln)(x);
        }

        // sin x
        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, sin_0_to_halfpi, float_type x)
        {
            // sin(x) = sum((-1)^n*x^(2n+1)/(2n+1)!,n=0,inf)=x-x^3/3!+x^5/5!-x^7/7!+...
            // 0 <= x <= pi/2
            float_type sin = 0.0, lastsin = 0.0;
            float_type add = x;
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            for (size_t i = 0; i < N; ++i)
            {
                lastsin = sin;
                sin += add;
                add *= -x * x / ((2 * i + 2) * (2 * i + 3));
                if (sin == lastsin)
                    break;
            }
            return sin;
        }

        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, sin, float_type x)
        { // sin(-x)= -sin(x)
            bool x_neg = false;
            if (x < 0)
            {
                x_neg = !x_neg;
                x = -x;
            }
            float_type pi = constexpr_name(pi)<Nrep, float_type>();
            float_type halfpi = pi / 2;
            float_type twopi = pi * 2;
            while (x > pi)
            {
                x -= twopi;
            } // -pi <= x <= pi
            if (x < 0)
            {
                x = -x;
                x_neg = !x_neg;
            } // 0 <= x <= pi
            if (x > halfpi)
            {
                x = pi - x;
            } // 0 <= x <= pi/2
            auto ret = constexpr_name(sin_0_to_halfpi)<Nrep>(x);
            if (x_neg)
            {
                ret = -ret;
            }
            return ret;
        }

        // cos x
        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, cos_0_to_halfpi, float_type x)
        {
            // cos(x) = sum((-1)^n*x^(2n)/(2n)!,n=0,inf)=1-x^2/2!+x^4/4!-x^6/6!+...
            // 0 <= x <= pi/2
            float_type cos = 0.0, lastcos = 0.0;
            float_type add = 1.0;
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            for (size_t i = 0; i < N; ++i)
            {
                lastcos = cos;
                cos += add;
                add *= -x * x / ((2 * i + 1) * (2 * i + 2));
                if (cos == lastcos)
                    break;
            }
            return cos;
        }

        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, cos, float_type x)
        { // cos(-x)= cos(x)
            bool x_neg = false;
            if (x < 0)
                x = -x;
            float_type pi = constexpr_name(pi)<Nrep, float_type>();
            float_type halfpi = pi / 2;
            float_type twopi = pi * 2;
            while (x > pi)
            {
                x -= twopi;
            } // -pi <= x <= pi
            if (x < 0)
            {
                x = -x;
            } // 0 <= x <= pi
            if (x > halfpi)
            {
                x = pi - x;
                x_neg = !x_neg;
            } // 0 <= x <= pi/2
            auto ret = constexpr_name(cos_0_to_halfpi)<Nrep>(x);
            if (x_neg)
            {
                ret = -ret;
            }
            return ret;
        }

        // arcsin x
        //  arcsin x
        template <int Nrep = 65536, typename float_type = double>
        constexpr_decl(float_type, asin_seris, float_type x)
        {
            // arcsin(x) = sum((2n)!/(4^n*(n!)^2)*(x^(2n+1)/(2n+1)),n=0,inf)=x+x^3/6+x^5*3/40+x^7*5/112+...
            // -1 <= x <= 1

            if (x < -1 || x > 1)
                return 0;

            float_type asin = 0.0, lastasin = 0.0;
            float_type add = x;
            size_t N = Nrep < 1 ? ~size_t(0) : Nrep;
            size_t i = 0;
            for (; i < N; ++i)
            {
                lastasin = asin;
                float_type n = float_type(i);
                asin += add / (2 * n + 1);
                // add *= x * x * (2 * n + 1) * (2 * n + 2) ;
                // add /= 4 * (n + 1) * (n + 1);
                // simplify
                // add *= x * x * (2 * n + 1) * 2 * (n + 1);
                // add /= 4 * (n + 1) * (n + 1);
                // eliminate 2 and n+1
                // add *= x * x * (2 * n + 1);
                // add /= 2 * (n + 1);
                // combine
                add *= x * x * (2 * n + 1) / (2 * (n + 1));
                if (asin == lastasin)
                    break;
            }
            return asin;
        }

#ifdef namespace_bionukg_constexprmath
    };
#endif // namespace_bionukg_constexprmath
#ifdef namespace_bionukg
};
#endif // namespace_bionukg
#endif // BIONUKG_CONSTEXPRMATH_H
