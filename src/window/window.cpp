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

        // Adjust window size for high DPI displays
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, true);

        // Use 4 samples for MSAA Anti-aliasing
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Screen Properties
        window = glfwCreateWindow(getWidthPx(),getHeightPx(),"openGLPlotLive",nullptr,nullptr);
        glfwMakeContextCurrent(window);

        // Initialise GLAD
        Window::initGLAD();

        // Set Input mode to remember mouse states
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

        // Setup Callbacks for user input
        glfwSetKeyCallback(window, GLPL::key_callback);
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowSizeCallback(window, GLPL::reDraw);
        glfwSetCursorPosCallback(window, GLPL::cursorMoved);
        glfwSetMouseButtonCallback(window, GLPL::mouseCallback);
        glfwSetScrollCallback(window, GLPL::mouseScrolled);


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

    void Window::handleMouseClick(int button, int action, int mods) {
        // State machine for determining action
        switch (currentAction) {
            case NO_ACTION: {
                if (action == GLFW_PRESS) {
                    glfwGetCursorPos(window, &xPressed, &yPressed);
                    // Check to start a new action
                    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
                        currentAction = LEFT_MOUSE_PRESSED;
                    } else if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_SHIFT) {
                        currentAction = LEFT_MOUSE_SHIFT_PRESSED;
                    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && mods == 0) {
                        currentAction = MIDDLE_MOUSE_PRESSED;
                    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && mods == 0) {
                        currentAction = RIGHT_MOUSE_PRESSED;
                    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && mods == GLFW_MOD_SHIFT) {
                        currentAction = RIGHT_MOUSE_SHIFT_PRESSED;
                    }
                }

                break;
            }
            case LEFT_MOUSE_PRESSED: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == 0) {
                        // Mouse has been clicked and released
                        for(auto &mousedObj : *mousedOverObjs) {
                            mousedObj->onLeftClick();
                        }
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;

                }
                break;
            }
            case LEFT_MOUSE_SHIFT_PRESSED: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_SHIFT) {
                        // Mouse has been clicked and released
                        for (auto &mousedObj : *mousedOverObjs) {
                            mousedObj->onLeftShiftClick();
                        }
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case MIDDLE_MOUSE_PRESSED: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_MIDDLE && mods == 0) {
                        // Mouse has been clicked and released
                        for (auto &mousedObj : *mousedOverObjs) {
                            mousedObj->onMiddleClick();
                        }
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case RIGHT_MOUSE_PRESSED: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_RIGHT && mods == 0) {
                        // Mouse has been clicked and released
                        for (auto &mousedObj : *mousedOverObjs) {
                            mousedObj->onRightClick();
                        }
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case RIGHT_MOUSE_SHIFT_PRESSED: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_RIGHT && mods == GLFW_MOD_SHIFT) {
                        // Mouse has been clicked and released
                        for (auto &mousedObj : *mousedOverObjs) {
                            mousedObj->onRightShiftClick();
                        }
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case LEFT_MOUSE_DRAG: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT) {
                        // Mouse dragging has stopped
                        Window::handleLeftMouseHeld(false);
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case LEFT_MOUSE_SHIFT_DRAG: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_LEFT) {
                        // Mouse dragging has stopped
                        Window::handleLeftShiftMouseHeld(false);
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case MIDDLE_MOUSE_DRAG: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
                        // Mouse dragging has stopped
                        Window::handleMiddleMouseHeld(false);
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case RIGHT_MOUSE_DRAG: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                        // Mouse dragging has stopped
                        Window::handleRightMouseHeld(false);
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }
            case RIGHT_MOUSE_SHIFT_DRAG: {
                if (action == GLFW_RELEASE) {
                    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                        // Mouse dragging has stopped
                        Window::handleRightShiftMouseHeld(false);
                    }
                    // Cancel current action
                    currentAction = NO_ACTION;
                }

                break;
            }

            default: {
                std::cout << "Unknown Action! " << currentAction << std::endl;
            }
        }

    }

    void Window::handleMouseMovement(double xpos, double ypos) {

        // Convert from pixel space to -1 to 1
        if (getWidthPx() > 0 && getHeightPx() > 0) {
            xpos = 2 * (xpos / getWidthPx()) - 1;
            ypos = -(2 * (ypos / getHeightPx()) - 1);

            // Update mouse over states
            bool changed = Window::updateMouseOverStates(xpos, ypos);

            // Update Hoverable states & cursor
            Window::updateHoverableStates(xpos, ypos);

            // Print changes in hovered overs
            if (changed) {
                Window::printSelection();
            }

            // Update Drag Action States
            Window::updateDragActionStates();

        }
    }

    void Window::handleMouseScroll(double xoffset, double yoffset) {
        // Check if mouse is over an object
        for(auto &mousedObj : *mousedOverObjs) {
            mousedObj->onScroll(xoffset, yoffset);
        }
    }

    void Window::updateSelection() {
        // Check if we should increment the selection
        if (toggleKeys[GLFW_KEY_SPACE]) {
            if (hoverableObjs.get()[0].size() > 1) {
                auto mousePos = selected->getLastMousePos();
                selected->setSelected(false);
                int index = -1;
                for (unsigned int i = 0; i < hoverableObjs->size(); i++) {
                    if (hoverableObjs.get()[0][i] == selected) {
                        if (i + 1 < hoverableObjs.get()[0].size()) {
                            index = (int)i + 1;
                        } else {
                            index = 0;
                        }
                        break;
                    }
                }
                if (index != -1) {
                    hoverableObjs.get()[0][index]->setSelected(true);
                    selected = hoverableObjs.get()[0][index];

                    // Print changes in hoverable over
                    Window::printSelection();
                }
                // Update mouse position
                if (selected != nullptr) {
                    selected->setLastMousePos(std::get<0>(mousePos), std::get<1>(mousePos));
                }

            }
            toggleKeys[GLFW_KEY_SPACE] = false;
        }
    }

    void Window::handleLeftMouseHeld(bool buttonHeld) {
        // Get mouse location
        double xpos, ypos;
        if (buttonHeld) {
            glfwGetCursorPos(window, &xpos, &ypos);
        } else {
            xpos = 0.0;
            ypos = 0.0;
        }
        // Convert from pixel space to -1 to 1
        xpos = 2 * (xpos / getWidthPx()) - 1;
        ypos = -(2 * (ypos / getHeightPx()) - 1);

        for(auto &draggingObj : *draggingObjs) {
            draggingObj->onLeftDrag(buttonHeld, xpos, ypos);
        }

        if (!buttonHeld) {
            // If released, drop dragging items
            draggingObjs->clear();
        }
    }

    void Window::handleLeftShiftMouseHeld(bool buttonHeld) {
        // Get mouse location`
        double xpos, ypos;
        if (buttonHeld) {
            glfwGetCursorPos(window, &xpos, &ypos);
        } else {
            xpos = 0.0;
            ypos = 0.0;
        }
        // Convert from pixel space to -1 to 1
        xpos = 2 * (xpos / getWidthPx()) - 1;
        ypos = -(2 * (ypos / getHeightPx()) - 1);

        for(auto &draggingObj : *draggingObjs) {
            draggingObj->onLeftShiftDrag(buttonHeld, xpos, ypos);
        }

        if (!buttonHeld) {
            // If released, drop dragging items
            draggingObjs->clear();
        }
    }

    void Window::handleRightMouseHeld(bool buttonHeld) {
        // Get mouse location
        double xpos, ypos;
        if (buttonHeld) {
            glfwGetCursorPos(window, &xpos, &ypos);
        } else {
            xpos = 0.0;
            ypos = 0.0;
        }
        // Convert from pixel space to -1 to 1
        xpos = 2 * (xpos / getWidthPx()) - 1;
        ypos = -(2 * (ypos / getHeightPx()) - 1);

        for(auto &draggingObj : *draggingObjs) {
            draggingObj->onRightDrag(buttonHeld, xpos, ypos);
        }

        if (!buttonHeld) {
            // If released, drop dragging items
            draggingObjs->clear();
        }

    }

    void Window::handleMiddleMouseHeld(bool buttonHeld) {
        // Get mouse location
        double xpos, ypos;
        if (buttonHeld) {
            glfwGetCursorPos(window, &xpos, &ypos);
        } else {
            xpos = 0.0;
            ypos = 0.0;
        }
        // Convert from pixel space to -1 to 1
        xpos = 2 * (xpos / getWidthPx()) - 1;
        ypos = -(2 * (ypos / getHeightPx()) - 1);

        for(auto &draggingObj : *draggingObjs) {
            draggingObj->onMiddleDrag(buttonHeld, xpos, ypos);
        }

        if (!buttonHeld) {
            // If released, drop dragging items
            draggingObjs->clear();
        }

    }

    void Window::handleRightShiftMouseHeld(bool buttonHeld) {
        // Get mouse location
        double xpos, ypos;
        if (buttonHeld) {
            glfwGetCursorPos(window, &xpos, &ypos);
        } else {
            xpos = 0.0;
            ypos = 0.0;
        }
        // Convert from pixel space to -1 to 1
        xpos = 2 * (xpos / getWidthPx()) - 1;
        ypos = -(2 * (ypos / getHeightPx()) - 1);

        for(auto &draggingObj : *draggingObjs) {
            draggingObj->onRightShiftDrag(buttonHeld, xpos, ypos);
        }

        if (!buttonHeld) {
            // If released, drop dragging items
            draggingObjs->clear();
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

    void Window::setKeysByIndex(int index, bool boolean, int mode) {
        switch (mode) {
            case 0: {
                keys[index] = boolean;
                break;
            }
            case GLFW_MOD_SHIFT: {
                shiftKeys[index] = boolean;
                break;
            }
            default: {
                std::cout << "Key modifier not implemented!" << std::endl;
            }
        }

    }


    void Window::setToggleKeysByIndex(int index, bool boolean, int mode) {
        switch (mode) {
            case 0: {
                toggleKeys[index] = boolean;
                break;
            }
            case GLFW_MOD_SHIFT: {
                shiftToggleKeys[index] = boolean;
                break;
            }
            default: {
                std::cout << "Key modifier not implemented!" << std::endl;
            }
        }

    }


    bool Window::getToggleKeyStateByIndex(int index, int mode) {
        switch (mode) {
            case 0: {
                return toggleKeys[index];
            }
            case GLFW_MOD_SHIFT: {
                return shiftToggleKeys[index];
            }
            default: {
                std::cout << "Key modifier not implemented!" << std::endl;
            }
        }

        return false;
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
        std::shared_ptr<ParentDimensions> parentDimensions = std::make_shared<ParentDimensions>(ParentDimensions{transform, 0, 0, getWidthPx(), getHeightPx(), shaderSetPt});

        return parentDimensions;
    }

    void Window::Draw() {

    }

    std::string Window::getID() {
        return "Window:" + std::to_string(x) + ":" + std::to_string(y);
    }

    void Window::addPlot(const std::shared_ptr<IDrawable>& plotPt) {
        // Store plot pointer
        children.push_back(plotPt);
    }

    bool Window::updateMouseOverStates(double xpos, double ypos) {
        // Create vector to store
        std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> newMousedOverObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();

        // Determine children that mouse is over
        for (auto &child : children) {
            if (child->canMouseOver()) {
                child->getMousedOverChildren(xpos, ypos, newMousedOverObjs);
            }
        }

        // Update mouse over states
        for(auto &oldMousedObj : *mousedOverObjs) {
            oldMousedObj->setHovered(false);
        }
        for (auto &mousedObj : *newMousedOverObjs) {
            mousedObj->setHovered(true);
            mousedObj->setLastMousePos(xpos, ypos);
        }

        // Check if the moused over objects have changed
        bool changed = false;
        if (newMousedOverObjs->size() != mousedOverObjs->size()) {
            changed = true;
        } else {
            for(unsigned int i=0; i < mousedOverObjs->size(); i ++) {
                if (newMousedOverObjs.get()[0][i].get() != mousedOverObjs.get()[0][i].get()) {
                    changed = true;
                    break;
                }
            }
        }

        this->mousedOverObjs = newMousedOverObjs;

        return changed;

    }

    void Window::updateHoverableStates(double xpos, double ypos) {
        // Determine which objects are hoverable
        std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>> newHoverableObjs = std::make_shared<std::vector<std::shared_ptr<GLPL::IDrawable>>>();
        int newCursor = 0;
        for(const auto& obj : *mousedOverObjs) {
            if (obj->isHoverable()) {
                newHoverableObjs->push_back(obj);
            }
            if (obj->isMouseOver(xpos, ypos, false) && obj->getHoverCursor() != 0) {
                newCursor = obj->getHoverCursor();
            }
        }

        // Check if cursor has changed
        if (newCursor != lastCursorType) {
            lastCursorType = newCursor;
            glfwDestroyCursor(lastCursor);
            lastCursor = glfwCreateStandardCursor(newCursor);
            glfwSetCursor(window, lastCursor);
        }

        // Handle selection
        if (!newHoverableObjs->empty()) {
            if (selected == nullptr) {
                // Select first object in list
                newHoverableObjs.get()[0][0]->setSelected(true);
                selected = newHoverableObjs.get()[0][0];
            }
        } else if (selected != nullptr) {
            // Reset selected
            selected->setSelected(false);
            selected = nullptr;
        }

        // Update mouse position
        if (selected != nullptr) {
            selected->setLastMousePos(xpos, ypos);
        }

        this->hoverableObjs = newHoverableObjs;

    }

    void Window::updateDragActionStates() {
        // Change to drag action if required
        double newXClick, newYClick;
        glfwGetCursorPos(window, &newXClick, &newYClick);
        switch (currentAction) {
            case LEFT_MOUSE_PRESSED: {
                // Check if mouse has moved
                if (newXClick != xPressed || newYClick != yPressed) {
                    currentAction = LEFT_MOUSE_DRAG;

                    // Add any new items that are moused over, to the dragging items
                    Window::updateDraggingItems();

                    // Update objects
                    Window::handleLeftMouseHeld(true);
                }

                break;
            }
            case LEFT_MOUSE_SHIFT_PRESSED: {
                // Check if mouse has moved
                if (newXClick != xPressed || newYClick != yPressed) {
                    currentAction = LEFT_MOUSE_SHIFT_DRAG;

                    // Add any new items that are moused over, to the dragging items
                    Window::updateDraggingItems();

                    // Update objects
                    Window::handleLeftShiftMouseHeld(true);
                }

                break;
            }
            case MIDDLE_MOUSE_PRESSED: {
                // Check if mouse has moved
                if (newXClick != xPressed || newYClick != yPressed) {
                    currentAction = MIDDLE_MOUSE_DRAG;

                    // Add any new items that are moused over, to the dragging items
                    Window::updateDraggingItems();

                    // Update objects
                    Window::handleMiddleMouseHeld(true);
                }

                break;
            }
            case RIGHT_MOUSE_PRESSED: {
                // Check if mouse has moved
                if (newXClick != xPressed || newYClick != yPressed) {
                    currentAction = RIGHT_MOUSE_DRAG;

                    // Add any new items that are moused over, to the dragging items
                    Window::updateDraggingItems();

                    // Update objects
                    Window::handleRightMouseHeld(true);
                }

                break;
            }
            case RIGHT_MOUSE_SHIFT_PRESSED: {
                // Check if mouse has moved
                if (newXClick != xPressed || newYClick != yPressed) {
                    currentAction = RIGHT_MOUSE_SHIFT_DRAG;

                    // Add any new items that are moused over, to the dragging items
                    Window::updateDraggingItems();

                    // Update objects
                    Window::handleRightShiftMouseHeld(true);
                }

                break;
            }
            case NO_ACTION:
            case LEFT_MOUSE_DRAG:
            case LEFT_MOUSE_SHIFT_DRAG:
            case RIGHT_MOUSE_DRAG:
            case RIGHT_MOUSE_SHIFT_DRAG:
            default: {
                break;
            }
        }

    }

    void Window::updateDraggingItems() {
        // Add any new items that are moused over, to the dragging items
        for (auto &mousedObj : *mousedOverObjs) {
            // Check if object is already being dragged
            if (!(std::find(draggingObjs->begin(), draggingObjs->end(), mousedObj) != draggingObjs->end())) {
                draggingObjs->push_back(mousedObj);
            }
        }
    }

    void Window::printSelection() {
        if (printSelected) {
            for (const auto &obj : *hoverableObjs) {
                if (obj->isSelected()) {
                    std::cout << "[" << obj->getID() << "], ";
                } else {
                    std::cout << obj->getID() << ", ";
                }
            }
            std::cout << std::endl;
        }
    }

};