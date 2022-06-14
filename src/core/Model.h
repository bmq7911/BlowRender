#pragma once
#include "Graphics/Object.h"
#include "Graphics/AABB.h"
#include "helper/ModelLoad.h"

class Model : public gpc::Object{
public:
    static Model* createModel(helper::Model* model ) {
        if (nullptr == model) {
            return nullptr;
        }
        Model* m = new Model();
        m->m_aabb = model->getAABB();
        m->m_ptrModel = model;
        return m;
    }
    helper::Model* getModel() const {
        return m_ptrModel;
    }
    void tick(float passTime, float deltaTime) {
        
    }
    
    bool hit(gpc::Ray const & ray, Float& t, glm::fvec3 &normal) const override {
        return false;
    }


    gpc::BVH const* getBVH() const override {
        return &m_aabb;
    }
private:
    Model() {}
private:
    gpc::AABB m_aabb;
    helper::Model* m_ptrModel;
};