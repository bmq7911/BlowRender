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
		ProjectionCamera(float Zoom, uint32_t width, uint32_t height, float Near, float Far, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 fornt = glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 to_mat() const override;
		glm::mat4 to_proj() const override;
		glm::mat4 to_view() const override;
		void setLookAt(glm::vec3 const& vir);
		void setLookDir(glm::vec3 const& dir);
		glm::vec3 getLookDir() const;
		glm::vec3 getXV() const;
		glm::vec3 getYV() const;
		//G::Ray   ClickRay(uint32_t x, uint32_t y ) const;
	protected:
		glm::vec3 m_pos;
		glm::vec3 m_front;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 up;
		float m_xangle;
		float m_yangle;
		float m_near;
		float m_far;

	};

	class MoveProjectionCamera : public ProjectionCamera {
	public:
		MoveProjectionCamera(float Zoom, uint32_t width, uint32_t height, float Near, float Far, glm::vec3 position = glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
			: ProjectionCamera(Zoom, width, height, Near, Far, position, lookAt, up)
			, m_speed(1.0f)
		{

		}
		glm::vec3 getPosition() const {
			return m_pos;
		}
		void moveFront(float deltaTime) {
			m_pos += deltaTime * m_speed * m_front;
		}
		void moveBack(float deltaTime) {
			m_pos -= deltaTime * m_speed * m_front;

		}
		void moveLeft(float deltaTime) {
			m_pos -= deltaTime * m_speed * m_right;
		}
		void moveRight(float deltaTime) {
			m_pos += deltaTime * m_speed * m_right;
		}
	private:
		float m_speed;
	};
}
	

