
#pragma once
#include "Graphics/Camera.h"
#include "glm/glm.hpp"
#include "Graphics/Ray.h"
namespace gpc {
    class RayCamera {
    public:
        RayCamera(uint32_t x, uint32_t y, Float yAngle,
            glm::fvec3 const& pos = glm::fvec3(0.0, 0.0, 0.0),
            glm::fvec3 const& lookDir = glm::fvec3(0.0, 0.0, -1.0),
            glm::fvec3 const& up = glm::fvec3(0.0, 1.0, 0.0));
        uint32_t XCount() const;
        uint32_t YCount() const;
        Ray at(uint32_t x, uint32_t y) const;
        void setCameraPos(glm::fvec3 const& pos);
        void setLookDir(glm::fvec3 const& dir);

        glm::fvec3 getLookDir() const;
        glm::fvec3 getCameraPos() const;
        glm::fvec3 getCameraUp() const;
    private:
        uint32_t m_x;
        uint32_t m_y;
        Float m_yAngle;
        Float m_xAngle;
        glm::fvec3 m_pos;
        glm::fvec3 m_lookDir;
        glm::fvec3 m_xDir;
        glm::fvec3 m_yDir;
        glm::fvec3 m_up;
    };
}