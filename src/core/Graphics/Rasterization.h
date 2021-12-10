#pragma once
#include <algorithm>
#include "BFloat.h"
#include "glm/glm.hpp"

namespace gpc {
    template<typename _FI>
    struct primitive {
        glm::vec4 pos; /// 归一化的空间坐标系
        _FI       data;
    };

    template<typename _FI>
    struct fragment {
        int32_t x;
        int32_t y;
        glm::vec4 pos;
        _FI       data;
    };

    /// 图元的装配和插值
    template<typename _FI>
    class  RasterizationPoint {
    public:
        primitive<_FI> rasterizePoint(glm::vec4 p) {
            if ((-1.0f <= p.x && p.x <= 1.0f) && (-1.0f <= p.y && p.y <= 1.0f)) {
                uint32_t x = 0;
                uint32_t y = 0;
                _ComputeScreenIndex(p, x, y);
                return primitive();
            }
            return primitive();
        }

    private:
        primitive<_FI> p1;
        primitive<_FI> p2;
        primitive<_FI> p3;
    };


    template<typename _FI>
    class RasterizationLine {
    public:
        RasterizationLine(float width, float  height)
            : m_width(width)
            , m_height(height)
        {
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
                    float k = float(oy - y1) / float(y0 - y1);
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
    class RasterizationTriangle {
        enum State {
            kLeftTop,
            kLeftButtom,
            kRightTop,
            kRightButtom,
            kTopLeft,
            kTopRight,
            kButtomLeft,
            kButtomRight,
        };
    public:
        RasterizationTriangle(primitive<_FI> const& p1,
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


        void interpolate(fragment<_FI>& io) {
            float a = ((p2_x - io.x) * (p3_y - io.y) - (p3_x - io.x) * (p2_y - io.y)) * d;
            float b = ((io.x - p1_x) * (p3_y - p1_y) - (p3_x - p1_x) * (io.y - p1_y)) * d;
            float c = (1.0f - a - b);
            io.pos = a * p1.pos + b * p2.pos + c * p3.pos;
            io.data = a * p1.data + b * p2.data + c * p3.data;
        }
        /// 重构这个算法
        template<typename FUNC>
        size_t rasterizeTriangle(primitive<_FI> const& p1, primitive<_FI> const& p2, primitive<_FI> const& p3, FUNC const& draw) {
            size_t count = 0;

            glm::ivec2 vp1 = ToViewport(p1.pos);
            glm::ivec2 vp2 = ToViewport(p2.pos);
            glm::ivec2 vp3 = ToViewport(p3.pos);

            AABB aabb = MakeAABB(vp1, vp2, vp3);
            int32_t dy = aabb.maxY - aabb.minY;
            int32_t dx = aabb.maxX - aabb.minX;

            ///  1.这里只考虑到了最长边在对角线上,而有可能最长边不在对角线上
            ///  2.在最长边我们可以减少_Weight的次数
            ///  4.这里的算法太低效了,处理的顶点数太多

            for (int32_t x = aabb.minX; x <= aabb.maxX; ++x) {
                for (int32_t y = aabb.minY; y <= aabb.maxY; ++y) {
                    glm::vec3 w = Weight(glm::ivec2(x, y));
                    if (w.x < -EPSILON || w.y < -EPSILON || w.z < -EPSILON) {
                        continue;
                    }
                    {
                        _Draw(x, y, w, draw);
                        count++;
                    }
                }
            }


            return count;
        }



        glm::ivec2 ToViewport(glm::vec4 const& p) {
            glm::ivec2 o;
            o.x = static_cast<int32_t>((kW * (p.x + 1.0f) + 0.5f));
            o.y = static_cast<int32_t>((kH * (p.y + 1.0f) + 0.5f));
            return o;
        }
    public:


        struct AABB {
            int32_t minX, minY;
            int32_t maxX, maxY;
        };

       
        AABB MakeAABB(glm::ivec2 const& p1, glm::ivec2 const& p2, glm::ivec2 const& p3) {
            AABB aabb;

            aabb.minX = std::min({ p1.x, p2.x, p3.x });
            aabb.maxX = std::max({ p1.x, p2.x, p3.x });
            aabb.minY = std::min({ p1.y, p2.y, p3.y });
            aabb.maxY = std::max({ p1.y, p2.y, p3.y });

            return aabb;
        }
        
        glm::vec3 Weight(glm::vec2 io) {
            float a = ((p2_x - io.x) * (p3_y - io.y) - (p3_x - io.x) * (p2_y - io.y)) * d;
            float b = ((io.x - p1_x) * (p3_y - p1_y) - (p3_x - p1_x) * (io.y - p1_y)) * d;
            float c = (1.0f - a - b);
            return glm::vec3(a, b, c);
        }
    private:
        uint32_t _OnAABB(glm::ivec3 const& p, AABB const& aabb) {
            const static uint32_t _X = 1;
            const static uint32_t _Y = 2;
            uint32_t ret = 0;
            if (p.x == aabb.minX || p.x == aabb.minY) {
                ret |= _X;
            }
            if (p.y == aabb.minY || p.y == aabb.maxY) {
                ret |= _Y;
            }
            return ret;
        }
        
        State _MakeAABB(glm::ivec2 const& p1, glm::ivec2 const& p2, glm::ivec2 const& p3, glm::ivec2 const*& op1, glm::ivec2 const*& op2) {
            AABB aabb;

            aabb.minX = std::min({ p1.x, p2.x, p3.x });
            aabb.maxX = std::max({ p1.x, p2.x, p3.x });
            aabb.minY = std::min({ p1.y, p2.y, p3.y });
            aabb.maxY = std::max({ p1.y, p2.y, p3.y });
            int32_t dx = aabb.maxX - aabb.minX;
            int32_t dy = aabb.maxY - aabb.minY;

            /// 1.这里我们需要找到一条最长线作为基准进行绘制

            if (dx > dy) { /// 遍历x
                op2 = &p1;
                if (op2->x < p2.x)
                    op2 = &p2;
                if (op2->x < p3.x)
                    op2 = &p3;

                op1 = &p1;
                if (op1->x > p2.x)
                    op1 = &p2;
                if (op1->x > p3.x)
                    op1 = &p3;
                glm::ivec2 const* o;
                if (&p1 != op1 && &p1 != op2) {
                    o = &p1;
                }
                else if (&p2 != op1 && &p2 != op2) {
                    o = &p2;
                }
                else {
                    o = &p3;
                }
                ///

                glm::ivec2 v01 = *op1 - *o;
                glm::ivec2 v02 = *op2 - *o;

                int32_t z = v01.x * v02.y - v02.x * v01.y;


                /// 这种if - else 语句是否可以转换为查表 原因是 if else ( if else 语句会破坏流水线结构 但是小代码应该还好)
                if (z > 0) { /// 上方
                    if (op2->y >= op1->y) {
                        return kLeftTop;
                    }
                    else {
                        return kRightTop;
                    }
                }
                else { // 下方
                    if (op2->y >= op1->y) {
                        return kLeftButtom;
                    }
                    else {
                        return kRightButtom;
                    }
                }

            }
            else { /// 遍历y
                op2 = &p1;
                if (op2->y < p2.y)
                    op2 = &p2;
                if (op2->y < p3.y)
                    op2 = &p3;

                op1 = &p1;
                if (op1->y > p2.y)
                    op1 = &p2;
                if (op1->y > p3.y)
                    op1 = &p3;


                glm::ivec2 const* o;
                if (&p1 != op1 && &p1 != op2) {
                    o = &p1;
                }
                else if (&p2 != op1 && &p2 != op2) {
                    o = &p2;
                }
                else {
                    o = &p3;
                }

                glm::ivec2 v01 = *op1 - *o;
                glm::ivec2 v02 = *op2 - *o;

                int32_t z = v01.x * v02.y - v02.x * v01.y;
                if (z > 0) { /// 右侧
                    if (op2->x >= op1->x) {
                        return kTopLeft;
                    }
                    else {
                        return kButtomLeft;
                    }
                }
                else { // 左侧
                    if (op2->x >= op1->x) {
                        return kTopRight;
                    }
                    else {
                        return kButtomRight;
                    }
                }
            }
        }
        
        template<typename FUNC>
        void _Draw(uint32_t x, uint32_t y, glm::vec3 const& w, FUNC const& draw) {
            fragment<_FI> e;
            e.x = x;
            e.y = y;
            e.pos = w.x * m_p1.pos + w.y * m_p2.pos + w.z * m_p3.pos;
            e.data = w.x * m_p1.data + w.y * m_p2.data + w.z * m_p3.data;
            draw(e);
        }


        void _Init() { ///在二维空间插值

            kH = (m_height - 1) / 2.0f;
            kW = (m_width - 1) / 2.0f;
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

