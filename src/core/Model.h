#pragma once
#include "Graphics/Object.h"
#include "Graphics/AABB.h"

class Model : public gpc::Object{
public:
    bool hit(gpc::Ray const & ray, Float& t, glm::fvec3 &normal) const override {
        return false;
    }
    gpc::BVH const* getBVH() const override {
        return &m_aabb;
    }
private:
    gpc::AABB m_aabb;
    glm::fvec3 m_pos;
};