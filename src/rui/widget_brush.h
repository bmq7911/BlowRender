#pragma once
#include <memory>
#include "widget_color.h"
#include "Graphics/FragmentShader.h"
#include "widget_vertex.h"
namespace rui {
	class widget_brush {
	public:
		virtual std::shared_ptr<gpc::FragmentShader<widget_vertex>> get_fragment_shader() const = 0;
	};
	class widget_color_brush : public widget_brush {
	public:
		class widget_fragment_shader : public gpc::FragmentShader<widget_vertex> {
		public:
			widget_fragment_shader(glm::vec4 const& color) 
				: m_color( color )
			{}
			void setColor(glm::vec4 const& color) {
				m_color = color;
			}
			glm::vec4 execute(gpc::fragment<widget_vertex>  const& v) {
				return m_color;
			}
		private:
			glm::vec4 m_color;
		};

		widget_color_brush( widget_color const & color  )
			: m_color( color )
		{
			glm::vec4 tcolor;
			tcolor.r = color.r() / 255.0f;
			tcolor.g = color.g() / 255.0f;
			tcolor.b = color.b() / 255.0f;
			tcolor.a = color.a() / 255.0f;
			m_shader = std::make_shared<widget_fragment_shader>( tcolor );
		}

		void setColor(widget_color const& color) {
			m_color = color;
			glm::vec4 tcolor;
			tcolor.r = color.r() / 255.0f;
			tcolor.g = color.g() / 255.0f;
			tcolor.b = color.b() / 255.0f;
			tcolor.a = color.a() / 255.0f;
			m_shader = std::make_shared<widget_fragment_shader>( tcolor );
		}

		std::shared_ptr<gpc::FragmentShader<widget_vertex>> get_fragment_shader() const {
			return m_shader;
		}
	private:
		widget_color m_color;
		std::shared_ptr<gpc::FragmentShader<widget_vertex>> m_shader;
	};
	class widget_texture_brush : public widget_brush {
		
	};
}