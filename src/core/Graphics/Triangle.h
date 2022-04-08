#pragma once
#include "glm/glm.hpp"
#include "Graphics/Object.h"

namespace gpc {
	class Triangle : public Object {
	public:
		bool hit(Ray const& ray, Float& t, glm::fvec3 & normal) const {
			
		}
		glm::fvec3 normal() const {
			glm::fvec3 v_ab = m_b - m_a;
			glm::fvec3 v_ac = m_c - m_a;
			return glm::cross(v_ab, v_ac);
		}
	private:
		glm::fvec3 m_a;
		glm::fvec3 m_b;
		glm::fvec3 m_c;
	};
}