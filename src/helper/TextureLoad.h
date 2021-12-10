#pragma once
#include <memory>
#include "Graphics/Texture.h"
#include "Graphics/AABB.h"

namespace helper {
    class TextureLoad {
    public:
        std::shared_ptr<gpc::Texture2d> makeTexture2D(const char * path) const;
    };
}