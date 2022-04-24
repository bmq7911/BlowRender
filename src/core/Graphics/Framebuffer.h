#pragma once
#include "Graphics/Image.h"
#include "glm/glm.hpp"
#include <functional>
#include "BFloat.h"

namespace gpc {
    class Framebuffer : public Image2d<glm::vec4i8> {
    public:
        typedef glm::vec4i8 value_type;
        Framebuffer(uint32_t width, uint32_t height);
        void draw_point(glm::ivec2 const& pos, Float depth, glm::vec4i8 const& pixel);

        void draw_point(uint32_t x, uint32_t y, Float depth, glm::vec4i8 const& pixel);
        Float depth_at(uint32_t x, uint32_t y) const;

        void write_depth(uint32_t x, uint32_t y, Float depth);
        void toData(unsigned char* data);
        void clearDepth(Float t);


    private:
        Image2d<Float> m_depth;
    };
}
