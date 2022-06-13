#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "win/win32.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexShader.h"
#include "Rasterize/RasterizePipeline.h"
#include "Graphics/ProjectionCamera.h"
#include "Graphics/Image.h"
#include "Graphics/Texture.h"
#include "helper/TextureLoad.h"
#include "helper/ModelLoad.h"
#include "Graphics/AABB.h"
#include "Ray/RayTracePipeline.h"
#include "scene.h"
#include "TestScene.h"
#include "rui/widget_root.h"

/*
 * 坐标结构是 ^ y
 *           |
 *          o|_____> x 因此对应的帧缓存之中的数据就是 y * width +x 我们的访问方式 (x,y) 形式
 *
 */
template<typename T>
class C {
public:

    template<typename U>
    friend C<U> operator* (C<U> const& c1, C<U> const& c2);
    template<typename U>
    friend C<U> operator* ( U const& v, C<U> const& c );
    template<typename U>
    friend C<U> operator+(C<U> const& c1, C<U> const& c2);

public:
    C() 
        : m_r(T{})
        , m_i(T{})
    {
    }
    C(T r, T i ) 
        : m_r( r)
        , m_i( i)
    {}
    C( const C & c) {
        m_r = c.m_r;
        m_i = c.m_i;
    }
    C& operator=(C const& c) {
        m_r = c.m_r;
        m_i = c.m_i;
        return *this;
    }

    T r() const {
        return m_r;
    }
    T i() const {
        return m_i;
    }
    T length() const {
    
    }
    T length2() const {
        return m_r * m_r + m_i * m_i;
    }
    C& operator *=(T const* v) {
        m_r *= v;
        m_i *= v;
        return *this;
    }
    C& operator+=(C const& t) {
        m_r += t.m_r;
        m_i += t.m_i;
        return *this;
    }
    C& operator-=(C const& t) {
        m_r -= t.m_r;
        m_i -= t.m_i;
        return *this;
    }

private:
    T m_r;
    T m_i;
};

template<typename T>
C<T> operator+(C<T> const& c1, C<T> const& c2) {
    C<T> c;
    c.m_r = c1.m_r + c2.m_r;
    c.m_i = c1.m_i + c2.m_i;
    return c;
}

template<typename T>
C<T> operator* (C<T> const& c1, C<T> const& c2) {
    C<T> c;
    c.m_r = c1.m_r * c2.m_r - c1.m_i * c2.m_i;
    c.m_i = c1.m_r * c2.m_i + c1.m_i * c2.m_r;
    return c;
}

template<typename T>
C<T> operator* ( T const& v , C<T> const& c ){
    C<T> t;
    t.m_r = v * c.m_r;
    t.m_i = v * c.m_i;
    return t;
}




class MyBlowWindow : public win::BlowWindow {
public:
    MyBlowWindow(const char* title, uint32_t width, uint32_t height, std::shared_ptr<gpc::Device> device);
    void tick(float passTime, float deltaTime) override;
    void initScene();
    void initAABB();
    void drawAABB( gpc::AABB const & aabb);
    void drawOTree(gpc::OTree<gpc::Object> const* tree);
    void processInput(float passTime, float deltaTime) override;
private:
    void _tickScene(float passTime, float deltaTime);
    void _tickIMGUI( float passTime, float deltaTime );
    void _InitJuliaSence();
    void _InitIMGUI() override;
private:
    std::shared_ptr<gpc::VertexBuffer<Vertex>> m_vertexBuffer;
    std::shared_ptr<gpc::VertexBuffer<Vertex>> m_vertexBufferLine;
    std::shared_ptr<gpc::VertexBuffer<glm::vec2>> m_vertexJulia;
    std::shared_ptr<gpc::RasterizePipeline<Vertex, ShaderPass>>   m_pipeline;
    std::shared_ptr<gpc::RasterizePipeline<glm::vec2, glm::vec4>> m_juliaPipeline;

    std::shared_ptr<gpc::RasterizePipeline<helper::Vertex, ShaderPass>> m_modelPipeline;
    std::shared_ptr<gpc::RasterizePipeline<glm::vec3, glm::vec3>>       m_aabbPipeline;

    std::shared_ptr<gpc::VertexBuffer<helper::Vertex>> m_modelVertexBuffer;
    std::shared_ptr<glm::mat4> m;
    std::shared_ptr<glm::mat4> m_pv;
    std::shared_ptr<gpc::VertexBuffer<glm::vec3>> m_aabbVertex;

    std::shared_ptr<gpc::MoveProjectionCamera> m_camera;
    std::shared_ptr<gpc::Texture2d> m_texture;
    std::shared_ptr<gpc::OTree<gpc::Object>> m_aabbTree;
    std::shared_ptr<gpc::scene> m_scene;
    std::shared_ptr<gpc::Device> m_device;
    std::shared_ptr<rui::widget_tree>      m_ruiRoot;
    std::shared_ptr<TestScene>             m_testScene;
    C<float> m_c;
    C<float> m_t;
};
