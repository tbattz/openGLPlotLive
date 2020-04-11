/*
 * window.h
 *
 *  Created on: 29Mar.,2017
 *      Author: bcub3d-desktop
 */

#ifndef P_WINDOW_H_
#define P_WINDOW_H_

// GLAD - Multi Language GL Loader-Generator
#include <glad/glad.h>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Standard Includes
#include <iostream>
#include <array>

// Project Includes
#include "IWindow.h"


namespace GLPL {
	// Define Fonts
	#ifdef _WIN32
		#define FONTPATH "C:/Windows/Fonts/Arial.ttf"
	#elif __linux__
		#define FONTPATH "/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf"
	#endif

	class Window : public IWindow {
	public:
	    // Constructor
	    Window(int windowWidth, int windowHeight, int transparentBackground=GLFW_FALSE);
	    ~Window();
	    // Functions
	    void setKeysByIndex(int index, bool boolean);
	    void setToggleKeysByIndex(int index, bool boolean);
	    bool getToggleKeyStateByIndex(int index);
	    GLFWwindow* getWindow();
	    void preLoopDraw(bool clearBuffer);
	    void postLoopDraw();
	    int getWidth();
	    int getHeight();
	    void setFrameless(bool framelessOn);
	    void setAlwaysOnTop(bool alwaysOnTop);
	    void setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	private:
	    // Functions
        void initGLFW();
        void updateStoredSize();
        void initGLAD();

	    // Data
        GLFWwindow* window;
	    int windowWidth;
	    int windowHeight;
	    std::array<GLfloat, 4> backgroundColor = {0.0f, 0.0f, 0.0f, 1.0f};
	    int transparentBackground;

        // Keys
        bool keys[1024];
        bool toggleKeys[1024];
	};

}


#endif /* P_WINDOW_H_ */
