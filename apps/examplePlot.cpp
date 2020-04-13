/*
 * examplePlot.cpp
 *
 *  Created on: 1Jan.,2017
 *      Author: bcub3d-desktop
 *
 *  Based on tutorials from http://www.learnopengl.com/#!Introduction
 */


// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Standard Includes
#include <memory>

// Project Includes
#include "../src/rendering/fonts.h"
#include "../src/window/window.h"
#include "../src/plot/plot.h"
#include "../src/lines/Line2DVecVec.h"
#include "../src/lines/Line2DVec.h"
#include "../src/lines/Line2DVecGLMV3.h"
#include "../src/lines/Line2DVecfVecGLMV3.h"
#include "../src/lines/Line2D2Vecs.h"
#include "../src/lines/Line2D2CircularVecs.h"
#include "../src/shadedLines/ShadedLine2D2CircularVecs.h"


int main(int argc, char **argv) {
	/* ======================================================
	 *                     Setup Window
	   ====================================================== */
    // Window Size
    int windowWidth  = 800;
    int windowHeight = 800;

	// Init GLFW
	std::shared_ptr<GLPL::IWindow> window = std::shared_ptr<GLPL::IWindow>(new GLPL::Window(windowWidth, windowHeight));
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
	float i = -2000.0;

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
    int j = 0;
    xVec10.reserve(2500);
    yVec10.reserve(2500);
    for(int i=-1000; i<1500; i++) {
        xVec10.push_back(i/500.0);
        yVec10.push_back(5 - 0.25*pow(i/500.0, 2));
    }


	/* ======================================================
	 *                	    Create Plot
	   ====================================================== */
	// Create Plot
	GLPL::Plot myplot(0.0, 0.25, 0.75, 0.75, window);

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
    std::shared_ptr<GLPL::Line2D2CircularVecs> line10 = std::shared_ptr<GLPL::Line2D2CircularVecs>(new GLPL::Line2D2CircularVecs(&xVec10, &yVec10));
	line4->setLineColour(LC_MAGENTA);
    line5->setLineColour(LC_CYAN);
	line6->setLineColour(LC_YELLOW);
	line7->setLineColour(LC_RED);
	line9->setLineColour(LC_PURPLE);
	line10->setLineColour(LC_SILVER);
	shaded9->setShadeColour(LC_PURPLE);

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
	myplot.getAxes()->setEqualAxes(true);


	/* ======================================================
	 *                     Drawing Loop
	   ====================================================== */
	// Game Loop
	while(!glfwWindowShouldClose(window->getWindow())) {

		// Pre-loop draw
		window2->preLoopDraw(true);

		// Update Plot Data
		GLPL::pt2D pt3;
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
		//}

		// Draw Plot
		myplot.Draw();

		// Post-loop draw
		window2->postLoopDraw();

	}

	// Close Window
	glfwTerminate();


	return 0;
}





