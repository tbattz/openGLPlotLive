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
#include <algorithm>

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


    /* ======================================================
     *                	    Create Plot
       ====================================================== */
	// Create Plot
	std::shared_ptr<GLPL::Plot> myplot = std::make_shared<GLPL::Plot>(0.0, 0.0, 1.0, 1.0, window2->getParentDimensions(), 2, 1);
	std::shared_ptr<GLPL::IDrawable> myPlotPt = std::dynamic_pointer_cast<GLPL::IDrawable>(myplot);
	window2->addPlot(myPlotPt);


	// Axes 1 - Spring Damping Live Plot - Autoscale over entire y range
	// Create axes
    std::shared_ptr<GLPL::Axes2D> axesPt = std::dynamic_pointer_cast<GLPL::Axes2D>(myplot->getAxes(0));
    // Add Lines
    std::shared_ptr<GLPL::ILine2D> line12 = axesPt->addLine(&xVec12, &yVec12, GLPL::SINGLE_LINE, LC_YELLOW, 0.5, "Underdamped 1");
    std::shared_ptr<GLPL::ILine2D> line13 = axesPt->addLine(&xVec13, &yVec13, GLPL::SINGLE_LINE, LC_CYAN, 0.5, "Underdamped 2");
    std::shared_ptr<GLPL::ILine2D> line15 = axesPt->addLine(&xVec15, &yVec15, GLPL::SINGLE_LINE, LC_BLUE, 0.5, "Upper bounds");
    std::shared_ptr<GLPL::ILine2D> line16 = axesPt->addLine(&xVec16, &yVec16, GLPL::SINGLE_LINE, LC_BLUE, 0.5, "Lower Bounds");
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
    axesPt->setButtonState("X Axes Limits Scaling", false);
    axesPt->setButtonState("Y Axes Limits Scaling", true);
    axesPt->showLegend(true);

    // Axes 2 - Spring Damping Live Plot - Manual scale over visible y range
    // Create axes
    std::shared_ptr<GLPL::Axes2D> axes2Pt = myplot->add2DAxes();
    // Add Lines
    std::shared_ptr<GLPL::ILine2D> line22 = axes2Pt->addLine(&xVec12, &yVec12, GLPL::SINGLE_LINE, LC_YELLOW, 0.5, "Underdamped 1");
    std::shared_ptr<GLPL::ILine2D> line23 = axes2Pt->addLine(&xVec13, &yVec13, GLPL::SINGLE_LINE, LC_CYAN, 0.5, "Underdamped 2");
    std::shared_ptr<GLPL::ILine2D> line25 = axes2Pt->addLine(&xVec15, &yVec15, GLPL::SINGLE_LINE, LC_BLUE, 0.5, "Upper bounds");
    std::shared_ptr<GLPL::ILine2D> line26 = axes2Pt->addLine(&xVec16, &yVec16, GLPL::SINGLE_LINE, LC_BLUE, 0.5, "Lower Bounds");
    // Get pointer to add data to internal store
    std::shared_ptr<GLPL::Line2D2Vecs> line22b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line22);
    std::shared_ptr<GLPL::Line2D2Vecs> line23b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line23);
    std::shared_ptr<GLPL::Line2D2Vecs> line25b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line25);
    std::shared_ptr<GLPL::Line2D2Vecs> line26b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line26);
    // Set axes configuration
    axes2Pt->setAxesBoxOn(false);
    axes2Pt->setButtonState("Grid", false);
    axes2Pt->setXLabel("Time (s)");
    axes2Pt->setYLabel("Displacement (m)");
    axes2Pt->setTitle("Spring Damping Over Time");
    axes2Pt->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);
    axes2Pt->setButtonState("X Axes Limits Scaling", false);
    axes2Pt->setButtonState("Y Axes Limits Scaling", false);
    axes2Pt->showLegend(true);



    // Create new values
    float yVal12 = 0;
    float yVal13 = 0;
    float yVal15 = 0;
    float yVal16 = 0;


    std::vector<std::shared_ptr<GLPL::Line2D2Vecs>> lines = {line12b, line13b, line15b, line16b};


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


        // Update Plot Data
        // Update graph 22
        line22b->updateInternalData();

        // Update graph 23
        line23b->updateInternalData();

        // Update Graph 25
        line25b->updateInternalData();

        // Update Graph 26
        line26b->updateInternalData();



        i += 1;

        // Get range of last 1000 points, provided the points in the a axis are in order
        unsigned int minInd = std::max((long)0, (long)line12b->dataPtX->size() - 1000);
        unsigned int maxInd = line12b->dataPtX->size()-1;
        float xmin = (*line12b->dataPtX)[minInd];
        float xmax = (*line12b->dataPtX)[maxInd];
        axesPt->setXAxesLimits(xmin, xmax);
        axes2Pt->setXAxesLimits(xmin, xmax);


        // Handle y axes for second plot
        float ymin = 0;
        float ymax = 0;
        for (auto line : lines) {
            for (unsigned int k = minInd; k < maxInd + 1; k++) {
                float yval = (*line->dataPtY)[k];
                if (yval < ymin) {
                    ymin = yval;
                } else if (yval > ymax) {
                    ymax = yval;
                }
            }
        }
        axes2Pt->setYAxesLimits(ymin, ymax);



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





