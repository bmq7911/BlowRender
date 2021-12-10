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
        uint32_t doTask(Tid3 const& tid) override {
            Ray<double> ray = m_camera->at(tid.x, tid.y);
            glm::vec4 color = m_scene->CollectColor(ray);
            m_fbo->draw_point(tid.y, tid.x, 1.0f, ConvertColor(color));
            return 0;
        }
        static glm::vec4i8 ConvertColor(glm::vec4 const& src);
    public:
        Dim3 getTaskDim() const override {
            Dim3 size;
            size.x = m_width;
            size.y = m_height;
            size.z = 1;
            return size;
        }
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
