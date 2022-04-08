#pragma once
#include "Graphics/Ray.h"
#include "Object.h"

namespace gpc {
    class AABB;

    struct BVH : public Object{
    public:

        BVH const* getBVH() const override {
            return this;
        }
        virtual bool hit(Ray const& ray) const = 0;
        virtual AABB toAABB() const = 0;
    };
}


