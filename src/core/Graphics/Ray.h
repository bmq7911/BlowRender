#pragma once
#include "glm/glm.hpp"
namespace gpc {
    template<typename _T = double>
    class Ray {
    public:
        Ray(glm::tvec3<_T> const& pos, glm::tvec3<_T> const& dir)
            : m_o(pos)
        {
            m_d = glm::normalize(dir);
        }
        glm::tvec3<_T> at(_T k) const {
            return m_o + k * m_d;
        }
        glm::tvec3<_T> o() const {
            return m_o;
        }
        glm::tvec3<_T> d() const {
            return m_d;
        }
    public:
        glm::tvec3<_T> m_o;
        glm::tvec3<_T> m_d;
    };
}
