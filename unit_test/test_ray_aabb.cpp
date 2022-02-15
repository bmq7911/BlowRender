#include "Ray.h"
#include "AABB.h"
#include "gtest/gtest.h"

class AABB_Ray_test : public ::testing::Test {
public:
    void SetUp() override {
    }
    void TearDown() override {
    
    }
};

TEST_F( AABB_Ray_test, test_ray_hit) {
    Ray<float> ray(glm::tvec3<float>(0.0f,0.0f,0.0f), glm::tvec3<float>( 0.0f,0.0f,1.0f) );
    




}

TEST_F(AABB_Ray_test, test_AABB_subAABB) {
    AABB<double> aabb(glm::tvec3<double>(-4.0, -4.0, -4.0), glm::tvec3<double>(4.0, 4.0, 4.0));
    for (size_t i = 0; i < 8; ++i) {
        AABB<double> tmp = aabb.subAABB(i);
        std::cout << " [ " << tmp.min().x << ",";
        std::cout << tmp.min().y << ",";
        std::cout << tmp.min().z << "][";
        std::cout << tmp.max().x << ",";
        std::cout << tmp.max().y << ",";
        std::cout << tmp.max().z << "]"<<std::endl;
    }

}