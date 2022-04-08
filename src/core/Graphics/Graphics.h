#pragma once
#include <math.h>
#include <algorithm>
#include "glm/glm.hpp"
#include "BFloat.h"

namespace gpc {

    class Liang_Barsky {
    public:
        Liang_Barsky(Float lx, Float rx, Float ty, Float by)
            : m_left(lx)
            , m_right(rx)
            , m_top(ty)
            , m_bottom(by)
        {}
        /// 处理平行情
        /// 很好的是浮点数能计算出负无穷
        bool cuteLine(glm::vec2 const& p0, glm::vec2 const& p1, glm::vec2& op1, glm::vec2& op2) {
            /// 本质上使用了放射几何
            glm::vec2 v = p1 - p0;


            /// k = (AP * n) / (AB . n )  p.x - a.x / 
            /// v = p1 - p0 ( x1 - x0, y1 - y0)
            /// Nleft   = (1.0f,0)     -> dLeft   = dot(v,N) = x1-x0         p = (m_lef,1 )   or (m_left,-1) -> k = AP .* N / ( AB .* N) ->  ( m_left - x0, 1 - y0 ) .* (1.0f,0 )     / AB .* N = ( m_left - x0)  / ( x1 - x0 )
            /// Nright  = (-1.0f,0)    -> dRight  = dot(v,N) = -(x1 - x0 )   p = (m_right,1 ) or (m_right,0) -> k = AP .* N / ( AB .* N) ->  (m_right - x0, 1 - y0 ) .* (-1.0f,0 )    / AB .* N = (- m_right + x0)  / ( -x1 + x0 )
            /// Ntop    = (0.0f,-1.0f) -> dTop    = dot(v,N) = -(y1 - y0)    p = (-1.0,m_top )or (1,m_top)   -> k = AP .* N / ( AB .* N) ->  (1 - x0, m_top - y0 )   .* (0.0f,-1.0f ) / AB .* N = (- m_top + y0)  / ( -y1 + y0 )
            /// Nbottom = (0.0f, 1.0f) -> dBottom = dot(v,N) = y1-y0         p = (-1.0,m_b )  or (1,m_b)     -> k = AP .* N / ( AB .* N) ->  (1 - x0, m_b - y0 )     .* (0.0f,1.0f )  / AB .* N = ( m_top - y0)  / ( y1 - y0 )

            Float dLeft = p1.x - p0.x;
            Float APLeft = m_left - p0.x;

            Float dRight = -(p1.x - p0.x);
            Float APRight = -m_right + p0.x;

            Float dTop = -(p1.y - p0.y);
            Float APTop = -m_top + p0.y;

            Float dBottom = p1.y - p0.y;
            Float APBottom = m_bottom - p0.y;

            /// 入矩形
            Float u1 = 0.0f;
            Float u2 = 1.0f;


            /// chu
            if (gpc::SignBit(dLeft)) {
                //if (dLeft < 0) {
                u2 = std::min(u2, APLeft / dLeft);
            }
            else {
                u1 = std::max(u1, APLeft / dLeft);
            }

            //if (dRight < 0) {
            if (gpc::SignBit(dRight)) {
                u2 = std::min(u2, APRight / dRight);
            }
            else {
                u1 = std::max(u1, APRight / dRight);
            }


            //if (dTop < 0) {
            if (gpc::SignBit(dTop)) {
                u2 = std::min(u2, APTop / dTop);
            }
            else {
                u1 = std::max(u1, APTop / dTop);
            }


            //if (dBottom < 0) {
            if (gpc::SignBit(dBottom)) {
                u2 = std::min(u2, APBottom / dBottom);
            }
            else {
                u1 = std::max(u1, APBottom / dBottom);
            }
            if (u1 > u2)
                return false;
            op1 = p0 + u1 * (p1 - p0);
            op2 = p0 + u2 * (p1 - p0);
            return true;
            ///1. 我们先不考虑平行的情况
            /// 
        }
    private:
        Float m_left, m_right, m_top, m_bottom;
    };

    /// 只能画水平线,不能画垂直线
    /// 光栅化的画线算法
    /// 
    class Bresenham {
    public:
        Bresenham(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
            : m_x0(x0), m_y0(y0), m_x1(x1), m_y1(y1)
            , m_swap(false)
        {
            m_dir = true;
            if ((x1 - x0) * (y1 - y0) < 0) {
                m_dir = false;
            }
            m_dx = abs((x1 - x0));
            m_dy = abs((y1 - y0));
            if (m_dx >= m_dy) {
                m_p = 2 * m_dy - m_dx;
                m_twoDy = 2 * m_dy;
                m_twoDyMinusDx = 2 * (m_dy - m_dx);
                m_x, m_y;
                if (x0 > x1) {
                    m_x = x1;
                    m_y = y1;
                    m_x1 = x0;
                }
                else {
                    m_x = x0;
                    m_y = y0;
                }
                m_swap = false;
            }
            else { /// m_dx < m_dy => k > 1
                m_p = 2 * m_dx - m_dy;
                m_twoDy = 2 * m_dx;
                m_twoDyMinusDx = 2 * (m_dx - m_dy);
                m_x, m_y;
                if (y0 > y1) {
                    m_x = x1;
                    m_y = y1;
                    m_y1 = y0;
                }
                else {
                    m_x = x0;
                    m_y = y0;
                }
                m_swap = true;
            }


        }


        template<typename FUNC, typename ... _Args>
        void draw(FUNC const& func, _Args && ...args) {
            bool bLoop = true;
            func(m_x, m_y, std::forward<_Args>(args) ...);
            if (m_swap) {
                if (false == m_dir) {
                    while (m_y < m_y1) {
                        m_y++;
                        if (m_p < 0)
                            m_p += m_twoDy;
                        else {
                            m_x--;
                            m_p += m_twoDyMinusDx;
                        }
                        func(m_x, m_y, std::forward<_Args>(args) ...);
                    }
                }
                else {
                    while (m_y < m_y1) {
                        m_y++;
                        if (m_p < 0)
                            m_p += m_twoDy;
                        else {
                            m_x++;
                            m_p += m_twoDyMinusDx;
                        }
                        func(m_x, m_y, std::forward<_Args>(args) ...);
                    }
                }
            }
            else {
                if (false == m_dir) { /// 
                    while (m_x < m_x1) {
                        m_x++;
                        if (m_p < 0)
                            m_p += m_twoDy;
                        else {
                            m_y--;
                            m_p += m_twoDyMinusDx;
                        }
                        func(m_x, m_y, std::forward<_Args>(args) ...);
                    }
                }
                else {
                    while (m_x < m_x1) {
                        m_x++;
                        if (m_p < 0)
                            m_p += m_twoDy;
                        else {
                            m_y++;
                            m_p += m_twoDyMinusDx;
                        }
                        func(m_x, m_y, std::forward<_Args>(args) ...);
                    }
                }
            }
        }

        bool getDrawPos(int32_t& ox, int32_t& oy) {
            ox = m_x;
            oy = m_y;
            if (m_swap) {
                if (m_y < m_y1) {
                    m_y++;
                    if (m_p < 0)
                        m_p += m_twoDy;
                    else {
                        if (m_dir == false)
                            m_x--;
                        else
                            m_x++;
                        m_p += m_twoDyMinusDx;
                    }
                    return true;
                }
                return false;
            }
            else {
                if (m_x < m_x1) {
                    m_x++;
                    if (m_p < 0)
                        m_p += m_twoDy;
                    else {
                        if (m_dir == false)
                            m_y--;
                        else
                            m_y++;
                        m_p += m_twoDyMinusDx;
                    }
                    return true;
                }
                return false;
            }

        }

    private:
        int32_t m_x0, m_y0, m_x1, m_y1;
        int32_t m_dx, m_dy, m_p, m_twoDy, m_twoDyMinusDx, m_x, m_y;
        bool m_swap;
        bool m_dir;
    };

    /// 三角形裁剪算法
    class SutherlandHodgman {
    public:
        SutherlandHodgman(Float left, Float right, Float top, Float buttom)
            : m_left(left)
            , m_right(right)
            , m_top(top)
            , m_buttom(buttom)
        {}
        int32_t ClipTriangle(glm::vec2(&in)[7], uint32_t count) {
            in[3] = in[0];
            Boundary winEdge = Boundary::kLeft;
            glm::vec2 out[7];
            glm::vec2* pIn = in;
            glm::vec2* pOut = out;
            for (; winEdge <= Boundary::kTop; winEdge = static_cast<Boundary>(winEdge + 1)) {
                uint32_t k = 0;
                for (size_t i = 0; i < count; ++i) {
                    int32_t t = _Type(pIn[i], pIn[i + 1], winEdge);
                    if (t == kII) {
                        pOut[k] = pIn[i + 1];
                        k++;
                    }
                    else if (t == kIO) {
                        glm::vec2 ret = _Intersect(pIn[i], pIn[i + 1], winEdge);
                        pOut[k] = ret;
                        k++;
                    }
                    else if (t == kOI) {
                        glm::vec2  ret = _Intersect(pIn[i], pIn[i + 1], winEdge);
                        pOut[k] = ret;
                        k++;
                        pOut[k] = pIn[i + 1];
                        k++;
                    }
                }
                count = k;
                pOut[k] = pOut[0];
                std::swap(pIn, pOut);
            }
            return count;
        }
    private:
        enum Boundary {
            kLeft = 0,
            kRight = 1,
            kButtom = 2,
            kTop = 3,
        };
        enum {
            kII = 3,
            kIO = 1,
            kOI = 2,
            kOO = 0,

        };
        char _Inside(glm::vec2 const& p, Boundary b) {
            switch (b) {
            case gpc::SutherlandHodgman::kLeft: /// 0
                if (p.x < m_left) /// p.x - m_left < 0
                    return 0;
                break;
            case gpc::SutherlandHodgman::kRight: //1
                if (p.x > m_right) /// m_right - p.x < 
                    return 0;
                break;
            case gpc::SutherlandHodgman::kButtom: //2
                if (p.y < m_buttom) /// p.y - m_buttom < 0
                    return 0;
                break;
            case gpc::SutherlandHodgman::kTop: //3
                if (p.y > m_top)   /// m_top - p.y < 0
                    return 0;
                break;
            default:
                break;
            }
            return 1;
        }
        bool _Cross(glm::vec2 const& p1, glm::vec2 const& p2, Boundary b) {
            if (_Inside(p1, b) == _Inside(p2, b))
                return false;
            else return true;
        }
        ///
        glm::vec2 _Intersect(glm::vec2 const& p1, glm::vec2 const& p2, Boundary winEdge) {
            glm::vec2 iPt;
            Float m;
            if (p1.x != p2.x)
                m = (p1.y - p2.y) / (p1.x - p2.x);
            switch (winEdge) {
            case Boundary::kLeft:
                iPt.x = m_left;
                iPt.y = p2.y + (m_left - p2.x) * m;
                break;
            case Boundary::kRight:
                iPt.x = m_right;
                iPt.y = p2.y + (m_right - p2.x) * m;
                break;
            case Boundary::kButtom:
                iPt.y = m_buttom;
                if (p1.x != p2.x)
                    iPt.x = p2.x + (m_buttom - p2.y) / m;
                else
                    iPt.x = p2.x;
                break;
            case Boundary::kTop:
                iPt.y = m_top;
                if (p1.x != p2.x)
                    iPt.x = p2.x + (m_top - p2.y) / m;
                else
                    iPt.x = p2.x;
                break;
            default:
                break;
            }
            return iPt;
        }

        int32_t _Type(glm::vec2 const& p1, glm::vec2 const& p2, Boundary b) {
            char b1 = _Inside(p1, b);
            char b2 = _Inside(p2, b) << 1;

            return b1 | b2;
            /*
            if (b1 && b2) { 01 | 10 == 11  = 3
                return kII;
            }
            else if (b1 && (!b2)) { /// 01 | 00  == 01 = 1
                return kIO;
            }
            else if((!b1) &&b2){ /// 00 | 10 = 10  = 2
                return kOI;
            }
            else {  /// 00 | 00 == 0 = 0
                return kOO;
            }
            */
        }

    private:
        Float m_left, m_right;
        Float m_top, m_buttom;
    };

    template<typename T, typename U = typename std::enable_if<std::is_floating_point<T>::value>::type>
    class BilinearInterpolation {
    public:
        template<typename _T1>
        struct float_trait {
            using type = void;
        };
        template<>
        struct float_trait<float> {
            using type = bool;
        };
        template<>
        struct float_trait<double> {
            using type = char;
        };
        template<>
        struct float_trait<long double> {
            using type = int;
        };

    public:
        explicit BilinearInterpolation(T sm, T sn, T dm, T dn)
            : sM(sm), sN(sn), dM(dm), dN(dn)
        {
            kM = (sM - 1) / (dM - 1);
            kN = (sN - 1) / (dN - 1);
        }
        glm::tvec4<T> weight(T x, T  y) const {
            Float tx = kM * x;
            Float ty = kN * y;
            /// 能否避免取整这个操作
            Float ox = _Floor(tx,typename float_trait<T>::type());
            Float oy = _Floor(ty,typename float_trait<T>::type());

            Float x12 = tx - ox;
            Float x11 = 1 - x12;

            Float y12 = ty - oy;
            Float y11 = 1 - y12;

            glm::fvec4 w;
            w.x = y12 * x11;
            w.y = x12 * y12;
            w.z = x11 * y11;
            w.w = x12 * y11;
            return w;
        }
    public:
        Float _Floor(Float value, bool) const {
            return floorf(value);
        }
        Float _Floor(Float value, char) const {
            return floor(value);
        }
        Float _Floor(Float value, int) const {
            return floorl(value);
        }
    public:
        Float kM, kN;
        Float sM, sN;
        Float dM, dN;
    };
}
