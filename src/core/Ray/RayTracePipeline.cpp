#include "RayTracePipeline.h"

namespace gpc {

    RayTracePipeline::RayTracePipeline(uint32_t width, uint32_t height, std::shared_ptr<gpc::Framebuffer> fbo)
        : m_width(width)
        , m_height(height)
        , m_sphere(glm::fvec3(0.0, 0.0, -15.0), 3.0)
        , m_sphere1(glm::fvec3(2.0, -2.0, -9.0), 3.0)
        , m_fbo(fbo)
    {
        m_scene = std::make_shared<gpc::scene>();
        m_scene->setLightDir(glm::fvec3(0.0, 0.0, -1.0));
        m_rayDir = glm::normalize(glm::fvec3(0.0, 0.7, -1.0));

        m_camera = std::make_shared<RayCamera>(width, height, 60.0);

        m_scene->addObject(&m_sphere);
        m_scene->addObject(&m_sphere1);
        m_scene->buildScene();
    }

    void RayTracePipeline::bindScene(std::shared_ptr<gpc::scene> scene) {
        m_scene = scene;
    }

    void RayTracePipeline::draw() {
        ///
        for (uint32_t j = 0; j < m_camera->YCount(); ++j) {
            for (uint32_t i = 0; i < m_camera->XCount(); ++i) {
                Ray ray = m_camera->at(i, j);
                Float t = std::numeric_limits<Float>::max();
                glm::fvec3 normal;
                auto obj = m_scene->hit(ray,t, normal );
                if (nullptr != obj) { /// 有可能是光源
                    
                }
                glm::vec4 color = m_scene->CollectColor(ray);
                m_fbo->draw_point(i, j, 1.0f, ConvertColor(color));
            }
        }
    }

    void RayTracePipeline::draw(uint32_t x, uint32_t y, glm::vec4 color) {
        m_fbo->draw_point(x, y, 1.0f, ConvertColor(color));
    }

    gpc::scene* RayTracePipeline::getScene() {
        return m_scene.get();
    }
    uint32_t RayTracePipeline::doTask(Tid3 const& tid) {
        Ray ray = m_camera->at(tid.x, tid.y);
        glm::vec4 color = m_scene->CollectColor(ray);
        m_fbo->draw_point(tid.y, tid.x, 1.0f, ConvertColor(color));
        return 0;
    }
    Dim3 RayTracePipeline::getTaskDim() const{
        Dim3 size;
        size.x = m_width;
        size.y = m_height;
        size.z = 1;
        return size;
    }
    glm::vec4i8 RayTracePipeline::ConvertColor(glm::vec4 const& src) {
        glm::vec4i8 color;
        color.x = int8_t(src.x * 255.0f + 0.5f);
        color.y = int8_t(src.y * 255.0f + 0.5f);
        color.z = int8_t(src.z * 255.0f + 0.5f);
        color.w = int8_t(src.w * 255.0f + 0.5f);
        return color;
    }
}
