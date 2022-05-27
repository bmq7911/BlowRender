#pragma once
#include "widget_container.h"
#include "widget_shape.h"
#include "widget_brush.h"

namespace rui {
	class widget_background : public widget_container {
	public:
		widget_background( widget_rect const &rect) 
			: widget_container( )
		{
			widget_color color(255,123,43, 255);
			m_brush = std::make_shared<widget_color_brush>( color );
			m_data.m_fragmentShader = m_brush->get_fragment_shader();
			m_shape = std::make_shared<widget_shape_rect>( rect );
		}
		void tick(float passTime, float deltaTime) override {
			_Gen_Render_Data();
		}
		widget_render_data* gen_render_data() override{
			return &m_data;
		}
	private:
		void _Gen_Render_Data( ) {
		}
	private:
		std::shared_ptr<widget_brush> m_brush;
		std::shared_ptr<iwidget_shape> m_shape;
		widget_render_data            m_data;
	};
}