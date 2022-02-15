#include "gtest/gtest.h"
#include "core/graphics/Graphics.h"
#include "utils/utils.h"
#include <immintrin.h>

class Test_liang_barsky : public ::testing::Test {
public:
    virtual void SetUp() {
    }
    virtual void TearDown() {}

};

TEST_F(Test_liang_barsky, test) {
    core::gpc::Liang_Barsky bl(-1.0f, 1.0f, 1.0f, -1.0f);
    {

        glm::vec2 p0(-3.0f, 0.0f);
        glm::vec2 p1(0.0f, 3.0f);
        glm::vec2 op0;
        glm::vec2 op1;
        EXPECT_EQ(bl.cuteLine(p0, p1, op0, op1), false);
        EXPECT_EQ(bl.cuteLine(p1, p0, op0, op1), false);
    }

    {

        glm::vec2 p0(-2.0f, -1.0f);
        glm::vec2 p1(1.0f, 2.0f);
        glm::vec2 op0;
        glm::vec2 op1;
        EXPECT_EQ(bl.cuteLine(p0, p1, op0, op1), true);
        EXPECT_EQ(op0.x, -1.0f);
        EXPECT_EQ(op0.y, 0.0f);
        
        EXPECT_EQ(op1.x, 0.0f);
        EXPECT_EQ(op1.y, 1.0f);
        
        EXPECT_EQ(bl.cuteLine(p1, p0, op0, op1), true);
    }

    {
        
    }

    {

        glm::vec2 p0(-2.0f, -1.0f);
        glm::vec2 p1(1.0f, 1.0f);
        glm::vec2 op0;
        glm::vec2 op1;
        EXPECT_EQ(bl.cuteLine(p0, p1, op0, op1), true);
        EXPECT_EQ(op0.x, -1.0f);
        EXPECT_EQ(op0.y, -0.3333333f);
        EXPECT_EQ(op1.x, 1.0f);
        EXPECT_EQ(op1.y, 1.0f);
        EXPECT_EQ(bl.cuteLine(p1, p0, op0, op1), true);
    }

}

int main(int argc, char* argv[]) {
    
    float op1[4] = {1.0f,2.0f,3.0f,4.0f};
    float op2[4] = { 4.0f,3.0f,2.0f,1.0f };
    
    /*
    float result[4];
    

    __m128 t1;
    __m128 t2;
    __m128 t3;
    t1 = _mm_loadu_ps(op1);
    t2 = _mm_loadu_ps(op2);
    t3 = _mm_add_ps(t1, t2);

    _mm_storeu_ps( result,t3 );
    
    float a = 10.0f;
    float b = 11.0f;
    float mul = a * b;
    */


    op1[0] = op1[0] + op2[0];
    op1[1] = op1[1] + op2[1];
    op1[2] = op1[2] + op2[2];
    op1[3] = op1[3] + op2[3];
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

}