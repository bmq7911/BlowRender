#pragma once
#include "Object.h"
#include "BFloat.h"
namespace gpc {
	struct BVH;
	class Light : public Object {
	public:
		virtual bool hit(Ray const& ray, Float& t, glm::fvec3& normal) const = 0;
		virtual BVH const* getBVH() const = 0;
		glm::fvec4 getLightColor() const {
			return m_color;
		}
		void setLightColor(glm::fvec4 const& __color) {
			m_color = __color;
		}
	private:
		glm::fvec4 m_color;
	};
}