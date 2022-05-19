#pragma once

namespace rui{
	class iwidget{
		virtual void tick( float passTime, float deltaTime ) = 0;
	};
	
	
}