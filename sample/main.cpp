
#include "Rasterizate.h"
#include "RayTrace.h"
#include "editor/MouseInputComponent.h"

/// 1.我需要在代码里自己实现sse指令吗?
/// 2.编译器在优化阶段能否发现自己执行的指令是SSE的
void rasterizate() {
    std::shared_ptr<gpc::Device> device = std::make_shared<gpc::Device>( );
    std::shared_ptr<MyBlowWindow> window = std::make_shared<MyBlowWindow>("test",800,600, device );
    window->initScene();
    window->mainloop();
    device->stop( );
}

void ray_trace() {
    std::shared_ptr<gpc::Device> device = std::make_shared<gpc::Device>( );
    std::shared_ptr<RayTraceBlowWindow> window = std::make_shared<RayTraceBlowWindow>("test",600,600, device );
    window->initScene();
    window->mainloop();
    device->stop( );
}

int main(int argc, char* argv[]) {
    //ray_trace();
    ed::IComponent* input = new ed::MouseInputComponent;
    auto ptr = ed::Cast< ed::MouseInputComponent>(input); /// 不论如何我们都无法做到这一步,除非时每一个 component class 都有一个id

    rasterizate();
    return 0;
}