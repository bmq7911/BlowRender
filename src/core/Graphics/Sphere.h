#pragma once
#include "Ray.h"
#include "Object.h"
#include "AABB.h"
namespace gpc {
    template<typename T>
    T min_greater(T v1, T v2, T min) {
        T t1 = std::max(v1, min);
        T t2 = std::max(v2, min);
        return std::min(t1, t2);
    }


    class Sphere : public BVH {
    public:
        Sphere(glm::fvec3 const& pos, Float const& r)
            : m_pos(pos)
            , m_r(r)
        {}
        glm::fvec3 o() const {
            return m_pos;
        }
        Float r() const {
            return m_r;
        }

        bool hit(Ray const& ray) const override {
            Float t;
            glm::fvec3 normal;
            return Sphere::hit(ray, t, normal);
        }
        AABB toAABB() const override {
            return AABB(glm::fvec3(m_pos - glm::fvec3(m_r, m_r, m_r)), glm::fvec3(m_pos + glm::fvec3(m_r, m_r, m_r)));
        }
        bool hit(Ray const& ray, Float& t, glm::fvec3& normal) const override {
            Float a = glm::dot(ray.d(), ray.d());
            Float b = Float(2) * glm::dot( ray.o() - m_pos, ray.d());
            Float c = glm::dot(ray.o() - m_pos, ray.o() - m_pos) - m_r * m_r;
            Float d = b * b - Float(4) * a * c;
            if (d < static_cast<Float>(0.0)) {
                return false;
            }
            else {
                if (d == static_cast<Float>(0.0)) {
                    t = static_cast<Float>(-0.5) * b / a;
                    normal = glm::normalize(ray.at(t) - m_pos);
                    return true;
                }
                else {
                    Float t1 = Float(0.5) * (-b + sqrt(d)) / a;
                    Float t2 = Float(0.5) * (-b - sqrt(d)) / a;
                    t = min_greater(t1, t2, static_cast<Float>(0));
                    if (0.0 == t) {
                        return false;
                    }
                    normal = glm::normalize(ray.at(t) - m_pos);
                    return true;
                }
            }

        }


    private:
        glm::fvec3 m_pos;
        Float m_r;
    };
}