//
// Created by tbatt on 13/04/2020.
//

#include "FramelessDraggableWindow.h"

namespace GLPL {

    FramelessDraggableWindow::FramelessDraggableWindow(int windowWidth, int windowHeight, bool transparentBackground,
                                                       bool alwaysOnTop) :
            GLPL::Window(windowWidth, windowHeight, transparentBackground, false) {
        // Set background color
        this->setFrameless(true);
        this->setAlwaysOnTop(alwaysOnTop);

        // Setup input callbacks
        glfwSetMouseButtonCallback(this->getWindow(), mouse_button_callback);
        glfwSetCursorPosCallback(this->getWindow(), cursor_position_callback);

    }

    void FramelessDraggableWindow::showWindow() {
        glfwShowWindow(this->getWindow());
    }

    void FramelessDraggableWindow::hideWindow() {
        glfwHideWindow(this->getWindow());
    }

    void FramelessDraggableWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
        // Get Window class
        GLPL::FramelessDraggableWindow* windowPt = (GLPL::FramelessDraggableWindow*) glfwGetWindowUserPointer(window);
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                windowPt->mouseHeld = true;
                // Update stored cursor position
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                windowPt->cursorX = floor(x);
                windowPt->cursorY = floor(y);
            } else if (action == GLFW_RELEASE) {
                windowPt->mouseHeld = false;
                windowPt->cursorX = 0;
                windowPt->cursorY = 0;
            }
        }
    }

    void FramelessDraggableWindow::cursor_position_callback(GLFWwindow *window, double x, double y) {
        // Get Window class
        GLPL::FramelessDraggableWindow* windowPt = (GLPL::FramelessDraggableWindow*) glfwGetWindowUserPointer(window);
        if (windowPt->mouseHeld) {
            windowPt->xOffset = (int)x - windowPt->cursorX;
            windowPt->yOffset = (int)y - windowPt->cursorY;

            int winPosX, winPosY;
            glfwGetWindowPos(window, &winPosX, &winPosY);
            glfwSetWindowPos(window, winPosX + windowPt->xOffset, winPosY + windowPt->yOffset);
        }
    }
}