#include "gtest/gtest.h"
#include "core/graphics/Graphics.h"


class SutherlandHodgman_Test : public ::testing::Test {
public:
    virtual void SetUp() {
    }
    virtual void TearDown() {}

};

TEST_F(SutherlandHodgman_Test, test) {
    core::gpc::SutherlandHodgman sh(0.0f, 10.0f, 10.0f, 0.0f);
    {
        glm::vec2 p1(1.0f, 1.0f);
        glm::vec2 p2(9.0f, 1.0f);
        glm::vec2 p3(5.0f, 9.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(3, sh.ClipTriangle(out,3));
        //EXPECT_EQ(out[0], p1);
        //EXPECT_EQ(out[1], p2);
        //EXPECT_EQ(out[2], p3);
        for (size_t i = 0; i < 3; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;
    }

    {
        glm::vec2 p1(1.0f, 1.0f);
        glm::vec2 p2(9.0f, 1.0f);
        glm::vec2 p3(5.0f, 12.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(4, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 4; ++i) {
            std::cout << "["<< out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;
    }

    {
        glm::vec2 p1(-3.0f, 1.0f);
        glm::vec2 p2(9.0f, 2.0f);
        glm::vec2 p3(5.0f, 9.0f);
        glm::vec2 out[7];
        
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(4, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 4; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;

        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(4, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 4; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;

    }

    {
        glm::vec2 p1(1.0f, 1.0f);
        glm::vec2 p2(12.0f, 2.0f);
        glm::vec2 p3(5.0f, 9.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(4, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 4; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;
    }

    {   
        glm::vec2 p1(-1.0f, 1.0f);
        glm::vec2 p2(12.0f, 2.0f);
        glm::vec2 p3(5.0f, 11.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(6, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 6; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;
    
    }
    {
        glm::vec2 p1(-1.0f, 1.0f);
        glm::vec2 p2(12.0f, 1.0f);
        glm::vec2 p3(-1.0f, 11.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(5, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 5; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;
        
    }

    {
        glm::vec2 p1(-1.0f, 1.0f);
        glm::vec2 p2(20.0f, 11.0f);
        glm::vec2 p3(-1.0f, 11.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(4, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 5; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;

    }

    {
        glm::vec2 p1(-20.0f, -1.0f);
        glm::vec2 p2(30.0f, -1.0f);
        glm::vec2 p3(5.0f, 100.0f);
        glm::vec2 out[7];
        out[0] = p1;
        out[1] = p2;
        out[2] = p3;
        EXPECT_EQ(4, sh.ClipTriangle(out, 3));
        for (size_t i = 0; i < 4; ++i) {
            std::cout << "[" << out[i].x << "," << out[i].y << "]" << std::endl;
        }
        std::cout << "*******************************" << std::endl;
    
    }


}
