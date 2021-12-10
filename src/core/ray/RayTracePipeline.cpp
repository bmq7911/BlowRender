#include "RayTracePipeline.h"

namespace gpc {

    RayTracePipeline::RayTracePipeline(uint32_t width, uint32_t height, std::shared_ptr<gpc::Framebuffer> fbo)
        : m_width(width)
        , m_height(height)
        , m_sphere(glm::tvec3<double>(0.0, 0.0, -15.0), 3.0)
        , m_sphere1(glm::tvec3<double>(2.0, -2.0, -9.0), 3.0)
        , m_fbo(fbo)
    {
        m_scene = std::make_shared<gpc::scene<double>>();
        m_scene->setLightDir(glm::tvec3<double>(0.0, 0.0, -1.0));
        m_rayDir = glm::normalize(glm::tvec3<double>(0.0, 0.7, -1.0));

        m_camera = std::make_shared<RayCamera>(width, height, 60.0);

        /*
        Sphere<double> s1(glm::tvec3<double>( -3.0,-3.0,-3.0), 2.0);
        Sphere<double> s2(glm::tvec3<double>( -3.0, 3.0,-3.0), 2.0);
        Sphere<double> s3(glm::tvec3<double>( 3.0,  3.0,-3.0), 2.0);
        Sphere<double> s4(glm::tvec3<double>( 3.0, -3.0,-3.0), 2.0);

        Sphere<double> s5(glm::tvec3<double>( -3.0,-3.0,3.0), 2.0);
        Sphere<double> s6(glm::tvec3<double>( -3.0, 3.0,3.0), 2.0);
        Sphere<double> s7(glm::tvec3<double>( 3.0,  3.0,3.0), 2.0);
        Sphere<double> s8(glm::tvec3<double>( 3.0, -3.0,3.0), 2.0);
        Sphere<double> s9(glm::tvec3<double>( 3.0, -3.0,3.0), 2.0);

        m_scene->addObject(&s1);
        m_scene->addObject(&s2);
        m_scene->addObject(&s3);
        m_scene->addObject(&s4);
        m_scene->addObject(&s5);
        m_scene->addObject(&s6);
        m_scene->addObject(&s7);
        m_scene->addObject(&s8);
        m_scene->addObject(&s9);
        */

        m_scene->addObject(&m_sphere);
        m_scene->addObject(&m_sphere1);
        m_scene->buildScene();
    }

    void RayTracePipeline::bindScene(std::shared_ptr<gpc::scene<double>> scene) {
        m_scene = scene;
    }

    void RayTracePipeline::draw() {
        ///
        for (uint32_t j = 0; j < m_camera->YCount(); ++j) {
            for (uint32_t i = 0; i < m_camera->XCount(); ++i) {
                Ray<double> ray = m_camera->at(i, j);
                glm::vec4 color = m_scene->CollectColor(ray);
                m_fbo->draw_point(i, j, 1.0f, ConvertColor(color));
            }
        }
    }

    void RayTracePipeline::draw(uint32_t x, uint32_t y, glm::vec4 color) {
        m_fbo->draw_point(x, y, 1.0f, ConvertColor(color));
    }

    gpc::scene<double>* RayTracePipeline::getScene() {
        return m_scene.get();
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
