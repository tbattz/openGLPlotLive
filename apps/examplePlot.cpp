/*
 * examplePlot.cpp
 *
 *  Created on: 1Jan.,2017
 *      Author: bcub3d-desktop
 *
 *  Based on tutorials from http://www.learnopengl.com/#!Introduction
 */



// Standard Includes
#include <memory>

// Project Includes
#include "../src/window/window.h"
#include "../src/lines/Line2DVecGLMV3.h"


int main(int argc, char **argv) {
	/* ======================================================
	 *                     Setup Window
	   ====================================================== */
    // Window Size
    int windowWidth  = 1600;
    int windowHeight = 800;

	// Init GLFW
	std::shared_ptr<GLPL::IWindow> window = std::shared_ptr<GLPL::IWindow>(new GLPL::Window(windowWidth, windowHeight,  false, false));
	std::shared_ptr<GLPL::Window> window2 = std::dynamic_pointer_cast<GLPL::Window>(window);

	/* ======================================================
	 *                	  Create Plot Data
   	   ====================================================== */
    // Graph 9
    std::vector<float> xVec9;
    std::vector<float> yVec9;
    xVec9.reserve(2500);
    yVec9.reserve(2500);
    for(int i=-1000; i<1500; i++) {
        xVec9.push_back(i/500.0);
        yVec9.push_back(0.75*sin(i/500.0));
    }

    // Graph 11
    std::vector<float> xVec11;
    std::vector<float> yVec11;
    xVec11.reserve(2000);
    yVec11.reserve(2000);
    for(int i=-2000; i<1501; i++) {
        xVec11.push_back(i/1000.0);
        yVec11.push_back(i/1000.0);
    }

    // Graph 11b
    std::vector<float> xVec11b;
    std::vector<float> yVec11b;
    xVec11b.reserve(2000);
    yVec11b.reserve(2000);
    for(int i=-2000; i<1501; i++) {
        xVec11b.push_back(i/1000.0);
        yVec11b.push_back(sin(i/1000.0));
    }

    // Graph 12 - Damping
    std::vector<float> xVec12 = {};
    std::vector<float> yVec12 = {};
    xVec12.reserve(2000);
    yVec12.reserve(2000);

    // Graph 13 - Damping 2
    std::vector<float> xVec13 = {};
    std::vector<float> yVec13 = {};
    xVec13.reserve(2000);
    yVec13.reserve(2000);

    // Graph 14 - Random Scatter Plot
    std::vector<float> xVec14;
    std::vector<float> yVec14;
    unsigned int n = 20;
    int LO = -2;
    int HI = 2;
    xVec14.reserve(n);
    yVec14.reserve(n);
    for(unsigned int i=0; i<n; i++) {
        float rX = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        float rY = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        xVec14.push_back(rX);
        yVec14.push_back(rY);
    }

    // Graph 15 - Damping Exponential
    std::vector<float> xVec15 = {};
    std::vector<float> yVec15 = {};
    xVec13.reserve(2000);
    yVec13.reserve(2000);

    // Graph 16 - Damping Exponential Mirror
    std::vector<float> xVec16 = {};
    std::vector<float> yVec16 = {};
    xVec13.reserve(2000);
    yVec13.reserve(2000);

    // Graph 17 - Log Scale
    std::vector<float> xVec17 = {};
    std::vector<float> yVec17 = {};
    xVec17.reserve(2000);
    yVec17.reserve(2000);
    for(unsigned int i=100; i<=100; i++) {
        xVec17.push_back(10*(i+1)/1000.0);
        yVec17.push_back(std::log10(500*(i+1)/1000.0));
    }
    for (unsigned int i=0; i<=1900; i++) {
        // Update Graph 17
        xVec17.push_back(5*(i+1));
        yVec17.push_back(std::log10(5*(i+1)));
    }


    /* ======================================================
     *                	    Create Plot
       ====================================================== */
	// Create Plot
	std::shared_ptr<GLPL::Plot> myplot = std::make_shared<GLPL::Plot>(0.0, 0.0, 1.0, 1.0, window2->getParentDimensions(), 2, 2);
	std::shared_ptr<GLPL::IDrawable> myPlotPt = std::dynamic_pointer_cast<GLPL::IDrawable>(myplot);
	window2->addPlot(myPlotPt);


	// Axes 1 - Spring Damping Live Plot
	// Create axes
    std::shared_ptr<GLPL::Axes2D> axesPt = std::dynamic_pointer_cast<GLPL::Axes2D>(myplot->getAxes(0));
    // Add Lines
    std::shared_ptr<GLPL::ILine2D> line12 = axesPt->addLine(&xVec12, &yVec12, GLPL::SINGLE_LINE, LC_YELLOW, 0.5);
    std::shared_ptr<GLPL::ILine2D> line13 = axesPt->addLine(&xVec13, &yVec13, GLPL::SINGLE_LINE, LC_CYAN, 0.5);
    std::shared_ptr<GLPL::ILine2D> line15 = axesPt->addLine(&xVec15, &yVec15, GLPL::SINGLE_LINE, LC_BLUE, 0.5);
    std::shared_ptr<GLPL::ILine2D> line16 = axesPt->addLine(&xVec16, &yVec16, GLPL::SINGLE_LINE, LC_BLUE, 0.5);
    // Get pointer to add data to internal store
    std::shared_ptr<GLPL::Line2D2Vecs> line12b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line12);
    std::shared_ptr<GLPL::Line2D2Vecs> line13b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line13);
    std::shared_ptr<GLPL::Line2D2Vecs> line15b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line15);
    std::shared_ptr<GLPL::Line2D2Vecs> line16b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line16);
    // Set axes configuration
    axesPt->setAxesBoxOn(false);
    axesPt->setButtonState("Grid", false);
    axesPt->setXLabel("Time (s)");
    axesPt->setYLabel("Displacement (m)");
    axesPt->setTitle("Spring Damping Over Time");
    axesPt->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);


    // Axes 2 - Simple Scatter, Line and Rectangle Plot
    // Create axes
    std::shared_ptr<GLPL::Axes2D> axes2Pt = myplot->add2DAxes();
    // Add Lines
    std::shared_ptr<GLPL::ILine2D> line9 = axes2Pt->addLine(&xVec9, &yVec9, GLPL::SINGLE_LINE, LC_RED, 0.5, "Sine Wave");
    std::shared_ptr<GLPL::ILine2D> line11 = axes2Pt->addLine(&xVec11, &yVec11, GLPL::SHADED_LINE, LC_GREEN, 0.5, "Shaded Triangle");
    // Get pointer to add data to internal store
    std::shared_ptr<GLPL::IScatterPlot> line9s = axes2Pt->addScatterPlot(&xVec14, &yVec14, LC_CYAN, 0.5, GLPL::MARKER_CIRCLE, "Markers");
    // Set line configuration
    line9s->setMarkerColour(glm::vec3(1.0f, 0.5f, 0.5f));
    line9s->setOpacityRatio(0.6);
    line9s->setMarkerOutlineColour(glm::vec3(0.0f, 1.0f, 1.0f));
    line9s->setOutlineOpacityRatio(0.8);
    line9s->setMarkerType(GLPL::MARKER_CIRCLE);
    line9s->setMarkerSizePx(15);
    // Set axes configuration
    axes2Pt->setTitle("Simple Plots");
    axes2Pt->setXLabel("x");
    axes2Pt->setYLabel("y");
    axes2Pt->showLegend(true);


    // Axes 3 - Log Plot
    // Create axes
    std::shared_ptr<GLPL::Axes2D> axes3Pt = myplot->add2DAxes();
    axes3Pt->setLogScale(true, 10, GLPL::X_AXES);
    // Add lines
    std::shared_ptr<GLPL::ILine2D> line17 = axes3Pt->addLine(&xVec17, &yVec17, GLPL::SINGLE_LINE, LC_CYAN, 0.5);
    std::shared_ptr<GLPL::ILine2D> line11b = axes3Pt->addLine(&xVec11, &yVec11, GLPL::SHADED_LINE, LC_GREEN, 0.5);
    // Add Scatter Plot
    std::shared_ptr<GLPL::IScatterPlot> line9bs = axes3Pt->addScatterPlot(&xVec14, &yVec14, LC_CYAN, 0.5);
    // Get pointer to data to internal store
    std::shared_ptr<GLPL::Line2D2Vecs> line17b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line17);
    // Set axes configuration
    axes3Pt->setXLabel("Log(x)");
    axes3Pt->setYLabel("y");
    axes3Pt->setTitle("Example Log Plot");
    axes3Pt->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);


    // Axes 4 - Test Plot
    // Create Axes
    std::shared_ptr<GLPL::Axes2D> axes4Pt = myplot->add2DAxes();
    // Add lines
    std::shared_ptr<GLPL::ILine2D> line30 = axes4Pt->addLine(&xVec11, &yVec11, GLPL::SHADED_LINE, LC_GREEN, 0.25);
    std::shared_ptr<GLPL::ILine2D> line30b = axes4Pt->addLine(&xVec11b, &yVec11b, GLPL::SINGLE_LINE, LC_RED, 0.5);
    // Set axes configuration
    axes4Pt->setXLabel("x");
    axes4Pt->setYLabel("y");
    axes4Pt->setTitle("Test Plot");
    axes4Pt->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);
    axes4Pt->setButtonState("Axes Limits Scaling", false);
    axes4Pt->setButtonState("Grid", false);
    axes4Pt->setAxesBoxOn(false);
    axes4Pt->setAxesLimits(-2.5, 2.5, -2.0, 1.5);


    // Create new values
    float yVal12 = 0;
    float yVal13 = 0;
    float yVal15 = 0;
    float yVal16 = 0;


	/* ======================================================
	 *                     Drawing Loop
	   ====================================================== */
	float i = 0;

	// Game Loop
	while(!glfwWindowShouldClose(window->getWindow())) {

		// Pre-loop draw
		window2->preLoopDraw(true);

		// Update Plot Data
        // Update graph 12
        line12b->dataPtX->push_back(i);
        yVal12 = cos((i) / (25*M_PI)) * exp(-(i)/(25*8*M_PI));
        line12b->dataPtY->push_back(yVal12);
        line12b->updateInternalData();

        // Update graph 13
        line13b-> dataPtX->push_back(i);
        yVal13 = cos((i) / (2*25*M_PI)) * exp(-(i)/(25*8*0.5*M_PI));
        line13b->dataPtY->push_back(yVal13);
        line13b->updateInternalData();

        // Update Graph 15
        line15b->dataPtX->push_back(i);
        yVal15 = exp(-(i)/(25*8*M_PI));
        line15b->dataPtY->push_back(yVal15);
        line15b->updateInternalData();

        // Update Graph 16
        line16b->dataPtX->push_back(i);
        yVal16 = -exp(-(i)/(25*8*M_PI));
        line16b->dataPtY->push_back(yVal16);
        line16b->updateInternalData();

        i += 1;

		// Draw Plot
		myplot->Draw();



		// For Debugging
		//myplot->drawBoundingBox();
		//myplot->drawMouseOverBox();
        //std::shared_ptr<GLPL::Axes> axesPt = myplot->getAxes(0);
        //axesPt->setPosition(axesPt->getLeft() + 0.001, axesPt->getBottom() - 0.001);


		// Post-loop draw
		window2->postLoopDraw();

	}

	// Close Window
	glfwTerminate();


	return 0;
}





