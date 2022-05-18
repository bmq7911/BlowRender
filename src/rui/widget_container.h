#pragma once
#include <memory>
#include <list>

#include "widget_base.h"
#include "widget_layout.h"
namespace rui{
    class widget_container : public iwidget{
    public:


    private:
        std::weak_ptr<iwidget> m_parent;
        std::list<std::shared_ptr<iwidget>> m_children;
        std::shared_ptr<iwidget_layout> m_layout;
    }

}