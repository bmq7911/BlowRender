#pragma once
#include <memory>
#include "graphics/VertexBuffer.h"
#include "graphics/Framebuffer.h"
#include "RayCamera.h"
#include "scene.h"
#include "Graphics/Sphere.h"
#include "Device/Device.h"

namespace gpc {
    /// 另外一个难点是场景的编辑,我们不好做到场景的管理


    class RayTracePipeline : public gpc::ITaskIssue {
    public:
        RayTracePipeline(uint32_t width, uint32_t height, std::shared_ptr<gpc::Framebuffer> fbo);
        void bindScene(std::shared_ptr<gpc::scene<double>>);
        void draw();
        void draw(uint32_t x, uint32_t, glm::vec4 color);
        gpc::scene<double>* getScene();
        
        void init(uint32_t size) override {
        }
        uint32_t doTask(Tid3 const& tid) override;
        
        Dim3 getTaskDim() const override;


        static glm::vec4i8 ConvertColor(glm::vec4 const& src);
    public:
        uint32_t m_width;
        uint32_t m_height;
        Sphere<double> m_sphere;
        Sphere<double> m_sphere1;
        glm::tvec3<double> m_rayDir;
        std::shared_ptr<gpc::scene<double>> m_scene;
        std::shared_ptr<gpc::Framebuffer> m_fbo;
        std::shared_ptr<RayCamera> m_camera;
    private:
    };

}
