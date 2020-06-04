//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_IDRAWABLE_H
#define OPENGLPLOTLIVE_PROJ_IDRAWABLE_H

// Standard Includes
#include <array>
#include <vector>

// GLAD - Multi Language GL Loader-Generator
#include <glad/glad.h>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Project Includes
#include "transforms.h"
#include "ShaderSet.h"


namespace GLPL {

    struct ParentPointers {
        std::shared_ptr<glm::mat4> parentTransform;
        std::shared_ptr<int> parentWidthPx;
        std::shared_ptr<int> parentHeightPx;
        std::shared_ptr<ShaderSet> shaderSet;
    };

    class IDrawable {
        // Anything that is draw must implement the following
    public:
        // Constructor if top level
        IDrawable(float x, float y, int widthPx, int heightPx);
        // Constructor if child
        IDrawable(float x, float y, float width, float height, ParentPointers parentPointers);

        // Functions
        std::array<float, 2> getPosition();
        float getWidth();
        float getHeight();
        int getWidthPx();
        int getHeightPx();
        float getLeft();
        float getTop();
        float getRight();
        float getBottom();

        void setPosition(float newX, float newY);
        void setSize(float newWidth, float newHeight);
        void setSizePx(int newWidthPx, int newHeightPx);
        void setParentInformation(std::shared_ptr<glm::mat4> newParentTransform,
                                  std::shared_ptr<int> newParentWidthPx,
                                  std::shared_ptr<int> newParentHeightPx);
        void setParentInformation(ParentPointers parentPointers);
        ParentPointers createParentPointers();

        virtual void Draw() = 0;
        void registerChild(std::shared_ptr<IDrawable> newChildPt);
        void drawBoundingBox();

    protected:
        // Data
        bool isTopLevel = true; // True if drawable is the top level, that is has no parents.
        float x = 0;			// Location of bottom left corner x position of the drawable relative to its parent in 0 to 1
        float y = 0;			// Location of bottom left corner y position of the drawable relative to its parent in 0 to 1
        float width = 1.0;      // Width of drawable as a proportion of the parents width
        float height = 1.0;		// Height of drawable as a proportion of the parents height
        int widthPx = 0;        // Width in pixels
        int heightPx = 0;       // Height in pixels
        std::shared_ptr<int> parentWidthPx = std::make_shared<int>(0);  // Parent width in pixels
        std::shared_ptr<int> parentHeightPx = std::make_shared<int>(0); // Parent height in pixels

        // Children
        std::vector<std::shared_ptr<IDrawable>> children; // List of child drawables

        // Buffers
        GLuint VAO, VBO;
        // Area
        std::vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
        glm::vec4 boundingBoxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        // Transforms
        std::shared_ptr<glm::mat4> parentTransform;     // Pointer to the transform of the parent
        glm::mat4 viewportTransform;                    // Transform only with the current drawable bounding box
        glm::mat4 overallTransform;                     // The overall transform for drawing relative to the window
        // Shader access
        std::shared_ptr<ShaderSet> shaderSetPt = nullptr;

        // Functions
        void createAndSetupBuffers();
        void updateTransforms();
        void updateChildren();

    };
}


#endif //OPENGLPLOTLIVE_PROJ_IDRAWABLE_H
