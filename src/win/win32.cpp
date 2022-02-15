#include "win/win32.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "log.h"

///目的   1.学习图形学的基本算法
///       2.学习使用SIMD等技术
///       3.使用多线程加速等

static const struct {
    float x, y;
    float u, v;
} vertices[6] = {
     -1.0f, 1.0f, 0.0f,1.0f,
     -1.0f, -1.0f ,0.0f,0.0f,
     1.0f,  -1.0f ,1.0f,0.0f,

     1.0f, -1.0f, 1.0f,0.0f,
     1.0f, 1.0f ,1.0f,1.0f,
     -1.0f, 1.0f ,0.0f,1.0f,

};

static const char* vertex_shader_text =
"#version 450 core\n"
"layout( location = 0) in vec4 aPos;\n"
"out vec2 oTex;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4( aPos.x, aPos.y, 0.0, 1.0);\n"
"    oTex = vec2(aPos.z,aPos.w);\n"
"}\n";

static const char* fragment_shader_text =
"#version 450 core\n"
"in vec2 oTex;\n"
"uniform sampler2D texFbo;\n"
"void main()\n"
"{\n"
"    gl_FragColor = texture(texFbo, oTex);\n"
"}\n";



namespace win {

    static GLenum glCheckError_(const char *file, int line) {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) {
            std::string error;
            switch (errorCode) {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }
            std::cout << error.c_str() << " | " << file << " (" << line << ")" << std::endl;
        }
        return errorCode;
    }
#define glCheckError() glCheckError_(__FILE__, __LINE__)

    static void __stdcall glDebugOutput(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam)
    {
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;

        switch (source) {
            case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
            case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
            case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
        } std::cout << std::endl;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
            case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
            case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
            case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
            case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
        } std::cout << std::endl;

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
            case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
        } std::cout << std::endl;
        std::cout << std::endl;
    }


    void BlowWindow::_ErrorCallback(int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    void BlowWindow::_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    BlowWindow::BlowWindow(const char* title, uint32_t width, uint32_t height)
        : m_width( 0)
        , m_height(0)
        , m_program(0)
        , m_texture2d(0)
        , m_window( nullptr)
        , m_data(nullptr)
    {
        _InitWindow( title, width, height);
    }

    bool BlowWindow::attachFrmaebuffer(std::shared_ptr<gpc::Framebuffer> fbo) {
        uint32_t size = fbo->getWidth() * fbo->getHeight() * sizeof(gpc::Framebuffer::value_type);
        m_data = (BYTE*)realloc(m_data, size);
        m_attchFramebuffer = fbo;
        return true;
    }

    std::shared_ptr<gpc::Framebuffer> BlowWindow::getFbo() {
        return m_attchFramebuffer;
    }
    
    void BlowWindow::render( float passTime, float deltaTime) {
        _SwapFrame();
    }

    void BlowWindow::beforeRender(float passTime, float deltaTime) {
    
    }

    void BlowWindow::afterRender(float passTime, float deltaTime) {
    
    }

    void BlowWindow::processInput(float passTime, float deltaTime) {

    }

    void BlowWindow::mainloop() {
        float passTime = 0.0f;
        float deltaTime = 0.0f;
        Watch t;
        Watch t1;
        Watch t2;
        t.start();
        t1.start();
        while (!glfwWindowShouldClose(m_window)) {
            LONGLONG c = t.end();
            passTime = static_cast<float>(t.toTime(c));
            c = t1.end();
            deltaTime = static_cast<float>(t1.toTime(c));
            t1.start( );
            processInput( passTime, deltaTime );
            t2.start();
            render(passTime, deltaTime);
            double rt = t2.toTime(t2.end());
            LOG_TRACE("FPS = ", "%d and deltaTime  = %f renderTime = %lf", int32_t(1.0f / deltaTime), deltaTime,rt);
            BlowWindow::render( passTime , deltaTime);
        }
        return;
    }

    bool BlowWindow::isKeyDown(uint32_t key) {
        if (glfwGetKey(m_window, key) == GLFW_PRESS)
            return true;
        return false;

    }
    
    bool BlowWindow::_InitWindow(const char* title, uint32_t width, uint32_t  height) {
        m_width = width;
        m_height = height;
        GLuint vertex_shader, fragment_shader ;
        if (!glfwInit()) {
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // comment this line in a release build! 

        glfwSetErrorCallback(_ErrorCallback);

        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!m_window) {
            glfwTerminate();
            return false;
        }

        glfwSetKeyCallback(m_window, _KeyCallback);

        glfwMakeContextCurrent(m_window);
        
        glfwSetCursorPosCallback(m_window, _MouseCallback);
        glfwSetScrollCallback(m_window, _ScrollCallback);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        GLint flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }


        glfwSwapInterval(1);

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertex_shader);
        glAttachShader(m_program, fragment_shader);
        glLinkProgram(m_program);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);



		glCreateVertexArrays( 1, &m_VAO);
		glCreateBuffers(1, &m_VBO);
		glNamedBufferStorage( m_VBO, sizeof(vertices ),vertices , 0);
		glBindVertexArray( m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glVertexAttribPointer( 0, 4, GL_FLOAT, GL_FALSE, 0, 0 );
			glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		
	
        glCreateTextures(GL_TEXTURE_2D, 1, &m_texture2d);
        glTextureStorage2D(m_texture2d, 1, GL_RGBA8, m_width, m_height);
        return true;
    }
    
    void BlowWindow::_SwapFrame() {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glViewport(0, 0, m_width, m_height);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (m_attchFramebuffer && nullptr != m_data) {
            m_attchFramebuffer->toData(m_data);
            glTextureSubImage2D( m_texture2d, 0,0,0, m_width,m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
            glBindTexture(GL_TEXTURE_2D, m_texture2d);
            glActiveTexture(GL_TEXTURE0);
			glUseProgram(m_program);
			glBindVertexArray( m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
        }


        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    void BlowWindow::_MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    void BlowWindow::_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    }

}

