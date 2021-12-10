#pragma once
#include <type_traits>
#include <algorithm>
#include "BVH.h"
#include "Ray.h"   
#include "BFloat.h"
namespace gpc {
#define GET_SIGN_BIT(x) (((signed char*) &x)[sizeof(x) - 1] >> 7 | 1)
    template<typename T>
    bool is_same_sign(T v1, T v2, T v3) {
        return (GET_SIGN_BIT(v1) == GET_SIGN_BIT(v2)) && (GET_SIGN_BIT(v2) == GET_SIGN_BIT(v3));
    }
    /// 中心半径存储方式
    template<typename T>
    class AABB : public BVH<T> { /// 在世界坐标系里
        struct AABB_OR {
            glm::tvec3<T> o;
            glm::tvec3<T> r;
        };
    public:
        static_assert(std::is_same<float, T>::value == true || std::is_same<double, T>::value == true, "AABB 的参数必须是 单精度或者双精度浮点数 ");
    public:
        enum CuteType {
            kX,
            kY,
            kZ,
        };

        AABB() {
        }

        AABB(glm::tvec3<T> min_, glm::tvec3<T> max_)
            : m_min(min_)
            , m_max(max_)
        {
        }
        bool in(glm::tvec3<T> const& p) const {
            return !((p.x < m_min.x || p.x > m_max.x) ||
                (p.y < m_min.y || p.y > m_max.y) ||
                (p.z < m_min.z || p.z > m_max.z));
        }

        bool in(AABB<T> const& aabb) const {
            return (aabb.m_min.x >= m_min.x && aabb.m_max.x <= m_max.x) &&
                (aabb.m_min.y >= m_min.y && aabb.m_max.y <= m_max.y) &&
                (aabb.m_min.z >= m_min.z && aabb.m_max.z <= m_max.z);

        }

        bool intersect(AABB<T> const& aabb) const {
            AABB_OR t1 = _ToAABB_OR();
            AABB_OR t2 = aabb._ToAABB_OR();
            glm::tvec3 oo = glm::abs(t1.o - t2.o);
            bool b = (oo.x > t1.r.x + t2.r.x) ||
                (oo.y > t1.r.y + t2.r.y) ||
                (oo.z > t1.r.z + t2.r.z);
            return !b;
        }
        /// 光线与包围盒的计算
        /// AABB 与光线求交还是比我想象的复杂
        bool hit(Ray<T> const& ray, T& t, glm::tvec3<T>& n) const override {
            T ox = ray.o().x;
            T oy = ray.o().y;
            T oz = ray.o().z;

            T idx = T(1) / ray.d().x;
            T idy = T(1) / ray.d().y;
            T idz = T(1) / ray.d().z;

            T tx_min, ty_min, tz_min;
            T tx_max, ty_max, tz_max;

            if (idx >= 0) {
                tx_min = (m_min.x - ox) * idx;
                tx_max = (m_max.x - ox) * idx;
            }
            else {
                tx_min = (m_max.x - ox) * idx;
                tx_max = (m_min.x - ox) * idx;
            }

            if (idy >= 0) {
                ty_min = (m_min.y - oy) * idy;
                ty_max = (m_max.y - oy) * idy;
            }
            else {
                ty_min = (m_max.y - oy) * idy;
                ty_max = (m_min.y - oy) * idy;
            }
            if (idz >= 0) {
                tz_min = (m_min.z - oz) * idz;
                tz_max = (m_max.z - oz) * idz;
            }
            else {
                tz_min = (m_max.z - oz) * idz;
                tz_max = (m_min.z - oz) * idz;
            }
            glm::tvec3<T> n0, n1;;
            T t0, t1;
            if (tx_min > ty_min) {
                t0 = tx_min;
                n0 = glm::tvec3<T>(-1, 0, 0);
            }
            else {
                t0 = ty_min;
                n0 = glm::tvec3<T>(0, -1, 0);
            }

            if (tz_min > t0) {
                t0 = tz_min;
                n0 = glm::tvec3<T>(0, 0, -1);
            }

            if (tx_max < ty_max) {
                t1 = tx_max;
                n1 = glm::tvec3<T>(1, 0, 0);
            }
            else {
                t1 = ty_max;
                n1 = glm::tvec3<T>(0, 1, 0);
            }

            if (tz_max < t1) {
                t1 = tz_max;
                n1 = glm::tvec3<T>(0, 0, 1);
            }
            /// 相交
            if (t0 < t1 && t1 > EPSILON) {
                if (t0 < T(0)) {
                    t = t1;
                    n = -n1;
                }
                else if (t0 >= T(0)) {
                    t = t0;
                    n = n0;
                }
                return true;
            }

            return false;
        }
        bool hit(Ray<T> const& ray) const override {
            if (in(ray.o()))
                return true;
            T t;
            glm::tvec3<T> n;
            return hit(ray, t, n);
        }
        BVH<T>const* getBVH() const override {
            return this;
        }
        AABB<T> toAABB() const override {
            return *this;
        }


    public:
        AABB<T> subAABB(uint32_t i) const {
            static const glm::tvec3<T> so[] = {
                {T(-1.0),T(-1.0), T(-1.0)},
                {T(-1.0),T(1.0), T(-1.0)},
                {T(1.0), T(1.0) , T(-1.0)},
                {T(1.0), T(-1.0), T(-1.0)},


                {T(-1.0),T(-1.0), T(1.0)},
                {T(-1.0),T(1.0),  T(1.0)},
                {T(1.0), T(1.0),  T(1.0)},
                {T(1.0), T(-1.0), T(1.0)},

            };
            if (i < 8) {
                glm::tvec3<T> o = T(0.5) * (m_min + m_max);
                glm::tvec3<T> r = T(0.25) * (m_max - m_min);
                AABB_OR aabb;
                aabb.o = o + so[i] * r;
                aabb.r = r;
                return _ToAABB(aabb);
            }
            return AABB();
        }
        inline AABB<T> centerAABB(T const& f) const {
            AABB_OR t1 = _ToAABB_OR();
            t1.r = t1.r * f;
            return _ToAABB(t1);
        }

        inline std::pair<AABB<T>, AABB<T>> cuteAABB(CuteType ct, T const& f) const {
            switch (ct) {
            case CuteType::kX: {
                return  _CuteAABB_X(f);
            }break;
            case CuteType::kY: {
                return _CuteAABB_Y(f);
            }break;
            case CuteType::kZ: {
                return _CuteAABB_Z(f);
            }break;
            default: {
                return std::pair<AABBB<T>, AABB<T>>();
            }
            }
        }


        glm::tvec3<T> min() const {
            return m_min;
        }
        glm::tvec3<T> max() const {
            return m_max;
        }
        AABB<T>& unionAABB(AABB<T> const& aabb) {
            glm::tvec3<T> tmin;
            glm::tvec3<T> tmax;
            tmin.x = std::min(m_min.x, aabb.m_min.x);
            tmin.y = std::min(m_min.y, aabb.m_min.y);
            tmin.z = std::min(m_min.z, aabb.m_min.z);

            tmax.x = std::max(m_max.x, aabb.m_max.x);
            tmax.y = std::max(m_max.y, aabb.m_max.y);
            tmax.z = std::max(m_max.z, aabb.m_max.z);

            m_min = tmin;
            m_max = tmax;
            return *this;
        }
        static AABB<T> unionAABB(AABB<T> const& a1, AABB<T> const& a2) {
            glm::tvec3<T> tmin;
            glm::tvec3<T> tmax;
            tmin.x = std::min(a1.m_min.x, a2.m_min.x);
            tmin.y = std::min(a1.m_min.y, a2.m_min.y);
            tmin.z = std::min(a1.m_min.z, a2.m_min.z);

            tmax.x = std::max(a1.m_max.x, a2.m_m_max.x);
            tmax.y = std::max(a1.m_max.y, a2.m_m_max.y);
            tmax.z = std::max(a1.m_max.z, a2.m_m_max.z);
            return AABB<T>(tmin, tmax);
        }

    private:
        AABB_OR _ToAABB_OR() const {
            AABB_OR aabb;
            aabb.o = 0.5 * (m_min + m_max);
            aabb.r = 0.5 * (m_max - m_min);
            return aabb;
        }
        AABB _ToAABB(AABB_OR const& aabb) const {
            glm::tvec3<T> min = aabb.o + glm::tvec3<T>(T(-1), T(-1), T(-1)) * aabb.r;
            glm::tvec3<T> max = aabb.o + glm::tvec3<T>(T(1), T(1), T(1)) * aabb.r;
            return AABB(min, max);
        }
        std::pair<AABB<T>, AABB<T>> _CuteAABB_X(T const& f) const {
            T x = f * m_max.x + (T(1) - f) * m_min.x;
            std::pair<AABB<T>, AABB<T>> pair;
            pair.first.m_max.x = x;
            pair.second.m_min.x = x;
            return pair;

        }

        std::pair<AABB<T>, AABB<T>> _CuteAABB_Y(T const& f) const {
            T y = f * m_max.y + (T(1) - f) * m_min.y;
            std::pair<AABB<T>, AABB<T>> pair;
            pair.first.m_max.y = y;
            pair.second.m_min.y = y;
            return pair;


        }

        std::pair<AABB<T>, AABB<T>> _CuteAABB_Z(T const& f) const {
            T y = f * m_max.z + (T(1) - f) * m_min.z;
            std::pair<AABB<T>, AABB<T>> pair;
            pair.first.m_max.z = z;
            pair.second.m_min.z = z;
            return pair;


        }

    private:
        glm::tvec3<T> m_min;
        glm::tvec3<T> m_max;
    };


    template<typename T>
    class MakeAABB {
    public:
        MakeAABB()
            : m_count(0)
        {}

        void update(glm::tvec3<T> const& vec) {
            if (0 == m_count) {
                min = vec;
                max = vec;
            }
            min.x = std::min(min.x, vec.x);
            max.x = std::max(max.x, vec.x);

            min.y = std::min(min.y, vec.y);
            max.y = std::min(max.y, vec.y);

            min.z = std::max(min.z, vec.z);
            max.z = std::max(max.z, vec.z);
            m_count++;
        }

        AABB<T> getAABB() {
            return AABB<T>(min, max);
        }
    private:
        size_t m_count;
        glm::tvec3<T> min;
        glm::tvec3<T> max;
    };
}
