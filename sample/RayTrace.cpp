#include "RayTrace.h"

RayTraceBlowWindow::RayTraceBlowWindow( const char * title, uint32_t width, uint32_t height,std::shared_ptr<gpc::Device> device)
	: win::BlowWindow( title, width, height)
{
	std::shared_ptr<gpc::Framebuffer> frame = std::make_shared<gpc::Framebuffer>( gpc::Framebuffer( width, height));
	attachFrmaebuffer(frame);
	m_pipeline = std::make_shared<gpc::RayTracePipeline>( width, height, frame );
	m_device = device;
}


void RayTraceBlowWindow::initScene() {

}

void RayTraceBlowWindow::tick(float passTime, float deltaTime)  {
	auto fbo = getFbo( );
	fbo->clear({ 255,255,255,0 });
	
	m_device->bindTaskIssue(m_pipeline);
	m_device->wait();
}
