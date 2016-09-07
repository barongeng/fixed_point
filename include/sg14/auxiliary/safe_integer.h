
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definitions and specializations that adapt boost::numeric::safe<> for use with @ref sg14::fixed_point

#if !defined(SG14_SAFE_INTEGER_H)
#define SG14_SAFE_INTEGER_H 1

#include <sg14/type_traits.h>

// from fork of github.com/robertramey/safe_numerics here:
// https://github.com/johnmcfarlane/safe_numerics/blob/fixed_point/include/safe_integer.hpp

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <safe_integer.hpp>
#pragma GCC diagnostic pop

namespace sg14 {
    template<class Rep>
    using _bns = boost::numeric::safe<Rep>;

    // sg14::make_signed<boost::numerics::safe<>>
    template<typename Rep>
    struct make_signed<_bns<Rep>> {
        using type = _bns<typename make_signed<Rep>::type>;
    };

    // sg14::make_unsigned<boost::numerics::safe<>>
    template<typename Rep>
    struct make_unsigned<_bns<Rep>> {
        using type = _bns<typename make_unsigned<Rep>::type>;
    };

    // sg14::set_width<boost::numerics::safe<>>
    template<typename Rep, _width_type MinNumBits>
    struct set_width<_bns<Rep>, MinNumBits> {
        using type = _bns<set_width_t<Rep, MinNumBits>>;
    };

    // sg14::width<boost::numerics::safe<>>
    template<typename Rep>
    struct width<_bns<Rep>> : width<Rep> {
    };
}

namespace std {
    // std::common_type<boost::numerics::safe<>, boost::numerics::safe<>>
    template<typename LhsRep, typename RhsRep>
    struct common_type<sg14::_bns<LhsRep>, sg14::_bns<RhsRep>> {
        using type = sg14::_bns<common_type_t<LhsRep, RhsRep>>;
    };

    // std::common_type<boost::numerics::safe<>, floating-point>
    template<typename LhsRep, typename Rhs>
    struct common_type<sg14::_bns<LhsRep>, Rhs/*, typename std::enable_if<std::is_floating_point<Rhs>::value>::type*/> {
        using type = Rhs;
    };

//    // std::common_type<boost::numerics::safe<>, integer>
//    template<typename LhsRep, typename Rhs>
//    struct common_type<sg14::_bns<LhsRep>, Rhs, typename std::enable_if<std::numeric_limits<Rhs>::integer>::type> : common_type<sg14::_bns<LhsRep>, sg14::_bns<Rhs>> {
//    };

    // std::common_type<not-safe, boost::numerics::safe<>>
    template<typename Lhs, typename RhsRep>
    struct common_type<Lhs, sg14::_bns<RhsRep>> : common_type<sg14::_bns<RhsRep>, Lhs> {
    };
}

#endif // SG14_SAFE_INTEGER_H
