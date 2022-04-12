#include "core/Graphics/AABB.h"

namespace gpc {
	
    AABB::AABB() {
    }

    AABB::AABB(glm::fvec3 min_, glm::fvec3 max_)
        : m_min(min_)
        , m_max(max_)
    {
    }
    bool AABB::in(glm::fvec3 const& p) const {
        return !((p.x < m_min.x || p.x > m_max.x) ||
            (p.y < m_min.y || p.y > m_max.y) ||
            (p.z < m_min.z || p.z > m_max.z));
    }

    bool AABB::in(AABB const& aabb) const {
        return (aabb.m_min.x >= m_min.x && aabb.m_max.x <= m_max.x) &&
            (aabb.m_min.y >= m_min.y && aabb.m_max.y <= m_max.y) &&
            (aabb.m_min.z >= m_min.z && aabb.m_max.z <= m_max.z);

    }

    bool AABB::intersect(AABB const& aabb) const {
        AABB_OR t1 = _ToAABB_OR();
        AABB_OR t2 = aabb._ToAABB_OR();
        glm::fvec3 oo = glm::abs(t1.o - t2.o);
        bool b = (oo.x > t1.r.x + t2.r.x) ||
            (oo.y > t1.r.y + t2.r.y) ||
            (oo.z > t1.r.z + t2.r.z);
        return !b;
    }
    /// 光线与包围盒的计算
    /// AABB 与光线求交还是比我想象的复杂
    bool AABB::hit(Ray const& ray, Float& t, glm::fvec3& n) const {
        Float ox = ray.o().x;
        Float oy = ray.o().y;
        Float oz = ray.o().z;

        Float idx = Float(1) / ray.d().x;
        Float idy = Float(1) / ray.d().y;
        Float idz = Float(1) / ray.d().z;

        Float tx_min, ty_min, tz_min;
        Float tx_max, ty_max, tz_max;

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
        glm::fvec3 n0, n1;;
        Float t0, t1;
        if (tx_min > ty_min) {
            t0 = tx_min;
            n0 = glm::fvec3(-1, 0, 0);
        }
        else {
            t0 = ty_min;
            n0 = glm::fvec3(0, -1, 0);
        }

        if (tz_min > t0) {
            t0 = tz_min;
            n0 = glm::fvec3(0, 0, -1);
        }

        if (tx_max < ty_max) {
            t1 = tx_max;
            n1 = glm::fvec3(1, 0, 0);
        }
        else {
            t1 = ty_max;
            n1 = glm::fvec3(0, 1, 0);
        }

        if (tz_max < t1) {
            t1 = tz_max;
            n1 = glm::fvec3(0, 0, 1);
        }
        /// 相交
        if (t0 < t1 && t1 > EPSILON) {
            if (t0 < Float(0)) {
                t = t1;
                n = -n1;
            }
            else if (t0 >= Float(0)) {
                t = t0;
                n = n0;
            }
            return true;
        }

        return false;
    }
    bool AABB::hit(Ray const& ray) const {
        if (in(ray.o()))
            return true;
        Float t;
        glm::fvec3 n;
        return hit(ray, t, n);
    }
    BVH const* AABB::getBVH() const {
        return this;
    }
    AABB AABB::toAABB() const {
        return *this;
    }


    AABB AABB::subAABB(uint32_t i) const {
        static const glm::fvec3 so[] = {
            {Float(-1.0),Float(-1.0), Float(-1.0)},
            {Float(-1.0),Float(1.0), Float(-1.0)},
            {Float(1.0), Float(1.0) , Float(-1.0)},
            {Float(1.0), Float(-1.0), Float(-1.0)},


            {Float(-1.0),Float(-1.0), Float(1.0)},
            {Float(-1.0),Float(1.0),  Float(1.0)},
            {Float(1.0), Float(1.0),  Float(1.0)},
            {Float(1.0), Float(-1.0), Float(1.0)},

        };
        if (i < 8) {
            glm::fvec3 o = Float(0.5) * (m_min + m_max);
            glm::fvec3 r = Float(0.25) * (m_max - m_min);
            AABB_OR aabb;
            aabb.o = o + so[i] * r;
            aabb.r = r;
            return _ToAABB(aabb);
        }
        return AABB();
    }
    AABB AABB::centerAABB(Float const& f) const {
        AABB_OR t1 = _ToAABB_OR();
        t1.r = t1.r * f;
        return _ToAABB(t1);
    }

    std::pair<AABB, AABB> AABB::cuteAABB(CuteType ct, Float const& f) const {
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
            return std::pair<AABB, AABB>();
        }
        }
    }


    glm::fvec3 AABB::min() const {
        return m_min;
    }
    glm::fvec3 AABB::max() const {
        return m_max;
    }
    AABB& AABB::unionAABB(AABB const& aabb) {
        glm::fvec3 tmin;
        glm::fvec3 tmax;
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
    AABB AABB::unionAABB(AABB const& a1, AABB const& a2) {
        glm::fvec3 tmin;
        glm::fvec3 tmax;
        tmin.x = std::min(a1.m_min.x, a2.m_min.x);
        tmin.y = std::min(a1.m_min.y, a2.m_min.y);
        tmin.z = std::min(a1.m_min.z, a2.m_min.z);

        tmax.x = std::max(a1.m_max.x, a2.m_max.x);
        tmax.y = std::max(a1.m_max.y, a2.m_max.y);
        tmax.z = std::max(a1.m_max.z, a2.m_max.z);
        return AABB(tmin, tmax);
    }

    AABB::AABB_OR AABB::_ToAABB_OR() const {
        AABB_OR aabb;
        aabb.o = Float(0.5) * (m_min + m_max);
        aabb.r = Float(0.5) * (m_max - m_min);
        return aabb;
    }
    AABB AABB::_ToAABB(AABB_OR const& aabb) const {
        glm::fvec3 min = aabb.o + glm::fvec3(Float(-1), Float(-1), Float(-1)) * aabb.r;
        glm::fvec3 max = aabb.o + glm::fvec3(Float(1), Float(1), Float(1)) * aabb.r;
        return AABB(min, max);
    }
    std::pair<AABB, AABB> AABB::_CuteAABB_X(Float const& f) const {
        Float x = f * m_max.x + (Float(1) - f) * m_min.x;
        std::pair<AABB, AABB> pair;
        pair.first.m_max.x = x;
        pair.second.m_min.x = x;
        return pair;

    }

    std::pair<AABB, AABB> AABB::_CuteAABB_Y(Float const& f) const {
        Float y = f * m_max.y + (Float(1) - f) * m_min.y;
        std::pair<AABB, AABB> pair;
        pair.first.m_max.y = y;
        pair.second.m_min.y = y;
        return pair;


    }

    std::pair<AABB, AABB> AABB::_CuteAABB_Z(Float const& f) const {
        Float y = f * m_max.z + (Float(1) - f) * m_min.z;
        std::pair<AABB, AABB> pair;
        pair.first.m_max.z = y;
        pair.second.m_min.z = y;
        return pair;


    }
}