/*
 * window.h
 *
 *  Created on: 29Mar.,2017
 *      Author: bcub3d-desktop
 */

#ifndef P_WINDOW_H_
#define P_WINDOW_H_

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// GLEW (OpenGL Extension Wrangler Library)
#define GLEW_STATIC
#include <GL/glew.h>


// Define Fonts
#ifdef _WIN32
	#define FONTPATH "C:/Windows/Fonts/Arial.ttf"
#elif __linux__
	#define FONTPATH "/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf"
#endif

// Screen Size
float screenHeight = 800.0;
float screenWidth  = 800.0;

// Keys
bool keys[1024];
bool toggleKeys[1024];



// Window Size Class
class WinDimensions {
public:
	GLFWwindow* window;
	int width;
	int height;

	WinDimensions(GLFWwindow* window) {
		this->window = window;

		updateStoredSize();
	}

	void updateStoredSize() {
		// Updates the stored window size used for scaling and
		// transformations with the current window size.
		glfwGetWindowSize(window,&width,&height);
		// Update Viewport Dimensions
		glViewport(0, 0, width, height);
	}
};

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

void reDraw(GLFWwindow* window, int width, int height) {
	// Redraw window on resize

}

GLFWwindow* initGLFW() {
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
	glfwSetWindowSizeCallback(window,reDraw);

	// Set viewport size
	glViewport(0,0,screenWidth,screenHeight); // Origin is bottom left

	// Test for objects in front of each other
	glEnable(GL_DEPTH_TEST);

	// Line Width
	glLineWidth(1);

	return window;
}

void initGLEW() {
	// Initialise GLEW - setup OpenGL pointers
	glewExperimental = GL_TRUE;
	glewInit();
}

void preLoopDraw(bool clearBuffer, WinDimensions* winDimPt) {
	// To be run before a drawing inside a loop or single draw call is used
	// clearBuffer:	True if the colour buffer should be reset
	//				Set to false for transparent background

	// Check Events
	glfwPollEvents();

	// Update window size
	winDimPt->updateStoredSize();

	if (clearBuffer) {
		// Clear the colour buffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}
void postLoopDraw(GLFWwindow* window) {
	// To be run after drawing inside a loop or after a single draw call is used
	// Swap buffers
	glfwSwapBuffers(window);
}




#endif /* P_WINDOW_H_ */
