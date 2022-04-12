#pragma once
#include "Graphics/AABB.h"
#include "helper/ModelLoad.h"
#include "Graphics/OTree.hpp"
#include "Graphics/KDTree.hpp"
#include "Model.h"
#include "Graphics/Light.h"
#include <unordered_set>
#include "BFloat.h"

namespace gpc{
    /// this class is very important for render
    /// 
    class scene  {
    public:
        using iterator       = typename std::unordered_set<Object*>::iterator;
        using const_iterator = typename std::unordered_set<Object*>::const_iterator;

    public:
        scene();
        void addObject(gpc::Object* object);
        
        void buildScene( );
        void setLightDir(glm::fvec3 const& dir);
        /// ����㷨��������,���߲���������
        /// ��������Ĳ����Ի��Ǻܺõ�
        glm::vec4 CollectColor(gpc::Ray& ray);
        bool hit(Ray const& ray);
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        const OTree<Object>* getOTree() const;
        /// ���ر�����hit�еĵ�һ������
        Object* hit(Ray const& ray, Float& t, glm::fvec3& normal);
        bool isLight(Object* obj);
    private:
        Object* _HitObject(OTree<Object>* node, Ray const& ray, Float& t, glm::fvec3& normal);
        void _UpdateSceneAABB(Object* obj);
    private:
        glm::fvec3 m_lightDir;
	    AABB       m_aabb;
	    OTree<Object>* m_otree;
        std::unordered_set<Object*> m_objects;
        std::vector<Light*>  m_lights;
    };
}
