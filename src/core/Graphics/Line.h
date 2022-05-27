#pragma once
#include "glm/glm.hpp"
#include "BFloat.h"

namespace gpc{
    class Line {
    public:
        Line(glm::fvec3 const& a, glm::fvec3 const& b)
            : m_a(a)
            , m_b(b)
        {}
        glm::fvec3 const& a() const {
            return m_a;
        }
        glm::fvec3 const& b() const {
            return m_b;
        }
        glm::fvec3 dir() const {
            return m_b - m_a;
        }
            
    private:
        glm::fvec3 m_a;
        glm::fvec3 m_b;
    };
    template<typename T>
    class Line2d {
    public:
        Line2d(glm::tvec2<T> const& a, glm::tvec2<T> const& b) 
            : m_a(a )
            , m_b(b )
        {
        }
        
        glm::tvec2<T> const& a() const {
            return m_a;
        }

        glm::tvec2<T> const& b() const {
            return m_b;
        }

        glm::tvec2<T> dir() const {
            return m_b - m_a;
        }

        glm::tvec2<T> normal() const {
            glm::tvec2<T> dir = dir();
            return glm::tvec2<T>(dir.y, -dir.x);
        }

    private:
        glm::tvec2<T> m_a;
        glm::tvec2<T> m_b;
    };

}