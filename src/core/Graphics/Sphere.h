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


    template<typename T>
    class Sphere : public BVH<T> {
    public:
        Sphere(glm::tvec3<T> const& pos, T const& r)
            : m_pos(pos)
            , m_r(r)
        {}
        glm::tvec3<T> o() const {
            return m_pos;
        }
        T r() const {
            return m_r;
        }

        bool hit(Ray<T> const& ray) const override {
            T t;
            glm::tvec3<T> normal;
            return Sphere<T>::hit(ray, t, normal);
        }
        AABB<T> toAABB() const override {
            return AABB<T>(glm::tvec3<T>(m_pos - glm::tvec3<T>(m_r, m_r, m_r)), glm::tvec3<T>(m_pos + glm::tvec3<T>(m_r, m_r, m_r)));
        }
        bool hit(Ray<T> const& ray, T& t, glm::tvec3<T>& normal) const override {
            T a = glm::dot(ray.d(), ray.d());
            T b = 2 * glm::dot(ray.o() - m_pos, ray.d());
            T c = glm::dot(ray.o() - m_pos, ray.o() - m_pos) - m_r * m_r;
            T d = b * b - 4 * a * c;
            if (d < static_cast<T>(0.0)) {
                return false;
            }
            else {
                if (d == static_cast<T>(0.0)) {
                    t = static_cast<T>(-0.5) * b / a;
                    normal = glm::normalize(ray.at(t) - m_pos);
                    return true;
                }
                else {
                    T t1 = 0.5 * (-b + sqrt(d)) / a;
                    T t2 = 0.5 * (-b - sqrt(d)) / a;
                    t = min_greater(t1, t2, static_cast<T>(0));
                    if (0.0 == t) {
                        return false;
                    }
                    normal = glm::normalize(ray.at(t) - m_pos);
                    return true;
                }
            }

        }


    private:
        glm::tvec3<T> m_pos;
        T m_r;
    };
}