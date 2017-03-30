/*
 * main.cpp
 *
 *  Created on: 1Jan.,2017
 *      Author: bcub3d-desktop
 *
 *  Based on tutorials from http://www.learnopengl.com/#!Introduction
 */

// openGL Includes
#include "../src/fonts.h"
#include "../src/line2d.h"
#include "../src/plot.h"
#include "../src/window.h"

int main(int argc, char* argv[]) {
	/* ======================================================
	 *                     Setup Window
	   ====================================================== */
	// Init GLFW
	GLFWwindow* window = initGLFW();

	// Create Window Dimensions Class
	WinDimensions winDim(window);

	// Initialise GLEW - setup OpenGL pointers
	initGLEW();

	/* ======================================================
	 *                  	  Shaders
	   ====================================================== */
	// Setup and compile shaders
	Shader plot2dShader("../Shaders/plot2d.vs","../Shaders/plot2d.frag");

	/* ======================================================
	 *                	  Create Plot Data
   	   ====================================================== */
	// Create Data
	// Graph 1
	vector<pt2D> graph;
	for(int i = 0; i < 2000; i++) {
		pt2D pt;
		float x = i/1000.0;
		pt.x = x;
		pt.y = x*x;
		graph.push_back(pt);
	}
	// Graph 2
	vector<pt2D> graph2;
	for(int i=-1000; i<1000; i++) {
		pt2D pt2;
		float x = i/1000.0;
		pt2.x = x;
		pt2.y = -x-1;
		graph2.push_back(pt2);
	}
	// Graph 3
	vector<pt2D> graph3;
	int i=-2000;

	// Graph 4 - Vector
	vector<float> graph4;
	for(int i=-1000; i<1500; i++) {
		graph4.push_back(i/1000.0);
		graph4.push_back(-i/1000.0);
 	}

	// Graph 5 - Vector of Vectors
	vector<vector<float>> graph5;


	/* ======================================================
	 *                	    Create Plot
	   ====================================================== */
	// Create Plot
	Plot myplot(0.0, 0.25, 0.75, 0.75, &winDim);

	// Create Lines
	Line2DPts plot1(graph);
	Line2DPts plot2(graph2);
	Line2DPts plot3(graph3);
	Line2DVec plot4(graph4);
	Line2DVecVec plot5(&graph5);

	// Add lines to axes
	myplot.axes.addLine(&plot1);
	myplot.axes.addLine(&plot2);
	myplot.axes.addLine(&plot3);
	myplot.axes.addLine(&plot4);
	myplot.axes.addLine(&plot5);


	/* ======================================================
	 *                     Drawing Loop
	   ====================================================== */
	// Game Loop
	while(!glfwWindowShouldClose(window)) {

		// Pre-loop draw
		preLoopDraw(true, &winDim);

		// Update Plot Data
		pt2D pt3;
		for(int j=0; j<10; j++) {
			float x = i/1000.0;
			pt3.x = x;
			pt3.y = sin(x);
			plot3.appendPt(pt3);
			i += 1;
		}
		// Update Graph 5
		i -= 10;
		for(int j=0; j < 10; j++) {
			vector<float> tempVec = {i/1000.0, 0.5*i/1000.0};
			graph5.push_back(tempVec);
			i += 1;
		}
		plot5.updateInternalData();

		// Draw Plot
		myplot.Draw(plot2dShader);

		// Post-loop draw
		postLoopDraw(window);

	}

	// Close Window
	glfwTerminate();


	return 0;
}





