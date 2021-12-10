#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Graphics/AABB.h"
#include "Graphics/Object.h"

namespace std {
    template<>
    struct hash<glm::ivec3> {
        size_t operator( ) (const glm::ivec3 key) const noexcept {
            return size_t(key.x + key.y + key.z);
        }
    };
}

namespace helper {
    template<typename T>
    struct Vertex {
        glm::tvec3<T> aPos;
        glm::tvec3<T> aNormal;
        glm::tvec4<T> aTangent;
        glm::tvec4<T> aJoint;
        glm::tvec4<T> aWeight;
        glm::tvec2<T> aTex;
    };
    
    template<typename T>
    class Nodel;

    template<typename T>
    class Mesh {
    public:
        uint32_t const* indexData() const {
            return m_index.data();
        }
        size_t indexCount() const {
            return m_index.size();
        }

    private:
        template<typename>
        friend class Model;
        std::vector<uint32_t> m_index;
    };
    template<typename T>
    class Model : public gpc::Object<T>{
        template<typename>
        friend class Mesh;
    public:
        size_t meshCount() const {
            m_meshs.size();
        }
        helper::Vertex<T> const* data() const {
            return m_vertexs.data();
        }
        size_t vertexCount() const {
            return m_vertexs.size();
        }
        const Mesh<T>* at(size_t index)const {
            return m_meshs.at(index);
        }
        const gpc::AABB<T>& getAABB() const {
            return m_aabb;
        }
        const gpc::BVH<T>* getBVH() const override{
            return &m_aabb;
        }
        bool hit(gpc::Ray<T>const & ray, T& t, glm::tvec3<T>& normal) const override {
            if (m_aabb.hit(ray)) {
                
            }
            return false;
        }

       
        static Model<T>* Model::parseModel(const char* path) {
            if (nullptr == path)
                return nullptr;
            std::string strPath(path);
            char buffer[1024] = { 0 };
            std::vector<glm::tvec3<T>> pos;
            std::vector<glm::tvec3<T>> normals;
            std::vector<glm::tvec2<T>> texcoords;
            std::vector<glm::ivec3>    faces;
            std::vector<glm::tvec4<T>> tangent;
            std::vector<glm::tvec4<T>> joint;
            std::vector<glm::tvec4<T>> weight;


            FILE* file = fopen(path, "rb");
            if (nullptr != file) {
                while (nullptr != fgets(buffer, 1024, file)) {
                    /// 这是顶点数据
                    glm::tvec3<T> aPos;
                    if (0 == strncmp(buffer, "v ", 2)) {
                        int nIterms = sscanf(buffer, "v %f %f %f",
                            &aPos.x, &aPos.y, &aPos.z);
                        if (3 != nIterms)
                            return nullptr;
                        pos.push_back(aPos);
                    }
                    else if (0 == strncmp(buffer, "vn ", 3)) {
                        glm::tvec3<T> aNormal;
                        int nIterms = sscanf(buffer, "vn %f %f %f",
                            &aNormal.x, &aNormal.y, &aNormal.z);
                        if (3 != nIterms)
                            return nullptr;
                        normals.push_back(aNormal);
                    }
                    else if (0 == strncmp(buffer, "vt ", 3)) {
                        glm::tvec2<T> aTex;
                        int nIterms = sscanf(buffer, "vt %f %f", &aTex.x, &aTex.y);
                        if (2 != nIterms)
                            return nullptr;
                        texcoords.push_back(aTex);
                    }
                    else if (0 == strncmp(buffer, "f ", 2)) {
                        int32_t v1_v, v1_n, v1_t;
                        int32_t v2_v, v2_n, v2_t;
                        int32_t v3_v, v3_n, v3_t;
                        int nIterms = sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                            &v1_v, &v1_t, &v1_n,
                            &v2_v, &v2_t, &v2_n,
                            &v3_v, &v3_t, &v3_n
                        );
                        if (9 != nIterms)
                            return nullptr;
                        faces.push_back(glm::ivec3(v1_v, v1_t, v1_n));
                        faces.push_back(glm::ivec3(v2_v, v2_t, v2_n));
                        faces.push_back(glm::ivec3(v3_v, v3_t, v3_n));
                    }
                    else if (0 == strncmp(buffer, "# ext.tangent ", 14)) {
                        glm::tvec4<T> aTangent;
                        int nIterms = sscanf(buffer, "# ext.tangent %f %f %f %f",
                            &aTangent.x, &aTangent.y, &aTangent.z, &aTangent.w);
                        if (4 != nIterms)
                            return nullptr;
                        tangent.push_back(aTangent);
                    }
                    else if (0 == strncmp(buffer, "# ext.joint ", 12)) {
                        glm::tvec4<T> aJoint;
                        int nIterms = sscanf(buffer, "# ext.joint %f %f %f %f",
                            &aJoint.x, &aJoint.y, &aJoint.z, &aJoint.w);
                        if (4 != nIterms)
                            return nullptr;
                        joint.push_back(aJoint);
                    }
                    else if (0 == strncmp(buffer, "# ext.weight ", 13)) {
                        glm::tvec4<T> aWeight;
                        int nIterms = sscanf(buffer, "# ext.weight %f %f %f %f",
                            &aWeight.x, &aWeight.y, &aWeight.z, &aWeight.w);
                        if (4 != nIterms)
                            return nullptr;
                        weight.push_back(aWeight);
                    }

                }
                fclose(file);
                /// 开始组建mesh
                return _BuildModel(pos, normals, texcoords, faces, tangent, joint, weight);
            }
            return nullptr;
        }

        static Model<T>* Model::_BuildModel(std::vector<glm::tvec3<T>>const& position, std::vector<glm::tvec3<T>> const& normals,
            std::vector<glm::tvec2<T>> const& texcoords, std::vector<glm::ivec3> const& faces,
            std::vector<glm::tvec4<T>> const& tangent, std::vector<glm::tvec4<T>> const& joint,
            std::vector<glm::tvec4<T>>const& weight) {
            /// 
            gpc::MakeAABB<T> make;
            std::vector<helper::Vertex<T>> vertexs;
            std::unordered_map<glm::ivec3, uint32_t> map;
            std::vector<uint32_t> indices;
            helper::Vertex<T> vertex;
            for (size_t i = 0; i < faces.size(); ++i) {
                glm::ivec3 face = faces[i];
                auto iter = map.find(face);
                if (iter != map.end()) {
                    uint32_t index = iter->second;
                    indices.push_back(index);
                }
                else {
                    make.update(vertex.aPos);
                    vertex.aPos = position[face.x - 1];
                    vertex.aTex = texcoords[face.y - 1];
                    vertex.aNormal = normals[face.z - 1];
                    if (tangent.size()) {
                        vertex.aTangent = tangent[face.x - 1];
                    }
                    else {
                        vertex.aTangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
                    }
                    if (joint.size()) {
                        vertex.aJoint = joint[face.x - 1];
                    }
                    else {
                        vertex.aJoint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                    }
                    if (weight.size()) {
                        vertex.aWeight = weight[face.x - 1];
                    }
                    else {
                        vertex.aWeight = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                    }
                    vertexs.push_back(vertex);
                    uint32_t index = static_cast<uint32_t>(vertexs.size() - 1);
                    indices.push_back(index);
                    map.insert(std::make_pair(face, index));
                }
            }


            Mesh<T>* mesh = new Mesh<T>;
            mesh->m_index = std::move(indices);
            Model<T>* model = new Model<T>;
            model->m_aabb = make.getAABB();
            model->m_meshs.push_back(mesh);
            model->m_vertexs = std::move(vertexs);
            return model;
        }
        
    private:
        std::vector<Mesh<T>*> m_meshs;
        std::vector<helper::Vertex<T>> m_vertexs;
        gpc::AABB<T> m_aabb;
    };
}
