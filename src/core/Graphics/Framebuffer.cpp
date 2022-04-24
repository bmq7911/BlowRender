#include "Framebuffer.h"

namespace gpc {
    Framebuffer::Framebuffer(uint32_t width, uint32_t height)
        : Image2d<glm::vec4i8>(width, height)
        , m_depth(width, height)
    {
    }

    void Framebuffer::draw_point(glm::ivec2 const& pos, Float depth, glm::vec4i8 const& pixel) {
        draw(pos.x, pos.y, pixel);
    }

    void Framebuffer::draw_point(uint32_t x, uint32_t y, Float depth, glm::vec4i8 const& pixel) {
        ///2.其余情况都能写入
        draw(x, y, pixel);
    }
    Float Framebuffer::depth_at(uint32_t x, uint32_t y) const {
        return m_depth.at(x, y);
    }
    void Framebuffer::write_depth(uint32_t x, uint32_t y, Float depth) {
        m_depth.draw(x, y, depth);
    }
    void Framebuffer::toData(unsigned char* data) {
        glm::vec4i8* ptr = reinterpret_cast<glm::vec4i8*>(data);
        for (uint32_t i = 0; i < getHeight(); ++i) {
            for (uint32_t k = 0; k < getWidth(); ++k) {
                ptr[i * getRelWidth() + k] = at(k, i);
            }
        }
    }
    void Framebuffer::clearDepth(Float t) {
        m_depth.clear(t);
    }
}