#pragma once
#include <memory>
#include "widget.h"

namespace rui {
	class widget_tree {
	public:

	private:
		std::shared_ptr<iwidget> m_root;
		
	};
}