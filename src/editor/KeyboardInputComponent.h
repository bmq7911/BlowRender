#pragma once
#include "IComponent.h"
namespace ed {
	class KeyboardInputComponent : public IComponent{
	public:
		virtual ~KeyboardInputComponent() {}
		ComponentType getComponentType() const override {
			return __component_type_trait<KeyboardInputComponent>::type;
		}
		
	private:
	
	};

}