#include "OrthoCamera.h"
namespace gpc {
    glm::mat4 OrthoCamera::to_mat() const {
        return to_proj() * to_view();
    }

    glm::mat4 OrthoCamera::to_proj() const {
        return glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
    }

    glm::mat4 OrthoCamera::to_view() const {
        return glm::lookAt(m_pos, m_pos + m_front, m_up);
    }
}

