#include "gtest/gtest.h"
#include "../src/util/util.h"
#include "../src/lines/Line2D2Vecs.h"
#include "../src/window/window.h"

// Open up private classes for testing
#define private public
#define protected public
#define class struct

namespace GLPL {

    TEST(Line2D2VecsTests, SortingWithSameXValuesTest) {
        // Create initial data
        std::vector<float> xVec;
        std::vector<float> yVec;
        // Square TL, TR, BR, BL, TL
        float xL = 1.0f;
        float xR = 2.0f;
        float yB = 3.0f;
        float yT = 4.0f;
        // Top Left
        xVec.push_back(xL);
        yVec.push_back(yT);
        // Top Right
        xVec.push_back(xR);
        yVec.push_back(yT);
        // Bottom Right
        xVec.push_back(xR);
        yVec.push_back(yB);
        // Bottom Left
        xVec.push_back(xL);
        yVec.push_back(yB);
        // Top Left
        xVec.push_back(xL);
        yVec.push_back(yT);

        // Window Size
        int windowWidth  = 1600;
        int windowHeight = 800;

        // Init GLFW
        std::shared_ptr<IWindow> window = std::shared_ptr<IWindow>(new GLPL::Window(windowWidth, windowHeight,  false, false));
        std::shared_ptr<Window> window2 = std::dynamic_pointer_cast<Window>(window);

        // Get shader set
        std::shared_ptr<ShaderSet> shaderSetPt = window2->getShaderSet();

        // Create Empty Parent Dimensions
        ParentDimensions parentDimensions = ParentDimensions{glm::mat4(0.0f), 1, 1, 1, 1, shaderSetPt};
        std::shared_ptr<ParentDimensions> newParentPointers = std::make_shared<ParentDimensions>(parentDimensions);

        // Create Line Object
        std::shared_ptr<Line2D2Vecs> lineObj = std::make_shared<Line2D2Vecs>(&xVec, &yVec, newParentPointers);

        // Check that the internal data and indices are sorted correctly
        std::vector<unsigned int> actualIndices = lineObj->getInternalIndices();
        std::vector<float> actualData = lineObj->getInternalData();
        for(unsigned int i=0; i<actualIndices.size(); i++) {
            int currInd = actualIndices[i];
            float x = actualData[2*currInd];
            float y = actualData[2*currInd + 1];
            EXPECT_EQ(x, xVec[i]);
            EXPECT_EQ(y, yVec[i]);
        }


    }

    TEST(Line2D2VecsTests, SortingWithSameXValuesTest2) {
        // Create initial data
        std::vector<float> xVec;
        std::vector<float> yVec;
        // Square TR, TL, BL, BR, TR
        float xL = 1.0f;
        float xR = 2.0f;
        float yB = 3.0f;
        float yT = 4.0f;
        // Top Right
        xVec.push_back(xR);
        yVec.push_back(yT);
        // Top Left
        xVec.push_back(xL);
        yVec.push_back(yT);
        // Bottom Left
        xVec.push_back(xL);
        yVec.push_back(yB);
        // Bottom Right
        xVec.push_back(xR);
        yVec.push_back(yB);
        // Top Right
        xVec.push_back(xR);
        yVec.push_back(yT);

        // Window Size
        int windowWidth  = 1600;
        int windowHeight = 800;

        // Init GLFW
        std::shared_ptr<IWindow> window = std::shared_ptr<IWindow>(new GLPL::Window(windowWidth, windowHeight,  false, false));
        std::shared_ptr<Window> window2 = std::dynamic_pointer_cast<Window>(window);

        // Get shader set
        std::shared_ptr<ShaderSet> shaderSetPt = window2->getShaderSet();

        // Create Empty Parent Dimensions
        ParentDimensions parentDimensions = ParentDimensions{glm::mat4(0.0f), 1, 1, 1, 1, shaderSetPt};
        std::shared_ptr<ParentDimensions> newParentPointers = std::make_shared<ParentDimensions>(parentDimensions);

        // Create Line Object
        std::shared_ptr<Line2D2Vecs> lineObj = std::make_shared<Line2D2Vecs>(&xVec, &yVec, newParentPointers);

        // Check that the internal data and indices are sorted correctly
        std::vector<unsigned int> actualIndices = lineObj->getInternalIndices();
        std::vector<float> actualData = lineObj->getInternalData();
        for(unsigned int i=0; i<actualIndices.size(); i++) {
            int currInd = actualIndices[i];
            float x = actualData[2*currInd];
            float y = actualData[2*currInd + 1];
            EXPECT_EQ(x, xVec[i]);
            EXPECT_EQ(y, yVec[i]);
        }

    }

};
