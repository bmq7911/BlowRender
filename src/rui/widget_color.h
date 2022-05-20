#pragma once
#include <glm/glm.hpp>
namespace rui {
	class widget_color {
	public:
		widget_color(uint8_t rgba) 
			: m_color( rgba, rgba, rgba, rgba)
		{}
		widget_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) 
			: m_color( r,g,b,a)
		{}
		widget_color(uint8_t rgb, uint8_t a) 
			: m_color( rgb, rgb,rgb,a)
		{
		
		}
		uint8_t r() const {
			return m_color.r;
		}
		uint8_t g() const {
			return m_color.g;
		}
		uint8_t b() const {
			return m_color.b;
		}
		uint8_t a() const {
			return m_color.a;
		}
	private:
		glm::tvec4<uint8_t> m_color;
	};
}