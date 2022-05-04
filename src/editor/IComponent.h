#pragma once
#include <stdint.h>
#include <memory>

namespace ed {
	/// <summary>
	/// 1. declare the enum ComponentType
	/// </summary>
	enum ComponentType : uint32_t {
#define DECL_COMPONENT_TYPE( x,y,z,... ) x,
#include "ComponentType.inl"
#undef DECL_COMPONENT_TYPE
		kUnknown = 0xFFFFFFFF,
	};

	template<typename T> struct __component_type_trait { static const ComponentType type = ComponentType::kUnknown; };
	
#define DECL_COMPONENT_TYPE(x,y,z, ... )class y;template<>struct __component_type_trait<y>{static const ComponentType type = ComponentType::x;};
#include "ComponentType.inl"
#undef DECL_COMPONENT_TYPE

	struct IComponent {
		virtual ~IComponent() {};
		virtual ComponentType getComponentType( ) const = 0;
	};


	template<typename T>
	T* Cast(IComponent* component ) {
		if (nullptr == component)
			return nullptr;
		if (component->getComponentType() != __component_type_trait<T>::type) {
			return nullptr;
		}
		return static_cast<T*>(component);
	}
	
	template<typename T>
	T const* Cast( IComponent const*  component ) {
		if (nullptr == component)
			return nullptr;
		if (component->getComponentType() != __component_type_trait<T>::type) {
			return nullptr;
		}
		return static_cast<T const *>(component);
	}

	template<typename T>
	std::shared_ptr<T> Cast(std::shared_ptr<IComponent> component ) {
		if (nullptr == component)
			return nullptr;
		if (component->getComponentType() != __component_type_trait<T>::type) {
			return nullptr;
		}
		return std::static_pointer_cast<T>( component );
	}


}
