#pragma once
#include "glm/glm.hpp"
#include "Graphics/Object.h"

namespace gpc {
	template<typename T>
	class Triangle : public Object<T> {
	public:
		bool hit(Ray<T> const& ray, T& t, glm::tvec3<T>& normal) const {
			
		}
		glm::tvec3<T> normal() const {
			glm::tvec3<T> v_ab = m_b - m_a;
			glm::tvec3<T> v_ac = m_c - m_a;
			return glm::cross(v_ab, v_ac);
		}
	private:
		glm::tvec3<T> m_a;
		glm::tvec3<T> m_b;
		glm::tvec3<T> m_c;
	};
}