#pragma once
#include "Graphics/Object.h"
#include "Graphics/AABB.h"

template<typename T>
class Model : public gpc::Object<T>{
public:
    bool hit(gpc::Ray<T>& ray, T& t, glm::tvec3<T> &normal) override {
               
    }
    gpc::BVH<T> const* getBVH() const override {
        return m_aabb;
    }
private:
    gpc::AABB<T> m_aabb;
    glm::tvec3<T> m_pos;
};