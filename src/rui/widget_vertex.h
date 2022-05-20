#pragma once
#include <glm/glm.hpp>
namespace rui {

    struct widget_vertex {
        glm::vec2 aPos;
        glm::vec2 aTex;
        widget_vertex operator + (widget_vertex const& p) const {
            widget_vertex t;
            t.aPos = aPos + p.aPos;
            t.aTex = aTex + p.aTex;
            return t;
        }
        widget_vertex& operator+=(widget_vertex const& p) {
            aPos += p.aPos;
            aTex += p.aTex;
            return *this;
        }
        widget_vertex operator * (float k) const {
            widget_vertex t;
            t.aPos = k * aPos;
            t.aTex = k * aTex;
            return t;
        }
        widget_vertex& operator*=(float k) {
            aPos *= k;
            aTex *= k;
            return *this;
        }
    };
    static inline widget_vertex operator*( float k, widget_vertex const& p ) {
        return p * k;
    }
}