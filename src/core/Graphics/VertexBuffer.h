#pragma once
#include <string.h>
#include <vector>

namespace gpc {
    template<typename V>
    class VertexBuffer {
        using byte = unsigned char;
        enum IndexType {
            kUINT32,
            kUINT16,
            kUINT8,
            kUnknown,
        };
    public:
        VertexBuffer()
            : m_type(kUnknown)
            , m_indexCount(0)
        {}

        void copyVertex(V const* data, size_t n) {
            m_vertex.resize(n);
            memcpy(m_vertex.data(), data, n * sizeof(V));
        }
        void copyIndex(uint32_t const* data, size_t n) {
            m_indexCount = n;
            m_index.resize(n * sizeof(uint32_t));
            memcpy(m_index.data(), data, n * sizeof(uint32_t));
            m_type = kUINT32;
        }
        void copyIndex(uint16_t const* data, size_t n) {
            m_indexCount = n;
            size_t count = (n + sizeof(uint32_t)) * sizeof(uint16_t) / sizeof(uint32_t);
            m_index.resize(count);
            memcpy(m_index.data(), data, n * sizeof(unsigned short));
            m_type = kUINT16;
        }
        void copyIndex(uint8_t const* data, size_t n) {
            m_indexCount = n;
            size_t count = (n + sizeof(uint32_t)) * sizeof(uint8_t) / sizeof(uint32_t);
            m_index.resize(count);
            memcpy(m_index.data(), data, n * sizeof(uint8_t));
            m_type = kUINT8;
        }
        size_t sizeVertex() const {
            return m_vertex.size();
        }
        size_t size() const {
            if (0 == m_indexCount)
                return sizeVertex();
            return m_indexCount;
        }
        V const& at(size_t i) const {
            size_t index = i;
            if (i < m_indexCount) {
                index = _At(i);
            }
            return m_vertex.at(index);
        }
        V const& atVertex(size_t i) const {
            return m_vertex.at(i);
        }
    private:
        size_t _At(size_t i) const {
            switch (m_type) {
            case kUINT32: {
                uint32_t const* p = reinterpret_cast<uint32_t const*>(m_index.data());
                return p[i];
            }break;
            case kUINT16: {
                const uint16_t* p = reinterpret_cast<uint16_t const*>(m_index.data());
                return p[i];
            }break;
            case kUINT8: {
                const uint8_t* p = reinterpret_cast<uint8_t const*>(m_index.data());
                return p[i];
            }break;
            default:
                break;
            }
            return size_t(-1);
        }
    private:
        std::vector<V> m_vertex;
        std::vector<uint32_t> m_index;
        IndexType m_type;
        size_t    m_indexCount;
    };

}
