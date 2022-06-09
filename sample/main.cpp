
#include "Rasterizate.h"
#include "RayTrace.h"
#include "editor/MouseInputComponent.h"

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
    
    ed::IComponent* input = new ed::MouseInputComponent;
    auto ptr = ed::Cast< ed::MouseInputComponent>(input); 

    rasterizate();
    //ray_trace();
    return 0;
}