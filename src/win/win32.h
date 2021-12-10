#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif ///NOMIXMAX
#include <windows.h>
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Framebuffer.h"

namespace win {
	class Watch{
	public:
		Watch() {
			QueryPerformanceFrequency(&m_freq);
		}
		
		void start(){
			QueryPerformanceCounter(&m_begin);
		}
		
		long long end(){
			 QueryPerformanceCounter(&m_end);
			 return m_end.QuadPart - m_begin.QuadPart;
		}
		
		double toTime( long long t ){
			return (double)t / double( m_freq.QuadPart);	
		}
		
	private:
		LARGE_INTEGER m_freq;
		LARGE_INTEGER m_begin;
		LARGE_INTEGER m_end;	
	};	
	
    class BlowWindow {
    public:
        BlowWindow() = delete;
        BlowWindow(const char* title, uint32_t width, uint32_t height);
        bool attachFrmaebuffer( std::shared_ptr<gpc::Framebuffer> fbo);
        std::shared_ptr<gpc::Framebuffer> getFbo();
        virtual void render( float passTime, float deltaTime);
        virtual void beforeRender(float passTime, float deltaTime);
        virtual void afterRender(float passTime, float deltaTime);
        virtual void processInput( float passTime, float deltaTime);
        virtual void initScene() = 0;
        void mainloop();
        bool isKeyDown(uint32_t);
    private:
        static void _ErrorCallback(int error, const char* description);
        static void _KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods);
        bool _InitWindow(const char* title, uint32_t width, uint32_t  height);
        void _SwapFrame();
        static void _MouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void _ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    private:
        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_program;
		uint32_t m_VAO;
		uint32_t m_VBO;
        uint32_t m_texture2d;
        GLFWwindow * m_window;
        unsigned char* m_data;
        std::shared_ptr<gpc::Framebuffer> m_attchFramebuffer;
    };
}