#pragma once
#include <memory>
#include "Graphics/Framebuffer.h"
#include "RayCamera.h"
#include "scene.h"
#include "Graphics/Sphere.h"
#include "Device/Device.h"

namespace gpc {
    /// 另外一个难点是场景的编辑,我们不好做到场景的管理


    class RayTracePipeline : public gpc::ITaskIssue {
    public:
        RayTracePipeline(uint32_t width, uint32_t height, std::shared_ptr<gpc::Framebuffer> fbo);
        void bindScene(std::shared_ptr<gpc::scene>);
        void draw(uint32_t x, uint32_t, glm::fvec4 color);
        gpc::scene * getScene();
        void init(uint32_t size) override {
        }
        glm::fvec4 collectColor(Ray const& ray, Float& t, glm::fvec3& normal);
        uint32_t   doTask(Tid3 const& tid) override;
        Dim3   getTaskDim() const override;
        static glm::vec4i8 ConvertColor(glm::fvec4 const& src);
        bool   greaterRayMaxDistance( Float t) const;
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
