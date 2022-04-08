#pragma once
#include "Object.h"
#include "BFloat.h"
namespace gpc {
	struct BVH;
	class Light : public Object {
	public:
		virtual bool hit(Ray const& ray, Float& t, glm::fvec3& normal) const = 0;
		virtual BVH const* getBVH() const = 0;
	private:
		
	};
}