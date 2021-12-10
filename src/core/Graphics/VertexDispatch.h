#pragma once
#include "VertexBuffer.h"
#include "Rasterization.h"
namespace gpc {
    /// 负责派送顶点

    template<typename V, typename _FI,typename I = uint32_t>
    class VertexDispatch {
    public:
        void bindVertexBuffer(std::shared_ptr<VertexBuffer<V>> buffer) {
            m_vertexBuffer = buffer;
        }
        void bindPrimitiveBuffer(std::vector<primitive<_FI>>& primitiveBuffer) {
            m_primitiveBuffer = std::move( primitiveBuffer );
        }
    protected:
        std::shared_ptr<VertexBuffer<V>> m_vertexBuffer;
        std::vector<primitive<_FI>>      m_primitiveBuffer;
    };


    template<typename V, typename _FI, typename I = uint32_t>
    class VertexDispatch_point : public VertexDispatch<V,_FI, I> {
    public:
        VertexDispatch_point()
            : m_index(0)
        {
        }
        bool dispatch(primitive<_FI> & v) {
            if (m_index < m_vertexBuffer->size()) {
                v = m_primitiveBuffer.at(m_index);
                m_index++;
                return true;
            }
            return false;
        }
    private:
        size_t m_index;
    };

    template<typename V, typename _FI,typename I = uint32_t>
    class VertexDispatch_line : public VertexDispatch<V,_FI, I> {
    public:
        VertexDispatch_line()
            :m_index(0)
        {}
        bool dispatch( primitive<_FI>& v1, primitive<_FI>& v2) {
            if (m_index + 1 < m_vertexBuffer->size()) {
                v1 = m_primitiveBuffer.at(m_index);
                v2 = m_primitiveBuffer.at(m_index + 1);
                m_index += 2;
                return true;
            }
            return false;
        }
    private:
        uint32_t m_index;
    };
    /// 形成 0,1,0,1,0,1,0,1的序列用异或
    template<typename V, typename _FI,typename I = uint32_t>
    class VertexDispatch_linestrip : public VertexDispatch<V, _FI,I> {
    public:
        VertexDispatch_linestrip()
            : m_index(0)
        {

        }
        bool dispatch(primitive<_FI> & v1, primitive<_FI>& v2) {
            if (m_index + 1 < m_vertexBuffer->size()) {
                v1 = m_primitiveBuffer.at(m_index);
                v2 = m_primitiveBuffer.at(m_index + 1);
                m_index++;
                //m_index += m_next; 
                //m_next = m_next ^ 1; ///  0,  0 ^ 1 -> 1, 1 ^ 1 -> 0, 0 ^1 =-> 1, 1 ^1 =0, 0 ^1 = 1,
                return true;         ///  0,  0+1 = 1,   1 + 0 = 1,  1 + 1 = 2, 2 + 0 = 2, 2 + 1 = 3,
            }
            return false;
        }
    private:
        uint32_t m_index;
    };

    template<typename V, typename _FI, typename I = uint32_t>
    class VertexDispatch_triangle : public VertexDispatch<V, _FI, I> {
    public:
        VertexDispatch_triangle()
            : m_index(0)
        {}
        bool dispatch(primitive<_FI> & v1, primitive<_FI> & v2, primitive<_FI> & v3) {
            if (m_index + 2 < m_vertexBuffer->size()) {
                v1 = m_primitiveBuffer.at(m_index);
                v2 = m_primitiveBuffer.at(m_index + 1);
                v3 = m_primitiveBuffer.at(m_index + 2);

                m_index += 3;
                return true;
            }
            return false;
        }
    private:
        size_t m_index;
    };

    template<typename V, typename _FI,typename I = uint32_t>
    class VertexDispatch_trianglestrip : public VertexDispatch<V, I> {
    public:
        VertexDispatch_trianglestrip()
            : m_index(0)
        {
        }
        bool dispatch( primitive<_FI>& v1, primitive<_FI>& v2, primitive<_FI>& v3) {
            if (m_index + 2 < m_vertexBuffer->size()) {
                v1 = m_primitiveBuffer.at(m_index);
                v2 = m_primitiveBuffer.at(m_index + 1);
                v3 = m_primitiveBuffer.at(m_index + 2);

                m_index += 1;
                return true;
            }
            return false;
        }
    private:
        size_t m_index;
    };

    template<typename V>
    class UserVertexDispatch {
    public:
        virtual bool dispatchPoint(V& v) = 0;
        virtual bool dispatchLine(V& v1, V& v2) = 0;
        virtual bool dispatchTriangle(V& v1, V& v2, V& v3) = 0;
    };
}
