#pragma once

#include <stdint.h>
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "BFloat.h"

namespace gpc {
    class OrthoCamera : public Camera {
    public:
        glm::fmat4 to_mat() const override;
        glm::fmat4 to_proj() const override;
        glm::fmat4 to_view() const override;
    protected:
        glm::fvec3 m_pos;
        glm::fvec3 m_front;
        glm::fvec3 m_up;
        glm::fvec3 m_vright;
        Float m_left;
        Float m_right;
        Float m_top;
        Float m_bottom;
        Float m_near;
        Float m_far;


    };
}
