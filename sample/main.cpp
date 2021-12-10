
#include "Rasterizate.h"
#include "RayTrace.h"

/// 1.我需要在代码里自己实现sse指令吗?
/// 2.编译器在优化阶段能否发现自己执行的指令是SSE的
void rasterizate() {
    std::shared_ptr<gpc::Device> device = std::make_shared<gpc::Device>( );
    std::shared_ptr<MyBlowWindow> window = std::make_shared<MyBlowWindow>("test",800,600, device );
    window->initScene();
    window->mainloop();
}

void ray_trace() {
    std::shared_ptr<gpc::Device> device = std::make_shared<gpc::Device>( );
    std::shared_ptr<RayTraceBlowWindow> window = std::make_shared<RayTraceBlowWindow>("test",600,600, device );
    window->initScene();
    window->mainloop();
}

int main(int argc, char* argv[]) {
    //ray_trace();
    rasterizate();
    return 0;
}