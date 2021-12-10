
#pragma once

#include "simd/sse.h"
namespace gpc{
    template<typename _T>
    class _tvec2 {
    public:
        _tvec2();
        _tvec2(_T x_, _T y_);
        _tvec2(_tvec2<_T> const&v);
        _tvec2& operator=(_tvec2<_T> const& v);
        _tvec2  operator +(_tvec2<_T> const& v) const;
        _tvec2& operator +=(_tvec2<_T> const& v);
        _tvec2  operator - (_tvec2<_T> const& v) const;
        _tvec2& operator -=(_tvec2<_T> const& v) ;
        _tvec2  operator *(_T const& k) const;
        _tvec2& operator *=(_T const& k);
        
        union {
            struct { _T x, y; };
            struct { _T r, g; };
            struct { _T s, t; };
            typename mm::__detail::simd_data<_T, 1>::type data;
        };
    };
#if defined(MATH_SUPPORT_SSE2)
#include "vec2.sse2.inl"
#else
#include "vec2.inl"
#endif
}