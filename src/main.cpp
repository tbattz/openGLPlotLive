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
	// Load Font Shader
	//Shader textShader = setupFontShader("../Shaders/font.vs", "../Shaders/font.frag",screenWidth,screenHeight);

	// Load Telemetry Font
	//GLFont telemFont = GLFont(FONTPATH);

	/* ======================================================
	 *                         Axes
   	   ====================================================== */
	/* Create Buffers */
	//GLuint VAO, VBO;
	//glGenVertexArrays(1,&VAO);
	//glGenBuffers(1,&VBO);

	/* Setup Buffers */
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//vector<GLfloat> boxVerts = { -1, -1,
	//							  1, -1,
	//							  1,  1,
	//							  -1, 1};
	//glBufferData(GL_ARRAY_BUFFER, boxVerts.size()*sizeof(GLfloat),&boxVerts[0],GL_STATIC_DRAW);

	/* Position Attributes */
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//glBindVertexArray(0);


	/* ======================================================
	 *                      Simple Plot
   	   ====================================================== */

	// Create Plot
	WindowProperties winProp(screenHeight,screenWidth);
	Plot myplot(0.375, 0.625, 0.75, 0.75, &winProp);

	vector<pt2D> graph;
	vector<pt2D> graph2;
	// Create Data
	for(int i = 0; i < 1000; i++) {
		pt2D pt;
		float x = i/1000.0;
		pt.x = x;
		pt.y = x*x;
		graph.push_back(pt);
	}

	for(int i=-1000; i<1000; i++) {
		pt2D pt2;
		float x = i/1000.0;
		pt2.x = x;
		pt2.y = x;
		graph2.push_back(pt2);
	}

	//Line2D plot1(graph);
	//Line2D plot2(graph2);

	float marginRatio = 0.05; // Ratio of screen (-1 to 1)
	float tickRatio = 0.025; //

	/* ======================================================
	 *                     Drawing Loop
	   ====================================================== */
	// Game Loop
	while(!glfwWindowShouldClose(window)) {

		// Check Events
		glfwPollEvents();

		// Clear the colour buffer
		//glClearColor(0.64f, 0.64f, 1.0f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Change viewport area
		glViewport(screenWidth*marginRatio,screenHeight*marginRatio,(1-(2*marginRatio))*screenWidth,(1-(2*marginRatio))*screenHeight);

		// Draw Lines
		float winHeight = screenHeight;
		float winWidth = screenWidth;
		float axesHeight = screenHeight*(1-(2*marginRatio)+(2*tickRatio));
		float axesWidth = screenWidth*(1-(2*marginRatio)+(2*tickRatio));
		glm::mat4 viewportTrans = viewportTransform(0.0, 0.0, axesWidth, axesHeight, winWidth, winHeight);
		glUniformMatrix4fv(glGetUniformLocation(plot2dShader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(viewportTrans));
		//plot1.Draw(plot2dShader);
		//plot2.Draw(plot2dShader);

		// Enable Scissor Test
		glEnable(GL_SCISSOR_TEST);
		// Set Scissor Area
		glScissor(screenWidth*(marginRatio+tickRatio),screenHeight*(marginRatio+tickRatio),(1-(2*marginRatio)+(2*tickRatio))*screenWidth,(1-(2*marginRatio)+(2*tickRatio))*screenHeight);
		// Disable Scissor Testing
		glDisable(GL_SCISSOR_TEST);

		// Change viewport back to full screen
		glViewport(0,0,screenWidth,screenHeight);

		// Draw Axes
		//viewportTrans = viewportTransform(0.0, 0.0, axesWidth, axesHeight, winWidth, winHeight);
		myplot.Draw(plot2dShader,viewportTrans);



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

