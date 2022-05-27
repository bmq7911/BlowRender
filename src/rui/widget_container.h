#pragma once
#include <memory>
#include <list>
#include <glm/glm.hpp>
#include "widget_base.h"
#include "widget_layout.h"
#include "widget_rect.h"
#include "widget_vertex.h"
namespace rui{

    class widget_container : public iwidget {
    public:
        widget_container(  ) 
        {
        }
        
        void tick(float passTime, float deltaTime) override {

        }
    private:
        std::weak_ptr<iwidget> m_parent;
        std::list<std::shared_ptr<iwidget>> m_children;
    };

}