#pragma once
namespace gpc {
#define _CENTER_AABB_CONST_FACTOR 1.2599210498948731647672106072782

    template<typename M, typename T = double>
    class NTree {
    public:
        NTree()
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
            m_child[8] = (nullptr);
        }
        NTree(NTree* parent, AABB<T> const& aabb)
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
            m_child[8] = (nullptr);
        }
        AABB<T> getAABB() const {
            return m_aabb;
        }
        void setAABB(AABB<T> aabb) {
            m_aabb = aabb;
        }
        NTree<M, T>* getParent() const {
            return m_parent;
        }
        void addNode(M* node) {
            m_node.push_back(node);
        }
        size_t sizeNode() const {
            return m_node.size();
        }
        M* atNode(size_t i) const {
            return m_node.at(i);
        }
        NTree<M, T>* atChild(size_t i) const {
            if (i < 8) {
                return m_child[i];
            }
            return nullptr;
        }

        //// hit 表
        bool hit(Ray<T>& ray, glm::tvec3<T>& pos, glm::tvec3<T>& normal) const {
            if (m_aabb.hit(ray)) {
                for (size_t i = 0; i < m_node.size(); ++i) {

                }

                for (size_t i = 0; i < 9; ++i) {
                    if (nullptr != m_child[i]) {
                        m_child[i]->hit(ray, pos, normal);
                    }
                }
            }
        }

        /// 这个递归函数一定要写好
        void mountNode(T gate, M* obj) {
            AABB<T> objAABB = obj->getBVH()->toAABB();
            glm::tvec3<T> dd = m_aabb.max() - m_aabb.min();
            if (dd.x <= gate && dd.y <= gate && dd.z <= gate) {
                m_node.push_back(obj);
            }
            else {
                for (size_t i = 0; i < 8; ++i) {
                    AABB<T> subAABB = m_aabb.subAABB(i);
                    if (nullptr == m_child[i]) { ///子节点为空
                        if (subAABB.in(objAABB)) { /// 这个物体在这个AABB中
                            m_child[i] = new  OTree<M, T>(this, subAABB);
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
                AABB<T> centerAABB = m_aabb.centerAABB(T(_CENTER_AABB_CONST_FACTOR));
                if (centerAABB.in(objAABB)) {
                    if (nullptr == m_child[8]) {
                        m_child[8] = new NTree<M, T>(this, centerAABB);
                    }
                    m_child[8]->mountNode(gate, obj);
                    return;
                }
                else {
                    m_node.push_back(obj);
                    return;
                }

            }
        }
    private:
        AABB<T>     m_aabb;
        NTree<M, T>* m_parent;
        NTree<M, T>* m_child[9];
        std::vector<M*> m_node; ////
    };
}
