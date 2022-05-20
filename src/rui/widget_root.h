#pragma once
#include <memory>
#include <set>

#include "widget.h"
#include "widget_background.h"
#include "widget_vertex.h"
#include "Rasterize/RasterizePipeline.h"
#include "Graphics/VertexShader.h"

namespace rui {

	class widget_vertex_shader : public gpc::VertexShader<widget_vertex, widget_vertex> {
	public:
		widget_vertex_shader(std::shared_ptr<glm::mat4> m)
			: m_model(m)
		{
		}
		void bind_model_matrix(std::shared_ptr<glm::mat4> m) {
			m_model = m;
		}
		glm::vec4 execute(widget_vertex const& vertex, widget_vertex& out) override {
			out.aTex = vertex.aTex;
			out.aPos = vertex.aPos;
			return (*m_model * glm::vec4(vertex.aPos, 0.0f, 1.0f));
		}
	private:
		std::shared_ptr<glm::mat4> m_model;
	};

	class widget_tree {
	public:
		widget_tree(std::shared_ptr<gpc::Device> device, std::shared_ptr<gpc::Framebuffer>fbo ) 
			: m_device( device )
			, m_fbo( fbo)
		{
			m_pipeline = std::make_shared<gpc::RasterizePipeline<widget_vertex, widget_vertex>>( m_device);
			m_pipeline->bindFramebuffer(m_fbo);
			m_modelMatrix = std::make_shared<glm::mat4>();

			uint32_t width = m_fbo->getWidth();
			uint32_t height = m_fbo->getHeight();
			(*m_modelMatrix) *= glm::scale(glm::vec3(2.0f / width, 2.0f / height ,1.0f));
			(*m_modelMatrix) *= glm::translate(glm::vec3(-(width * 0.5f), -(height * 0.5f), 0.0f));
			m_vertexShader = std::make_shared<widget_vertex_shader>( m_modelMatrix);

			m_pipeline->bindVertexShader(m_vertexShader);
			m_vertexBuffer = std::make_shared<gpc::VertexBuffer<widget_vertex>>();
			widget_rect rect(0, 0, width, height);
			m_root = std::make_shared<widget_background>( rect);
		}

		std::shared_ptr<widget_background> get_widget_background() {
			return m_root;
		}

		void tick(float passTime, float deltaTime) {
			m_root->tick(passTime, deltaTime);
			auto data = m_root->gen_render_data( );
			if (nullptr != data) {
				if (data->m_fragmentShader) {
					m_pipeline->bindFragmentShader(data->m_fragmentShader);
				}
				else {
					return;
				}
				if (data->m_vertexBuffer) {
					m_pipeline->bindVertexBuffer(data->m_vertexBuffer);
				}
				else {
					return;
				}
				m_pipeline->draw(gpc::PrimitiveType::kTriangle);
			}
		}

	private:
		std::shared_ptr<gpc::Device>                                          m_device;
		std::shared_ptr<gpc::Framebuffer>                                     m_fbo;
		std::shared_ptr<gpc::RasterizePipeline<widget_vertex, widget_vertex>> m_pipeline;
		std::shared_ptr<gpc::VertexBuffer<widget_vertex>>                     m_vertexBuffer;
		std::shared_ptr<glm::mat4>                                            m_modelMatrix;
		std::shared_ptr<widget_vertex_shader>                                 m_vertexShader;
		std::shared_ptr<widget_background>                                    m_root;
		std::list<std::vector<std::shared_ptr<iwidget>>>                      z_sort;
	};
}