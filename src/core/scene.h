#pragma once
#include "Graphics/AABB.h"
#include "helper/ModelLoad.h"
#include "Graphics/OTree.hpp"
#include "Graphics/KDTree.hpp"
#include "Model.h"

namespace gpc{
    /// this class is very important for render
    /// 
    template<typename T>
    class scene {
    public:
        scene() 
            : m_otree( nullptr)
        {
        }

        void addObject( gpc::Object<T>* objcet ) {
            if (nullptr == m_otree) {
                m_otree = new OTree<Object<T>,T>();
            }
            m_objects.push_back(objcet);
        }

        void buildScene() {
            if (m_objects.empty()) {
                return;
            }
            m_aabb = m_objects.at(0)->getBVH()->toAABB();
            /// 尺度信息
            for (size_t  i = 1; i < m_objects.size(); ++i) {
                m_aabb = m_aabb.unionAABB(m_objects.at(i)->getBVH()->toAABB());
            }
            m_otree = new OTree<Object<T>,T>(nullptr, m_aabb);

            for (size_t i = 0; i < m_objects.size(); ++i) {
                m_otree->mountNode( 1.0, m_objects[i]);
            }
        }
        
        void setLightDir( glm::tvec3<T> const& dir) {
            m_lightDir = glm::normalize(dir);
        }

        /// 这个东西的并行性还是很好的
        glm::vec4 CollectColor( gpc::Ray<T>& ray ) {
            glm::tvec3<T> normal;
            T t = std::numeric_limits<T>::max();
            /// 光线被照亮
            if (true == m_aabb.hit(ray)) {
                Object<T>*obj = _HitObject(m_otree, ray, t, normal);
                if (nullptr != obj) {
                    T tmp = glm::dot( - ray.d(), normal);
                    if (tmp > 0) {
                        float c = static_cast<float>(tmp);
                        return glm::vec4(tmp, tmp, tmp, 1.0f);
                    }
                    else {
                        return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
                    }
                }
            }
            return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            //// 这里就是背景颜色,这里就可能是一个立方体采样而已
        }

        bool hit(Ray<T>const & ray) {
            return m_aabb.hit(ray);
        }
        
        const OTree<Object<T>, T>* getOTree() const {
            return m_otree;
        }
    private:
        Object<T>* _HitObject( OTree<Object<T>,T>* node, 
                               Ray<T> const & ray , T& t ,glm::tvec3<T>& normal ) {
            Object<T> *retObj = nullptr;
            if (nullptr != node && true == node->hit(ray)) {
                    for (size_t i = 0; i < node->sizeNode(); ++i) {
                        Object<T>* obj = node->atNode(i);
                        if (nullptr != obj) {
                            T tmpT=T (0);
                            glm::tvec3<T> tmpNormal;
                            if (true == obj->hit(ray, tmpT, tmpNormal)) {
                                if (tmpT < t){
                                    t = tmpT;
                                    normal = tmpNormal;
                                    retObj = obj;
                                }
                            }
                        }
                    }
                    Object<T> * subObj = nullptr;
                    T tmpT = t;
                    glm::tvec3<T> tmpNormal = normal;
                    for (size_t i = 0; i < node->sizeChild(); ++i) {
                        subObj = _HitObject(node->atChild(i), ray, tmpT, tmpNormal);
                        if (nullptr != subObj && tmpT < t) {
                            t = tmpT;
                            normal = tmpNormal;
                            retObj = subObj;
                        }
                    }
                    return retObj;
            }
            else {
                return nullptr;
            }
        }

    private:
        glm::tvec3<T> m_lightDir;
	    AABB<T> m_aabb;
	    OTree<Object<T>,T>* m_otree;
        std::vector<Object<T>*> m_objects;
    };
}
