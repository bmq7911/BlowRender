#include "core/scene.h"

namespace gpc {
    scene::scene()
        : m_otree(nullptr)
    {
    }

    void scene::addObject(gpc::Object* object) {
        if (nullptr == m_otree) {
            m_otree = new OTree<Object>();
        }
        if (nullptr != object) {
            m_objects.insert(object);
            _UpdateSceneAABB(object);
        }
    }
    void scene::buildScene() {
        if (m_objects.empty()) {
            return;
        }
        m_aabb = (*begin())->getBVH()->toAABB();
        /// 尺度信息
        //for (size_t  i = 1; i < m_objects.size(); ++i) {
        //    m_aabb = m_aabb.unionAABB(m_objects.at(i)->getBVH()->toAABB());
        //}
        for (auto iter = begin(); iter != end(); ++iter) {
            m_aabb = m_aabb.unionAABB((*iter)->getBVH()->toAABB());
        }
        m_otree = new OTree<Object>(nullptr, m_aabb);

        for (auto iter = begin(); iter != end(); ++iter) {
            m_otree->mountNode(1.0, *iter);
        }
    }

    void scene::setLightDir(glm::fvec3 const& dir) {
        m_lightDir = glm::normalize(dir);
    }

    /// 这个算法并不合理,或者不该在这里
    /// 这个东西的并行性还是很好的
    glm::vec4 scene::CollectColor(gpc::Ray& ray) {
        glm::fvec3 normal;
        Float t = std::numeric_limits<Float>::max();

        /// 光线被照亮
        if (true == m_aabb.hit(ray)) {
            Object* obj = _HitObject(m_otree, ray, t, normal);
            if (nullptr != obj) {
                /// <summary>
                /// 这里做次级反射
                /// </summary>
                /// <param name="ray"></param>
                /// <returns></returns>
                gpc::Ray second_ray(ray.at(t), glm::reflect(ray.d(), normal));

                /// <summary>
                /// 这里就是简单的递归过程,但是这个过程会很复杂,如何控制递归的层次就很复杂了
                /// </summary>
                /// <param name="ray"></param>
                /// <returns></returns>
                glm::vec4 color = CollectColor(second_ray);

                /// <summary>
                ///  这里充当了着色过程
                /// </summary>
                /// <param name="ray"></param>
                /// <returns></returns>
                Float tmp = glm::dot(-ray.d(), normal);
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

    bool scene::hit(Ray const& ray) {
        return m_aabb.hit(ray);
    }

    scene::iterator scene::begin() {
        return m_objects.begin();
    }

    scene::iterator scene::end() {
        return m_objects.end();
    }

    scene::const_iterator scene::begin() const {
        return m_objects.begin();
    }

    scene::const_iterator scene::end() const {
        return m_objects.end();
    }

    const OTree<Object>* scene::getOTree() const {
        return m_otree;
    }
    /// 返回被光纤hit中的第一个物体
    Object* scene::hit(Ray const& ray, Float& t, glm::fvec3& normal) {
        if (nullptr != m_otree) {
            Object* obj;
            if (true == m_otree->hit(ray, t, normal, obj)) {
                return obj;
            }
            else {
                return nullptr;
            }
        }
        return nullptr;
    }
    bool isLight(Object* obj) {
        return false;
    }
    Object* scene::_HitObject(OTree<Object>* node,
        Ray const& ray, Float& t, glm::fvec3& normal) {
        Object* retObj = nullptr;
        if (nullptr != node && true == node->hit(ray)) {
            for (size_t i = 0; i < node->sizeNode(); ++i) {
                Object* obj = node->atNode(i);
                if (nullptr != obj) {
                    Float tmpT = Float(0);
                    glm::fvec3 tmpNormal;
                    if (true == obj->hit(ray, tmpT, tmpNormal)) {
                        if (tmpT < t) {
                            t = tmpT;
                            normal = tmpNormal;
                            retObj = obj;
                        }
                    }
                }
            }
            Object* subObj = nullptr;
            Float tmpT = t;
            glm::fvec3 tmpNormal = normal;
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

    void scene::_UpdateSceneAABB(Object* obj) {
        if (nullptr != obj) {
            BVH const* bvh = obj->getBVH();
            AABB aabb = bvh->toAABB();
            m_aabb = AABB::unionAABB(m_aabb, aabb);
        }
    }

}