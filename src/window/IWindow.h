//
// Created by bcub3d-desktop on 28/3/20.
//

#ifndef OPENGLPLOTLIVE_IWINDOW_H
#define OPENGLPLOTLIVE_IWINDOW_H

// GLEW (OpenGL Extension Wrangler Library)
#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace GLPL {

    class IWindow {
    public:
        IWindow();

        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual GLFWwindow *getWindow() = 0;
    };

}


#endif //OPENGLPLOTLIVE_IWINDOW_H
