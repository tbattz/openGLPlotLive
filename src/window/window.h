/*
 * window.h
 *
 *  Created on: 29Mar.,2017
 *      Author: bcub3d-desktop
 */

#ifndef P_WINDOW_H_
#define P_WINDOW_H_

// GLEW (OpenGL Extension Wrangler Library)
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

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
	    Window(int windowWidth, int windowHeight);
	    // Functions
	    void setKeysByIndex(int index, bool boolean);
	    void setToggleKeysByIndex(int index, bool boolean);
	    bool getToggleKeyStateByIndex(int index);
	    GLFWwindow* getWindow();
	    void preLoopDraw(bool clearBuffer);
	    void postLoopDraw();
	    int getWidth();
	    int getHeight();

	private:
	    // Functions
        void initGLFW();
        void updateStoredSize();
        void initGLEW();

	    // Data
        GLFWwindow* window;
	    int windowWidth;
	    int windowHeight;

        // Keys
        bool keys[1024];
        bool toggleKeys[1024];
	};

}


#endif /* P_WINDOW_H_ */
