
#pragma once
#include "Graphics/Camera.h"
#include "glm/glm.hpp"
#include "Graphics/Ray.h"
namespace gpc {
    class RayCamera {
    public:
        RayCamera(uint32_t x, uint32_t y, double yAngle,
            glm::tvec3<double>const& pos = glm::tvec3<double>(0.0, 0.0, 0.0),
            glm::tvec3<double>const& lookDir = glm::tvec3<double>(0.0, 0.0, -1.0),
            glm::tvec3<double>const& up = glm::tvec3<double>(0.0, 1.0, 0.0));
        uint32_t XCount() const;
        uint32_t YCount() const;
        Ray<double> at(uint32_t x, uint32_t y) const;
        void setCameraPos(glm::tvec3<double> const& pos);
        void setLookDir(glm::tvec3<double> const& dir);

        glm::tvec3<double> getLookDir() const;
        glm::tvec3<double> getCameraPos() const;
        glm::tvec3<double> getCameraUp() const;
    private:
        uint32_t m_x;
        uint32_t m_y;
        double m_yAngle;
        double m_xAngle;
        glm::tvec3<double> m_pos;
        glm::tvec3<double> m_lookDir;
        glm::tvec3<double> m_xDir;
        glm::tvec3<double> m_yDir;
        glm::tvec3<double> m_up;
    };
}