#pragma once

#include <stdint.h>
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"

namespace gpc {
    class OrthoCamera : public Camera {
    public:
        glm::mat4 to_mat() const override;
        glm::mat4 to_proj() const override;
        glm::mat4 to_view() const override;
    protected:
        glm::vec3 m_pos;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_vright;
        float m_left;
        float m_right;
        float m_top;
        float m_bottom;
        float m_near;
        float m_far;


    };
}
