#pragma once
#include <stdio.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "BFloat.h"
namespace gpc {
    enum CameraMovement {
        kForward,
        kBackward,
        kLeft,
        kRight,
        kUp,
        kDown,
    };
    enum RotateAxis {
        kFrontAxis,
        kUpAxis,
        kRightAxis,

    };

    class Camera {
    public:
        virtual glm::fmat4 to_mat()  const = 0;
        virtual glm::fmat4 to_proj() const = 0;
        virtual glm::fmat4 to_view() const = 0;

    };
}
