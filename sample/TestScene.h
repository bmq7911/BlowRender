#pragma once
#include <memory>
#include "scene.h"
#include "Graphics/ProjectionCamera.h"
#include "helper/ModelLoad.h"
#include "SceneShader.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"


class TestScene {
public:
	TestScene( std::shared_ptr<gpc::Device> device,std::shared_ptr<gpc::Framebuffer> fbo)
		: m_device( device )
		, m_fbo( fbo )
	{
		m_camera = std::make_shared<gpc::MoveProjectionCamera>(60.0f, 800, 600, 0.1f, 100.0f);
		m_camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

		m_view = m_camera->to_view();
		m_proj = m_camera->to_proj();
		m_mvp = m_proj * m_view * m_model;

		m_scene = std::make_shared<gpc::scene>();
		auto model = helper::Model::parseModel("cube.obj");
		auto cube1 = Model::createModel(model);
		auto cube2 = Model::createModel(model);
	
		m_scene->addObject(cube1);
		m_scene->addObject(cube2);
		m_lightPos = glm::fvec3(10.0f, 10.0f, 10.0f);
		m_lightColor = glm::fvec3(1.0f, 0.0f, 0.0f);

		m_basicVertexShader = std::make_shared<BasicVertexShader>(&m_model, &m_modelT, &m_view, &m_proj, &m_mvp);
		m_pointLightFragmentShader = std::make_shared<PointLightFragmentShader>( &m_lightPos, &m_lightColor );
		m_pipeline = std::make_shared<gpc::RasterizePipeline<helper::Vertex, helper::Vertex>>( device );

		m_pipeline->bindVertexShader(m_basicVertexShader);
		m_pipeline->bindFragmentShader(m_pointLightFragmentShader);
		m_pipeline->bindFramebuffer(fbo);
	}
	
	/// <summary>
	/// 光纤追踪实际比这个好写些
	void tick(float passTime, float deltaTime) {
		_tickIMGUI(passTime, deltaTime);

		auto beg = m_scene->begin();
		auto end = m_scene->end();
		for (; beg != end; ++beg) {
			Model* model = static_cast<Model*>(*beg);
			if (nullptr != model) {
				model->tick(passTime, deltaTime);
				m_pipeline->bindVertexBuffer(model->getModel()->getVertexBuffer());
				m_pipeline->draw(gpc::PrimitiveType::kTriangle);
			}
		//	(*beg)->
		}

	}
	
private:
	void _tickIMGUI( float passTime, float deltaTime ) {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Scene info");                          // Create a window called "Hello, world!" and append into it.
		
		glm::fvec3 cameraPos = m_camera->getPosition();
		glm::fvec3 cameraLookDir = m_camera->getLookAt();
		ImGui::InputFloat3("camera pos",(float*)(&cameraPos));
		ImGui::InputFloat3("look at",(float*)(&cameraLookDir));
		m_camera->setPositon(cameraPos);
		m_camera->setLookAt( cameraLookDir);
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();


		m_view = m_camera->to_view();
		m_proj = m_camera->to_proj();
		m_mvp  = m_proj * m_view * m_model;


	}
private:
	std::shared_ptr<gpc::Device>                                           m_device;
	std::shared_ptr<gpc::Framebuffer>                                      m_fbo;

	std::shared_ptr<BasicVertexShader>                                     m_basicVertexShader;
	std::shared_ptr<PointLightFragmentShader>                              m_pointLightFragmentShader;
	std::shared_ptr<gpc::RasterizePipeline<helper::Vertex,helper::Vertex>> m_pipeline;

	std::shared_ptr<gpc::MoveProjectionCamera>                             m_camera;
	std::shared_ptr<gpc::scene>                                            m_scene;

	glm::mat4 m_model;
	glm::mat4 m_view;
	glm::mat4 m_proj;
	glm::mat4 m_mvp;

	glm::mat3 m_modelT;
	glm::vec3 m_lightPos;
	glm::vec3 m_lightColor;
};