#pragma once
#ifndef bionukg_SVGraphic_h
#define bionukg_SVGraphic_h


#include <cmath>
#include <string>
#include <concepts>
#include "./bionukg_constexprmath.h"
#ifdef namespace_bionukg
namespace bionukg
{
#endif // namespace_bionukg

    // point
    template <typename data_type>
    class svg_point_t;
    using svg_point = svg_point_t<double>;
    // line
    template <typename data_type>
    class svg_line_t;
    using svg_line = svg_line_t<double>;
    // concept for shapes:
    template <typename T>
    concept svg_shape_concept = requires(T a, const T &const_a) {
        {
            const_a.contains(svg_point_t<T>())
        } -> std::same_as<bool>;
    };

    template <typename data_type>
    class svg_circle_t;
    using svg_circle = svg_circle_t<double>;

    // ellipse
    template <typename data_type>
    class svg_ellipse_t;
    using svg_ellipse = svg_ellipse_t<double>;
    // hyperbola
    template <typename data_type>
    class svg_hyperbola_t;
    using svg_hyperbola = svg_hyperbola_t<double>;

    // convex polygon
    template <typename data_type, int edges = 3>
    class svg_polygon_t;
    template <typename data_type>
    class svg_triangle_t;
    using svg_quadrangle = svg_polygon_t<double, 4>;

    // composite shapes of logic operations

    template <typename data_type>
    class svg_logic_t;
    using svg_logic = svg_logic_t<double>;

    template <typename data_type>
    class svg_logic_and_t;
    using svg_logic_and = svg_logic_and_t<double>;
    using svg_and = svg_logic_and_t<double>;
    template <typename data_type>
    class svg_logic_or_t;
    using svg_logic_or = svg_logic_or_t<double>;
    using svg_or = svg_logic_or_t<double>;
    template <typename data_type>
    class svg_logic_xor_t;
    using svg_logic_xor = svg_logic_xor_t<double>;
    using svg_xor = svg_logic_xor_t<double>;
    template <typename data_type>
    class svg_logic_not_t;
    using svg_logic_not = svg_logic_not_t<double>;

    // implementations

    template <typename data_type>
    class svg_point_t
    {
    public:
        data_type x;
        data_type y;

        constexpr svg_point_t(data_type x_, data_type y_ = 0)
            : x(x_), y(y_){};
        constexpr svg_point_t(const svg_point_t<data_type> &p)
            : x(p.x), y(p.y){};
        constexpr svg_point_t()
            : x(0), y(0){};

        template <typename svg_shape_concept>
        constexpr bool inside(const svg_shape_concept &s) const noexcept
        {
            return s.contains(*this);
        }

        constexpr data_type squared_distance_to_constexpr(data_type x_, data_type y_) const noexcept
        {
            return (x - x_) * (x - x_) + (y - y_) * (y - y_);
        }

        constexpr data_type squared_distance_to_constexpr(const svg_point_t<data_type> &p) const noexcept
        {
            return squared_distance_to_constexpr(p.x, p.y);
        }

        constexpr data_type distance_to_constexpr(data_type x_, data_type y_) const noexcept
        {
            return constexpr_name(sqrt)(squared_distance_to_constexpr(x_, y_));
        }

        constexpr data_type distance_to_constexpr(const svg_point_t<data_type> &p) const noexcept
        {
            return distance_to_constexpr(p.x, p.y);
        }

        data_type distance_to(data_type x_, data_type y_) const noexcept
        {
            return sqrt(squared_distance_to_constexpr(x_, y_));
        }
        data_type distance_to(const svg_point_t<data_type> &p) const noexcept
        {
            return distance_to(p.x, p.y);
        }

        constexpr svg_point_t<data_type> shift(data_type dx, data_type dy) const noexcept
        {
            return svg_point_t<data_type>(x + dx, y + dy);
        }

        constexpr svg_point_t<data_type> shift(const svg_point_t<data_type> &p) const noexcept
        {
            return shift(p.x, p.y);
        }

        constexpr svg_point_t<data_type> rotate(data_type angle, const svg_point_t<data_type> &center = svg_point_t<data_type>()) const noexcept
        {
            // clockwise is positive
            data_type cos_a = constexpr_name(cos)(angle);
            data_type sin_a = constexpr_name(sin)(angle);
            return svg_point_t<data_type>(center.x + (x - center.x) * cos_a - (y - center.y) * sin_a,
                                          center.y + (x - center.x) * sin_a + (y - center.y) * cos_a);
        }
    };

    template <typename data_type>
    class svg_edge_t
    {
    public:
        virtual svg_edge_t *shift_ptr(const svg_point_t<data_type> &p) const noexcept = 0;
        virtual bool contains(data_type x, data_type y) const noexcept = 0;
        bool contains(const svg_point_t<data_type>& p) const noexcept
        {
            return contains(p.x, p.y);
        }
    };

    template <typename data_type>
    class svg_shape_t : public svg_edge_t<data_type>
    {
    public:
        virtual data_type area() const noexcept = 0;
    };

    template <typename data_type>
    class svg_line_t : public svg_edge_t<data_type>
    {
    public:
        svg_point_t<data_type> start;
        svg_point_t<data_type> end;
        // Ax + By + C = 0
        constexpr data_type A_() const noexcept
        {
            return end.y - start.y;
        }
        constexpr data_type B_() const noexcept
        {
            return start.x - end.x;
        }
        constexpr data_type C_() const noexcept
        {
            return end.x * start.y - start.x * end.y;
        }

        constexpr svg_line_t(const svg_point_t<data_type> &start_, const svg_point_t<data_type> &end_)
            : start(start_), end(end_){};

        constexpr svg_line_t<data_type> midperpendicular() const noexcept
        {
            auto midpoint = svg_point_t<data_type>((start.x + end.x) / 2, (start.y + end.y) / 2);
            return svg_line_t<data_type>(svg_point_t<data_type>(midpoint.x + A_() / 2, midpoint.y + B_() / 2),
                                         svg_point_t<data_type>(midpoint.x - A_() / 2, midpoint.y - B_() / 2));
        }

        constexpr svg_point_t<data_type> intersection(const svg_line_t<data_type> &l) const noexcept
        {
            data_type det = A_() * l.B_() - l.A_() * B_();
            return svg_point_t<data_type>((B_() * l.C_() - l.B_() * C_()) / det, (l.A_() * C_() - A_() * l.C_()) / det);
        }

        constexpr svg_line_t<data_type> shift(data_type dx, data_type dy) const noexcept
        {
            return svg_line_t<data_type>(start.shift(dx, dy), end.shift(dx, dy));
        }

        constexpr svg_line_t<data_type> shift(const svg_point_t<data_type> &p) const noexcept
        {
            return shift(p.x, p.y);
        }

        virtual svg_line_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_line_t<data_type>(start.shift(p), end.shift(p));
        }

        constexpr bool parallel_to(const svg_line_t<data_type> &l, double err = 1e-10) const noexcept
        {
            auto det = A_() * l.B_() - l.A_() * B_();
            return det * det < err;
        }

        constexpr bool perpendicular_to(const svg_line_t<data_type> &l, double err = 1e-10) const noexcept
        {
            auto det = A_() * l.A_() + B_() * l.B_();
            return det * det < err;
        }

        constexpr data_type squared_distance_to_constexpr(data_type x_, data_type y_) const noexcept
        {
            data_type det = A_() * x_ + B_() * y_ + C_();
            return det * det / (A_() * A_() + B_() * B_());
        }

        constexpr data_type squared_distance_to_constexpr(const svg_point_t<data_type> &p) const noexcept
        {
            return squared_distance_to_constexpr(p.x, p.y);
        }

        constexpr data_type distance_to_constexpr(data_type x_, data_type y_) const noexcept
        {
            return constexpr_name(abs)(A_() * x_ + B_() * y_ + C_()) / constexpr_name(sqrt)(A_() * A_() + B_() * B_());
        }
        constexpr data_type distance_to_constexpr(const svg_point_t<data_type> &p) const noexcept
        {
            return distance_to_constexpr(p.x, p.y);
        }
        constexpr static data_type distance_to_constexpr(const svg_point_t<data_type> &l_start, const svg_point_t<data_type> &l_end, const svg_point_t<data_type> &p) noexcept
        {
            return svg_line_t<data_type>(l_start, l_end).distance_to_constexpr(p);
        }

        data_type distance_to(data_type x_, data_type y_) const noexcept
        {
            return abs(A_() * x_ + B_() * y_ + C_()) / sqrt(A_() * A_() + B_() * B_());
        }
        data_type distance_to(const svg_point_t<data_type> &p) const noexcept
        {
            return distance_to(p.x, p.y);
        }
        static data_type distance_to(const svg_point_t<data_type> &l_start, const svg_point_t<data_type> &l_end, const svg_point_t<data_type> &p) noexcept
        {
            return svg_line_t<data_type>(l_start, l_end).distance_to(p);
        }

        constexpr bool side(data_type x, data_type y) const noexcept
        {
            return A_() * x + B_() * y + C_() > 0;
        }
        constexpr bool side(const svg_point_t<data_type> &p) const noexcept 
        {
            return side(p.x, p.y);
        }
        virtual constexpr bool contains(data_type x, data_type y) const noexcept override
        {
            return side(x, y);
        }

        constexpr static bool side(const svg_point_t<data_type> &l_start, const svg_point_t<data_type> &l_end, const svg_point_t<data_type> &p) noexcept
        {
            return (l_end.y - l_start.y) * p.x + (l_start.x - l_end.x) * p.y + (l_end.x * l_start.y - l_start.x * l_end.y) > 0;
        }
    };

    template <typename data_type>
    class svg_circle_t : public svg_shape_t<data_type>
    {
    public:
        data_type cx;
        data_type cy;
        data_type r_2;

        enum circle_init_type
        {
            center_radius,
            center_pointOnEdge,
            lineCrossCenter_2PointsOnEdge,
            _3PointsOnEdge,
            radius_2PointsOnEdge_side,

        };

        // center and radius
        constexpr svg_circle_t(data_type cx_, data_type cy_, data_type r_)
            : cx(cx_), cy(cy_), r_2(r_ * r_){};
        constexpr svg_circle_t(const svg_point_t<data_type> &center, data_type r_)
            : cx(center.x), cy(center.y), r_2(r_ * r_){};
        // shift
        constexpr svg_circle_t<data_type> shift(data_type dx, data_type dy) const noexcept
        {
            return svg_circle_t<data_type>(cx + dx, cy + dy, r_2);
        }

        constexpr svg_circle_t<data_type> shift(const svg_point_t<data_type> &p) const noexcept
        {
            return shift(p.x, p.y);
        }

        virtual svg_circle_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_circle_t<data_type>(cx + p.x, cy + p.y, r_2);
        }

        virtual constexpr bool contains(data_type x, data_type y) const noexcept override
        {
            return (x - cx) * (x - cx) + (y - cy) * (y - cy) < r_2 * r_2;
        }

        virtual constexpr data_type area() const noexcept override
        {
            return r_2 * constexpr_name(pi)<data_type>();
        }

        constexpr data_type perimeter() const noexcept
        {
            return 2 * constexpr_name(pi)<data_type> * constexpr_name(sqrt)(r_2);
        }

        constexpr data_type distance_to_constexpr(data_type x, data_type y) const noexcept
        {
            return svg_point_t<data_type>(x, y).distance_to_constexpr(cx, cy) - constexpr_name(sqrt)(r_2);
        }

        constexpr data_type distance_to_constexpr(const svg_point_t<data_type> &p) const noexcept
        {
            return distance_to_constexpr(p.x, p.y);
        }
    
        constexpr data_type distance_to(data_type x, data_type y) const noexcept
        {
            return svg_point_t<data_type>(x, y).distance_to(cx, cy) - sqrt(r_2);
        }

        constexpr data_type distance_to(const svg_point_t<data_type> &p) const noexcept
        {
            return distance_to(p.x, p.y);
        }

        std::string hlsl_corecode() const noexcept
		{
			return
				"float2 center = float2(" + std::to_string(cx) + ", " + std::to_string(cy) + ");\n"
				"float radius = " + std::to_string(sqrt(r_2)) + ";\n"
				"float dist = length(p - center);\n"
				"return dist < radius ? 0xffffffff : 0xff000000;\n";
		}

    };

    template <typename data_type>
    class svg_ellipse_t : public svg_shape_t<data_type>
    {
    public:
        svg_point_t<data_type> f[2];
        data_type d;

        constexpr svg_ellipse_t(const svg_point_t<data_type> &f0, const svg_point_t<data_type> &f1, data_type d_)
            : f{f0, f1}, d(d_){};

        constexpr svg_ellipse_t(const svg_point_t<data_type> &center, data_type a, data_type b, data_type angle = 0)
        {
            data_type cos_a = constexpr_name(cos)(angle);
            data_type sin_a = constexpr_name(sin)(angle);

            // d = 2a
            d = 2 * a;
            // focal_distance_constexpr = 2c = 2*sqrt(a^2-b^2)
            data_type c = constexpr_name(sqrt)(a * a - b * b);
            // f0 = center+(c*cos(a), c*sin(a))
            f[0] = svg_point_t<data_type>(center.x + c * cos_a, center.y + c * sin_a);
            // f1 = center-(c*cos(a), c*sin(a))
            f[1] = svg_point_t<data_type>(center.x - c * cos_a, center.y - c * sin_a);
        }

        // get center
        constexpr svg_point_t<data_type> center() const noexcept
        {
            return svg_point_t<data_type>((f[0].x + f[1].x) / 2, (f[0].y + f[1].y) / 2);
        }

        // get focal distance (2c)
        constexpr data_type squared_focal_distance() const noexcept
        {
            return f[0].squared_distance_to_constexpr(f[1]);
        }

        constexpr data_type focal_distance() const noexcept
        {
            return constexpr_name(sqrt)(squared_focal_distance());
        }

        constexpr data_type c_() const noexcept
        {
            return focal_distance() / 2;
        }

        // get major axis length (2a)
        constexpr data_type squared_major_axis_length() const noexcept
        {
            return d * d;
        }

        constexpr data_type major_axis_length() const noexcept
        {
            return d;
        }

        constexpr data_type a_() const noexcept
        {
            return d / 2;
        }
        // get minor axis length (2b)
        // c_()^2 + b_()^2 = a_()^2 => b_()^2 = a_()^2 - c_()^2
        constexpr data_type squared_minor_axis_length() const noexcept
        {
            return squared_major_axis_length() - squared_focal_distance();
        }

        constexpr data_type minor_axis_length() const noexcept
        {
            return constexpr_name(sqrt)(squared_minor_axis_length());
        }

        constexpr data_type b_() const noexcept
        {
            return constexpr_name(sqrt)(squared_minor_axis_length());
        }

        // shift
        constexpr svg_ellipse_t<data_type> shift(data_type dx, data_type dy) const noexcept
        {
            return svg_ellipse_t<data_type>(f[0].shift(dx, dy), f[1].shift(dx, dy), d);
        }

        constexpr svg_ellipse_t<data_type> shift(const svg_point_t<data_type> &p) const noexcept
        {
            return shift(p.x, p.y);
        }

        virtual svg_ellipse_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_ellipse_t<data_type>(f[0].shift(p), f[1].shift(p), d);
        }

        virtual constexpr bool contains(data_type x, data_type y) const noexcept override
        {
            // return (f[0].distance_to_constexpr(x, y) + f[1].distance_to_constexpr(x, y)) < d;
            data_type d0_2 = f[0].squared_distance_to_constexpr(x, y);
            data_type d1_2 = f[1].squared_distance_to_constexpr(x, y);
            data_type d2_ = d * d - d0_2 - d1_2;
            return 4 * d0_2 * d1_2 < d2_ * d2_;
        }
        std::string hlsl_corecode() const noexcept
        {
            return "float2 f0 = float2(" + std::to_string(f[0].x) + ", " + std::to_string(f[0].y) + ");\n"
                   "float2 f1 = float2(" + std::to_string(f[1].x) + ", " + std::to_string(f[1].y) + ");\n"
                   "float d = " + std::to_string(d) + ";\n"
                   "float d0_2 = length(p - f0);\n"
                   "float d1_2 = length(p - f1);\n"
                   "d0_2*=d0_2;"
                   "d1_2*=d1_2;"
                   "float d2_ = d * d - d0_2  - d1_2 ;\n"
                   "return 4 * d0_2 * d1_2 < d2_ * d2_ ? 0xffffffff : 0xff000000;\n";
        }

        virtual constexpr data_type area() const noexcept override
        { // pi * a * b
            return constexpr_name(pi)<data_type>() * a_() * b_();
        }

        constexpr data_type perimeter_Ramanujan_approximation() const noexcept
        { // pi * (a+b) * (1+3h/(10+sqrt(4-3h))), h = (a-b)^2/(a+b)^2
            data_type a_add_b = a_() + b_();
            data_type h = (a_() - b_()) / a_add_b;
            h *= h;
            return constexpr_name(pi)<data_type>() * a_add_b * (1 + 3 * h / (10 + constexpr_name(sqrt)(4 - 3 * h)));
        }
   

    };

    template <typename data_type>
    class svg_hyperbola_t : public svg_edge_t<data_type>
    {
    public:
        svg_point_t<data_type> f[2];
        data_type d;

        constexpr svg_hyperbola_t(const svg_point_t<data_type> &f0, const svg_point_t<data_type> &f1, data_type d_)
            : f{f0, f1}, d(d_){};

        // get center
        constexpr svg_point_t<data_type> center() const noexcept
        {
            return svg_point_t<data_type>((f[0].x + f[1].x) / 2, (f[0].y + f[1].y) / 2);
        }
        // shift
        constexpr svg_hyperbola_t<data_type> shift(data_type dx, data_type dy) const noexcept
        {
            return svg_hyperbola_t<data_type>(f[0].shift(dx, dy), f[1].shift(dx, dy), d);
        }

        constexpr svg_hyperbola_t<data_type> shift(const svg_point_t<data_type> &p) const noexcept
        {
            return shift(p.x, p.y);
        }

        virtual svg_hyperbola_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_hyperbola_t<data_type>(f[0].shift(p), f[1].shift(p), d);
        }
        // contains
        virtual constexpr bool contains(data_type x, data_type y) const noexcept override
        {
            data_type d0 = f[0].distance_to_constexpr(x, y);
            data_type d1 = f[1].distance_to_constexpr(x, y);
            return (d0 - d1) * (d0 - d1) < d * d;
        }

        std::string hlsl_corecode() const noexcept
        {
            return
				"float2 f0 = float2(" + std::to_string(f[0].x) + ", " + std::to_string(f[0].y) + ");\n"
				"float2 f1 = float2(" + std::to_string(f[1].x) + ", " + std::to_string(f[1].y) + ");\n"
				"float d = " + std::to_string(d) + ";\n"
				"float d0 = length(p - f0);\n"
				"float d1 = length(p - f1);\n"
				"return (d0 - d1) * (d0 - d1) < d * d ? 0xffffffff : 0xff000000;\n";
        }



    };

    template <typename data_type, int edges>
    class svg_polygon_t : public svg_shape_t<data_type>
    {
    public:
        svg_point_t<data_type> p[edges];

        constexpr svg_polygon_t(const svg_point_t<data_type> (&p_)[edges])
            : p()
        {
            for (int i = 0; i < edges; i++)
                p[i] = p_[i];
        }

        // shift
        constexpr svg_polygon_t<data_type, edges> shift(data_type dx, data_type dy) const noexcept
        {
            svg_polygon_t<data_type, edges> result;
            for (int i = 0; i < edges; i++)
                result.p[i] = this->p[i].shift(dx, dy);
            return result;
        }

        constexpr svg_polygon_t<data_type, edges> shift(const svg_point_t<data_type> &p_) const noexcept
        {
            return shift(p_.x, p_.y);
        }

        virtual svg_polygon_t<data_type, edges> *shift_ptr(const svg_point_t<data_type> &p_) const noexcept override
        {
            svg_polygon_t<data_type, edges> *result = new svg_polygon_t<data_type, edges>();
            for (int i = 0; i < edges; i++)
                result->p[i] = this->p[i].shift(p_);
            return result;
        }

        // or a variadic template
        template <typename... Args>
        constexpr svg_polygon_t(Args... args) : p{args...}
        {
            static_assert(sizeof...(Args) == edges, "count of points must be equal to edges");
            static_assert(sizeof...(Args) > 2, "edges must be greater than 2");
        }

        constexpr bool convex() const noexcept
        {
            return svg_polygon_t<data_type, edges>::convex(this->p);
        }

        template <int N>
        constexpr static bool convex(const svg_point_t<data_type> (&p_)[N]) noexcept
        {
            bool sign = svg_line_t<data_type>::side(p_[0], p_[1], p_[2]);
            for (int i = 1; i < N; i++)
            {
                if (svg_line_t<data_type>::side(p_[i], p_[(i + 1) % N], p_[(i + 2) % N]) != sign)
                    return false;
            }
            return true;
        }

        constexpr static bool convex(const svg_point_t<data_type> &p0, const svg_point_t<data_type> &p1,
                                     const svg_point_t<data_type> &p2, const svg_point_t<data_type> &p3) noexcept
        {
            bool result[4] = {
                svg_line_t<data_type>::side(p0, p1, p2),
                svg_line_t<data_type>::side(p1, p2, p3),
                svg_line_t<data_type>::side(p2, p3, p0),
                svg_line_t<data_type>::side(p3, p0, p1)};
            return result[0] == result[1] && result[1] == result[2] && result[2] == result[3];
        }

        constexpr bool clockwise() const noexcept
        {
            return svg_line_t<data_type>::side(p[0], p[1], p[2]);
        }

        constexpr bool contains(const svg_point_t<data_type> &p_) const noexcept
        {
            for (int i = 0; i < edges; i++)
            {
                if (svg_line_t<data_type>::side(p[i], p[(i + 1) % edges], p_) !=
                    svg_line_t<data_type>::side(p[i], p[(i + 1) % edges], p[(i + 2) % edges]))
                    return false;
            }
            return true;
        };

        virtual constexpr bool contains(data_type x, data_type y) const noexcept override
        {
            return contains(svg_point_t<data_type>(x, y));
        }

        constexpr data_type edge_length(int i) const noexcept
        {
            return this->p[i].distance_to_constexpr(this->p[(i + 1) % edges]);
        }

        constexpr data_type edge_length_squared(int i) const noexcept
        {
            return this->p[i].squared_distance_to_constexpr(this->p[(i + 1) % edges]);
        }

        constexpr data_type perimeter() const noexcept
        {
            data_type result = 0;
            for (int i = 0; i < edges; i++)
                result += edge_length(i);
            return result;
        }

        virtual constexpr data_type area() const noexcept override
        {
            auto trangle_area = [](const svg_point_t<data_type> &p0, const svg_point_t<data_type> &p1, const svg_point_t<data_type> &p2) -> data_type { // A=0.5(x1(y2-y3)+x2(y3-y1)+x3(y1-y2))
                data_type A = (p0.x * (p1.y - p2.y) + p1.x * (p2.y - p0.y) + p2.x * (p0.y - p1.y)) / 2;
                return constexpr_name(abs)(A);
            };
            data_type result = 0;
            for (int i = 1; i < edges - 1; i++)
                result += trangle_area(p[0], p[i], p[i + 1]);
            return result;
        }
    };

    template <typename data_type>
    class svg_triangle_t : public svg_polygon_t<data_type, 3>
    {
    public:
        constexpr svg_triangle_t(const svg_point_t<data_type> &p0, const svg_point_t<data_type> &p1, const svg_point_t<data_type> &p2)
            : svg_polygon_t<data_type, 3>({p0, p1, p2}){};
        // or a point array ref
        constexpr svg_triangle_t(const svg_point_t<data_type> (&p_)[3])
            : svg_polygon_t<data_type, 3>(p_){};

        constexpr bool convex() const noexcept { return true; }
        constexpr bool contains(const svg_point_t<data_type> &p_) const noexcept 
        {
            return svg_line_t<data_type>::side(this->p[0], this->p[1], this->p[2]) == svg_line_t<data_type>::side(this->p[0], this->p[1], p_) &&
                   svg_line_t<data_type>::side(this->p[1], this->p[2], this->p[0]) == svg_line_t<data_type>::side(this->p[1], this->p[2], p_) &&
                   svg_line_t<data_type>::side(this->p[2], this->p[0], this->p[1]) == svg_line_t<data_type>::side(this->p[2], this->p[0], p_);
        };

        virtual constexpr bool contains(data_type x, data_type y) const noexcept override
        {
            return contains(svg_point_t<data_type>(x, y));
        }

        constexpr data_type edge_length(int i) const noexcept
        {
            return this->p[i].distance_to_constexpr(this->p[(i + 1) % 3]);
        }

        constexpr data_type edge_length_squared(int i) const noexcept
        {
            return this->p[i].squared_distance_to_constexpr(this->p[(i + 1) % 3]);
        }

        constexpr static data_type squared_area(const svg_point_t<data_type> &p0, const svg_point_t<data_type> &p1, const svg_point_t<data_type> &p2) noexcept
        { // A=0.5(x1(y2-y3)+x2(y3-y1)+x3(y1-y2))
            data_type A = (p0.x * (p1.y - p2.y) + p1.x * (p2.y - p0.y) + p2.x * (p0.y - p1.y)) / 2;
            return A * A;
        }

        constexpr static data_type area(const svg_point_t<data_type> &p0, const svg_point_t<data_type> &p1, const svg_point_t<data_type> &p2) noexcept
        { // A=0.5(x1(y2-y3)+x2(y3-y1)+x3(y1-y2))
            data_type A = (p0.x * (p1.y - p2.y) + p1.x * (p2.y - p0.y) + p2.x * (p0.y - p1.y)) / 2;
            return constexpr_name(abs)(A);
        }

        virtual constexpr data_type area() const noexcept override
        { // A=0.5(x1(y2-y3)+x2(y3-y1)+x3(y1-y2))
            return area(this->p[0], this->p[1], this->p[2]);
        }

        constexpr static data_type squared_area(const data_type &edge0len, const data_type &edge1len, const data_type &edge2len) noexcept
        { // Heron's formula
            data_type s = (edge0len + edge1len + edge2len) / 2;
            return (s * (s - edge0len) * (s - edge1len) * (s - edge2len));
        }

        constexpr static data_type area(const data_type &edge0len, const data_type &edge1len, const data_type &edge2len) noexcept
        { // Heron's formula
            return constexpr_name(sqrt)(squared_area(edge0len, edge1len, edge2len));
        }
    };
    using svg_triangle = svg_triangle_t<double>;

    template <typename data_type>
    class svg_square_t : public svg_polygon_t<data_type, 4>
    {
    public:
        constexpr svg_square_t(const svg_point_t<data_type> &p,
                               data_type edge_length = (data_type)1,
                               data_type angle = (data_type)0)
            : svg_polygon_t<data_type, 4>(
                  {p,
                   p.shift(edge_length, 0).rotate(angle, p),
                   p.shift(edge_length, edge_length).rotate(angle, p),
                   p.shift(0, edge_length).rotate(angle, p)}){};

        constexpr data_type edge_length() const noexcept
        {
            return this->p[0].distance_to_constexpr(this->p[1]);
        }

        constexpr data_type edge_length_squared() const noexcept
        {
            return this->p[0].squared_distance_to_constexpr(this->p[1]);
        }

        virtual constexpr data_type area() const noexcept override
        {
            return edge_length_squared();
        }

        constexpr data_type perimeter() const noexcept
        {
            return 4 * edge_length();
        }

        constexpr bool convex() const noexcept
        {
            return true;
        }
    };

    template <typename data_type>
    class svg_logic_t : public svg_edge_t<data_type>
    {
    public:
        const svg_edge_t<data_type> *a;
        const svg_edge_t<data_type> *b;

        constexpr svg_logic_t(const svg_edge_t<data_type> *a_, const svg_edge_t<data_type> *b_) noexcept
            : a(a_), b(b_){};

    };

    template <typename data_type>
    class svg_logic_and_t : public svg_logic_t<data_type>
    {
    public:
        constexpr svg_logic_and_t(const svg_edge_t<data_type> *a_, const svg_edge_t<data_type> *b_) noexcept : svg_logic_t<data_type>(a_, b_){};
        virtual constexpr bool contains(data_type x,data_type y) const noexcept override
        {
            return this->a->contains(x,y) && this->b->contains(x,y);
        }

        virtual svg_logic_and_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_logic_and_t<data_type>(this->a->shift_ptr(p), this->b->shift_ptr(p));
        }
    };

    template <typename data_type>
    class svg_logic_or_t : public svg_logic_t<data_type>
    {
    public:
        constexpr svg_logic_or_t(const svg_edge_t<data_type> *a_, const svg_edge_t<data_type> *b_) noexcept : svg_logic_t<data_type>(a_, b_){};
        virtual constexpr bool contains(data_type x,data_type y) const noexcept override
        {
            return this->a->contains(x,y) ||this->b->contains(x,y);
        }

        virtual svg_logic_or_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_logic_or_t<data_type>(this->a->shift_ptr(p), this->b->shift_ptr(p));
        }
    };

    template <typename data_type>
    class svg_logic_not_t : public svg_logic_t<data_type>
    {
    public:
        constexpr svg_logic_not_t(const svg_edge_t<data_type> *a_) noexcept : svg_logic_t<data_type>(a_, nullptr){};
        virtual constexpr bool contains(data_type x,data_type y) const noexcept override
        {
            return !this->a->contains(x,y);
        }

        virtual svg_logic_not_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_logic_not_t<data_type>(this->a->shift_ptr(p));
        }
    };

    template <typename data_type>
    class svg_logic_xor_t : public svg_logic_t<data_type>
    {
    public:
        constexpr svg_logic_xor_t(const svg_edge_t<data_type> *a_, const svg_edge_t<data_type> *b_) noexcept : svg_logic_t<data_type>(a_, b_){};
        virtual constexpr bool contains(data_type x,data_type y) const noexcept override
        {
            return this->a->contains(x,y) != this->b->contains(x,y);
        }

        virtual svg_logic_xor_t<data_type> *shift_ptr(const svg_point_t<data_type> &p) const noexcept override
        {
            return new svg_logic_xor_t<data_type>(this->a->shift_ptr(p), this->b->shift_ptr(p));
        }
    };

#ifdef namespace_bionukg
}
#endif // namespace_bionukg
#endif // bionukg_SVGraphic_h
