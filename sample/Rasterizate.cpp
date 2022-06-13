#include <iostream>
#include "Rasterizate.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "SceneShader.h"


class JuliaVertexShader : public gpc::VertexShader<glm::vec2, glm::vec4> {
public:
    glm::vec4 execute(glm::vec2 const& vertex, glm::vec4 & out) {
        out.x = vertex.x;
        out.y = vertex.y;
        out.z = 0.0f;
        out.w = 1.0f;
        return out;
    }
};

class JuliaFragmentShader : public gpc::FragmentShader<glm::vec4>{
public:
    JuliaFragmentShader(C<float>* c, C<float>* t)
        : m_c(c)
        , m_t(t)
    {

    }

    glm::vec4 execute(gpc::fragment< glm::vec4>  const& v) override {

        C<float> c( m_c->r() * (v.pos.x + 1.0f) / 2.0f - 1.6f,
                    m_c->i() * (v.pos.y + 1.0f) / 2.0f - 1.2f);
        C<float> t( m_t->r(), m_t->i());

        size_t k = 0;
        for ( k = 0; k < 256; ++k) {
            if ( c.length2() > 4.0f) {
                break;
            }
            c = c * c + t;
        }
        if (k == 256) {
            return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        }
        else {
            return glm::vec4( k / 255.0f ,k / 255.0f, k / 255.0f , 1.0f );
        }

    }
private:
    C<float>* m_c;
    C<float>* m_t;
};


MyBlowWindow::MyBlowWindow(const char* title, uint32_t width, uint32_t height, std::shared_ptr<gpc::Device> device)
    : win::BlowWindow(title, width, height)
    , m_device( device )
    , m_c( 3.2f, 2.4f )
    , m_t( -0.75f, 0.0f )
{
   
    m = std::make_shared<glm::mat4>();
    std::shared_ptr<gpc::Framebuffer> frame = std::make_shared<gpc::Framebuffer>(gpc::Framebuffer(width, height));
    m_ruiRoot = std::make_shared<rui::widget_tree>(device, frame);
    attachFrmaebuffer(frame);
    m_testScene = std::make_shared<TestScene>(device, frame);
}

/// soft render
void MyBlowWindow::tick(float passTime, float deltaTime)  {
    auto fbo = getFbo();
    fbo->clear({ 0,0,0,0 });
    fbo->clearDepth(1.0f);
    const static float speed = 2.0f * 3.1415926f / 50.0f;
    m_testScene->tick(passTime, deltaTime);
    
    //_tickScene( passTime, deltaTime );
    //_tickIMGUI(passTime, deltaTime);
    
}

/// we tick the sence,but how to pass the pipeline
void MyBlowWindow::_tickScene(float passTime, float deltaTime) {
    
}
void MyBlowWindow::_tickIMGUI(float passTime, float deltaTime) {
    static float f = 0.0f;
    static int counter = 0;
    static bool show_demo_window = true;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    float cr = m_c.r();
    float ci = m_c.i();
    float tr = m_t.r();
    float ti = m_t.i();
    ImGui::SliderFloat("c.r", &cr, -4.0f, 4.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat("c.i", &ci, -4.0f, 4.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    ImGui::SliderFloat("t.r", &tr, -4.0f, 4.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderFloat("t.i", &ti, -4.0f, 4.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
   
    m_c = C<float>(cr, ci);
    m_t = C<float>(tr, ti);
    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}




void MyBlowWindow::initScene() {
    
    m_texture = helper::TextureLoad().makeTexture2D("a.jpeg");

    m_camera = std::make_shared<gpc::MoveProjectionCamera>(60.0f, 800, 600, 0.1f, 10.0f, glm::vec3(0.0f, 0.0f, 10.0f));
    m_camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));


    
    initAABB();
    _InitJuliaSence();
}

void MyBlowWindow::initAABB() {

    m_pv = std::make_shared<glm::mat4>();
    m_aabbVertex = std::make_shared<gpc::VertexBuffer<glm::vec3>>();
    m_aabbPipeline = std::make_shared<gpc::RasterizePipeline<glm::vec3, glm::vec3>>( m_device );
    m_aabbPipeline->bindVertexShader(std::make_shared<AABBVertexShader>(m_pv ));
    m_aabbPipeline->bindFragmentShader(std::make_shared<AABBFragmentShader>());
    m_aabbPipeline->bindFramebuffer(getFbo());

    m_scene = std::make_shared<gpc::scene>();

    gpc::Sphere *s1 = new gpc::Sphere(glm::fvec3(-3.0, -3.0, -3.0), 2.0);
    gpc::Sphere *s2 = new gpc::Sphere(glm::fvec3(-3.0, 3.0, -3.0), 2.0);
    gpc::Sphere *s3 = new gpc::Sphere(glm::fvec3(3.0, 3.0, -3.0), 2.0);
    gpc::Sphere *s4 = new gpc::Sphere(glm::fvec3(3.0, -3.0, -3.0), 2.0);

    gpc::Sphere *s5= new gpc::Sphere(glm::fvec3(-3.0, -3.0, 3.0), 2.0);
    gpc::Sphere *s6= new gpc::Sphere(glm::fvec3(-3.0, 3.0, 3.0), 2.0);
    gpc::Sphere *s7= new gpc::Sphere(glm::fvec3(3.0, 3.0, 3.0), 2.0);
    gpc::Sphere *s8= new gpc::Sphere(glm::fvec3(3.0, -3.0, 3.0), 2.0);
    gpc::Sphere *s9= new gpc::Sphere(glm::fvec3(3.0, -3.0, 3.0), 2.0);

    m_scene->addObject(s1);
    m_scene->addObject(s2);
    m_scene->addObject(s3);
    m_scene->addObject(s4);
    m_scene->addObject(s5);
    m_scene->addObject(s6);
    m_scene->addObject(s7);
    m_scene->addObject(s8);
    m_scene->addObject(s9);
    m_scene->buildScene();
}

void MyBlowWindow::drawOTree(gpc::OTree<gpc::Object> const* tree) {
    if (nullptr != tree) {
        gpc::AABB const& aabb = tree->getAABB();
        drawAABB(aabb);
        for (size_t i = 0; i < 8; ++i) {
            auto child =  tree->atChild(i);
            if (nullptr != child)
                drawOTree(child);
        }
    }
}
/// 这个东西的难点一定是在场景编辑里面
void MyBlowWindow::drawAABB( gpc::AABB const& aabb) {
    glm::vec3 vertex[8];
    glm::vec3 c = 0.5f * (aabb.max() + aabb.min());
    glm::vec3 r = 0.5f * (aabb.max() - aabb.min());
    
    vertex[0] = c + glm::vec3( -1.0f,-1.0f,-1.0f ) * r;
    vertex[1] = c + glm::vec3( 1.0f, -1.0f,-1.0f ) * r;
    vertex[2] = c + glm::vec3( 1.0f, 1.0f, -1.0f ) * r;
    vertex[3] = c + glm::vec3( -1.0f,1.0f, -1.0f ) * r;
    vertex[4] = c + glm::vec3( -1.0f,-1.0f,1.0f ) * r;
    vertex[5] = c + glm::vec3( 1.0f, -1.0f,1.0f ) * r;
    vertex[6] = c + glm::vec3( 1.0f, 1.0f, 1.0f ) * r;
    vertex[7] = c + glm::vec3( -1.0f,1.0f, 1.0f ) * r;
    
    uint8_t index[24] = {  0,1,  1,2,  2,3,  3,0, 0,4, 1,5, 2,6, 3,7, 4,5 ,5,6, 6,7, 7,4  };

    m_aabbVertex->copyVertex( vertex,8);
    m_aabbVertex->copyIndex( index, 24);
    m_aabbPipeline->bindVertexBuffer( m_aabbVertex );
    
}

void MyBlowWindow::_InitJuliaSence() {

    glm::fvec2 vertex[] = {
        {-1.0f, -1.0f },
        {1.0f,  -1.0f },
        {1.0f,   1.0f},

        {1.0f,   1.0f },
        {-1.0f,  1.0f},
        {-1.0f, -1.0f},

    };

    m_vertexJulia = std::make_shared<gpc::VertexBuffer<glm::fvec2 >>();
    m_vertexJulia->copyVertex(vertex, sizeof(vertex) / sizeof(vertex[0]));


    m_juliaPipeline = std::make_shared<gpc::RasterizePipeline<glm::fvec2,glm::fvec4>>( m_device );

    m_juliaPipeline->bindVertexBuffer( m_vertexJulia );
    m_juliaPipeline->bindFramebuffer(getFbo());
    m_juliaPipeline->bindVertexShader( std::make_shared<JuliaVertexShader>());
    m_juliaPipeline->bindFragmentShader(std::make_shared<JuliaFragmentShader>( &m_c,&m_t));

}

void MyBlowWindow::_InitIMGUI() {
    ImGui::StyleColorsDark();

}

void MyBlowWindow::processInput(float passTime, float deltaTime)  {
    if (isKeyDown(GLFW_KEY_W))
        m_camera->moveFront(deltaTime);
    if (isKeyDown(GLFW_KEY_S))
        m_camera->moveBack(deltaTime);
    if (isKeyDown(GLFW_KEY_A))
        m_camera->moveLeft(deltaTime);
    if (isKeyDown(GLFW_KEY_D))
        m_camera->moveRight(deltaTime);
}


