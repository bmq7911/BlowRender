#pragma once

#include <vector>
#include "Object.h"
#include "AABB.h"

namespace gpc {
    template<typename M>
    class OTree {
    public:
        OTree()
            : m_parent(nullptr)
        {
            m_child[0] = (nullptr);
            m_child[1] = (nullptr);
            m_child[2] = (nullptr);
            m_child[3] = (nullptr);
            m_child[4] = (nullptr);
            m_child[5] = (nullptr);
            m_child[6] = (nullptr);
            m_child[7] = (nullptr);
        }
        OTree(OTree* parent, AABB const& aabb)
            : m_parent(parent)
            , m_aabb(aabb)
        {
            m_child[0] = (nullptr);
            m_child[1] = (nullptr);
            m_child[2] = (nullptr);
            m_child[3] = (nullptr);
            m_child[4] = (nullptr);
            m_child[5] = (nullptr);
            m_child[6] = (nullptr);
            m_child[7] = (nullptr);
        }
        const AABB& getAABB() const {
            return m_aabb;
        }

        void setAABB(AABB aabb) {
            m_aabb = aabb;
        }
        
        OTree<M>* getParent() const {
            return m_parent;
        }
        void addNode(M* node) {
            m_node.push_back(node);
        }
        size_t sizeNode() const {
            return m_node.size();
        }
        size_t sizeChild() const {
            return 8;
        }
        M* atNode(size_t i) const {
            return m_node.at(i);
        }
        OTree<M>* atChild(size_t i) const {
            if (i < 8) {
                return m_child[i];
            }
            return nullptr;
        }
        bool hit(Ray const& ray) const {
            return m_aabb.hit(ray);
        }
        //// hit 表
        //// 把光线转到AABB空间是否更好处理

        bool hit(Ray const& ray, Float& t, glm::fvec3& normal, M*& model) const {
            if (m_aabb.hit(ray)) {
                Float tmpT1 = std::numeric_limits<Float>::max(); /// tmpT1 保持与光源相交最近的点
                Float tmpT2 = std::numeric_limits<Float>::max();
                glm::tvec3<T> tmpNormal1;
                glm::tvec3<T> tmpNormal2;

                bool bHit = false;
                for (size_t i = 0; i < sizeNode(); ++i) { /// 先与这些物体相交,原因是这些物体都比较的大,有可能会遮挡其他小物体
                    if (atNode(i)->hit(ray, tmpT2, tmpNormal2) && tmpT2 < tmpT1) {
                        bHit = true;
                        tmpT1 = tmpT2;
                        tmpNormal1 = tmpNormal2;
                        //glm::tvec3<T> pos = ray.at(tmpT);
                    }
                }
                for (size_t i = 0; i < 8; ++i) {
                    if (nullptr != atChild(i)) {
                        Float tmpT3;
                        glm::fvec3 tmpNormal3;
                        if (true == atChild(i)->getAABB()->hit(ray, tmpT3, tmpNormal3) && tmpT3 < tmpT1) {
                            auto hit = atChild(i)->hit(ray, tmpT3, tmpNormal3);
                            if (true == hit) {
                                bHit = true;
                                if (tmpT3 < tmpT1) {
                                    tmpT1 = tmpT3;
                                    tmpNormal1 = tmpNormal3;
                                }
                            }
                        }
                    }
                }
                t = tmpT1;
                normal = tmpNormal1;
                return bHit;
            }
            return false;
        }

        /// 这个递归函数一定要写好
        void mountNode(Float gate, M* obj) {
            AABB objAABB = obj->getBVH()->toAABB();
            glm::fvec3 dd = m_aabb.max() - m_aabb.min();
            if (dd.x <= gate && dd.y <= gate && dd.z <= gate) {
                m_node.push_back(obj);
            }
            else {
                for (uint32_t i = 0; i < 8; ++i) {
                    AABB subAABB = m_aabb.subAABB(i);
                    if (nullptr == m_child[i]) { ///子节点为空
                        if (subAABB.in(objAABB)) { /// 这个物体在这个AABB中
                            m_child[i] = new OTree<M>(this, subAABB);
                            m_child[i]->mountNode(gate, obj);
                            return;
                        }
                    }
                    else {
                        if (subAABB.in(objAABB)) {
                            m_child[i]->mountNode(gate, obj);
                            return;
                        }
                    }
                }
                m_node.push_back(obj);
            }
        }
    private:
        AABB   m_aabb;
        OTree<M>* m_parent;
        OTree<M>* m_child[8];
        std::vector<M*> m_node; ////
    };
}
