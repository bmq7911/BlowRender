#pragma once
#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "widget_base.h"
#include "widget_layout.h"
#include "widget_rect.h"
namespace rui{
    struct Vertex {
        glm::vec3 aPos;
        glm::vec2 aTex;
        Vertex operator + (Vertex const& p) const {
            Vertex t;
            t.aPos = aPos + p.aPos;
            t.aTex = aTex + p.aTex;
            return t;
        }
        Vertex& operator+=(Vertex const& p) {
            aPos += p.aPos;
            aTex += p.aTex;
            return *this;
        }
        Vertex operator * (float k) const {
            Vertex t;
            t.aPos = k * aPos;
            t.aTex = k * aTex;
            return t;
        }
        Vertex& operator*=(float k) {
            aPos *= k;
            aTex *= k;
            return *this;
        }
    };
    static inline Vertex operator*( float k, Vertex const& p ) {
        return p * k;
    }


    struct VertexShader : public gpc::VertexShader<Vertex, Vertex> {
    public:
        VertexShader(std::shared_ptr<glm::mat4> m) 
            : m_model( m )
        {
        }
        glm::vec4 execute(Vertex const& vertex, Vertex& out) override {
            out.aTex = vertex.aTex;
            out.aPos = vertex.aPos;
            return (*m_model * glm::vec4(vertex.aPos, 1.0f));
        }
    private:
        std::shared_ptr<glm::mat4> m_model;
    };
    class FragmentShader : public gpc::FragmentShader<Vertex> {
    public:
        glm::vec4 execute(gpc::fragment<Vertex> const& primitive) override {
            return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        }

    };
    class widget_container : public iwidget {
    public:
        widget_container( std::shared_ptr<gpc::Device> device,std::shared_ptr<gpc::Framebuffer> fbo) 
            : m_position( 10,10 )
            , m_size( 100 , 100 )
            , m_color(1.0f,1.0f,1.0f,1.0f)
            , m_device( device )
        {
            std::shared_ptr<glm::mat4> model = std::make_shared<glm::mat4>();
            *model = *model * glm::scale(glm::vec3(1 / 400.0f, 1 / 300.0f, 1));
            *model = *model * glm::translate(glm::vec3(-400, -300,0));
            m_buffer = std::make_shared<gpc::VertexBuffer<Vertex>>( );
            m_vertexShader = std::make_shared<VertexShader>( model );
            m_fragmenShader = std::make_shared<FragmentShader>();
            m_pipeline = std::make_shared<gpc::RasterizePipeline<Vertex, Vertex>>( device );
            m_pipeline->bindVertexShader(m_vertexShader);
            m_pipeline->bindFragmentShader(m_fragmenShader);
            m_pipeline->bindFramebuffer(fbo);
        }

        void tick(float passTime, float deltaTime) {
            m_pipeline->setEnableDepthTest(false);
            m_pipeline->setEnableDepthWrite(false);
            _GenVertex();
            m_pipeline->bindVertexBuffer(m_buffer);
            m_pipeline->draw(gpc::PrimitiveType::kTriangle);
        }
    private:
        void _GenVertex() {
            Vertex V[] = {
                { {m_position.x,m_position.x + m_size.x,0},{0,0}},
                { {110,10,0},{0,0}},
                { {110,110,0},{0,0}},

                { {110,110,0},{0,0}},
                { {10,110,0},{0,0}},
                { {10,10,0},{0,0}},
            };
            m_buffer->copyVertex(V, 6);
        }
    private:
        glm::i32vec2 m_position;
        glm::i32vec2 m_size;
        rui::widget_rect m_rect;

        glm::vec4    m_color;
        std::shared_ptr<gpc::RasterizePipeline<Vertex,Vertex>> m_pipeline;
        std::shared_ptr<gpc::VertexBuffer<Vertex>> m_buffer;
        std::shared_ptr<VertexShader> m_vertexShader;
        std::shared_ptr<FragmentShader>m_fragmenShader;
        std::weak_ptr<iwidget> m_parent;
        std::list<std::shared_ptr<iwidget>> m_children;
        std::shared_ptr<iwidget_layout> m_layout;
        std::shared_ptr<gpc::Device>    m_device;
    };

}