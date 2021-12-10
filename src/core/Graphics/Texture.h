#pragma once
#include <vector>
#include "Graphics/Image.h"
#include "glm/glm.hpp"
#include "Graphics/Graphics.h"

namespace helper {
    class TextureLoad;
}
namespace gpc {
    /// 只支持3中纹理
    class Texture2d {
        using byte = unsigned char;
    public:
        Texture2d(int32_t width, int32_t height)
            : m_image(width, height)
            , m_iwidth(width)
            , m_iheight(height)
            , m_bilinerInterpolation(float(width), float(height), float(width), float(height))
        {
            m_width = width - 1.0f;
            m_height = height - 1.0f;
        }
        /// 复杂性来自于采样与插值
        glm::vec4 lookup(float x, float y) const {
            int ix = static_cast<int>(x * m_width - 0.5f);
            int iy = static_cast<int>(y * m_height - 0.5f);

            return _at(ix, iy);
        }
        glm::vec4 lookup_biliner_interpolation(float x, float y) const {
            float fx = x * m_width;
            float fy = y * m_height;
            glm::vec4 w = m_bilinerInterpolation.weight(fx, fy);
            int32_t ix = int32_t(fx);
            int32_t iy = int32_t(fy);
            return w.x * _at(ix, iy + 1) + w.y * _at(ix + 1, iy) + w.z * _at(ix, iy) + w.w * _at(ix + 1, iy);
        }

        glm::vec4 lookup_wrap_repeat(float x, float y) const {
            int ix = static_cast<int>(x * m_width - 0.5f);
            int iy = static_cast<int>(x * m_width - 0.5f);
            ix = ix % m_iwidth;
            iy = iy & m_iheight;
            return _at(ix, iy);
        }

        glm::vec4 look_wrap_color(float x, float y, glm::vec4 const& color) const {
            int ix = static_cast<int>(x * m_width - 0.5f);
            int iy = static_cast<int>(x * m_height - 0.5f);
            if (0 <= ix && ix < m_iwidth && 0 <= iy && iy < m_iheight) {
                return _at(ix, iy);
            }
            else {
                return color;
            }
        }
        float width() const {
            return m_width + 1;
        }
        float height() const {
            return m_height + 1;
        }

        friend class helper::TextureLoad;

    private:
        glm::vec4 _at(int32_t x, int32_t y) const {
            x = std::max(0, std::min(x, m_iwidth - 1));
            y = std::max(0, std::min(x, m_iheight - 1));
            glm::tvec4<byte> t = m_image.at(x, y);
            glm::vec4 c;
            c.x = t.x / 255.0f;
            c.y = t.y / 255.0f;
            c.z = t.z / 255.0f;
            c.w = t.w / 255.0f;
            return c;
        }
    private:
        float m_width;
        float m_height;
        int32_t m_iwidth;
        int32_t m_iheight;
        gpc::Image2d<glm::tvec4<byte>> m_image;
        gpc::BilinearInterpolation<float> m_bilinerInterpolation;

    };

    class Texture2dMip {
    public:
        glm::vec4 at(float x, float y, float mip) {

        }
    private:
        std::vector<Texture2d* > m_mipTexture;
    };

    class CubeTexture {


    };
}