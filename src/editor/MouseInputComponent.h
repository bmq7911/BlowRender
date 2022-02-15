#pragma once
#include "IComponent.h"
namespace ed {
	class MouseInputComponent : public IComponent {
	public:
		virtual ~MouseInputComponent() {}
		ComponentType getComponentType() const final override {
			return __component_type_trait<MouseInputComponent>::type;
		}

	};

}