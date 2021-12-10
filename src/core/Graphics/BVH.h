#pragma once
#include "Graphics/Ray.h"
#include "Object.h"

namespace gpc {
    template<typename T>
    class AABB;

    template<typename T>
    struct BVH : public Object<T> {
    public:

        BVH<T> const* getBVH() const override {
            return this;
        }
        virtual bool hit(Ray<T> const& ray) const = 0;
        virtual AABB<T> toAABB() const = 0;
    };
}


