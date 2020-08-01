//
// Created by bcub3d-desktop on 4/7/20.
//

#include "AxesLineTicks.h"

namespace GLPL {

    AxesLineTicks::AxesLineTicks(AxesDirection axesDirection,
                                 std::shared_ptr<ParentDimensions> parentDimensions) :
                                 ConstantXYDrawable(0.0f, 0.0f, 1.0f, 1.0f, CONSTANT_SCALE, CONSTANT_SIZE, std::move(parentDimensions)) {
        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.78f, 0.0f, 1.0f);

        // Generate Axes Line
        this->axesDirection = axesDirection;
        AxesLineTicks::generateAxesLine();

        // Setup Buffers
        AxesLineTicks::createAndSetupAxesLineBuffer();

        // Set size
        AxesLineTicks::updateSize();

    }

    void AxesLineTicks::generateAxesLine() {
        switch(axesDirection) {
            case X_AXES_TOP: {
                // Line on bottom
                lineVerts = {-1.0, -1.0, 1.0, -1.0};
                setXYScale(CONSTANT_SCALE, CONSTANT_SIZE);
                setAttachLocation(BOTTOM_LEFT);
                setPosition(0.0f, 1.0f);
                break;
            }
            case X_AXES_BOTTOM: {
                // Line on top
                lineVerts = {-1.0, 1.0, 1.0, 1.0};
                setXYScale(CONSTANT_SCALE, CONSTANT_SIZE);
                setAttachLocation(TOP_LEFT);
                setPosition(0.0f, 0.0f);
                break;
            }
            case X_AXES_CENTRE: {
                // Line in centre
                lineVerts = {-1.0, 0.0, 1.0, 0.0};
                setXYScale(CONSTANT_SCALE, CONSTANT_SIZE);
                setAttachLocation(CENTRE);
                setPosition(0.5f, 0.5f);
                break;
            }
            case Y_AXES_LEFT: {
                // Line on right
                lineVerts = {1.0, -1.0, 1.0, 1.0};
                setXYScale(CONSTANT_SIZE, CONSTANT_SCALE);
                setAttachLocation(BOTTOM_RIGHT);
                setPosition(0.0f, 0.0f);
                break;
            }
            case Y_AXES_RIGHT: {
                // Line on left
                lineVerts = {-1.0, -1.0, -1.0, 1.0};
                setXYScale(CONSTANT_SIZE, CONSTANT_SCALE);
                setAttachLocation(BOTTOM_LEFT);
                setPosition(1.0f, 0.0f);
                break;
            }
            case Y_AXES_CENTRE: {
                // Line in centre
                lineVerts = {0.0, -1.0, 0.0, 1.0};
                setXYScale(CONSTANT_SIZE, CONSTANT_SCALE);
                setAttachLocation(CENTRE);
                setPosition(0.5f, 0.5f);
                break;
            }
            default: {
                std::cout << "Invalid Axes Direction!" << std::endl;
            }
        }
    }

    void AxesLineTicks::updateSize() {
        switch (axesDirection) {
            case X_AXES_TOP:
            case X_AXES_BOTTOM:
            case X_AXES_CENTRE: {
                ConstantXYDrawable::setSize(1.0f, (float)majorTickLengthPx);
                break;
            }
            case Y_AXES_LEFT:
            case Y_AXES_RIGHT:
            case Y_AXES_CENTRE: {
                ConstantXYDrawable::setSize((float)majorTickLengthPx, 1.0f);
                break;
            }
            default: {
                std::cout << "Invalid Axes Direction!" << std::endl;
            }
        }
    }


    void AxesLineTicks::setMinMax(float newMin, float newMax) {
        valMin = newMin;
        valMax = newMax;
        // Regenerate axes lines
        // TODO
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
        glDrawArrays(GL_LINES,0,4);
        glBindVertexArray(0);
    }

    void AxesLineTicks::Draw() {
        // Draw Axes line
        AxesLineTicks::drawAxesLine();
    }


}