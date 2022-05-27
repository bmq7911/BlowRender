#pragma once
#include <stdint.h>
#include "Graphics/AABB2d.h"
#include "widget_rect.h"
#include "widget_vertex.h"
#include "Graphics/Ray.h"
#include "Graphics/Line.h"

namespace rui{

    class widget_vertex_stream{
    public:
        gpc::AABB2d<uint32_t> to_aabb() const {
            return m_aabb;
        }

        static widget_vertex_stream widget_vertex_stream_intersection( widget_vertex_stream const& vs1, widget_vertex_stream const& vs2){
            gpc::AABB2d<uint32_t> aabb1 = vs1.to_aabb();
            gpc::AABB2d<uint32_t> aabb2 = vs2.to_aabb();
            if( false == gpc::AABB2d<uint32_t>::isIntersect(aabb1,aabb2)){
                

            }
            else{
                

            }
        }
        widget_vertex_stream widget_vertex_stream_union( ){

        }
    private:
        gpc::AABB2d<uint32_t> m_aabb;
        std::vector<widget_vertex> m_vertexs;
    };
 

    template<typename V>
    class SutherlandHodgmanForArbitraryShape{
    public:
        
    private:
        template<typename T>
        static uint32_t _IsIntersect( gpc::Ray2d<T> const& ray, gpc::Line2d<T> const& line){
            glm::tvec2<T> ao = glm::vec2<T>( ray.o().x - line.a(), ray.o().y - line.b());
            glm::tvec2<T> normal = line.normal();
            float t = glm::dot( ao, normal) / glm::dot( ray.d(), normal );
        }
    private:
        widget_vertex_stream m_vs1;
        widget_vertex_stream m_vs2;        
    };
    

    class iwidget_shape{
        virtual gpc::AABB2d<uint32_t> to_aabb( ) const = 0;
        //virtual widget_vertex_stream to_vertex_stream() const = 0;

    };

    class widget_shape_rect : public iwidget_shape{
    public:
        widget_shape_rect(widget_rect const& rect) 
            : m_rect( rect )
            , m_aabb( 0,0,0,0 )
        {
        }

        gpc::AABB2d<uint32_t> to_aabb() const override{
            return m_aabb;
        }
        //widget_vertex_stream to_vertex_stream() const {
        //    return widget_vertex_stream();
        //}
    private:
        widget_rect m_rect;
        gpc::AABB2d<uint32_t> m_aabb;
    };

    class widget_shape_sphere : public iwidget_shape{
    public:

        gpc::AABB2d<uint32_t> to_aabb() const override{
            
        }

        //widget_vertex_stream to_vertex_stream() const override{
        //    
        //}
    private:
        glm::tvec2<uint32_t> m_o;
        uint32_t             m_r;
    };

    class widget_shape_region : public iwidget_shape {
    public:
        gpc::AABB2d<uint32_t> to_aabb() const override {

        }
        //widget_vertex_stream to_vertex_stream() const override {
        //
        //}
    };
}