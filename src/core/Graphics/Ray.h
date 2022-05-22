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
        glm::fvec3 const& o() const {
            return m_o;
        }
        glm::fvec3 const& d() const {
            return m_d;
        }
    public:
        glm::fvec3 m_o;
        glm::fvec3 m_d;
    };
    template<typename T = Float>
    class Ray2d{
    public:
        Ray2d( glm::fvec2 const& pos, glm::fvec2 const& dir)
            : m_o( pos )
        {
            m_d = glm::normalize( dir );
        }
        glm::tvec2<T> at( T k) const{
            return m_o + k * m_d;
        }
        glm::tvec2<T> o() const{
            return m_o;
        }
        glm::tvec2<T> d() const{
            return m_d;
        }
    private:
        glm::tvec2<T> m_o;
        glm::tvec2<T> m_d;
    };
}
