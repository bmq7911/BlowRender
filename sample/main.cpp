
#include "Rasterizate.h"
#include "RayTrace.h"
#include "editor/MouseInputComponent.h"

/// 1.����Ҫ�ڴ������Լ�ʵ��sseָ����?
/// 2.���������Ż��׶��ܷ����Լ�ִ�е�ָ����SSE��
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
    auto ptr = ed::Cast< ed::MouseInputComponent>(input); /// ����������Ƕ��޷�������һ��,����ʱÿһ�� component class ����һ��id

    rasterizate();
    return 0;
}