
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief all definitions related to the `sg14::fixed_point` type

#if !defined(SG14_FIXED_POINT_H)
#define SG14_FIXED_POINT_H 1

#include <limits>

#include "type_traits.h"

#include "bits/common.h"

#if defined(SG14_EXCEPTIONS_ENABLED)

#include <stdexcept>

#endif

#include "bits/fixed_point_def.h"

#include "bits/make_fixed.h"

#include "bits/fixed_point_named.h"

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point-aware _fixed_point_impl definitions

    namespace _fixed_point_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::is_fixed_point

        template<class T>
        struct is_fixed_point;

        template<class T>
        struct is_fixed_point
                : public std::false_type {
        };

        template<class Rep, int Exponent>
        struct is_fixed_point<fixed_point<Rep, Exponent>>
                : public std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::float_of_same_size

        template<class T>
        using float_of_same_size = set_width_t<float, width<T>::value>;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_fixed_point_impl::common_type_mixed

        template<class Lhs, class Rhs, class _Enable = void>
        struct common_type_mixed;

        // given a fixed-point and an integer type,
        // generates a fixed-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, int LhsExponent, class RhsInteger>
        struct common_type_mixed<
                fixed_point<LhsRep, LhsExponent>,
                RhsInteger,
                typename std::enable_if<is_integral<RhsInteger>::value>::type>
                : std::common_type<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsInteger, 0>> {
        };

        // given a fixed-point and a floating-point type,
        // generates a floating-point type that is as big as both of them (or as close as possible)
        template<class LhsRep, int LhsExponent, class Float>
        struct common_type_mixed<
                fixed_point<LhsRep, LhsExponent>,
                Float,
                typename std::enable_if<std::is_floating_point<Float>::value>::type>
                : std::common_type<float_of_same_size<LhsRep>, Float> {
        };
    }
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::common_type<> specializations related to sg14::sg14::fixed_point<>

    // std::common_type<fixed_point<>>
    template<class Rep, int Exponent>
    struct common_type<sg14::fixed_point<Rep, Exponent>> {
        using type = sg14::fixed_point<
                typename std::common_type<Rep>::type,
                Exponent>;
    };

    // std::common_type<fixed_point<>, not-fixed-point>
    template<class LhsRep, int LhsExponent, class Rhs>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, Rhs> {
        static_assert(!sg14::_fixed_point_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");
        using type = typename sg14::_fixed_point_impl::common_type_mixed<sg14::fixed_point<LhsRep, LhsExponent>, Rhs>::type;
    };

    // std::common_type<not-fixed-point, fixed_point<>>
    template<class Lhs, class RhsRep, int RhsExponent>
    struct common_type<Lhs, sg14::fixed_point<RhsRep, RhsExponent>> {
        static_assert(!sg14::_fixed_point_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");
        using type = typename sg14::_fixed_point_impl::common_type_mixed<sg14::fixed_point<RhsRep, RhsExponent>, Lhs>::type;
    };

    // std::common_type<fixed_point<>, fixed_point<>>
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, sg14::fixed_point<RhsRep, RhsExponent>> {
        using _result_rep = typename std::common_type<LhsRep, RhsRep>::type;

        // exponent is the lower of the two operands' unless that could cause overflow in which case it is adjusted downward
        static constexpr int _capacity = std::numeric_limits<_result_rep>::digits;
        static constexpr int _ideal_max_top = sg14::_impl::max(
                sg14::fixed_point<LhsRep, LhsExponent>::integer_digits,
                sg14::fixed_point<RhsRep, RhsExponent>::integer_digits);
        static constexpr int _ideal_exponent = sg14::_impl::min(LhsExponent, RhsExponent);
        static constexpr int _exponent = ((_ideal_max_top-_ideal_exponent)<=_capacity) ? _ideal_exponent :
                                         _ideal_max_top-_capacity;

        using type = sg14::fixed_point<_result_rep, _exponent>;
    };
}

namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) comparison operators

    template<class Rep, int Exponent>
    constexpr bool operator==(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()==rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator!=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()!=rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator<(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()<rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator>(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()>rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator>=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()>=rhs.data();
    }

    template<class Rep, int Exponent>
    constexpr bool operator<=(
            const fixed_point<Rep, Exponent>& lhs,
            const fixed_point<Rep, Exponent>& rhs)
    {
        return lhs.data()<=rhs.data();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ fixed_point) arithmetic operators

    // negate
    template<class RhsRep, int RhsExponent>
    constexpr auto operator-(const fixed_point<RhsRep, RhsExponent>& rhs)
    -> fixed_point<decltype(-rhs.data()), RhsExponent>
    {
        using result_type = fixed_point<decltype(-rhs.data()), RhsExponent>;
        return result_type::from_data(-rhs.data());
    }

    // add
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator+(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::add_tag>(lhs,
            rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::add_tag>(lhs,
                rhs);
    }

    // subtract
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator-(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::subtract_tag>(
            lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::subtract_tag>(
                lhs, rhs);
    }

    // multiply
    template<
            class LhsRep, int LhsExponent,
            class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::multiply_tag>(
            lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::multiply_tag>(
                lhs, rhs);
    }

    // divide
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(_fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::divide_tag>(
            lhs, rhs))
    {
        return _fixed_point_impl::operate<_fixed_point_impl::arithmetic_operator_tag, _impl::divide_tag>(
                lhs, rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    template<class Lhs, class Rhs>
    constexpr auto operator==(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)==static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator!=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)!=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator>=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)>=static_cast<common_type>(rhs);
    }

    template<class Lhs, class Rhs>
    constexpr auto operator<=(const Lhs& lhs, const Rhs& rhs)
    -> typename std::enable_if<
            _fixed_point_impl::is_fixed_point<Lhs>::value || _fixed_point_impl::is_fixed_point<Rhs>::value, bool>::type
    {
        using common_type = _impl::common_type_t<Lhs, Rhs>;
        return static_cast<common_type>(lhs)<=static_cast<common_type>(rhs);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // (fixed_point @ non-fixed_point) arithmetic operators

    // fixed-point, integer -> fixed-point
    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs + fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs + fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs - fixed_point<RhsInteger, 0>{rhs})
    {
        return lhs - fixed_point<RhsInteger, 0>{rhs};
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator*(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs*fixed_point<RhsInteger>(rhs))
    {
        return lhs*fixed_point<RhsInteger>(rhs);
    }

    template<
            class LhsRep, int LhsExponent,
            class RhsInteger,
            typename = typename std::enable_if<is_integral<RhsInteger>::value>::type>
    constexpr auto operator/(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsInteger& rhs)
    -> decltype(lhs/fixed_point<RhsInteger>{rhs})
    {
        return lhs/fixed_point<RhsInteger>{rhs};
    }

    // integer. fixed-point -> fixed-point
    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator+(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger, 0>{lhs} + rhs)
    {
        return fixed_point<LhsInteger, 0>{lhs} + rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator-(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}-rhs)
    {
        return fixed_point<LhsInteger>{lhs}-rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator*(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}*rhs)
    {
        return fixed_point<LhsInteger>{lhs}*rhs;
    }

    template<
            class LhsInteger,
            class RhsRep, int RhsExponent,
            typename = typename std::enable_if<is_integral<LhsInteger>::value>::type>
    constexpr auto operator/(const LhsInteger& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)
    -> decltype(fixed_point<LhsInteger>{lhs}/rhs)
    {
        return fixed_point<LhsInteger>{lhs}/rhs;
    }

    // fixed-point, floating-point -> floating-point
    template<class LhsRep, int LhsExponent, class RhsFloat, typename = typename std::enable_if<std::is_floating_point<RhsFloat>::value>::type>
    constexpr auto operator+(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat, typename = typename std::enable_if<std::is_floating_point<RhsFloat>::value>::type>
    constexpr auto operator-(const fixed_point<LhsRep, LhsExponent>& lhs, const RhsFloat& rhs)-> _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator*(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<RhsFloat>::value, RhsFloat>::type>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)*rhs;
    }

    template<class LhsRep, int LhsExponent, class RhsFloat>
    constexpr auto operator/(
            const fixed_point<LhsRep, LhsExponent>& lhs,
            const RhsFloat& rhs)
    -> _impl::common_type_t<
            fixed_point<LhsRep, LhsExponent>,
            typename std::enable_if<std::is_floating_point<RhsFloat>::value, RhsFloat>::type>
    {
        using result_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, RhsFloat>;
        return static_cast<result_type>(lhs)/rhs;
    }

    // floating-point, fixed-point -> floating-point
    template<class LhsFloat, class RhsRep, int RhsExponent, typename = typename std::enable_if<std::is_floating_point<LhsFloat>::value>::type>
    constexpr auto operator+(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)+static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent, typename = typename std::enable_if<std::is_floating_point<LhsFloat>::value>::type>
    constexpr auto operator-(const LhsFloat& lhs, const fixed_point<RhsRep, RhsExponent>& rhs)-> _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<LhsFloat, fixed_point<RhsRep, RhsExponent>>;
        return static_cast<result_type>(lhs)-static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator*(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t<
            typename std::enable_if<std::is_floating_point<LhsFloat>::value, LhsFloat>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs*static_cast<result_type>(rhs);
    }

    template<class LhsFloat, class RhsRep, int RhsExponent>
    constexpr auto operator/(
            const LhsFloat& lhs,
            const fixed_point<RhsRep, RhsExponent>& rhs)
    -> _impl::common_type_t<
            typename std::enable_if<std::is_floating_point<LhsFloat>::value, LhsFloat>::type,
            fixed_point<RhsRep, RhsExponent>>
    {
        using result_type = _impl::common_type_t<fixed_point<RhsRep, RhsExponent>, LhsFloat>;
        return lhs/
                static_cast<result_type>(rhs);
    }

    template<class LhsRep, int Exponent, class Rhs>
    fixed_point<LhsRep, Exponent>& operator+=(fixed_point<LhsRep, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs = lhs+fixed_point<LhsRep, Exponent>(rhs);
    }

    template<class LhsRep, int Exponent, class Rhs>
    fixed_point<LhsRep, Exponent>& operator-=(fixed_point<LhsRep, Exponent>& lhs, const Rhs& rhs)
    {
        return lhs = lhs-fixed_point<LhsRep, Exponent>(rhs);
    }

    template<class LhsRep, int Exponent>
    template<class Rhs>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator*=(const Rhs& rhs)
    {
        _r *= static_cast<rep>(rhs);
        return *this;
    }

    template<class LhsRep, int Exponent>
    template<class Rhs>
    fixed_point<LhsRep, Exponent>&
    fixed_point<LhsRep, Exponent>::operator/=(const Rhs& rhs)
    {
        _r /= static_cast<rep>(rhs);
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr fixed_point<LhsRep, LhsExponent>
    operator<<(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    {
        return fixed_point<LhsRep, LhsExponent>::from_data(lhs.data() << rhs);
    };

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr fixed_point<LhsRep, LhsExponent>
    operator>>(const fixed_point<LhsRep, LhsExponent>& lhs, const Rhs& rhs)
    {
        return fixed_point<LhsRep, LhsExponent>::from_data(lhs.data() >> rhs);
    };
}

#include "bits/fixed_point_extras.h"

#endif	// SG14_FIXED_POINT_H
