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
        widget_container( rui::widget_rect const &rect ) 
            : m_rect( rect )
        {
        }
        
        void tick(float passTime, float deltaTime) override {

        }
        rui::widget_rect const& get_rect() const {
            return m_rect;
        }
    private:
        rui::widget_rect m_rect;
        std::weak_ptr<iwidget> m_parent;
        std::list<std::shared_ptr<iwidget>> m_children;
    };

}