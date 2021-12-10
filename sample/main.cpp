
#include "Rasterizate.h"
#include "RayTrace.h"

/// 1.����Ҫ�ڴ������Լ�ʵ��sseָ����?
/// 2.���������Ż��׶��ܷ����Լ�ִ�е�ָ����SSE��
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