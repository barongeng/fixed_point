
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <limits>

#include <sg14/type_traits.h>

using sg14::_type_traits_impl::first_fit;
using std::is_same;
using sg14::is_signed;
using sg14::is_unsigned;
using sg14::make_signed;
using sg14::make_unsigned;

////////////////////////////////////////////////////////////////////////////////
// sg14::width

using sg14::width;

static_assert(width<std::int8_t>::value == 8, "sg14::width test failed");
static_assert(width<std::uint8_t>::value == 8, "sg14::width test failed");
static_assert(width<std::int16_t>::value == 16, "sg14::width test failed");
static_assert(width<std::uint16_t>::value == 16, "sg14::width test failed");
static_assert(width<std::int32_t>::value == 32, "sg14::width test failed");
static_assert(width<std::uint32_t>::value == 32, "sg14::width test failed");
static_assert(width<std::int64_t>::value == 64, "sg14::width test failed");
static_assert(width<std::uint64_t>::value == 64, "sg14::width test failed");

#if defined(SG14_INT128_ENABLED)
static_assert(width<SG14_INT128>::value == 128, "sg14::width test failed");
static_assert(width<SG14_UINT128>::value == 128, "sg14::width test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::width_v

#if (__cplusplus >= 201402L)

using sg14::width_v;

static_assert(width_v<std::int8_t> == 8, "sg14::width_v test failed");
static_assert(width_v<std::uint8_t> == 8, "sg14::width_v test failed");
static_assert(width_v<std::int16_t> == 16, "sg14::width_v test failed");
static_assert(width_v<std::uint16_t> == 16, "sg14::width_v test failed");
static_assert(width_v<std::int32_t> == 32, "sg14::width_v test failed");
static_assert(width_v<std::uint32_t> == 32, "sg14::width_v test failed");
static_assert(width_v<std::int64_t> == 64, "sg14::width_v test failed");
static_assert(width_v<std::uint64_t> == 64, "sg14::width_v test failed");

#if defined(SG14_INT128_ENABLED)
static_assert(width_v<SG14_INT128> == 128, "sg14::width_v test failed");
static_assert(width_v<SG14_UINT128> == 128, "sg14::width_v test failed");
#endif

#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::_type_traits_impl::first_fit_t

static_assert(
        std::is_same<typename first_fit<16, std::tuple<std::int8_t, std::int16_t, std::int32_t>>::type, std::int16_t>::value, "");
static_assert(
        std::is_same<typename first_fit<16, std::tuple<std::int32_t, std::int16_t, std::int8_t>>::type, std::int32_t>::value, "");

////////////////////////////////////////////////////////////////////////////////
// some random sg14::set_width

#if defined(SG14_INT128_ENABLED)
static_assert(is_same<sg14::set_width<unsigned long, 78>::type, SG14_UINT128>::value, "sg14::set_width_t test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// sg14::set_width_t

using sg14::set_width_t;

static_assert(is_same<set_width_t<std::uint8_t, 8>, std::uint8_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int8_t, 16>, std::int16_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint16_t, 24>, std::uint32_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int16_t, 32>, std::int32_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint32_t, 40>, std::uint64_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int32_t, 48>, std::int64_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint64_t, 56>, std::uint64_t>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int64_t, 64>, std::int64_t>::value, "sg14::set_width_t test failed");
#if defined(SG14_INT128_ENABLED)
static_assert(is_same<set_width_t<std::uint8_t, 72>, SG14_UINT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int8_t, 80>, SG14_INT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint16_t, 88>, SG14_UINT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int16_t, 96>, SG14_INT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint32_t, 104>, SG14_UINT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int32_t, 112>, SG14_INT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::uint64_t, 120>, SG14_UINT128>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<std::int64_t, 128>, SG14_INT128>::value, "sg14::set_width_t test failed");
#endif

static_assert(is_same<set_width_t<double, 8>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 16>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 24>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 32>, float>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 40>, double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 48>, double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 56>, double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 64>, double>::value, "sg14::set_width_t test failed");
#if !defined(_MSC_VER)
static_assert(is_same<set_width_t<long double, 72>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 80>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 88>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<long double, 96>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 104>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<double, 112>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<long double, 120>, long double>::value, "sg14::set_width_t test failed");
static_assert(is_same<set_width_t<float, 128>, long double>::value, "sg14::set_width_t test failed");
#endif

////////////////////////////////////////////////////////////////////////////////
// lots more width / set_width

template<typename T, int NumBits>
struct test_built_in_set_width;

template<typename T>
struct test_built_in_set_width<T, 0> {
};

template<typename T, int NumBits>
struct test_built_in_set_width : test_built_in_set_width<T, NumBits-1> {
    // get alias to result
    using result_type = set_width_t<T, NumBits>;

    static_assert(width<result_type>::value >= NumBits,
            "result of set_width must be at least the desired width in bits");

    static_assert(is_signed<T>::value==is_signed<result_type>::value,
            "incorrect signage in result of set_width_t (according to is_signed)");
    static_assert(is_unsigned<T>::value==is_unsigned<result_type>::value,
            "incorrect signage in result of set_width_t (according to is_unsigned)");

    static_assert(std::numeric_limits<result_type>::is_specialized, "numeric_limits<result_type> is not specialized");
    static_assert(std::numeric_limits<T>::is_signed==std::numeric_limits<result_type>::is_signed,
            "incorrect signage in result of set_width_t (according to numeric_limits)");

    static_assert(is_same<typename make_signed<result_type>::type, set_width_t<typename make_signed<T>::type, NumBits>>::value,
            "incorrect signage in result of set_width_t");
    static_assert(is_same<typename make_unsigned<result_type>::type, set_width_t<typename make_unsigned<T>::type, NumBits>>::value,
            "incorrect signage in result of set_width_t");
};

template<typename T>
struct test_built_in_width {
    static_assert(width<T>::value==sizeof(T)*CHAR_BIT,
            "incorrect assumption about width of built-in integral type, T");
};

template<typename T>
struct test_built_in
        : test_built_in_width<T>,
#if defined(SG14_INT128_ENABLED)
          test_built_in_set_width<T, 128>
#else
          test_built_in_set_width<T, 64>
#endif
{
    static_assert(std::numeric_limits<T>::is_specialized, "numeric_limits<T> is not specialized");
};

template
struct test_built_in<char>;
template
struct test_built_in<unsigned char>;
template
struct test_built_in<signed char>;
template
struct test_built_in<unsigned short>;
template
struct test_built_in<signed short>;
template
struct test_built_in<unsigned>;
template
struct test_built_in<signed>;
template
struct test_built_in<unsigned long>;
template
struct test_built_in<signed long>;
template
struct test_built_in<unsigned long long>;
template
struct test_built_in<signed long long>;
#if defined(SG14_INT128_ENABLED)
template
struct test_built_in<SG14_UINT128>;
template
struct test_built_in<SG14_INT128>;
#endif
