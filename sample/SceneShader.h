#pragma once
#include "Graphics/VertexShader.h"
#include <glm/glm.hpp>

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

/// uniform 变量怎么传递进去
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
    /// 把uniform作为成员放到这里怎么样
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

class BasicVertexShader : public gpc::VertexShader<helper::Vertex,helper::Vertex> {
public:
    BasicVertexShader(glm::mat4* model_, glm::mat3* modelT_, glm::mat4* view_, glm::mat4* proj_, glm::mat4* mvp_) 
        : model( model_), modelT(modelT_),view( view_), proj(proj_), mvp(mvp_)
    {}
    glm::vec4 execute( helper::Vertex const& vertex, helper::Vertex & out) override {
        glm::vec4 t = (*mvp) * glm::vec4( vertex.aPos ,1.0f);

        out.aPos     = glm::mat3((*model)) * vertex.aPos;
        out.aNormal = glm::normalize(glm::vec3((*modelT) * vertex.aNormal));
        out.aTex     = vertex.aTex;
        out.aTangent = vertex.aTangent;
        out.aJoint   = vertex.aJoint;
        out.aWeight  = vertex.aWeight;
        return t;
    }
private:
    glm::mat4* model;
    glm::mat3* modelT;
    glm::mat4* view;
    glm::mat4* proj;
    glm::mat4* mvp;
};

class PointLightFragmentShader : public gpc::FragmentShader<helper::Vertex> {
public:
    PointLightFragmentShader( glm::vec3 * lightPos_, glm::vec3 * lightColor_)
        : lightPos(lightPos_)
        , lightColor( lightColor_)
    {
    
    }

    glm::vec4 execute(gpc::fragment<helper::Vertex> const& primitive) {
        return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        helper::Vertex const& v = primitive.data;
        glm::vec3 dir = glm::normalize( *lightPos- v.aPos);
        Float d = glm::dot(dir, v.aNormal);
        if (d < 0.0f) {
            return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else {
            return glm::vec4( d * (* lightColor), 1.0f);
        }
    }
private:
    glm::vec3* lightPos;
    glm::vec3* lightColor;
};