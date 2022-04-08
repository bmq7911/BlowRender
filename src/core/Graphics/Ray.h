#pragma once
#include "glm/glm.hpp"
#include "BFloat.h"

namespace gpc {
    class Ray {
    public:
        Ray(glm::fvec3 const& pos, glm::fvec3 const& dir)
            : m_o(pos)
        {
            m_d = glm::normalize(dir);
        }
        glm::fvec3 at( Float k) const {
            return m_o + k * m_d;
        }
        glm::fvec3 o() const {
            return m_o;
        }
        glm::fvec3 d() const {
            return m_d;
        }
    public:
        glm::fvec3 m_o;
        glm::fvec3 m_d;
    };
}
