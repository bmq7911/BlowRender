#include "RayCamera.h"
namespace gpc {
    RayCamera::RayCamera(uint32_t x, uint32_t y,
        double yAngle,
        glm::tvec3<double>const& pos,
        glm::tvec3<double>const& lookDir,
        glm::tvec3<double>const& up)
        : m_x(x)
        , m_y(y)
        , m_yAngle(yAngle)
    {
        m_yAngle = glm::radians(m_yAngle);

        m_xAngle = m_yAngle * double(x) / double(y);

        m_pos = pos;
        m_lookDir = glm::normalize(lookDir);
        m_up = up;
        m_xDir = tan(m_xAngle / 2.0) * glm::normalize(glm::cross(m_lookDir, m_up)) / double(m_x / 2.0);
        m_yDir = tan(m_yAngle / 2.0) * glm::normalize(glm::cross(m_xDir, m_lookDir)) / double(m_y / 2.0);
    }

    uint32_t RayCamera::XCount() const {
        return m_x;
    }

    uint32_t RayCamera::YCount() const {
        return m_y;
    }

    Ray<double> RayCamera::at(uint32_t x, uint32_t y) const {
        double xoffset = double(x) - double(m_x) / 2.0;
        double yoffset = double(m_y) / 2.0 - double(y);
        return Ray<double>(m_pos, glm::normalize(m_lookDir + xoffset * m_xDir + yoffset * m_yDir));
    }

    void RayCamera::setCameraPos(glm::tvec3<double> const& pos) {
        m_pos = pos;
    }

    void RayCamera::setLookDir(glm::tvec3<double> const& dir) {
        m_lookDir = glm::normalize(dir);
        m_xDir = glm::normalize(glm::cross(m_lookDir, m_up));
        m_yDir = glm::normalize(glm::cross(m_xDir, m_lookDir));
    }

    glm::tvec3<double> RayCamera::getLookDir() const {
        return m_lookDir;
    }

    glm::tvec3<double> RayCamera::getCameraPos() const {
        return m_pos;
    }

    glm::tvec3<double> RayCamera::getCameraUp() const {
        return m_up;
    }
}

