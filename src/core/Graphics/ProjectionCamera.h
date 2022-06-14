#pragma once
#include <stdint.h>
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Camera.h"

namespace gpc {
	class ProjectionCamera : public Camera {
	public:
		ProjectionCamera(Float Zoom, uint32_t width, uint32_t height, Float Near, Float Far, glm::fvec3 position = glm::fvec3(0.0f, 0.0f, 0.0f), glm::fvec3 fornt = glm::fvec3(0.0f, 0.0f, -1.0f), glm::fvec3 up = glm::fvec3(0.0f, 1.0f, 0.0f));
		glm::fmat4 to_mat() const override;
		glm::fmat4 to_proj() const override;
		glm::fmat4 to_view() const override;
		void setLookAt(glm::fvec3 const& vir);
		glm::fvec3 getLookAt() const;
		void setLookDir(glm::fvec3 const& dir);
		glm::fvec3 getLookDir() const;
		glm::fvec3 getXV() const;
		glm::fvec3 getYV() const;
		//G::Ray   ClickRay(uint32_t x, uint32_t y ) const;
	protected:
		glm::fvec3 m_pos;
		glm::fvec3 m_front;
		glm::fvec3 m_up;
		glm::fvec3 m_right;
		glm::fvec3 up;
		glm::fvec3 m_at;
		Float m_xangle;
		Float m_yangle;
		Float m_near;
		Float m_far;

	};

	class MoveProjectionCamera : public ProjectionCamera {
	public:
		MoveProjectionCamera(Float Zoom, uint32_t width, uint32_t height, Float Near, Float Far, glm::fvec3 position = glm::fvec3(0.0f, 0.0f, 10.0f), glm::fvec3 lookAt = glm::fvec3(0.0f, 0.0f, 0.0f), glm::fvec3 up = glm::fvec3(0.0f, 1.0f, 0.0f))
			: ProjectionCamera(Zoom, width, height, Near, Far, position, lookAt, up)
			, m_speed(1.0f)
		{

		}
		glm::fvec3 getPosition() const {
			return m_pos;
		}
		void setPositon(glm::fvec3 const& pos) {
			m_pos = pos;
		}
		void moveFront(Float deltaTime) {
			m_pos += deltaTime * m_speed * m_front;
		}
		void moveBack(Float deltaTime) {
			m_pos -= deltaTime * m_speed * m_front;

		}
		void moveLeft(Float deltaTime) {
			m_pos -= deltaTime * m_speed * m_right;
		}
		void moveRight(Float deltaTime) {
			m_pos += deltaTime * m_speed * m_right;
		}
	private:
		Float m_speed;
	};
}
	

