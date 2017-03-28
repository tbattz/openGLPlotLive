/*
 * main.cpp
 *
 *  Created on: 1Jan.,2017
 *      Author: bcub3d-desktop
 *
 *  Based on tutorials from http://www.learnopengl.com/#!Introduction
 */

// Standard Includes
#include <string>
#include <unistd.h>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>


// GLEW (OpenGL Extension Wrangler Library)
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// openGL Includes
#include "../src/shader.h"
#include "../src/fonts.h"
#include "../src/line2d.h"
#include "../src/plot.h"
#include "../src/transforms.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Free Type Library
#include <ft2build.h>
#include FT_FREETYPE_H

#include <vector>
using std::vector;

// Keys
bool keys[1024];
bool toggleKeys[1024];

// Define Fonts
#ifdef _WIN32
	#define FONTPATH "C:/Windows/Fonts/Arial.ttf"
#elif __linux__
	#define FONTPATH "/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf"
#endif

// Screen Size
float screenHeight = 800.0;
float screenWidth  = 800.0;

// Functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void window_size_callback(GLFWwindow* window, int width, int height);


int main(int argc, char* argv[]) {
	/* ======================================================
	 *                     Setup Window
	   ====================================================== */
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); // Set core profile

	// Screen Properties
	GLFWwindow* window = glfwCreateWindow(screenWidth,screenHeight,"openGLPlotLive",nullptr,nullptr);
	glfwMakeContextCurrent(window);

	// Setup Callbacks for user input
	glfwSetKeyCallback(window,key_callback);
	glfwSetWindowSizeCallback(window,window_size_callback);

	// Initialise GLEW - setup OpenGL pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Set viewport size
	glViewport(0,0,screenWidth,screenHeight); // Origin is bottom left

	// Test for objects in front of each other
	glEnable(GL_DEPTH_TEST);

	// Line Antialisaing
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1);*/

	// Line Width
	glLineWidth(1);

	/* ======================================================
	 *                  	  Shaders
	   ====================================================== */
	// Setup and compile shaders
	Shader plot2dShader("../Shaders/plot2d.vs","../Shaders/plot2d.frag");

	/* ======================================================
	 *                         Fonts
	   ====================================================== */
	// Load Telemetry Font
	//GLFont telemFont = GLFont(FONTPATH);

	/* ======================================================
	 *                      Simple Plot
   	   ====================================================== */

	// Create Window Dimensions Class
	WinDimensions winDim(screenWidth,screenHeight);

	// Create Plot
	Plot myplot(0.0, 0.25, 0.75, 0.75, &winDim);

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
	int j = 0;
	/*for(int i=-3000; i<3000 ; i++) {
		pt2D pt3;
		float x = i/1000.0;
		pt3.x = x;
		pt3.y = sin(x);
		graph3.push_back(pt3);
	}*/

	// Graph 4 - Vector
	vector<float> graph4;
	for(int i=-1000; i<1500; i++) {
		graph4.push_back(i/1000.0);
		graph4.push_back(-i/1000.0);
 	}

	// Graph 5 - Vector of Vectors
	vector<vector<float>> graph5;
	for(int i=-1000; i < 2000; i++) {
		vector<float> tempVec = {i/1000.0, 0.5*i/1000.0};
		graph5.push_back(tempVec);
	}

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

	float marginRatio = 0.05; // Ratio of screen (-1 to 1)
	float tickRatio = 0.025; //

	/* ======================================================
	 *                     Drawing Loop
	   ====================================================== */
	// Game Loop
	while(!glfwWindowShouldClose(window)) {

		// Check Events
		glfwPollEvents();

		// Update window size
		glfwGetWindowSize(window,&(winDim.width),&(winDim.height));

		// Clear the colour buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Update Plot Data
		pt2D pt3;
		for(int j=0; j<10; j++) {
			float x = i/1000.0;
			pt3.x = x;
			pt3.y = sin(x);
			plot3.appendPt(pt3);
			i += 1;
		}

		// Draw Plot
		myplot.Draw(plot2dShader);

		// Font Test
		std::stringstream sh;
		sh << "12";
		//(&telemFont)->RenderText(&textShader,sh.str(),0.0f,0.0f,1.0f,glm::vec3(1.0f, 1.0f, 0.0f),1);

		// Swap buffers
		glfwSwapBuffers(window);

		// Sleep
		//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}

	glfwTerminate();


	return 0;
}

// Smoothing key input callback
void key_callback(GLFWwindow* window,int key,int scancode, int action, int mode) {
	// ESC to close window
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,GL_TRUE);
	}
	// Set key states
	if(action == GLFW_PRESS) {
		keys[key] = true;
		// Set toggle states
		toggleKeys[key] = !toggleKeys[key];
	} else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

// Window Resized
void window_size_callback(GLFWwindow* window, int width, int height) {
	// Update stored window size variable
	screenWidth  = width;
	screenHeight = height;
	// Update Viewport Dimensions
	glViewport(0, 0, width, height);
}

