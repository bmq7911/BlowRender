#include <iostream>
#include "Rasterizate.h"



MyBlowWindow::MyBlowWindow(const char* title, uint32_t width, uint32_t height, std::shared_ptr<gpc::Device> device)
    : win::BlowWindow(title, width, height)
    , m_device( device )
{
    m = std::make_shared<glm::mat4>();
    std::shared_ptr<gpc::Framebuffer> frame = std::make_shared<gpc::Framebuffer>(gpc::Framebuffer(width, height));
    attachFrmaebuffer(frame);
}

/// soft render
void MyBlowWindow::render(float passTime, float deltaTime)  {
    auto fbo = getFbo();
    fbo->clear({ 0,0,0,0 });
    fbo->clearDepth(1.0f);
    const static float speed = 2.0f * 3.1415926f / 50.0f;
    //glm::mat4 mat;
    //glm::vec3 pos = m_camera->getPosition();
    //mat = glm::rotate(mat, passTime * speed, glm::vec3(0.0f, 1.0f, 0.0f));
    //*m = m_camera->to_mat() * mat;
    m_pipeline->bindVertexBuffer(m_vertexBuffer);
    m_pipeline->draw( gpc::PrimitiveType::kTriangle);
    //m_modelPipeline->draw(gpc::PrimitiveType::kTriangle);
    //*m_pv = m_camera->to_mat();
    //const gpc::OTree<gpc::Object<float>, float>* otree = m_scene->getOTree();
    //drawOTree(otree);
}

void MyBlowWindow::initScene() {
    
    m_texture = helper::TextureLoad().makeTexture2D("cottage_diffuse.png");
    /*
    helper::Model<float>* model = helper::Model<float>::parseModel("cottage_obj.obj");
    m_modelVertexBuffer = std::make_shared<gpc::VertexBuffer<helper::Vertex<float>> >();
    m_modelVertexBuffer->copyVertex(model->data(), model->vertexCount());
    m_modelVertexBuffer->copyIndex(model->at(0)->indexData(), model->at(0)->indexCount());
    */

    m_camera = std::make_shared<gpc::MoveProjectionCamera>(60.0f, 800, 600, 0.1f, 10.0f, glm::vec3(0.0f, 0.0f, 10.0f));
    m_camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    Vertex vertex[] = {


        {{-0.5f, -0.5f, 0.0f,  },{1.0f,0.0f,0.0f},{0.0f,0.0f}},
        {{0.5f, -0.5f, 0.0f,   },{0.0f,1.0f,0.0f},{1.0f,0.0f}},
        {{0.0f,   0.5f, 0.0f, },{0.0f,0.0f,1.0f},{1.0f,1.0f}},

    };

    m_vertexBuffer = std::make_shared<gpc::VertexBuffer<Vertex>>();
    m_vertexBuffer->copyVertex(vertex, sizeof(vertex) / sizeof(vertex[0]));
    std::vector<Vertex> line;
    for (float i = 0; i < 599.0f; i += 10.0f) {
        line.push_back({ {-1.0f,-1.0f,0.0f},{1.0f,0.0f,0.0} });
        line.push_back({ {-0.7f, -1.0f + 2.0f * i / 600.0f,0.0f},{0.0f,1.0f,0.0} });
    }

    for (size_t i = 0; i < 100; ++i) {
        line.push_back({ { -5.0f + i * 5.0f / 100.0f, -2.5f, 0.0f }, { 1.0f,0.0f,0.0f } });
        line.push_back({ { 0.0f + i * 5.0f / 100.0f, 2.5f, 0.0f }, { 0.0f,0.0f,1.0f } });

    }
    

    m_vertexBufferLine = std::make_shared<gpc::VertexBuffer<Vertex>>();
    m_vertexBufferLine->copyVertex(line.data(), line.size());


    m_pipeline = std::make_shared<gpc::RasterizePipeline<Vertex, ShaderPass >>( m_device );
    /*
    m_modelPipeline = std::make_shared<gpc::RasterizePipeline<helper::Vertex<float>, ShaderPass>>( m_device );
    m_modelPipeline->setEnableDepthTest(true);
    m_modelPipeline->setEnableDepthWrite(true);
    m_modelPipeline->bindVertexBuffer(m_modelVertexBuffer);
    m_modelPipeline->bindFramebuffer(getFbo());
    m_modelPipeline->bindVertexShader(std::make_shared<ModelVertexShader>(m));
    */



    m_pipeline->bindVertexBuffer(m_vertexBuffer);
    m_pipeline->bindFramebuffer(getFbo());
    m_pipeline->bindVertexShader(std::make_shared<VertexShader>(m));

    std::shared_ptr<FragmentShader> fs = std::make_shared<FragmentShader>(glm::vec3(200.0f, 200.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    fs->texture = m_texture;
    std::shared_ptr< ParallelLightFragmentShader> plfs = std::make_shared<ParallelLightFragmentShader>(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    m_pipeline->bindFragmentShader(fs);
    //m_modelPipeline->bindFragmentShader(fs);


    initAABB();
}

void MyBlowWindow::initAABB() {
    m_pv = std::make_shared<glm::mat4>();
    m_aabbVertex = std::make_shared<gpc::VertexBuffer<glm::vec3>>();
    m_aabbPipeline = std::make_shared<gpc::RasterizePipeline<glm::vec3, glm::vec3>>( m_device );
    m_aabbPipeline->bindVertexShader(std::make_shared<AABBVertexShader>(m_pv ));
    m_aabbPipeline->bindFragmentShader(std::make_shared<AABBFragmentShader>());
    m_aabbPipeline->bindFramebuffer(getFbo());

    m_scene = std::make_shared<gpc::scene<float>>();

    gpc::Sphere<float> *s1 = new gpc::Sphere<float>(glm::tvec3<float>(-3.0, -3.0, -3.0), 2.0);
    gpc::Sphere<float> *s2 = new gpc::Sphere<float>(glm::tvec3<float>(-3.0, 3.0, -3.0), 2.0);
    gpc::Sphere<float> *s3 = new gpc::Sphere<float>(glm::tvec3<float>(3.0, 3.0, -3.0), 2.0);
    gpc::Sphere<float> *s4 = new gpc::Sphere<float>(glm::tvec3<float>(3.0, -3.0, -3.0), 2.0);

    gpc::Sphere<float> *s5= new gpc::Sphere<float>(glm::tvec3<float>(-3.0, -3.0, 3.0), 2.0);
    gpc::Sphere<float> *s6= new gpc::Sphere<float>(glm::tvec3<float>(-3.0, 3.0, 3.0), 2.0);
    gpc::Sphere<float> *s7= new gpc::Sphere<float>(glm::tvec3<float>(3.0, 3.0, 3.0), 2.0);
    gpc::Sphere<float> *s8= new gpc::Sphere<float>(glm::tvec3<float>(3.0, -3.0, 3.0), 2.0);
    gpc::Sphere<float> *s9= new gpc::Sphere<float>(glm::tvec3<float>(3.0, -3.0, 3.0), 2.0);

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

void MyBlowWindow::drawOTree(gpc::OTree<gpc::Object<float>, float> const* tree) {
    if (nullptr != tree) {
        gpc::AABB<float> const& aabb = tree->getAABB();
        drawAABB(aabb);
        for (size_t i = 0; i < 8; ++i) {
            auto child =  tree->atChild(i);
            if (nullptr != child)
                drawOTree(child);
        }
    }
}
/// 这个东西的难点一定是在场景编辑里面
void MyBlowWindow::drawAABB( gpc::AABB<float> const& aabb) {
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
    //m_aabbPipeline->draw(gpc::PrimitiveType::kLine);
    
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


