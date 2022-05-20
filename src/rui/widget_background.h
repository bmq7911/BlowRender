#pragma once
#include "widget_container.h"
#include "widget_brush.h"
namespace rui {
	class widget_background : public widget_container {
	public:
		widget_background( widget_rect const &rect) 
			: widget_container(  rect )
		{
			widget_color color(255,123,43, 255);
			m_brush = std::make_shared<widget_color_brush>( color );
			m_vertex = std::make_shared<gpc::VertexBuffer<widget_vertex>>( );
			m_data.m_fragmentShader = m_brush->get_fragment_shader();
			m_data.m_vertexBuffer = m_vertex;
		}
		void tick(float passTime, float deltaTime) override {
			_Gen_Render_Data();
		}
		widget_render_data* gen_render_data() override{
			return &m_data;
		}
	private:
		void _Gen_Render_Data() {
			rui::widget_rect const& rect = get_rect();
			widget_vertex V[] = {
				{ { 0,0},{0,0}},
				{ { rect.width() - 1,0},{1.0f,0}},
				{ { rect.width() - 1, rect.height() - 1},{1.0f,1.0f}},
				
				{ { rect.width() - 1, rect.height() - 1},{1.0f,1.0f}},
				{ { 0, rect.height() - 1},{0,1.0}},
				{ { 0,0},{0,0}},
			};
			m_vertex->copyVertex( V, 6 );
		}
	private:
		std::shared_ptr<widget_brush> m_brush;
		std::shared_ptr<gpc::VertexBuffer<widget_vertex>> m_vertex;
		widget_render_data            m_data;
	};
}