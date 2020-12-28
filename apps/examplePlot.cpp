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
#define _USE_MATH_DEFINES
#include <cmath>

// Project Includes
#include "../src/rendering/fonts.h"
#include "../src/window/window.h"
#include "../src/lines/Line2DVecVec.h"
#include "../src/lines/Line2DVecGLMV3.h"
#include "../src/lines/Line2D2Vecs.h"


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
	// Create Data
	// Graph 1
	std::vector<GLPL::pt2D> data1;
	data1.reserve(2000);
	for(int i = 0; i < 2000; i++) {
        GLPL::pt2D pt;
		float x = i/1000.0;
		pt.x = x;
		pt.y = x*x;
		data1.push_back(pt);
	}
	// Graph 2
    std::vector<GLPL::pt2D> data2;
	for(int i=-1000; i<1000; i++) {
        GLPL::pt2D pt2;
		float x = i/1000.0;
		pt2.x = x;
		pt2.y = -x-1;
		data2.push_back(pt2);
	}
	// Graph 3
    std::vector<GLPL::pt2D> data3;
	//float i = -2000.0;

	// Graph 4 - Vector (Line uses template types)
    std::vector<float> data4;
    data4.reserve(2*2500);
	for(int i=-1000; i<1500; i++) {
		data4.push_back(i/1000.0);
		data4.push_back(-i/1000.0);
 	}

	// Graph 5 - Vector of Vectors
    std::vector<std::vector<float>> data5;

	// Graph 6 - Vector of glm::dvec3
    std::vector<glm::dvec3> data6;

	// Graph 7 - Vector of floats - time
    std::vector<float> data7;

    // Graph 8 - Vector (Line uses template types)
    std::vector<double> data8;
    data8.reserve(2*2500);
    for(int i=-1000; i<1500; i++) {
        data8.push_back(-pow(i/1000.0,2));
        data8.push_back(i/1000.0);
    }

    // Graph 9 - X and Y vectors
    std::vector<float> xVec9;
    std::vector<float> yVec9;
    xVec9.reserve(2500);
    yVec9.reserve(2500);
    for(int i=-1000; i<1500; i++) {
        xVec9.push_back(i/500.0);
        yVec9.push_back(0.75*sin(i/500.0));
    }

    // Graph 10 - X and Y circular vectors
    std::vector<float> xVec10;
    std::vector<float> yVec10;
    //int j = 0;
    xVec10.reserve(2500);
    yVec10.reserve(2500);
    for(int i=-1000; i<1500; i++) {
        xVec10.push_back(i/500.0);
        yVec10.push_back(5 - 0.25*pow(i/500.0, 2));
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


    /* ======================================================
     *                	    Create Plot
       ====================================================== */
	// Create Plot
	std::shared_ptr<GLPL::Plot> myplot = std::make_shared<GLPL::Plot>(0.0, 0.0, 1.0, 1.0, window2->getParentDimensions());
	std::shared_ptr<GLPL::IDrawable> myPlotPt = std::dynamic_pointer_cast<GLPL::IDrawable>(myplot);
	window2->addPlot(myPlotPt);
	/*GLPL::Plot myplot(0.0, 0.25, 0.75, 0.75, window);

	// Create Lines
	std::shared_ptr<GLPL::Line2DPts> line1 = std::shared_ptr<GLPL::Line2DPts>(new GLPL::Line2DPts(&data1));
    std::shared_ptr<GLPL::Line2DPts> line2 = std::shared_ptr<GLPL::Line2DPts>(new GLPL::Line2DPts(&data2));
    std::shared_ptr<GLPL::Line2DPts> line3 = std::shared_ptr<GLPL::Line2DPts>(new GLPL::Line2DPts(&data3));
    std::shared_ptr<GLPL::Line2DVec<float>> line4 = std::shared_ptr<GLPL::Line2DVec<float>>(new GLPL::Line2DVec<float>(&data4));
    std::shared_ptr<GLPL::Line2DVecVec> line5 = std::shared_ptr<GLPL::Line2DVecVec>(new GLPL::Line2DVecVec(&data5));
    std::shared_ptr<GLPL::Line2DVecGLMV3> line6 = std::shared_ptr<GLPL::Line2DVecGLMV3>(new GLPL::Line2DVecGLMV3(&data6, 2, 1));
    std::shared_ptr<GLPL::Line2DVecfVecGLMV3> line7 = std::shared_ptr<GLPL::Line2DVecfVecGLMV3>(new GLPL::Line2DVecfVecGLMV3(&data7, &data6, 0));
    std::shared_ptr<GLPL::Line2DVec<double>> line8 = std::shared_ptr<GLPL::Line2DVec<double>>(new GLPL::Line2DVec<double>(&data8));
    std::shared_ptr<GLPL::Line2D2Vecs> line9 = std::shared_ptr<GLPL::Line2D2Vecs>(new GLPL::Line2D2Vecs(&xVec9, &yVec9));
    std::shared_ptr<GLPL::ShadedLine2D2CircularVecs> shaded9 = std::shared_ptr<GLPL::ShadedLine2D2CircularVecs>(new GLPL::ShadedLine2D2CircularVecs(&xVec9, &yVec9));
    std::shared_ptr<GLPL::Line2D2CircularVecs<float, float>> line10 = std::shared_ptr<GLPL::Line2D2CircularVecs<float, float>>(new GLPL::Line2D2CircularVecs<float, float>(&xVec10, &yVec10));
	line4->setLineColour(LC_MAGENTA);
    line5->setLineColour(LC_CYAN);
	line6->setLineColour(LC_YELLOW);
	line7->setLineColour(LC_RED);
	line9->setLineColour(LC_PURPLE);
	line10->setMarkerColour(LC_SILVER);
	shaded9->setShadeColour(LC_PURPLE);
	shaded9->setOpacityRatio(0.5);

	// Add lines to axes
    myplot.addLine(line1);
	myplot.addLine(line2);
	myplot.addLine(line3);
	myplot.addLine(line4);
	myplot.addLine(line5);
	myplot.addLine(line6);
	myplot.addLine(line7);
    myplot.addLine(line8);
    myplot.addLine(line9);
    myplot.addLine(line10);
    myplot.addShadedLine(shaded9);
	myplot.getAxes()->setAutoScaleRound(false);
	myplot.getAxes()->setEqualAxes(true);*/

	// Axes 1
    std::shared_ptr<GLPL::Axes> axesPt = myplot->getAxes(0);
    std::shared_ptr<GLPL::Axes> axes2Pt = myplot->addAxes(0.5f, 0.0f, 0.5f, 1.0f);
    std::shared_ptr<GLPL::ILine2D> line9 = axes2Pt->addLine(&xVec9, &yVec9, GLPL::SINGLE_LINE, LC_RED, 0.5);
    std::shared_ptr<GLPL::IScatterPlot> line9s = axes2Pt->addScatterPlot(&xVec14, &yVec14, LC_CYAN, 0.5);
    std::shared_ptr<GLPL::ILine2D> line11 = axes2Pt->addLine(&xVec11, &yVec11, GLPL::SHADED_LINE, LC_GREEN, 0.5);
    axesPt->setAxesBoxOn(false);
    axesPt->setButtonState("Grid", false);
    axesPt->setXLabel("Time (s)");
    axesPt->setYLabel("Displacement (m)");
    axesPt->setTitle("Spring Damping Over Time");
    axesPt->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);
    line9s->setMarkerColour(glm::vec3(1.0f, 0.5f, 0.5f));
    line9s->setOpacityRatio(0.6);
    line9s->setMarkerOutlineColour(glm::vec3(0.0f, 1.0f, 1.0f));
    line9s->setOutlineOpacityRatio(0.8);
    line9s->setMarkerType(GLPL::CIRCLE);
    line9s->setMarkerSizePx(15);

    // Axes 2
    //std::shared_ptr<GLPL::Axes> axes2Pt = myplot->addAxes(0.5f, 0.0f, 0.5f, 1.0f);
    //std::shared_ptr<GLPL::ILine2D> line12 = axes2Pt->addLine(&xVec11, &yVec11, GLPL::SINGLE_LINE, LC_YELLOW, 0.5);
    std::shared_ptr<GLPL::ILine2D> line12 = axesPt->addLine(&xVec12, &yVec12, GLPL::SINGLE_LINE, LC_YELLOW, 0.5);
    std::shared_ptr<GLPL::Line2D2Vecs> line12b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line12);
    //axesPt->addLine(&xVec9, &yVec9, GLPL::SINGLE_LINE, LC_RED);
    std::shared_ptr<GLPL::ILine2D> line13 = axesPt->addLine(&xVec13, &yVec13, GLPL::SINGLE_LINE, LC_CYAN, 0.5);
    std::shared_ptr<GLPL::Line2D2Vecs> line13b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line13);
    std::shared_ptr<GLPL::ILine2D> line15 = axesPt->addLine(&xVec15, &yVec15, GLPL::SINGLE_LINE, LC_BLUE, 0.5);
    std::shared_ptr<GLPL::Line2D2Vecs> line15b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line15);
    std::shared_ptr<GLPL::ILine2D> line16 = axesPt->addLine(&xVec16, &yVec16, GLPL::SINGLE_LINE, LC_BLUE, 0.5);
    std::shared_ptr<GLPL::Line2D2Vecs> line16b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line16);
    axes2Pt->setTitle("Simple Plots");
    axes2Pt->setXLabel("x");
    axes2Pt->setYLabel("y");

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
		/*GLPL::pt2D pt3;
		for(int j=0; j<10; j++) {
			float x = i/1000.0;
			pt3.x = x;
			pt3.y = sin(x);
			data3.push_back(pt3);
			i += 1;
		}
		// Update Graph 5
		i -= 10;
		for(int j=0; j < 10; j++) {
            std::vector<float> tempVec = {i/2000.0, i/1000.0};
			data5.push_back(tempVec);
			i += 1;
		}
		line5->updateInternalData();

		// Update Graph 6
		i -= 10;
		for(int j=0; j<10; j++) {
			glm::dvec3 tempVec = glm::dvec3(i/2000.0,2*i*i/(2000.0*2000.0),sin(i*5.0/2000.0));
			data6.push_back(tempVec);
			i += 1;
		}
		line6->updateInternalData();

		// Update graph 7 - time
		i-=10;
		for(int j=0; j<10; j++) {
			data7.push_back(i/1000.0);
			i += 1;
		}
		line7->updateInternalData();

		// Update graph 10 index
		j += 10;
		if (j>2499) {
		    j = 0;
		}
		line10->updateInternalData(j);

		// Update Axes Limits
		//if(i > 500) {
		//	myplot.axes.updateAxesLimits(-2.0,2.0,-2.0,2.0);
		//}*/

        /// Update graph 12
        /*line12b->dataPtX->push_back(2 + i);
        yVal12 += 2*((rand() % 100)/100.0) - 1.0;
        line12b->dataPtY->push_back(yVal12);
        //std::cout << yVal12 << std::endl;
        line12b->updateInternalData();
        i += 1;*/
        line12b->dataPtX->push_back(i);
        yVal12 = cos((i) / (25*M_PI)) * exp(-(i)/(25*8*M_PI));
        line12b->dataPtY->push_back(yVal12);
        line12b->updateInternalData();

        // Update graph 13
        line13b->dataPtX->push_back(i);
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
        //i -= 1;


		// Draw Plot
		//std::shared_ptr<GLPL::Axes> axesPt = myplot->getAxes(0);
        //axesPt->setPosition(axesPt->getLeft() + 0.001, axesPt->getBottom() + 0.001);
		myplot->Draw();
		//myplot->drawBoundingBox();
		//myplot->drawMouseOverBox();
		// TODO - Convert children vector to set, automatic ordering given the function

		// Post-loop draw
		window2->postLoopDraw();

	}

	// Close Window
	glfwTerminate();


	return 0;
}





