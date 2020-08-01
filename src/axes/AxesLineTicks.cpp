//
// Created by bcub3d-desktop on 4/7/20.
//

#include "AxesLineTicks.h"

namespace GLPL {

    AxesLineTicks::AxesLineTicks(float x, float y, AxesDirection axesDirection,
                                 std::shared_ptr<ParentDimensions> parentDimensions) :
                                 ConstantXYDrawable(x, y, 1.0f, 20.0f, CONSTANT_SCALE, CONSTANT_SIZE, std::move(parentDimensions)) {
        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.78f, 0.0f, 1.0f);

        // Generate Axes Line
        this->axesDirection = axesDirection;
        AxesLineTicks::generateAxesLineVertices();

        // Setup Buffers
        AxesLineTicks::createAndSetupAxesLineBuffer();

        // Set size
        ConstantXYDrawable::setSize(1.0f, 20);

    }

    void AxesLineTicks::generateAxesLineVertices() {
        switch(axesDirection) {
            case X_AXES_TOP: {
                // Line on bottom
                lineVerts = {-1.0, -1.0, 1.0, -1.0};
                break;
            }
            case X_AXES_BOTTOM: {
                // Line on top
                lineVerts = {-1.0, 1.0, 1.0, 1.0};
                break;
            }
            case X_AXES_CENTRE: {
                // Line in centre
                lineVerts = {-1.0, 0.0, 1.0, 0.0};
                break;
            }
            case Y_AXES_LEFT: {
                // Line on right
                lineVerts = {1.0, -1.0, 1.0, 1.0};
                break;
            }
            case Y_AXES_RIGHT: {
                // Line on left
                lineVerts = {-1.0, -1.0, -1.0, 1.0};
                break;
            }
            case Y_AXES_CENTRE: {
                // Line in centre
                lineVerts = {0.0, -1.0, 0.0, 1.0};
                break;
            }
            default: {
                std::cout << "Incorrect Axes Direction!" << std::endl;
            }
        }
    }

    void AxesLineTicks::createAndSetupAxesLineBuffer() {
        // Create Buffers
        glGenVertexArrays(1,&lineVAO);
        glGenBuffers(1,&lineVBO);

        // Setup Buffers
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER,lineVBO);
        glBufferData(GL_ARRAY_BUFFER, lineVerts.size()*sizeof(GLfloat),&lineVerts[0],GL_STATIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glBindVertexArray(0);
    }

    void AxesLineTicks::drawAxesLine() {
        // Draw bounding box
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(axesLineColor));
        glBindVertexArray(lineVAO);
        glDrawArrays(GL_LINE_LOOP,0,4);
        glBindVertexArray(0);
    }

    void AxesLineTicks::Draw() {
        // Draw Axes line
        AxesLineTicks::drawAxesLine();
    }


}