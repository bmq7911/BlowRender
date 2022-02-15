#pragma once
#include <memory>
#include <utility>

#include "Graphics/VertexDispatch.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/VertexShader.h"
#include "Graphics/FragmentShader.h"
#include "Graphics/VertexDispatch.h"
#include "Graphics/PrimitiveAssembly.h"
#include "Graphics/Rasterization.h"
#include "Graphics/Graphics.h"
#include "log.h"
#include "BFloat.h"
#include "device/Device.h"

/// 发掘任务的并行性


namespace gpc {

    enum PrimitiveType {
        kPoint,
        kLine,
        kLineStrip,
        kTriangle,
        kTriangleStrip,
    };

    enum CullMode {
        kFront,
        kBack,
        kFrontBack,
        kNone,
    };

    enum FrontFace {
        kClockWise,
        kCounterClockWise,
    };

    struct depth_compare_equal {
        bool operator()(float old_, float new_) {
            return old_ == new_;
        }
    };

    struct depth_compare_less {
        bool operator()(float old_, float new_) {
            return new_ < old_;
        }
    };

    struct depth_compare_lessequal {
        bool operator() (float old_, float new_) {
            return new_ <= old_;
        }
    };

    struct depth_compare_greater {
        bool operator()(float old_, float new_) {
            return new_ > old_;
        }
    };

    struct depth_compare_greaterequal {
        bool operator() (float old_, float new_) {
            return new_ >= old_;
        }
    };

    template<typename _VI, typename _FI>
    class RasterizePipeline;

    template<typename _VI,typename _FI>
    class VertexShaderTaskIssue : public ITaskIssue {
    public:
        VertexShaderTaskIssue(std::shared_ptr<gpc::VertexBuffer<_VI>> vertexBuffer, RasterizePipeline<_VI, _FI>* pipeline)
            : m_vertexBuffer(vertexBuffer)
            , m_primitives(vertexBuffer->sizeVertex())
            , m_rasterizePipeline(pipeline)
        {
        }
        void init(uint32_t size) override { }
   
        uint32_t doTask(Tid3 const& tid) override{
            _VI v = m_vertexBuffer->atVertex(tid.x);
            _FI fi;
            glm::vec4 pos = m_rasterizePipeline->_ExecuteVertexShader(v, fi);
            m_primitives[tid.x].data = fi;
            m_primitives[tid.x].pos = pos;
            return 0;
        }
        Dim3 getTaskDim() const override{
            Dim3 size;
            size.x = m_primitives.size();
            size.y = 1;
            size.z = 1;
            return size;
        }
        std::vector<primitive<_FI>>& getPrimitiveBuffer() {
            return m_primitives;
        }
    private:
        std::shared_ptr<gpc::VertexBuffer<_VI>> m_vertexBuffer;
        std::vector<primitive<_FI>> m_primitives;
        
        friend class RasterizePipeline<_VI,_FI>;
        RasterizePipeline<_VI, _FI>* m_rasterizePipeline;
    };

    template<typename _VI, typename _FI >
    class FragmentShaderTaskIssue : public ITaskIssue {
    public:
        FragmentShaderTaskIssue( RasterizePipeline<_VI,_FI> * rasterizePipeline, TriangleInterpolate<_FI>* rasterizetor ,uint32_t x,uint32_t y, uint32_t width, uint32_t height ) 
            : m_minX(x)
            , m_minY( y )
            , m_pipeline( rasterizePipeline )
            , m_rasterizetor(rasterizetor)
        {
            m_size.x = width;
            m_size.y = height;
            m_size.z = 1;
        }

        void init(uint32_t count) override { }

        uint32_t doTask(Tid3 const& tid) override {
            glm::vec2 pix(tid.x + m_minX, tid.y + m_minY);
            glm::vec3 w = m_rasterizetor->weight( pix );
            if ( w.x < -EPSILON || w.y <-EPSILON || w.z < -EPSILON ) {
                return 0;
            }
            else {
                fragment<_FI> e;
                e.x = pix.x;
                e.y = pix.y;
                m_rasterizetor->interpolate(e, w);
                m_pipeline->_ExecuteFragmentShader(e );
            }
            
            return 0;
        }

        Dim3 getTaskDim() const override {
            return m_size;
        }
    private:
        
        uint32_t m_minX;
        uint32_t m_minY;
        Dim3     m_size;
        friend class RasterizePipeline<_VI, _FI>;
        RasterizePipeline<_VI, _FI>* m_pipeline;
        TriangleInterpolate<_FI>* m_rasterizetor;
    };
    
    template<typename _VI, typename _FI>
    class LineFragmentShaderTaskIssue : public ITaskIssue {
    public:
        LineFragmentShaderTaskIssue(RasterizePipeline<_VI, _FI>* rasterizePipeline,float width, float height,
                                     primitive<_FI> const& vs, primitive<_FI> const& ve,
                                     primitive<_FI> const&rs, primitive<_FI> const&re )
            : m_pipeline( rasterizePipeline )
            , m_width( width)
            , m_height( height )
            , m_vs( vs )
            , m_ve( ve )
            , m_rs( rs )
            , m_re( re )
        {
        }
        void init(uint32_t count) override {

            RasterizationLine<_FI> RL( m_width, m_height );
            uint32_t vertexCount = RL.count(m_rs, m_re);
            if (vertexCount < count) {
                m_segs.push_back( std::make_pair(m_rs, m_re));
                m_count = 1;
            }
            else {
                auto deltaP = (m_re.pos - m_rs.pos) / float(count);
                glm::vec4 pos = m_rs.pos;
                for (size_t i = 0; i < count; ++i) {
                    primitive<_FI> s;
                    primitive<_FI> e;
                    s.pos = pos;
                    pos += deltaP;
                    e.pos = pos;
                    /// <summary>
                    ///  这样误差的积累会不会导致问题
                    /// </summary>
                    /// <param name="count"></param>
                    RL.rasterizeLine(m_vs, m_ve, s);
                    RL.rasterizeLine(m_vs, m_ve, e);
                    m_segs.push_back(std::make_pair(s,e));
                }
                m_count = count;
            }
        }

        uint32_t doTask(Tid3 const& tid) override {
            RasterizationLine<_FI> RL( m_width,m_height );
            auto draw = [this](fragment<_FI> const& e)->void {
                this->draw(e);
            };
            RL.rasterizeLine(m_segs[tid.x].first, m_segs[tid.x].second ,  draw );
            return 0;
        }
        Dim3 getTaskDim() const override {
            Dim3 dim;
            dim.x = m_count;
            dim.y = 1;
            dim.z = 1;
            return dim;
        }
        void draw(fragment<_FI> const& e ) {
            m_pipeline->_ExecuteFragmentShader(e );
        }
    private:
        friend class RasterizePipeline<_VI, _FI>;
        RasterizePipeline<_VI, _FI>* m_pipeline;
        float m_width;
        float m_height;
        primitive<_FI> m_vs;
        primitive<_FI> m_ve;
        primitive<_FI> m_rs;
        primitive<_FI> m_re;
        std::vector<std::pair<primitive<_FI>, primitive<_FI>>> m_segs;
        uint32_t m_count;
    };

    template< typename _VI, typename _FI>
    class RasterizePipeline {
        
        friend VertexShaderTaskIssue<_VI, _FI>;
        friend FragmentShaderTaskIssue<_VI, _FI>;
        friend LineFragmentShaderTaskIssue<_VI, _FI>;

#define COUNTER_CLOCK_WISE -1
#define CLOCK_WISE         1
    public:
        RasterizePipeline( std::shared_ptr<gpc::Device> device )
            : m_bDepth(false)
            , m_bDepthWrite(false)
            , m_depthCompare(depth_compare_less())
            , m_ClockWise(CLOCK_WISE)
            , m_cullMode(CullMode::kNone)
            , m_epsilon(FLT_EPSILON)
            , m_device( device )
        {}
        void setEnableDepthTest(bool bDepth) {
            m_bDepth = bDepth;
        }
        void setEnableDepthWrite(bool bWrite) {
            m_bDepthWrite = bWrite;
        }
        void setFrontFace(FrontFace frontFace) {
            if (frontFace == FrontFace::kCounterClockWise)
                m_ClockWise = COUNTER_CLOCK_WISE;
            m_ClockWise = CLOCK_WISE;
        }

        void setCullMode(CullMode cullMode) {
            m_cullMode = cullMode;
        }

        void bindVertexBuffer(std::shared_ptr<VertexBuffer<_VI>> vbo) {
            m_vbo = vbo;
        }

        void bindFramebuffer(std::shared_ptr<gpc::Framebuffer> fbo) {
            m_width = (float)fbo->getWidth();
            m_height = (float)fbo->getHeight();
            //m_epsilon = 4.0f / (m_width * m_height);
            m_epsilon = 1e-05f;
            m_kW = (m_width - 1) / 2.0f;
            m_kH = (m_height - 1) / 2.0f;
            m_fbo = fbo;
        }

        void bindVertexShader(std::shared_ptr<gpc::VertexShader<_VI, _FI>> vertexShader) {
            m_vertexShader = vertexShader;
        }

        void bindFragmentShader(std::shared_ptr<gpc::FragmentShader<_FI>> fragmentShader) {
            m_fragmentShader = fragmentShader;
        }

        void draw(PrimitiveType type) {
            switch (type) {
            case gpc::kPoint:
                //_DrawPoint( );
                break;
            case gpc::kLine:
                _DrawLineMultiThread<gpc::VertexDispatch_line<_VI,_FI>>();
                break;
            case gpc::kLineStrip:
                //_DrawLineStrip( );
                _DrawLineMultiThread<gpc::VertexDispatch_linestrip<_VI,_FI>>( );
                break;
            case gpc::kTriangle: {
                if (m_cullMode == CullMode::kFrontBack) {///
                    return;
                }
                else {
                    _DrawTriangleMultiThread<gpc::VertexDispatch_triangle<_VI,_FI>>( );
                }
            }break;
            case gpc::kTriangleStrip: {
                if (m_cullMode == CullMode::kFrontBack) {
                    return;
                }
                else {
                    _DrawTriangleMultiThread<gpc::VertexDispatch_trianglestrip<_VI,_FI>>();
                }
            } break;
            default:
                break;
            }
        }

    private:
        glm::vec4 _ExecuteVertexShader(_VI const& v, _FI& fi) {
            return m_vertexShader->execute(v, fi);
        }
        void _ExecuteFragmentShader( fragment<_FI> const & e) {
            _Draw(e);
        }
        
        template<typename Dispatch>
        void _DrawTriangleMultiThread(  ) {
            /// <summary>
            /// 1.execute vertex shader
            /// </summary>
            std::shared_ptr<VertexShaderTaskIssue<_VI,_FI>>  vertexExecute = std::make_shared<VertexShaderTaskIssue<_VI,_FI>>( m_vbo, this );
            m_device->bindTaskIssue( vertexExecute);
            m_device->wait( );
            Dispatch vertexDispatch;
            /// 2. Vertex Dispatch for main thread
            vertexDispatch.bindVertexBuffer(m_vbo);
            vertexDispatch.bindPrimitiveBuffer( vertexExecute->getPrimitiveBuffer( ));
            primitive<_FI> e1, e2, e3;

            /// <summary>
            /// 主线程循环dispatch 顶点
            /// </summary>
            while (true == vertexDispatch.dispatch(e1, e2, e3)) {
                /// 这个是在主线程之中做的,
                /// 2.1 图元方向判定,进行剔除
                glm::vec4 pos1 = e1.pos / e1.pos.w;
                glm::vec4 pos2 = e2.pos / e2.pos.w;
                glm::vec4 pos3 = e3.pos / e3.pos.w;
                
                int32_t z = _CounterClockWise( pos1, pos2, pos3 );
                if (z == 0) { /// 三角形退化成线,我们选择不绘制
                    //LOG_TRACE("asembly triangle", "discard the primitive with the point on line");
                    return;
                }
                else {
                    auto t = z + m_ClockWise;
                    /// 将浮点数或者复杂的逻辑比较转换为 整数比较
                    ////   设置正反方向   z > 0 z < 0
                    /// 1. 顺时针 1      正面 1 反面 -1 ///剔除背面 1 + -1 = 0  /// 剔除正面 1 +1 = 2
                    /// 2. 逆时针 -1     反面 1 正面 -1 ///剔除背面 -1 + 1 = 0  /// 剔除正面 -1 + -1 = -2

                    ////   设置正反方向   z > 0 z < 0
                    /// 1. 顺时针 1      正面 -1 反面 1 ///剔除背面 1 + 1 = 2   /// 剔除正面 1 + -1 = 0 
                    /// 2. 逆时针 -1     反面 -1 正面 1 ///剔除背面 -1 + -1 = -2 /// 剔除正面 -1 + 1 = 0 

                    ///    我们是 if 逻辑,我们能否变成最基本的算术运算,减小逻辑复杂度
                    if ((m_cullMode == CullMode::kBack) && (!t)) {
                        //LOG_TRACE("assembly triangle", "discard the primitive with the CullMode::kBack");
                        return;
                    }
                    else if ((m_cullMode == CullMode::kFront) && (t)) {
                        //LOG_TRACE("assembly triangle", "discard the primitive with the CullMode::kFront");
                        return;
                    }
                    else { /// CullMode::kNone

                    }
                }

                /// 3.图元的三维剪切还是投影后进行二维剪切,生成更多图元,
                SutherlandHodgman sh(-1.0f, 1.0f, 1.0f, -1.0f);
                glm::vec2 out[7];
                primitive<_FI> ele[7];
                out[0] = glm::vec2(pos1.x, pos1.y);
                out[1] = glm::vec2(pos2.x, pos2.y);
                out[2] = glm::vec2(pos3.x, pos3.y);

                size_t pointCount = sh.ClipTriangle(out, 3);
                if (pointCount < 3) {
                    return;
                }
                else {
                    /// 的考虑误差的积累(运算导致的)
                    TriangleInterpolate<_FI> pat(e1, e2, e3, m_width, m_height);
                    auto draw = [this](fragment<_FI> const& e) {
                        _Draw(e);
                    };

                    size_t primitiveCount = 0;
                    for (size_t i = 1; i < pointCount - 1; ++i) {
                        ele[0].pos.x = out[0].x;
                        ele[0].pos.y = out[0].y;

                        ele[i].pos.x = out[i].x;
                        ele[i].pos.y = out[i].y;

                        ele[i + 1].pos.x = out[i + 1].x;
                        ele[i + 1].pos.y = out[i + 1].y;

                        glm::ivec2 vp1 = pat.toViewport(ele[0].pos);
                        glm::ivec2 vp2 = pat.toViewport(ele[i].pos);
                        glm::ivec2 vp3 = pat.toViewport(ele[i+1].pos);

                        typename TriangleInterpolate<_FI>::AABB aabb = pat.makeAABB( vp1, vp2, vp3);
                        std::shared_ptr<FragmentShaderTaskIssue<_VI, _FI>> fragmentShaderExecute = std::make_shared<FragmentShaderTaskIssue<_VI,_FI> >( this,&pat ,aabb.minX,aabb.minY,aabb.maxX - aabb.minX + 1,   aabb.maxY - aabb.minY + 1 );
                        m_device->bindTaskIssue(fragmentShaderExecute);
                        m_device->wait( );
                    }
                }
            
            }

        }

        template<typename Dispatch>
        void _DrawLineMultiThread( ) {
            std::shared_ptr<VertexShaderTaskIssue<_VI, _FI>> vertexExecute = std::make_shared<VertexShaderTaskIssue<_VI, _FI>>(m_vbo, this);
            m_device->bindTaskIssue(vertexExecute);
            m_device->wait( );
            Dispatch vertexDispatch;
            vertexDispatch.bindVertexBuffer(m_vbo);
            vertexDispatch.bindPrimitiveBuffer( vertexExecute->getPrimitiveBuffer( ));
            primitive<_FI> e1, e2;
            
            auto draw = [this](fragment<_FI>  const& e) {
                _Draw(e);
            };
            while (true == vertexDispatch.dispatch(e1, e2)) {
                /// 这是在二维空间上的剪切
                Liang_Barsky lb(-1.0f, 1.0f, 1.0f, -1.0f);
                glm::vec2 p1, p2;

                /// 先对线进行剪切
                if (false == lb.cuteLine(glm::vec2(e1.pos.x, e1.pos.y), glm::vec2(e2.pos.x, e2.pos.y), p1, p2)) {
                    return;
                }
                else {

                    primitive<_FI> e11;
                    e11.pos = glm::vec4(p1.x, p1.y, 0.0f, 1.0f);
                    primitive<_FI> e22;
                    e22.pos = glm::vec4(p2.x, p2.y, 0.0f, 1.0f);

                    std::shared_ptr<LineFragmentShaderTaskIssue<_VI, _FI>> fragmentShaderExecute = std::make_shared<LineFragmentShaderTaskIssue<_VI, _FI> >(this, m_fbo->getWidth(), m_fbo->getHeight(), e1,e2,e11,e22  );
                    m_device->bindTaskIssue(fragmentShaderExecute);
                    m_device->wait();

                }
            }
        }

    private:
        int _CounterClockWise(glm::vec4 const& v1, glm::vec4 const& v2, glm::vec4 const& v3) {
            glm::vec3 vp1 = glm::vec3(glm::vec2(v2 - v1), 0.0f);  /// 经过了透视除法 -1.0 - 1.0 之间 -100.0 - -100.0
            glm::vec3 vp2 = glm::vec3(glm::vec2(v3 - v2), 0.0f);

            glm::vec3 cross = glm::cross(vp1, vp2);
            /// 这里的这个EPSILON怎么选取
            if (-m_epsilon < cross.z && cross.z < m_epsilon)
                return 0;

            uint32_t bits =gpc::FloatToBits<uint32_t>(cross.z);
            return  (0x80000000 & bits) ? -1 : 1;
        }

        inline glm::vec4i8 _FragmentShader(fragment<_FI> const& e) {
            glm::vec4 fcolor = m_fragmentShader->execute(e);
            glm::vec4i8 icolor;
            icolor.r = int8_t(255.0f * fcolor.r + 0.5f);
            icolor.g = int8_t(255.0f * fcolor.g + 0.5f);
            icolor.b = int8_t(255.0f * fcolor.b + 0.5f);
            icolor.a = int8_t(255.0f * fcolor.a + 0.5f);
            return icolor;
        }

        inline glm::vec4i8 _Blend(glm::vec4i8 const& src, glm::vec4i8 const& color) {

            return color;
        }
        /// 这里是提前测试深度值
        void _Draw(fragment<_FI> const& e) {
            if (true == _DepthTest(e.x, e.y, e.pos.z)) {
                glm::vec4i8 icolor = _FragmentShader(e);
                glm::vec4i8 src = m_fbo->at(e.x, e.y);
                icolor = _Blend(src, icolor);
                m_fbo->draw_point(e.x, e.y, e.pos.z, icolor);
                if (true == m_bDepthWrite)
                    m_fbo->write_depth(e.x, e.y, e.pos.z);
            }

        }

        bool _DepthTest(int32_t x, int32_t y, float d) const {
            if (m_bDepth && !(m_depthCompare(m_fbo->depth_at(x, y), d))) {
                return false;
            }
            return true;
        }
    private:
        bool                                          m_bDepth;
        bool                                          m_bDepthWrite;
        std::function<bool(float, float)>             m_depthCompare;
        int32_t                                       m_ClockWise;
        CullMode                                      m_cullMode;
        float                                         m_width;
        float                                         m_height;
        float                                         m_kW;
        float                                         m_kH;
        float                                         m_epsilon;
        std::shared_ptr<gpc::VertexBuffer<_VI> >      m_vbo;
        std::shared_ptr<gpc::Framebuffer>             m_fbo;
        std::shared_ptr<gpc::VertexShader<_VI, _FI>>  m_vertexShader;
        std::shared_ptr<gpc::FragmentShader<_FI>>     m_fragmentShader;
        std::shared_ptr<gpc::Device>                  m_device;
    };

}
	