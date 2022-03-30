#pragma once
#include "Object.h"
namespace gpc {
	template<typename T>
	struct BVH;
	template<typename T>
	class Light : public Object<T> {
	public:
		virtual bool hit(Ray<T> const& ray, T& t, glm::tvec3<T>& normal) const = 0;
		virtual BVH<T> const* getBVH() const = 0;
	private:
		
	};
}