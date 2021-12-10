#pragma once

namespace gpc {
    /// ����ôʵ��
    template<typename _IN, typename _OUT>
    class VertexShader {
    public:
        virtual glm::vec4 execute(_IN const& vertex, _OUT& out) = 0;
    };
}