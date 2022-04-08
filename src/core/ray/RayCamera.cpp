#include "RayCamera.h"
namespace gpc {
    RayCamera::RayCamera(uint32_t x, uint32_t y,
        Float yAngle,
        glm::fvec3 const& pos,
        glm::fvec3 const& lookDir,
        glm::fvec3 const& up)
        : m_x(x)
        , m_y(y)
        , m_yAngle(yAngle)
    {
        m_yAngle = glm::radians(m_yAngle);

        m_xAngle = m_yAngle * double(x) / double(y);

        m_pos = pos;
        m_lookDir = glm::normalize(lookDir);
        m_up = up;
        m_xDir = Float(std::tan(m_xAngle / 2.0)) * glm::normalize(glm::cross(m_lookDir, m_up)) / Float( m_x / 2.0);
        m_yDir = Float(std::tan(m_yAngle / 2.0)) * glm::normalize(glm::cross(m_xDir, m_lookDir)) / Float(m_y / 2.0);
    }

    uint32_t RayCamera::XCount() const {
        return m_x;
    }

    uint32_t RayCamera::YCount() const {
        return m_y;
    }

    Ray RayCamera::at(uint32_t x, uint32_t y) const {
        Float xoffset = Float(x) - Float(m_x) / 2.0;
        Float yoffset = Float(m_y) / 2.0 - Float(y);
        return Ray(m_pos, glm::normalize(m_lookDir + xoffset * m_xDir + yoffset * m_yDir));
    }

    void RayCamera::setCameraPos(glm::fvec3 const& pos) {
        m_pos = pos;
    }

    void RayCamera::setLookDir(glm::fvec3 const& dir) {
        m_lookDir = glm::normalize(dir);
        m_xDir = glm::normalize(glm::cross(m_lookDir, m_up));
        m_yDir = glm::normalize(glm::cross(m_xDir, m_lookDir));
    }

    glm::fvec3 RayCamera::getLookDir() const {
        return m_lookDir;
    }

    glm::fvec3 RayCamera::getCameraPos() const {
        return m_pos;
    }

    glm::fvec3 RayCamera::getCameraUp() const {
        return m_up;
    }
}

