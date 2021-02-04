#include "gtest/gtest.h"
#include "../src/plot/plot.h"
#include "../src/window/window.h"


namespace GLPL {

    class PlotLayoutParamTests
            : public ::testing::TestWithParam<std::tuple<unsigned int, unsigned int, unsigned int, std::vector<std::vector<float>>>> {
    public:
        static void SetUpTestSuite() {
            // Window Size
            int windowWidth = 1600;
            int windowHeight = 800;

            // Init GLFW
            std::shared_ptr<IWindow> window = std::shared_ptr<IWindow>(
                    new GLPL::Window(windowWidth, windowHeight, false, false));
            window2 = std::dynamic_pointer_cast<Window>(window);

            // Get shader set
            std::shared_ptr<ShaderSet> shaderSetPt = window2->getShaderSet();

        }

        static std::shared_ptr<Window>  window2;

    };

    std::shared_ptr<Window> PlotLayoutParamTests::window2 = nullptr;


    TEST_P(PlotLayoutParamTests, testPlotLayouts) {
        unsigned int numHorizontal = std::get<0>(GetParam());
        unsigned int numVertical = std::get<1>(GetParam());
        unsigned int axesCount = std::get<2>(GetParam());
        const std::vector<std::vector<float>>& expectedPositions = std::get<3>(GetParam());

        // Create Plot
        std::shared_ptr<Plot> myPlotPt = std::make_shared<GLPL::Plot>(0.0, 0.0, 1.0, 1.0,
                                                window2->getParentDimensions(), numHorizontal, numVertical);
        window2->addPlot(myPlotPt);

        // Add New Axes
        for (unsigned int i = 1; i < axesCount; i++) {
            std::shared_ptr<GLPL::Axes2D> axesPt = myPlotPt->add2DAxes();
        }

        // Get each axes and check position
        for (unsigned int i = 0; i < axesCount; i++) {
            std::vector<float> axesAreaInfo = myPlotPt->getAxes(i)->getAxesInfo();
            // Check values are expected
            for (unsigned int j = 0; j < 4; j++) {
                ASSERT_NEAR(axesAreaInfo[j], expectedPositions[i][j], 1e-2);
            }
        }

    }

    INSTANTIATE_TEST_CASE_P(
            PlotLayoutTests,
            PlotLayoutParamTests,
            ::testing::Values(
                    // numHor, numVert, axesCount <<x, y, width, height>, ...>
                    // origin is the bottom left corner
                    std::make_tuple(1, 1, 1, std::vector<std::vector<float>>{{0.0, 0.0, 1.0, 1.0}}),
                    std::make_tuple(2, 1, 1, std::vector<std::vector<float>>{{0.0, 0.0, 0.5, 1.0}}),
                    std::make_tuple(2, 1, 2, std::vector<std::vector<float>>{{0.0, 0.0, 0.5, 1.0},
                                                                             {0.5, 0.0, 0.5, 1.0}}),
                    std::make_tuple(2, 2, 1, std::vector<std::vector<float>>{{0.0, 0.5, 0.5, 0.5}}),
                    std::make_tuple(2, 2, 2, std::vector<std::vector<float>>{{0.0, 0.5, 0.5, 0.5},
                                                                             {0.5, 0.5, 0.5, 0.5}}),
                    std::make_tuple(2, 2, 3, std::vector<std::vector<float>>{{0.0, 0.5, 0.5, 0.5},
                                                                             {0.5, 0.5, 0.5, 0.5},
                                                                             {0.0, 0.0, 0.5, 0.5}}),
                    std::make_tuple(2, 2, 4, std::vector<std::vector<float>>{{0.0, 0.5, 0.5, 0.5},
                                                                             {0.5, 0.5, 0.5, 0.5},
                                                                             {0.0, 0.0, 0.5, 0.5},
                                                                             {0.5, 0.0, 0.5, 0.5}}),
                    std::make_tuple(3, 2, 1, std::vector<std::vector<float>>{{0.0, 0.5, 0.3333, 0.5}}),
                    std::make_tuple(3, 2, 2, std::vector<std::vector<float>>{{0.0, 0.5, 0.3333, 0.5},
                                                                             {0.3333, 0.5, 0.3333, 0.5}}),
                    std::make_tuple(3, 2, 3, std::vector<std::vector<float>>{{0.0, 0.5, 0.3333, 0.5},
                                                                             {0.3333, 0.5, 0.3333, 0.5},
                                                                             {0.6666, 0.5, 0.3333, 0.5}}),
                    std::make_tuple(3, 2, 4, std::vector<std::vector<float>>{{0.0, 0.5, 0.3333, 0.5},
                                                                             {0.3333, 0.5, 0.3333, 0.5},
                                                                             {0.6666, 0.5, 0.3333, 0.5},
                                                                             {0.0, 0.0, 0.3333, 0.5}}),
                    std::make_tuple(3, 2, 5, std::vector<std::vector<float>>{{0.0, 0.5, 0.3333, 0.5},
                                                                             {0.3333, 0.5, 0.3333, 0.5},
                                                                             {0.6666, 0.5, 0.3333, 0.5},
                                                                             {0.0, 0.0, 0.3333, 0.5},
                                                                             {0.3333, 0.0, 0.3333, 0.5}}),
                    std::make_tuple(3, 2, 6, std::vector<std::vector<float>>{{0.0, 0.5, 0.3333, 0.5},
                                                                             {0.3333, 0.5, 0.3333, 0.5},
                                                                             {0.6666, 0.5, 0.3333, 0.5},
                                                                             {0.0, 0.0, 0.3333, 0.5},
                                                                             {0.3333, 0.0, 0.3333, 0.5},
                                                                             {0.6666, 0.0, 0.3333, 0.5}})
                    )

    );


}