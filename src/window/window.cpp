//
// Created by bcub3d-desktop on 28/3/20.
//

#include "window.h"

#include <utility>
#include "inputCallbacks.h"


namespace GLPL {

    Window::Window(int windowWidth, int windowHeight, bool transparentBackground, bool focusOnShow) :
    TopLevelDrawable(0.0f, 0.0f, windowWidth, windowHeight) {
        // Set window options
        this->transparentBackground = transparentBackground;
        this->focusOnShow = focusOnShow;

        // Initialise GLFW
        Window::initGLFW();

        // Create Shader Set
        shaderSetPt = std::make_shared<ShaderSet>();

        // Update Stored Size
        Window::updateStoredSize();
    }

    Window::~Window() {

    }

    void Window::initGLFW() {
        // Init GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE); // Set core profile

        // Make background transparent
        if (transparentBackground) {
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        }

        // Set window to not steal focus
        if (!focusOnShow) {
            glfwWindowHint(GLFW_FOCUS_ON_SHOW, false);
        }

        // Use 4 samples for MSAA Anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Screen Properties
        window = glfwCreateWindow(getWidthPx(),getHeightPx(),"openGLPlotLive",nullptr,nullptr);
        glfwMakeContextCurrent(window);

        // Initialise GLAD
        Window::initGLAD();

        // Setup Callbacks for user input
        glfwSetKeyCallback(window, GLPL::key_callback);
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, GLPL::reDraw);

        // Set viewport size
        glViewport(0,0,getWidthPx(),getHeightPx()); // Origin is bottom left

        // Test for objects in front of each other
        glEnable(GL_DEPTH_TEST);

        // Enable Anti-aliasing
        glEnable(GL_MULTISAMPLE);

        // Depth testing to ensure transparency works for shaded lines
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDepthFunc(GL_LEQUAL);

        // Line Width
        glLineWidth(1);
    }

    void Window::updateStoredSize() {
        // Updates the stored window size used for scaling and
        // transformations with the current window size.
        // Get previous size
        std::pair<int, int> oldSize = getSizePx();
        // Get new size
        int windowWidth, windowHeight;
        glfwGetWindowSize(window,&windowWidth,&windowHeight);
        // Update Viewport Dimensions
        glViewport(0, 0, windowWidth, windowHeight);
        // Update children
        if (windowWidth != oldSize.first || windowHeight != oldSize.second) {
            // Update the stored size
            TopLevelDrawable::setSize((float)windowWidth, (float)windowHeight);
            for (unsigned int i = 0; i < children.size(); i++) {
                this->children[i]->setParentDimensions(this->getParentDimensions());
            }
        }
    }

    void Window::updateStoredSize(int newWidth, int newHeight) {
        // Set new size
        TopLevelDrawable::setSize((float)newWidth, (float)newHeight);
        // Update children
        for(unsigned int i = 0; i < children.size(); i++) {
            this->children[i]->setParentDimensions(this->getParentDimensions());
        }
    }

    void Window::updateSizePx() {

    }

    void Window::initGLAD() {
        // Initialise GLAD - setup OpenGL pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }
    }

    void Window::setKeysByIndex(int index, bool boolean) {
        keys[index] = boolean;
    }

    void Window::setToggleKeysByIndex(int index, bool boolean) {
        toggleKeys[index] = boolean;
    }

    bool Window::getToggleKeyStateByIndex(int index) {
        return toggleKeys[index];
    }

    GLFWwindow* Window::getWindow() {
        return this->window;
    }

    void Window::preLoopDraw(bool clearBuffer) {
        // To be run before a drawing inside a loop or single draw call is used
        // clearBuffer:	True if the colour buffer should be reset
        //				Set to false for transparent background

        // Check Events
        glfwPollEvents();

        // Make this windows context current
        glfwMakeContextCurrent(this->getWindow());

        // Update window size
        Window::updateStoredSize();

        if (clearBuffer) {
            // Clear the colour buffer
            GLfloat red = backgroundColor[0];
            GLfloat green = backgroundColor[1];
            GLfloat blue = backgroundColor[2];
            GLfloat alpha = backgroundColor[3];
            glClearColor(red, green, blue, alpha);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void Window::postLoopDraw() {
        // Swap buffers
        // To be run after drawing inside a loop or after a single draw call is used
        glfwSwapBuffers(Window::getWindow());
    }

    void Window::setFrameless(bool framelessOn) {
        glfwSetWindowAttrib(window, GLFW_DECORATED, !framelessOn);
    }

    void Window::setAlwaysOnTop(bool alwaysOnTop) {
        glfwSetWindowAttrib(window, GLFW_FLOATING , alwaysOnTop);
    }

    void Window::setBackgroundColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
        this->backgroundColor = {red, green, blue, alpha};
    }

    std::shared_ptr<ShaderSet> Window::getShaderSet() {
        return shaderSetPt;
    }

    std::shared_ptr<ParentDimensions> Window::getParentDimensions() {
        // Create parent pointers
        std::shared_ptr<ParentDimensions> parentDimensions = std::make_shared<ParentDimensions>(ParentDimensions{transform, 0.0, 0.0, getWidthPx(), getHeightPx(), shaderSetPt});

        return parentDimensions;
    }

    void Window::Draw() {

    }

    void Window::addPlot(const std::shared_ptr<IDrawable>& plotPt) {
        // Store plot pointer
        children.push_back(plotPt);
    }

};