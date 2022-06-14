#include "ModelLoad.h"

namespace helper {

        uint32_t const* Mesh::indexData() const {
            return m_index.data();
        }
        size_t Mesh::indexCount() const {
            return m_index.size();
        }

        size_t Model::meshCount() const {
            return m_meshs.size();
        }
        helper::Vertex const* Model::data() const {
            return m_vertexs.data();
        }
        size_t Model::vertexCount() const {
            return m_vertexs.size();
        }
        const Mesh*Model:: at(size_t index)const {
            return m_meshs.at(index);
        }
        const gpc::AABB& Model::getAABB() const {
            return m_aabb;
        }
        const gpc::BVH* Model::getBVH() const {
            return &m_aabb;
        }
        bool Model::hit(gpc::Ray const& ray, Float& t, glm::fvec3& normal) const {
            if (m_aabb.hit(ray)) {

            }
            return false;
        }

        std::shared_ptr<gpc::VertexBuffer<helper::Vertex>> Model::getVertexBuffer() const {
            return m_vertexBuffer;
        }

        Model* Model::parseModel(const char* path) {
            if (nullptr == path)
                return nullptr;
            std::string strPath(path);
            char buffer[1024] = { 0 };
            std::vector<glm::fvec3> pos;
            std::vector<glm::fvec3> normals;
            std::vector<glm::fvec2> texcoords;
            std::vector<glm::ivec3>    faces;
            std::vector<glm::fvec4> tangent;
            std::vector<glm::fvec4> joint;
            std::vector<glm::fvec4> weight;


            FILE* file = fopen(path, "rb");
            if (nullptr != file) {
                while (nullptr != fgets(buffer, 1024, file)) {
                    glm::fvec3 aPos;
                    if (0 == strncmp(buffer, "v ", 2)) {
                        int nIterms = sscanf(buffer, "v %f %f %f",
                            &aPos.x, &aPos.y, &aPos.z);
                        if (3 != nIterms)
                            return nullptr;
                        pos.push_back(aPos);
                    }
                    else if (0 == strncmp(buffer, "vn ", 3)) {
                        glm::fvec3 aNormal;
                        int nIterms = sscanf(buffer, "vn %f %f %f",
                            &aNormal.x, &aNormal.y, &aNormal.z);
                        if (3 != nIterms)
                            return nullptr;
                        normals.push_back(aNormal);
                    }
                    else if (0 == strncmp(buffer, "vt ", 3)) {
                        glm::fvec2 aTex;
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
                        glm::fvec4 aTangent;
                        int nIterms = sscanf(buffer, "# ext.tangent %f %f %f %f",
                            &aTangent.x, &aTangent.y, &aTangent.z, &aTangent.w);
                        if (4 != nIterms)
                            return nullptr;
                        tangent.push_back(aTangent);
                    }
                    else if (0 == strncmp(buffer, "# ext.joint ", 12)) {
                        glm::fvec4 aJoint;
                        int nIterms = sscanf(buffer, "# ext.joint %f %f %f %f",
                            &aJoint.x, &aJoint.y, &aJoint.z, &aJoint.w);
                        if (4 != nIterms)
                            return nullptr;
                        joint.push_back(aJoint);
                    }
                    else if (0 == strncmp(buffer, "# ext.weight ", 13)) {
                        glm::fvec4 aWeight;
                        int nIterms = sscanf(buffer, "# ext.weight %f %f %f %f",
                            &aWeight.x, &aWeight.y, &aWeight.z, &aWeight.w);
                        if (4 != nIterms)
                            return nullptr;
                        weight.push_back(aWeight);
                    }

                }
                fclose(file);
                return _BuildModel(pos, normals, texcoords, faces, tangent, joint, weight);
            }
            
            
            
            return nullptr;
        }

        Model* Model::_BuildModel(std::vector<glm::fvec3>const& position, std::vector<glm::fvec3> const& normals,
            std::vector<glm::fvec2> const& texcoords, std::vector<glm::ivec3> const& faces,
            std::vector<glm::fvec4> const& tangent, std::vector<glm::fvec4> const& joint,
            std::vector<glm::fvec4>const& weight) {
            /// 
            gpc::MakeAABB make;
            std::vector<helper::Vertex> vertexs;
            std::unordered_map<glm::ivec3, uint32_t> map;
            std::vector<uint32_t> indices;
            helper::Vertex vertex;
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
                    //uint32_t index = static_cast<uint32_t>(vertexs.size() - 1);
                    //indices.push_back(index);
                    //map.insert(std::make_pair(face, index));
                }
            }


            Mesh* mesh = new Mesh;
            mesh->m_index = std::move(indices);
            Model* model = new Model;
            model->m_aabb = make.getAABB();
            model->m_meshs.push_back(mesh);
            model->m_vertexs = std::move(vertexs);
            model->m_vertexBuffer = std::make_shared<gpc::VertexBuffer<helper::Vertex>>();
            model->m_vertexBuffer->copyVertex( model->m_vertexs.data(), model->m_vertexs.size() );
            //model->m_vertexBuffer->copyIndex(mesh->m_index.data(), mesh->m_index.size());
            return model;
        }


}
