#include "ProjectionCamera.h"
namespace gpc {
	ProjectionCamera::ProjectionCamera(Float Zoom, uint32_t width, uint32_t height, Float Near, Float Far, glm::fvec3 position, glm::fvec3 lookAt, glm::fvec3 up) {
		// View 矩阵信息
		m_pos = position;
		m_front = glm::normalize(lookAt - position);
		m_right = glm::normalize(glm::cross(up, m_front));
		m_up = glm::normalize(glm::cross(m_front, m_right));
		this->up = m_up;
		/// project信息

		m_near = Near;
		m_far = Far;
		m_yangle = glm::radians(Zoom);
		Float tx = std::tan(m_yangle / 2.0) * width / height;
		m_xangle = atanf(tx) * 2.0;
	}

	glm::fmat4 ProjectionCamera::to_mat() const {
		return to_proj() * to_view();
	}

	glm::fmat4 ProjectionCamera::to_proj() const {
		return glm::perspective(m_yangle, std::tan(Float(m_xangle / 2.0f)) / std::tan(Float( m_yangle / 2.0f)), m_near, m_far);
	}

	glm::fmat4 ProjectionCamera::to_view() const {
		return glm::lookAt(m_pos, m_pos + m_front, m_up);
	}

	void ProjectionCamera::setLookAt(glm::fvec3 const& at) {
		m_front = glm::normalize(at - m_pos);
		m_right = glm::normalize(glm::cross(up, m_front));
		m_up = glm::normalize(glm::cross(m_front, m_right));
	}
	void ProjectionCamera::setLookDir(glm::fvec3 const& v) {
		m_front = glm::normalize(v);
		m_right = glm::normalize(glm::cross(up, m_front));
		m_up = glm::normalize(glm::cross(m_front, m_right));
	}
}

