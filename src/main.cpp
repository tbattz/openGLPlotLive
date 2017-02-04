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
glm::mat4 viewportTransform(float xc, float yc, float width, float height, float winWidth, float winHeight);


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

	/* ======================================================
	 *                  	  Shaders
	   ====================================================== */
	// Setup and compile shaders
	Shader plot2dShader("../Shaders/plot2d.vs","../Shaders/plot2d.frag");

	/* ======================================================
	 *                         Fonts
	   ====================================================== */
	// Load Font Shader
	//Shader textShader = setupFontShader("../Shaders/font.vs", "../Shaders/font.frag",screenWidth,screenHeight);

	// Load Telemetry Font
	//GLFont telemFont = GLFont(FONTPATH);

	/* ======================================================
	 *                      Simple Plot
   	   ====================================================== */

	vector<pt2D> graph;
	// Create Data
	for(int i = 0; i < 2000; i++) {
		pt2D pt;
		float x = i/1000.0;
		pt.x = x;
		pt.y = x*x;
		graph.push_back(pt);
	}

	Line2D plot1(graph);

	float marginRatio = 0.1;

	/* ======================================================
	 *                     Drawing Loop
	   ====================================================== */
	// Game Loop
	while(!glfwWindowShouldClose(window)) {

		// Check Events
		glfwPollEvents();

		// Clear the colour buffer
		glClearColor(0.64f, 0.64f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Change viewport area
		glViewport(screenWidth*marginRatio,screenHeight*marginRatio,(1-(2*marginRatio))*screenWidth,(1-(2*marginRatio))*screenHeight);
		// Set Scissor Area
		glScissor(screenWidth*marginRatio,screenHeight*marginRatio,(1-(2*marginRatio))*screenWidth,(1-(2*marginRatio))*screenHeight);


		// Enable Scissor Test
		glEnable(GL_SCISSOR_TEST);

		// Draw Lines
		glm::mat4 viewportTrans = viewportTransform(0.0, 0.0, screenWidth/4.0, screenHeight/4.0, screenWidth, screenHeight);
		glUniformMatrix4fv(glGetUniformLocation(plot2dShader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(viewportTrans));
		plot1.Draw(plot2dShader);

		// Change viewport back to full screen
		glViewport(0,0,screenWidth,screenHeight);
		// Disable Scissor Testing
		glDisable(GL_SCISSOR_TEST);


		// Swap buffers
		glfwSwapBuffers(window);

		// Sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
}

// Transform to custom viewports
glm::mat4 viewportTransform(float xc, float yc, float width, float height, float winWidth, float winHeight) {
	// Creates transform matrix for a custom sized viewport
	// xc:			The center x coordinate (in -1 to 1)
	// yc:			The center left corner y coordinate (in -1 to 1)
	// width:		The width of the new viewport
	// height:		The height of the new viewport
	// winWidth: The width of the original window
	// winHeight:	The height of the original window

	// Translate by offset
	glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(xc, yc, 0));

	// Scale new viewport
	float scaleX = width/winWidth;
	float scaleY = height/winHeight;
	glm::mat4 scale = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));

	return scale;
}
