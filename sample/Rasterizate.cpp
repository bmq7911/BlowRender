#include <iostream>
#include "Rasterizate.h"

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

template<typename T>
class C {
public:

    template<typename U>
    friend C<U> operator* (C<U> const& c1, C<U> const& c2);
    template<typename U>
    friend C<U> operator* ( U const& v, C<U> const& c );
    template<typename U>
    friend C<U> operator+(C<U> const& c1, C<U> const& c2);

public:
    C() 
        : m_r(T{})
        , m_i(T{})
    {
    }
    C(T r, T i ) 
        : m_r( r)
        , m_i( i)
    {}
    C( const C & c) {
        m_r = c.m_r;
        m_i = c.m_i;
    }
    C& operator=(C const& c) {
        m_r = c.m_r;
        m_i = c.m_i;
        return *this;
    }

    T r() const {
        return m_r;
    }
    T i() const {
        return m_i;
    }
    T length() const {
    
    }
    T length2() const {
        return m_r * m_r + m_i * m_i;
    }
    C& operator *=(T const* v) {
        m_r *= v;
        m_i *= v;
        return *this;
    }
    C& operator+=(C const& t) {
        m_r += t.m_r;
        m_i += t.m_i;
        return *this;
    }
    C& operator-=(C const& t) {
        m_r -= t.m_r;
        m_i -= t.m_i;
        return *this;
    }

private:
    T m_r;
    T m_i;
};

template<typename T>
C<T> operator+(C<T> const& c1, C<T> const& c2) {
    C<T> c;
    c.m_r = c1.m_r + c2.m_r;
    c.m_i = c1.m_i + c2.m_i;
    return c;
}

template<typename T>
C<T> operator* (C<T> const& c1, C<T> const& c2) {
    C<T> c;
    c.m_r = c1.m_r * c2.m_r - c1.m_i * c2.m_i;
    c.m_i = c1.m_r * c2.m_i + c1.m_i * c2.m_r;
    return c;
}
template<typename T>
C<T> operator* ( T const& v , C<T> const& c ){
    C<T> t;
    t.m_r = v * c.m_r;
    t.m_i = v * c.m_i;
    return t;
}

class JuliaFragmentShader : public gpc::FragmentShader<glm::vec4>{
public:
    glm::vec4 execute(gpc::fragment< glm::vec4>  const& v) override {

        C<float> c( 3.2f * (v.pos.x + 1.0f)/2.0f - 1.6f,
                    2.4f *( v.pos.y + 1.0f)/2.0f - 1.2f);
        C<float> t(-0.75f, 0.0f);

        size_t k = 0;
        for ( k = 0; k < 256; ++k) {
            if ( c.length2() > 4.0f) {
                break;
            }
            c = c * c + t;
        }
        if (k == 256) {
            return glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        }
        else {
            return glm::vec4( k / 255.0f ,k / 255.0f, k / 255.0f , 1.0f );
        }

    }

};


MyBlowWindow::MyBlowWindow(const char* title, uint32_t width, uint32_t height, std::shared_ptr<gpc::Device> device)
    : win::BlowWindow(title, width, height)
    , m_device( device )
{
    m = std::make_shared<glm::mat4>();
    std::shared_ptr<gpc::Framebuffer> frame = std::make_shared<gpc::Framebuffer>(gpc::Framebuffer(width, height));
    m_ruiRoot = std::make_shared<rui::widget_tree>(device, frame);
    attachFrmaebuffer(frame);
}

/// soft render
void MyBlowWindow::tick(float passTime, float deltaTime)  {
    auto fbo = getFbo();
    fbo->clear({ 0,0,0,0 });
    fbo->clearDepth(1.0f);
    const static float speed = 2.0f * 3.1415926f / 50.0f;

    //m_juliaPipeline->draw(gpc::PrimitiveType::kTriangle);
    //m_container->tick(passTime, deltaTime);
    m_ruiRoot->tick(passTime, deltaTime);
}


void MyBlowWindow::initScene() {
    
    m_texture = helper::TextureLoad().makeTexture2D("a.jpg");

    m_camera = std::make_shared<gpc::MoveProjectionCamera>(60.0f, 800, 600, 0.1f, 10.0f, glm::vec3(0.0f, 0.0f, 10.0f));
    m_camera->setLookAt(glm::vec3(0.0f, 0.0f, 0.0f));

    Vertex vertex[] = {

        {{-1.0f, -1.0f, 0.0f,  },{1.0f,0.0f,0.0f},{0.0f,0.0f}},
        {{1.0f, -1.0f, 0.0f,   },{0.0f,1.0f,0.0f},{1.0f,0.0f}},
        {{1.0f,   1.0f, 0.0f, },{0.0f,0.0f,1.0f},{1.0f,1.0f}},
        
        {{1.0f,   1.0f, 0.0f, },{0.0f,0.0f,1.0f},{1.0f,1.0f}},
        {{-1.0f,  1.0f, 0.0f, },{0.0f,0.0f,1.0f},{0.0f,1.0f}},
        {{-1.0f,  -1.0f, 0.0f, },{0.0f,0.0f,1.0f},{0.0f,0.0f}},

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



    m_pipeline->bindVertexBuffer(m_vertexBuffer);
    m_pipeline->bindFramebuffer(getFbo());
    m_pipeline->bindVertexShader(std::make_shared<VertexShader>(m));

    std::shared_ptr<FragmentShader> fs = std::make_shared<FragmentShader>(glm::vec3(200.0f, 200.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    fs->texture = m_texture;
    std::shared_ptr< ParallelLightFragmentShader> plfs = std::make_shared<ParallelLightFragmentShader>(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    m_pipeline->bindFragmentShader(fs);


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
    m_juliaPipeline->bindFragmentShader(std::make_shared<JuliaFragmentShader>());

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


