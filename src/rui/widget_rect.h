#pragma once
#include <stdint.h>
#include <glm/glm.hpp>
namespace rui {
	class widget_rect {
	public:
		widget_rect() 
			: m_postion( 0, 0 )
			, m_size( 0, 0 )
		{}

		widget_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
			: m_postion( x, y)
			, m_size( width, height)
		{}
		widget_rect(widget_rect const& rect) {
			m_postion = rect.m_postion;
			m_size = rect.m_size;
		}
		widget_rect& operator=(widget_rect const& rect) {
			if (this != &rect) {
				m_postion = rect.m_postion;
				m_size = rect.m_size;
			}
			return *this;
		}

		uint32_t x() const {
			return m_postion.x;
		}

		uint32_t y() const {
			return m_postion.y;
		}
		uint32_t width() const {
			return m_size.x;
		}

		uint32_t height() const {
			return m_size.y;
		}
		void setPosition(uint32_t x, uint32_t y) {
			m_postion.x = x;
			m_postion.y = y;
		}
		void setSize(uint32_t width, uint32_t height) {
			m_size.x = width;
			m_size.y = height;
		}
	private:
		glm::tvec2<uint32_t> m_postion;
		glm::tvec2<uint32_t> m_size;
	};
}