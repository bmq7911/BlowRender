#pragma once 

namespace gpc {
    template<typename _T>
    class _tvec3 {

    public:
        union {
            struct { _T x, y, z; };
            struct { _T r, g, b; };
            struct { _T s, t, p; };
           
        };
    };
}