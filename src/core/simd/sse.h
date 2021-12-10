#pragma once
#include "config.h"
#if defined( MATH_SUPPORT_SSE )
#include <xmmintrin.h>
#elif defined( MATH_SUPPORT_SSE2)
#include <xmmintrin.h>
#include <emmintrin.h>
#elif defined(MATH_SUPPORT_SSE3)
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#elif defined(MATH_SUPPORT_SSE4)
#include <xmmintrin.h>
#include <emmintrin.h>
#include <pmmintrin.h>
#include <smmintrin.h>
#endif ///

#if ENABLE( SUPPORT_CXX_11 )
#define ALIGN_AS( x) alignas( x )
#else
#define ALIGN_AS(x) 
#endif ////


namespace gpc {
    namespace __detail {
        template<size_t _M = 4>
        constexpr size_t  __size(size_t N) {
            return (N % _M ? (N / _M + 1) * _M : N);
        }
        template<typename T, size_t N = 1>
        struct simd_data {
            typedef T type[__size(4 * N)];
        };

#ifdef MATH_SUPPORT_SSE
        template<>
        struct simd_data<float, 4> {
            typedef __m128 type;

        };
        template<>
        struct simd_data<int, 4> {
            typedef __m128i type;
        };
        template<>
        struct simd_data<uint32_t, 4> {
            typedef __m128i type;
        };
        struct simd_data<double, 4> {
            typedef __256d type;
        };

#endif ///MATH_SUPPORT_SSE

    }
}

