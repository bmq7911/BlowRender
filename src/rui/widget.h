#pragma once

#include "Graphics/VertexBuffer.h"
#include "Graphics/FragmentShader.h"
#include "widget_vertex.h"

namespace rui{

	struct widget_render_data {
		std::shared_ptr<gpc::VertexBuffer<widget_vertex>> m_vertexBuffer;
		std::shared_ptr<gpc::FragmentShader<widget_vertex>> m_fragmentShader;
	};
	class iwidget{
		virtual void tick( float passTime, float deltaTime ) = 0;
		virtual widget_render_data*  gen_render_data() = 0;
	};
	
	
}