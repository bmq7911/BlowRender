#pragma once
#include "Rasterization.h"
namespace gpc {
    template<typename _FI>
    class FragmentShader {
    public:
        virtual glm::vec4 execute(gpc::fragment<_FI>  const& v) = 0;

    };
}
