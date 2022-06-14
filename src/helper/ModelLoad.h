#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Graphics/AABB.h"
#include "Graphics/VertexBuffer.h"

namespace std {
    template<>
    struct hash<glm::ivec3> {
        size_t operator( ) (const glm::ivec3 key) const noexcept {
            return size_t(key.x + key.y + key.z);
        }
    };
}

namespace helper {
    struct Vertex {
        glm::fvec3 aPos;
        glm::fvec3 aNormal;
        glm::fvec4 aTangent;
        glm::fvec4 aJoint;
        glm::fvec4 aWeight;
        glm::fvec2 aTex;

        Vertex operator + (Vertex const& p) const {
            Vertex t;
            t.aNormal  = this->aNormal + p.aNormal;
            t.aPos     = this->aPos + p.aPos;
            t.aTex     = this->aTex + p.aTex;
            t.aTangent = this->aTangent + p.aTangent;
            t.aJoint   = this->aJoint + p.aJoint;
            t.aWeight  = this->aWeight + p.aWeight;
            return t;
        }
        Vertex& operator+=(Vertex const& p) {
            aNormal += p.aNormal;
            aPos    += p.aPos;
            aTex    += p.aTex;
            aTangent += p.aTangent;
            aJoint += p.aJoint;
            aWeight += p.aWeight;
            return *this;
        }
        Vertex operator *(float k) const {
            Vertex t;
            t.aNormal = k * aNormal;
            t.aPos = k * aPos;
            t.aTex = k * aTex;
            t.aTangent = k * aTangent;
            t.aJoint = k * aJoint;
            t.aWeight = k * aWeight;
            return t;
        }
        Vertex& operator*=(float k) {
            aNormal *= k;
            aPos *= k;
            aTex *= k;
            aTangent *= k;
            aJoint *= k;
            aWeight *= k;
            return *this;
        }

    };
    static inline Vertex operator*(float k, Vertex const& p) {
        return p * k;
    }
    class Nodel;

    class Mesh {
    public:
        uint32_t const* indexData() const;
        size_t indexCount() const;

    private:
        friend class Model;
        std::vector<uint32_t> m_index;
    };

    class Model : public gpc::Object{
        friend class Mesh;
    public:
        size_t meshCount() const;
        helper::Vertex const* data() const;
        size_t vertexCount() const;
        const Mesh* at(size_t index)const;
        const gpc::AABB& getAABB() const;
        const gpc::BVH* getBVH() const override;
        bool hit(gpc::Ray const& ray, Float& t, glm::fvec3& normal) const override;
        std::shared_ptr<gpc::VertexBuffer<helper::Vertex>> getVertexBuffer() const;
        static Model* parseModel(const char* path);
        static Model* _BuildModel(std::vector<glm::fvec3>const& position, std::vector<glm::fvec3> const& normals,
            std::vector<glm::fvec2> const& texcoords, std::vector<glm::ivec3> const& faces,
            std::vector<glm::fvec4> const& tangent, std::vector<glm::fvec4> const& joint,
            std::vector<glm::fvec4>const& weight);
    private:
        std::vector<Mesh*> m_meshs;
        std::vector<helper::Vertex> m_vertexs;
        std::shared_ptr<gpc::VertexBuffer<helper::Vertex>> m_vertexBuffer;
        gpc::AABB m_aabb;
    };

}
