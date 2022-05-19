#pragma once

#include <iostream>

#include "glm/glm.hpp"
#include "RayTrace.h"
#include "win/win32.h"
#include "Ray/RayTracePipeline.h"
#include "Graphics/AABB.h"
#include "scene.h"


class RayTraceBlowWindow : public win::BlowWindow {
public:
	RayTraceBlowWindow(const char* title, uint32_t width, uint32_t height,std::shared_ptr<gpc::Device>);
    void initScene( ) override;
	void tick(float passTime, float deltaTime) override;
public:
	std::shared_ptr<gpc::RayTracePipeline> m_pipeline;
	std::shared_ptr<gpc::Device> m_device;
};