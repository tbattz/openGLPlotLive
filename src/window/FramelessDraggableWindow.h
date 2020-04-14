//
// Created by tbatt on 13/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_FRAMELESSDRAGGABLEWINDOW_H
#define OPENGLPLOTLIVE_PROJ_FRAMELESSDRAGGABLEWINDOW_H

// GLM Mathematics
#include <glm/glm.hpp>

// Project Includes
#include "window.h"


namespace GLPL {
    class FramelessDraggableWindow : public GLPL::Window {
    public:
        // Constructor
        FramelessDraggableWindow(int windowWidth, int windowHeight, bool transparentBackground,
                                 bool alwaysOnTop);

        // Functions
        void showWindow();
        void hideWindow();

    private:
        // Functions
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        static void cursor_position_callback(GLFWwindow* window, double x, double y);

        // Data
        bool mouseHeld = false;
        int cursorX = 0;
        int cursorY = 0;
        int xOffset = 0;
        int yOffset = 0;
    };

}


#endif //OPENGLPLOTLIVE_PROJ_FRAMELESSDRAGGABLEWINDOW_H
