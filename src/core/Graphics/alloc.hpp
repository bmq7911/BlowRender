#pragma once
#include <new>
#include <utility>
#include <stdlib.h>
namespace gpc {

    template<typename _T>
    inline _T* _allocate(size_t size, _T*) {
        _T* tmp = (_T*)malloc(size * sizeof(_T));
        return tmp;
    }
    template<typename _T, typename ... _Args>
    inline void construct(_T* p, _Args && ... args) {
        new (p)_T(std::forward<_Args>(args)...);
    }

    template<typename _T>
    class alloctor {
    public:
        typedef _T           value_type;
        typedef _T* pointer;
        typedef _T const* const_pointer;
        typedef _T& reference;
        typedef _T const& const_reference;
        typedef size_t       size_type;
        typedef ptrdiff_t    difference_type;

    public:
        template<typename _U>
        struct rebind {
            typedef alloctor<_U> other;
        };
        pointer allocate(size_t n, const void* hint = nullptr) {
            return _allocate(n, pointer(0));
        }
    };

}
