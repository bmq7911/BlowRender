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

    void RayTracePipeline::draw(uint32_t x, uint32_t y, glm::fvec4 color) {
        m_fbo->draw_point(x, y, 1.0f, ConvertColor(color));
    }

    gpc::scene* RayTracePipeline::getScene() {
        return m_scene.get();
    }

    glm::fvec4 RayTracePipeline::collectColor(Ray const& ray, Float& t, glm::fvec3& normal) {
        if (greaterRayMaxDistance(t)) {
            return glm::fvec4(Float(0), Float(0), Float(0), Float(0));
        }
        Float length = std::numeric_limits<Float>::max();
        auto obj = m_scene->hit(ray, length, normal);
        if (nullptr != obj) {
            if (m_scene->isLight(obj)) {
                gpc::Light* light = static_cast<gpc::Light*>(obj);
                light->getLightColor( );
                /// 遇到光源了,说明会被照亮,这里我们如何计算这个信息呢,第一种也就是碰撞表
                return glm::fvec4(1.0, 0, 0, 1.0);
                /// 
            }
            else { /// 没遇到光源
                t += length;
                glm::fvec3 rray_dir = glm::reflect(ray.d(), normal);
                Ray rray(ray.at(t), rray_dir);
                return collectColor(rray, t, normal);
            }
        }
        return glm::fvec4(0.0, 0.0, 0.0, 0.0);
    }

    uint32_t RayTracePipeline::doTask(Tid3 const& tid) {
        Ray ray = m_camera->at(tid.x, tid.y);
        Float t = std::numeric_limits<Float>::max();
        glm::fvec3 normal;
        glm::fvec4 color = collectColor(ray, t, normal);
        draw(tid.x, tid.y, color);
        return 0;
    }


    bool RayTracePipeline::greaterRayMaxDistance(Float t) const {
        //return true;
        return false;
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
