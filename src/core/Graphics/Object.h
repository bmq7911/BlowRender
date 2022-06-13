#pragma once
#include "Graphics/Ray.h"
namespace gpc {

    struct BVH;
    
    // this class just for ray trace
    struct Object {
    public:
        virtual bool hit(Ray const& ray, Float & t, glm::fvec3 & normal) const = 0;
        virtual BVH const* getBVH() const = 0;
    };
}