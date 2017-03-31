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
	vector<pt2D> data1;
	for(int i = 0; i < 2000; i++) {
		pt2D pt;
		float x = i/1000.0;
		pt.x = x;
		pt.y = x*x;
		data1.push_back(pt);
	}
	// Graph 2
	vector<pt2D> data2;
	for(int i=-1000; i<1000; i++) {
		pt2D pt2;
		float x = i/1000.0;
		pt2.x = x;
		pt2.y = -x-1;
		data2.push_back(pt2);
	}
	// Graph 3
	vector<pt2D> data3;
	int i = -2000;

	// Graph 4 - Vector
	vector<float> data4;
	for(int i=-1000; i<1500; i++) {
		data4.push_back(i/1000.0);
		data4.push_back(-i/1000.0);
 	}

	// Graph 5 - Vector of Vectors
	vector<vector<float>> data5;


	/* ======================================================
	 *                	    Create Plot
	   ====================================================== */
	// Create Plot
	Plot myplot(0.0, 0.25, 0.75, 0.75, &winDim);

	// Create Lines
	Line2DPts line1(&data1);
	Line2DPts line2(&data2);
	Line2DPts line3(&data3);
	Line2DVec line4(&data4);
	Line2DVecVec line5(&data5);

	// Add lines to axes
	myplot.axes.addLine(&line1);
	myplot.axes.addLine(&line2);
	myplot.axes.addLine(&line3);
	myplot.axes.addLine(&line4);
	myplot.axes.addLine(&line5);


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
			data3.push_back(pt3);
			i += 1;
		}
		// Update Graph 5
		i -= 10;
		for(int j=0; j < 10; j++) {
			vector<float> tempVec = {i/1000.0, 0.5*i/1000.0};
			data5.push_back(tempVec);
			i += 1;
		}
		line5.updateInternalData();

		// Update Axes Limits
		if(i > 500) {
			myplot.axes.updateAxesLimits(-2.0,2.0,-2.0,2.0);
		}

		// Draw Plot
		myplot.Draw(plot2dShader);

		// Post-loop draw
		postLoopDraw(window);

	}

	// Close Window
	glfwTerminate();


	return 0;
}





