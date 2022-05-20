#pragma once
#include <algorithm>
#include "BFloat.h"
#include "glm/glm.hpp"
#include "Graphics.h"

namespace gpc {
    template<typename _FI>
    struct primitive {
        glm::fvec4 pos; /// 归一化的空间坐标系
        _FI       data;
    };

    template<typename _FI>
    struct fragment {
        int32_t x;
        int32_t y;
        glm::fvec4 pos;
        _FI       data;
    };

    /// 图元的装配和插值
    template<typename _FI>
    class  RasterizationPoint {
    public:
        primitive<_FI> rasterizePoint(glm::fvec4 p) {
            if ((-1.0f <= p.x && p.x <= 1.0f) && (-1.0f <= p.y && p.y <= 1.0f)) {
                uint32_t x = 0;
                uint32_t y = 0;
                //_ComputeScreenIndex(p, x, y);
                return primitive<_FI>();
            }
            return primitive<_FI>();
        }

    private:
        primitive<_FI> p1;
        primitive<_FI> p2;
        primitive<_FI> p3;
    };


    template<typename _FI>
    class RasterizationLine {
    public:
        RasterizationLine(Float width, Float  height)
            : m_width(width)
            , m_height(height)
        {
        }

        size_t count( primitive<_FI> const& p1, primitive<_FI> const& p2 ) {
            int32_t x0 = 0.5f * (p1.pos.x + 1.0f) * (m_width - 1.0f);
            int32_t y0 = 0.5f * (p1.pos.y + 1.0f) * (m_height - 1.0f);
            int32_t x1 = 0.5f * (p2.pos.x + 1.0f) * (m_width - 1.0f);
            int32_t y1 = 0.5f * (p2.pos.y + 1.0f) * (m_height - 1.0f);
            return std::max(std::abs(x1 - x0) + 1, std::abs(y1 - y0) + 1);
        }
        /// 这里装配线的时候,是否考虑使用 Bresenham 算法
        template<typename FUNC>
        size_t rasterizeLine(primitive<_FI> const& p1, primitive<_FI> const& p2, FUNC const& draw) {
            size_t count = 0;

            int32_t x0 = 0.5f * (p1.pos.x + 1.0f) * (m_width - 1.0f);
            int32_t y0 = 0.5f * (p1.pos.y + 1.0f) * (m_height - 1.0f);
            int32_t x1 = 0.5f * (p2.pos.x + 1.0f) * (m_width - 1.0f);
            int32_t y1 = 0.5f * (p2.pos.y + 1.0f) * (m_height - 1.0f);

            Bresenham b(x0, y0, x1, y1);
            bool ret = true;
            int32_t ox, oy;

            if (x0 == x1) {
                while (ret) {
                    ret = b.getDrawPos(ox, oy);
                    /// 插值
                    Float k = Float(oy - y1) / Float(y0 - y1);
                    fragment<_FI> f;
                    f.x = ox;
                    f.y = oy;
                    f.pos = k * p1.pos + (1 - k) * p2.pos;
                    f.data = k * p1.data + (1 - k) * p2.data;
                    draw(f);
                    count++;
                }
            }
            else if (y0 == y1) {
                while (ret) {
                    ret = b.getDrawPos(ox, oy);
                    /// 插值
                    float t1 = float(ox - x1);
                    float t2 = float(x0 - x1);
                    float k = float(ox - x1) / float(x0 - x1);

                    fragment<_FI> f;
                    f.x = ox;
                    f.y = oy;
                    f.pos = k * p1.pos + (1 - k) * p2.pos;
                    f.data = k * p1.data + (1 - k) * p2.data;
                    draw(f);
                    count++;
                }
            }
            else if (fabs(y0 - y1) > fabs(x0 - x1)) { /// dy > dx 
                float m = float(x0 - x1) / float(y0 - y1); /// dx / dy 
                while (ret) {
                    ret = b.getDrawPos(ox, oy);
                    /// 插值
                    float kx = float(ox - x1) / float(x0 - x1);
                    float ky = float(oy - y1) / float(y0 - y1);

                    float k = ky * m + (1 - m) * kx;
                    fragment<_FI> f;
                    f.x = ox;
                    f.y = oy;
                    f.pos = k * p1.pos + (1 - k) * p2.pos;
                    f.data = k * p1.data + (1 - k) * p2.data;
                    draw(f);
                    count++;
                }
            }
            else {
                float m = float(y0 - y1) / float(x0 - x1); /// dy / dx
                while (ret) {
                    ret = b.getDrawPos(ox, oy);
                    /// 插值
                    float kx = float(ox - x1) / float(x0 - x1);
                    float ky = float(oy - y1) / float(y0 - y1);

                    float k = kx * m + (1 - m) * ky;

                    fragment<_FI> f;
                    f.x = ox;
                    f.y = oy;
                    f.pos = k * p1.pos + (1 - k) * p2.pos;
                    f.data = k * p1.data + (1 - k) * p2.data;
                    draw(f);
                    count++;
                }
            }
            return count;
        }

        void rasterizeLine(primitive<_FI> const& p1, primitive<_FI> const& p2, primitive<_FI>& p3) {
            float k = 0.0f;
            if (p1.pos.x == p2.pos.x) {
                k = float(p3.pos.y - p2.pos.y) / float(p1.pos.y - p2.pos.y);

            }
            else if (p1.pos.y == p2.pos.y) {
                k = float(p3.pos.x - p2.pos.x) / float(p1.pos.x - p2.pos.x);
            }
            else if (fabs(p1.pos.y - p2.pos.y) > fabs(p1.pos.x - p2.pos.x)) { /// dy > dx 
                float m = float(p1.pos.x - p2.pos.x) / float(p1.pos.y - p2.pos.y); /// dx / dy 
                float kx = float(p3.pos.x - p2.pos.x) / float(p1.pos.x - p2.pos.x);
                float ky = float(p3.pos.y - p2.pos.y) / float(p1.pos.y - p2.pos.y);
                k = ky * m + (1 - m) * kx;

            }
            else {
                float m = float(p1.pos.y - p2.pos.y) / float(p1.pos.x - p2.pos.x); /// dy / dx
                    /// 插值
                float kx = float(p3.pos.x - p2.pos.x) / float(p1.pos.x - p2.pos.x);
                float ky = float(p3.pos.y - p2.pos.y) / float(p1.pos.y - p2.pos.y);
                k = kx * m + (1 - m) * ky;

            }
            p3.pos = k * p1.pos + (1 - k) * p2.pos;
            p3.data = k * p1.data + (1 - k) * p2.data;
        }
    private:
        float m_width;
        float m_height;
    };


    template<typename _FI>
    class TriangleInterpolate {
    public:
        struct AABB {
            int32_t minX, minY;
            int32_t maxX, maxY;
        };

        TriangleInterpolate(primitive<_FI> const& p1,
            primitive<_FI> const& p2,
            primitive<_FI> const& p3, float width, float height)
            : m_p1(p1)
            , m_p2(p2)
            , m_p3(p3)
            , m_width(width)
            , m_height(height)
        {
            _Init();
        }

        void interpolate(fragment<_FI>& io, glm::vec3 const& w) {
            io.pos = w.x * m_p1.pos + w.y * m_p2.pos + w.z * m_p3.pos;
            io.data = w.x * m_p1.data + w.y * m_p2.data + w.z* m_p3.data;
        }

        glm::ivec2 toViewport(glm::vec4 const& p) {
            glm::ivec2 o;
            o.x = static_cast<int32_t>((kW * (p.x + 1.0f) + 0.5f));
            o.y = static_cast<int32_t>((kH * (p.y + 1.0f) + 0.5f));
            return o;
        }
       
        AABB makeAABB(glm::ivec2 const& p1, glm::ivec2 const& p2, glm::ivec2 const& p3) {
            AABB aabb;
            aabb.minX = std::min({ p1.x, p2.x, p3.x });
            aabb.maxX = std::max({ p1.x, p2.x, p3.x });
            aabb.minY = std::min({ p1.y, p2.y, p3.y });
            aabb.maxY = std::max({ p1.y, p2.y, p3.y });
            return aabb;
        }
        
        glm::vec3 weight(glm::vec2 io) {
            float a = ((p2_x - io.x) * (p3_y - io.y) - (p3_x - io.x) * (p2_y - io.y)) * d;
            float b = ((io.x - p1_x) * (p3_y - p1_y) - (p3_x - p1_x) * (io.y - p1_y)) * d;
            float c = (1.0f - a - b);
            return glm::vec3(a, b, c);
        }
    private:
        void _Init() { ///在二维空间插值

            //kH = (m_height - 1) / 2.0f;
            //kW = (m_width - 1) / 2.0f;
            kH = (m_height ) / 2.0f;
            kW = (m_width ) / 2.0f;
            p1_x = kW * (m_p1.pos.x + 1.0f);
            p1_y = kH * (m_p1.pos.y + 1.0f);

            p2_x = kW * (m_p2.pos.x + 1.0f);
            p2_y = kH * (m_p2.pos.y + 1.0f);

            p3_x = kW * (m_p3.pos.x + 1.0f);
            p3_y = kH * (m_p3.pos.y + 1.0f);

            //d = 1.0f / ((m_p2.pos.x - m_p1.pos.x) * (m_p3.pos.y - m_p1.pos.y) - (m_p3.pos.x - m_p1.pos.x) * (m_p2.pos.y - m_p1.pos.y));
            d = 1.0f / ((p2_x - p1_x) * (p3_y - p1_y) - (p3_x - p1_x) * (p2_y - p1_y));
        }
    private:
        primitive<_FI> m_p1;
        float p1_x, p1_y;
        primitive<_FI> m_p2;
        float p2_x, p2_y;
        primitive<_FI> m_p3;
        float p3_x, p3_y;
        float          kW, kH;
        float          m_width;
        float          m_height;
        float          d;
    };
}

