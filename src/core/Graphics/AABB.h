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
    class AABB : public BVH { /// 在世界坐标系里
        struct AABB_OR {
            glm::fvec3 o;
            glm::fvec3 r;
        };
    public:
        enum CuteType {
            kX,
            kY,
            kZ,
        };

        AABB();

        AABB(glm::fvec3 min_, glm::fvec3 max_);
        bool in(glm::fvec3 const& p) const;
        bool in(AABB const& aabb) const;
        bool intersect(AABB const& aabb) const;
        /// 光线与包围盒的计算
        /// AABB 与光线求交还是比我想象的复杂
        bool hit(Ray const& ray, Float& t, glm::fvec3& n) const override;
        bool hit(Ray const& ray) const override;
        BVH const* getBVH() const override;
        AABB toAABB() const override;

    public:
        AABB subAABB(uint32_t i) const;
        AABB centerAABB(Float const& f) const;
        std::pair<AABB, AABB> cuteAABB(CuteType ct, Float const& f) const;

        glm::fvec3 min() const;
        glm::fvec3 max() const;
        AABB& unionAABB(AABB const& aabb);
        static AABB unionAABB(AABB const& a1, AABB const& a2);
    private:
        AABB_OR _ToAABB_OR() const;
        AABB _ToAABB(AABB_OR const& aabb) const;
        std::pair<AABB, AABB> _CuteAABB_X(Float const& f) const;

        std::pair<AABB, AABB> _CuteAABB_Y(Float const& f) const;
        std::pair<AABB, AABB> _CuteAABB_Z(Float const& f) const;
    private:
        glm::fvec3 m_min;
        glm::fvec3 m_max;
    };



    class MakeAABB {
    public:
        MakeAABB()
            : m_count(0)
        {}

        void update(glm::fvec3 const& vec) {
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

        AABB getAABB() {
            return AABB(min, max);
        }
    private:
        size_t m_count;
        glm::fvec3 min;
        glm::fvec3 max;
    };
}
