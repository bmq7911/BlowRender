#pragma once
#include <memory>
#include "Graphics/FragmentShader.h"
#include "widget_vertex.h"

namespace rui {
	class iwidget_painter {
	public:
		virtual std::shared_ptr<gpc::FragmentShader<widget_vertex>> get_fragment_shader() const = 0;

	};
}