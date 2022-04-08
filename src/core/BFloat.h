#pragma once
#include <stdint.h>
#include <string.h>
#include <cfloat>
#include <cmath>
#include <limits>
#include <type_traits>
#include "glm/glm.hpp"

#ifdef USING_DOUBLE
typedef double Float;
#else 
typedef float  Float;
#endif /// UNSING_DOUBLE

namespace glm {
    using fvec2 = glm::tvec2<Float>;
    template fvec2;

    using fvec3 = glm::tvec3<Float>;
    template fvec3;

    using fvec4 = glm::tvec4<Float>;
    template fvec4;

    using fmat2 = glm::tmat2x2<Float>;
    template fmat2;

    using fmat2x2 = glm::tmat2x2<Float>;
    template fmat2x2;

    using fmat2x3 = glm::tmat2x3<Float>;
    template fmat2x3;

    using fmat2x4 = glm::tmat2x4<Float>;
    template fmat2x4;

    using fmat3 = glm::tmat3x3<Float>;
    template fmat3;
    using fmat3x2 = glm::tmat3x2<Float>;
    template fmat3x2;
    using fmat3x3 = glm::tmat3x3<Float>;
    template fmat3x3;
    using fmat3x4 = glm::tmat3x4<Float>;
    
    template fmat3x4;
    using fmat4 = glm::tmat4x4<Float>;
    template fmat4;
    using fmat4x2 = glm::tmat4x2<Float>;
    template fmat4x2;
    using fmat4x3 = glm::tmat4x3<Float>;
    template fmat4x3;
    using fmat4x4 = glm::tmat4x4<Float>;
    template fmat4x4;

}

namespace gpc {
#define EPSILON 1e-5f
    template<typename T>
    static constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type Infinity() {
        return std::numeric_limits<T>::infinity();
    }
    template<typename T>
    static constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type Epsilon() {
        return std::numeric_limits<T>::epsilon();
    }

    // Floating-point Constants
    static double dinfinity = Infinity<double>();
    static double finfinity = Infinity<float>();

    template<typename T>
    static constexpr typename std::enable_if<std::is_floating_point<T>::value, T>::type MachineEpsilon() {
        return std::numeric_limits<T>::epsilon();
    }


    template <typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, bool>
        IsNaN(T v) {
        return std::isnan(v);
    }

    template <typename T>
    inline std::enable_if_t<std::is_integral<T>::value, bool> IsNaN(T v) {
        return false;
    }

    template <typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, bool>
        IsInf(T v) {
        return std::isinf(v);
    }

    template <typename T>
    inline std::enable_if_t<std::is_integral<T>::value, bool> IsInf(T v) {
        return false;
    }

    template <typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, bool> IsFinite(T v) {
        return std::isfinite(v);
    }

    template <typename T>
    inline std::enable_if_t<std::is_integral<T>::value, bool> IsFinite(T v) {
        return true;
    }

    template<typename T>
    inline typename std::enable_if<std::is_floating_point<T>::value, T>::type fma(T const& a, T const& b, T const& c) {
        return std::fma(a, b, c);
    }


    template <class To, class From>
    typename std::enable_if_t<sizeof(To) == sizeof(From) &&
        std::is_trivially_copyable_v<From> &&
        std::is_trivially_copyable_v<To>,
        To>
        bit_cast(const From& src) noexcept {
        static_assert(std::is_trivially_constructible_v<To>,
            "This implementation requires the destination type to be trivially "
            "constructible");
        To dst;
        std::memcpy(&dst, &src, sizeof(To));
        return dst;
    }

    template<typename T, typename U, typename U1 = typename std::enable_if<sizeof(T) == sizeof(U)>::type >
    static inline T FloatToBits(U const& f) {
        return bit_cast<T, U>(f);
    }

    template<typename T, typename U, typename U1 = typename std::enable_if<sizeof(T) == sizeof(U)>::type>
    static inline T BitsToFloat(U const& i) {
        return bit_cast<T, U>(i);
    }

    inline int32_t Exponent(float v) {
        return (FloatToBits<uint32_t>(v) >> 23) - 127;
    }

    inline int64_t Exponent(double v) {
        return (FloatToBits<uint64_t>(v) >> 52) - 1023;
    }

    inline int Significand(float v) {
        return FloatToBits<uint32_t>(v) & ((1 << 23) - 1);
    }
    inline int64_t Significand(double v) {
        return FloatToBits<uint64_t>(v) & ((1ull << 52) - 1);
    }

    inline uint32_t SignBit(float v) {
        return FloatToBits<uint32_t>(v) & 0x80000000;
    }
    inline uint64_t SignBit(double v) {
        return FloatToBits<uint64_t>(v) & 0x8000000000000000;
    }


    inline float NextFloatUp(float v) {
        if (IsInf(v) && v > 0.f)
            return v;
        if (v == -0.f)
            v = 0.f;

        uint32_t ui = FloatToBits<uint32_t>(v);
        if (v >= 0)
            ++ui;
        else
            --ui;
        return BitsToFloat<float>(ui);
    }


    inline float NextFloatDown(float v) {
        // Handle infinity and positive zero for _NextFloatDown()_
        if (IsInf(v) && v < 0.)
            return v;
        if (v == 0.f)
            v = -0.f;
        uint32_t ui = FloatToBits<uint32_t>(v);
        if (v > 0)
            --ui;
        else
            ++ui;
        return BitsToFloat<float>(ui);
    }
    template<typename T>
    inline constexpr float gamma(int n) {
        return (n * MachineEpsilon<T>()) / (1 - n * MachineEpsilon<T>());
    }
    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T> AddRoundUp(T a, T b) {
        return NextFloatUp(a + b);
    }
    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        AddRoundDown(T a, T b) {
        return NextFloatDown(a + b);
    }


    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        SubRoundUp(T a, T b) {
        return AddRoundUp(a, -b);
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        SubRoundDown(T a, T b) {
        return AddRoundDown(a, -b);
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        MulRoundUp(T a, T b) {
        return NextFloatUp(a * b);
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        MulRoundDown(T a, T b) {
        return NextFloatDown(a * b);
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        DivRoundUp(T a, T b) {
        return NextFloatUp(a / b);
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        DivRoundDown(T a, T b) {
        return NextFloatDown(a / b);
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        SqrtRoundUp(T a) {
        return NextFloatUp(std::sqrt(a));
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        SqrtRoundDown(T a) {
        return std::max<Float>(0, NextFloatDown(std::sqrt(a)));
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        FMARoundUp(T a, T b, T c) {
        return NextFloatUp(fma(a, b, c));
    }

    template<typename T>
    inline std::enable_if_t<std::is_floating_point<T>::value, T>
        FMARoundDown(T a, T b, T c) {
        return NextFloatDown(fma(a, b, c));
    }

    inline double NextFloatUp(double v) {
        if (IsInf(v) && v > 0.)
            return v;
        if (v == -0.f)
            v = 0.f;
        uint64_t ui = FloatToBits<uint64_t>(v);
        if (v >= 0.)
            ++ui;
        else
            --ui;
        return BitsToFloat<double>(ui);
    }

    inline double NextFloatDown(double v) {
        if (IsInf(v) && v < 0.)
            return v;
        if (v == 0.f)
            v = -0.f;
        uint64_t ui = FloatToBits<uint64_t>(v);
        if (v > 0.)
            --ui;
        else
            ++ui;
        return BitsToFloat<double>(ui);
    }

    inline double FlipSign(double a, double b) {
        return BitsToFloat<double>(FloatToBits<uint64_t>(a) ^ SignBit(b));
    }
}
