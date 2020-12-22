//
// Created by bcub3d-desktop on 28/3/20.
//

#ifndef OPENGLPLOTLIVE_INPUTCALLBACKS_H
#define OPENGLPLOTLIVE_INPUTCALLBACKS_H

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Project Includes
#include "window.h"

namespace GLPL {

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
        // Get Window class
        GLPL::Window *windowPt = (GLPL::Window *) glfwGetWindowUserPointer(window);
        // ESC to close window
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        // Set key states
        if (action == GLFW_PRESS) {
            windowPt->setKeysByIndex(key, true);
            // Set toggle states
            windowPt->setToggleKeysByIndex(key, !windowPt->getToggleKeyStateByIndex(key));
        } else if (action == GLFW_RELEASE) {
            windowPt->setKeysByIndex(key, false);
        }

        // Handle selection
        windowPt->updateSelection();
    }

    static void reDraw(GLFWwindow *window, int width, int height) {
        // Get Window class
        auto *windowPt = (GLPL::Window *) glfwGetWindowUserPointer(window);
        // Redraw window on resize
        windowPt->updateStoredSize(width, height);

    }

    static void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            // Get Window class
            auto *windowPt = (GLPL::Window *) glfwGetWindowUserPointer(window);
            // Handle mouse release
            windowPt->handleMouseRelease();
        }
    }

    static void cursorMoved(GLFWwindow *window, double xpos, double ypos) {
        // Get Window class
        auto *windowPt = (GLPL::Window *) glfwGetWindowUserPointer(window);
        // Handle mouse movement
        windowPt->handleMouseMovement(xpos, ypos);
    }
}


#endif //OPENGLPLOTLIVE_INPUTCALLBACKS_H
