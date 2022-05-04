#pragma once
#include <memory>
#include "Graphics/Framebuffer.h"
#include "RayCamera.h"
#include "scene.h"
#include "Graphics/Sphere.h"
#include "Device/Device.h"

namespace gpc {
    /// ����һ���ѵ��ǳ����ı༭,���ǲ������������Ĺ���


    class RayTracePipeline : public gpc::ITaskIssue {
    public:
        RayTracePipeline(uint32_t width, uint32_t height, std::shared_ptr<gpc::Framebuffer> fbo);
        void bindScene(std::shared_ptr<gpc::scene>);
        void draw();
        void draw(uint32_t x, uint32_t, glm::vec4 color);
        gpc::scene * getScene();
        
        void init(uint32_t size) override {
        }
        uint32_t doTask(Tid3 const& tid) override;
        
        Dim3 getTaskDim() const override;


        static glm::vec4i8 ConvertColor(glm::vec4 const& src);
    public:
        uint32_t m_width;
        uint32_t m_height;
        Sphere m_sphere;
        Sphere m_sphere1;
        glm::fvec3 m_rayDir;
        std::shared_ptr<gpc::scene> m_scene;
        std::shared_ptr<gpc::Framebuffer> m_fbo;
        std::shared_ptr<RayCamera> m_camera;
    private:
    };

}