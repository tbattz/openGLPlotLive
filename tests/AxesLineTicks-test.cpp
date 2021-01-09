#include "gtest/gtest.h"
#include "../src/util/util.h"
#include "../src/lines/Line2D2Vecs.h"
#include "../src/window/window.h"




namespace GLPL {

    class AxesLineTickMultipleParamTests :public ::testing::TestWithParam<std::tuple<double, std::string, unsigned int, double, unsigned int>> {
    public:
        static void SetUpTestSuite() {
            // Window Size
            int windowWidth = 1600;
            int windowHeight = 800;

            // Init GLFW
            std::shared_ptr<IWindow> window = std::shared_ptr<IWindow>(
                    new GLPL::Window(windowWidth, windowHeight, false, false));
            std::shared_ptr<Window> window2 = std::dynamic_pointer_cast<Window>(window);

            // Get shader set
            std::shared_ptr<ShaderSet> shaderSetPt = window2->getShaderSet();

            // Create Empty Parent Dimensions
            ParentDimensions parentDimensions = ParentDimensions{glm::mat4(0.0f), 1, 1, 1, 1, shaderSetPt};
            std::shared_ptr<ParentDimensions> newParentPointers = std::make_shared<ParentDimensions>(parentDimensions);

            // Create Axes Line Ticks
            AxesDirection axesDirection = X_AXES_CENTRE;
            std::shared_ptr<AxesLineTicks> newAxes = std::make_shared<AxesLineTicks>(axesDirection, newParentPointers);
        }

        static std::shared_ptr<AxesLineTicks> newAxes;

    };

    std::shared_ptr<AxesLineTicks> AxesLineTickMultipleParamTests::newAxes = nullptr;


    TEST_P(AxesLineTickMultipleParamTests, value2NeatStrTests) {
        double inValue = std::get<0>(GetParam());
        const std::string& expectedStr = std::get<1>(GetParam());
        unsigned int maxCharSuggestion = std::get<2>(GetParam());
        double expSwapover = std::get<3>(GetParam());
        unsigned int minDecimal = std::get<4>(GetParam());

        std::string outString = newAxes->value2NeatStr(inValue, maxCharSuggestion, expSwapover, minDecimal);
        EXPECT_EQ(outString, expectedStr);
    }

    INSTANTIATE_TEST_CASE_P(
            AxesLineTicksTextTests,
            AxesLineTickMultipleParamTests,
            ::testing::Values(
                    std::make_tuple(1.00, "1.000", 4, 1000, 1),
                    std::make_tuple(10.00, "10.00", 4, 1000, 1),
                    std::make_tuple(100.0, "100.0", 4, 1000, 1),
                    std::make_tuple(220.5454, "220.5", 4, 1000, 1),
                    std::make_tuple(0.1, "0.10", 4, 1000, 1),
                    std::make_tuple(0.01, "0.01", 4, 1000, 1),
                    std::make_tuple(0.001, "1.0e-3", 4, 1000, 1),
                    std::make_tuple(1000.0, "1.0e+3", 4, 1000, 1),
                    std::make_tuple(0.0001, "1.0e-4", 4, 1000, 1),
                    std::make_tuple(1e11, "1.0e+11", 4, 1000, 1),
                    std::make_tuple(-1.00, "-1.0", 4, 1000, 1),
                    std::make_tuple(-10.00, "-10.0", 4, 1000, 1),
                    std::make_tuple(-100.0, "-100.0", 4, 1000, 1),
                    std::make_tuple(-0.1, "-0.10", 4, 1000, 1),
                    std::make_tuple(-0.01, "-0.01", 4, 1000, 1),
                    std::make_tuple(-0.001, "-1.0e-3", 4, 1000, 1),
                    std::make_tuple(-1000.0, "-1.0e+3", 4, 1000, 1),
                    std::make_tuple(-0.0001, "-1.0e-4", 4, 1000, 1),
                    std::make_tuple(-1e11, "-1.0e+11", 4, 1000, 1),
                    std::make_tuple(-2.17e-3, "-2.2e-3", 4, 1000, 1)
                    )
            );

}
