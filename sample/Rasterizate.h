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

#include "rui/widget_root.h"
/*
 * ����ṹ�� ^ y
 *           |
 *          o|_____> x ��˶�Ӧ��֡����֮�е����ݾ��� y * width +x ���ǵķ��ʷ�ʽ (x,y) ��ʽ
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



struct Vertex {
    glm::vec3 aPos;
    glm::vec3 aNormal;
    glm::vec2 aTex;    
    Vertex operator + (Vertex const& p) const {
        Vertex t;
        t.aNormal= this->aNormal + p.aNormal;
        t.aPos = this->aPos + p.aPos;
        t.aTex = this->aTex + p.aTex;
        return t;
    }
    Vertex & operator+=(Vertex const& p) {
        aNormal += p.aNormal;
        aPos += p.aPos;
        aTex += p.aTex;
        return *this;
    }
    Vertex operator *(float k) const {
        Vertex t;
        t.aNormal = k * aNormal;
        t.aPos = k * aPos;
        t.aTex = k * aTex;
        return t;
    }
    Vertex & operator*=(float k) {
        aNormal *= k;
        aPos *= k;
        aTex *= k;
        return *this;
    }
};
static inline Vertex operator*(float k, Vertex const& p) {
    return p * k;
}

struct ShaderPass {
    glm::vec3 Normal;
    glm::vec3 pos;
    glm::vec2 tex;
    ShaderPass operator + (ShaderPass const& p) const {
        ShaderPass t;
        t.Normal = this->Normal + p.Normal;
        t.pos = this->pos + p.pos;
        t.tex = this->tex + p.tex;
        return t;
    }
    ShaderPass& operator+=(ShaderPass const& p) {
        Normal += p.Normal;
        pos += p.pos;
        tex += p.tex;
        return *this;
    }
    ShaderPass operator *(float k) const {
        ShaderPass t;
        t.Normal = k * Normal;
        t.pos = k * pos;
        t.tex = k * tex;
        return t;
    }
    ShaderPass& operator*=(float k) {
        Normal *= k;
        pos *= k;
        tex *= k;
        return *this;
    }

};

static inline ShaderPass operator*(float k, ShaderPass const& p) {
    return p * k;
}

/// uniform ������ô���ݽ�ȥ
class VertexShader : public gpc::VertexShader<Vertex, ShaderPass> {
public:
    VertexShader(std::shared_ptr<glm::mat4> m)
        : m_Model(m)
    {}
    glm::vec4 execute(Vertex const& vertex, ShaderPass& out) override {

        out.Normal = vertex.aNormal;
        out.pos = vertex.aPos;
        out.tex = vertex.aTex;
        return (*m_Model * glm::vec4(vertex.aPos, 1.0f));
        //return glm::vec4(vertex.aPos, 1.0f);
    }

private:
    /// ��uniform��Ϊ��Ա�ŵ�������ô��
    std::shared_ptr<glm::mat4> m_Model;
};


class FragmentShader : public gpc::FragmentShader<ShaderPass> {
public:
    FragmentShader(glm::vec3 const& pos_, glm::vec3 const& color_)
        : pos(pos_)
        , color(color_)
    {}
    glm::vec4 execute(gpc::fragment<ShaderPass> const& primitive) override {
        /*
        glm::vec3 dir = glm::normalize(pos - primitive.data.pos);
        float d = glm::dot(dir, glm::normalize(primitive.data.Normal));
        if (d < 0.0f) {
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            glm::vec4 tex_color = texture->lookup(primitive.data.tex.x, primitive.data.tex.y);
            if (primitive.x < 400)
                return tex_color;
            return glm::vec4(color * d, 1.0f);
        }
        */
        return texture->lookup(primitive.data.tex.x, primitive.data.tex.y);
    }
public:
    glm::vec3 pos;
    glm::vec3 color;
    std::shared_ptr<gpc::Texture2d> texture;

};

class ParallelLightFragmentShader : public gpc::FragmentShader<ShaderPass> {
public:
    ParallelLightFragmentShader(glm::vec3 const& dir_, glm::vec3 const& color_)
        : dir(dir_)
        , color(color_)
    {}
    glm::vec4 execute(gpc::fragment<ShaderPass> const& primitive) override {

        float d = glm::dot(dir, primitive.data.Normal);
        if (d < 0.0f) {
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            return glm::vec4(color * d, 1.0f);
        }
    }
public:
    glm::vec3 dir;
    glm::vec3 color;

};

class ModelVertexShader : public gpc::VertexShader<helper::Vertex, ShaderPass> {
public:
    ModelVertexShader(std::shared_ptr<glm::mat4> m)
        : m_Model(m)
    {
    }
    glm::vec4 execute(helper::Vertex const& vertex, ShaderPass& out) {
        out.Normal = vertex.aNormal;
        out.pos = vertex.aPos;
        out.tex = vertex.aTex;
        return (*m_Model * glm::vec4(vertex.aPos, 1.0f));
    }

private:
    std::shared_ptr<glm::mat4> m_Model;
};



class AABBVertexShader : public gpc::VertexShader<glm::vec3,glm::vec3> {
public:
    AABBVertexShader(std::shared_ptr<glm::mat4> m) 
        : m_pv( m )
    {
    }
    glm::vec4 execute(glm::vec3 const& vertex, glm::vec3 & out) {
        return (*m_pv * glm::vec4(vertex, 1.0f));
    }
private:
    std::shared_ptr<glm::mat4> m_pv;
};
class AABBFragmentShader : public gpc::FragmentShader<glm::vec3> {
public:
    glm::vec4 execute( gpc::fragment<glm::vec3> const& primitive ) {
        return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
};

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
    void _InitJuliaSence();
    void _InitIMGUI() override;
    void _tickIMGUI( float passTime, float deltaTime );
private:
    std::shared_ptr<gpc::VertexBuffer<Vertex>> m_vertexBuffer;
    std::shared_ptr<gpc::VertexBuffer<Vertex>> m_vertexBufferLine;
    std::shared_ptr<gpc::VertexBuffer<glm::vec2>> m_vertexJulia;
    std::shared_ptr<gpc::RasterizePipeline<Vertex, ShaderPass>> m_pipeline;
    std::shared_ptr<gpc::RasterizePipeline<glm::vec2, glm::vec4>> m_juliaPipeline;

    std::shared_ptr<gpc::RasterizePipeline<helper::Vertex, ShaderPass>> m_modelPipeline;
    std::shared_ptr<gpc::RasterizePipeline<glm::vec3, glm::vec3>> m_aabbPipeline;

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
    C<float> m_c;
    C<float> m_t;
};
