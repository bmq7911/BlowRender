#pragma once
#include "Graphics/Ray.h"
namespace gpc {
    template<typename T>
    struct BVH;

    template<typename T>
    struct Object {
    public:
        virtual bool hit(Ray<T> const& ray, T& t, glm::tvec3<T>& normal) const = 0;
        virtual BVH<T>const* getBVH() const = 0;
    };
}