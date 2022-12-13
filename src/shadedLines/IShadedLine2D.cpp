//
// Created by tbatt on 12/04/2020.
//

#include "IShadedLine2D.h"


namespace GLPL {

    IShadedLine2D::IShadedLine2D(std::shared_ptr<ParentDimensions> parentDimensions) :
        ILine2D(std::move(parentDimensions)) {

    }

    void IShadedLine2D::createAndSetupBuffers(int vertDataSizeBytes, int indicesDataSizeBytes,
            const void *vertDataAddress, const void *indicesDataAddress, int strideBytes, int glType) {
        /* Create Buffers */
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glGenBuffers(1, &lineEBO);

        /* Setup Buffers */
        // VAO
        glBindVertexArray(lineVAO);
        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, vertDataSizeBytes, vertDataAddress, GL_DYNAMIC_DRAW);
        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSizeBytes, indicesDataAddress, GL_DYNAMIC_DRAW);

        /* Position Attributes */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, glType, GL_FALSE, strideBytes, (GLvoid *) 0);

        glBindVertexArray(0); // Unbind VAO
    }


    void IShadedLine2D::createAndSetupLegendBuffers(int vertDataSizeBytes, int indicesDataSizeBytes,
                                              const void *vertDataAddress, const void *indicesDataAddress, int strideBytes, int glType) {
        /* Create Buffers */
        glGenVertexArrays(1, &legendLineVAO);
        glGenBuffers(1, &legendLineVBO);
        glGenBuffers(1, &legendLineEBO);

        /* Setup Buffers */
        // VAO
        glBindVertexArray(legendLineVAO);
        // VBO
        glBindBuffer(GL_ARRAY_BUFFER, legendLineVBO);
        glBufferData(GL_ARRAY_BUFFER, vertDataSizeBytes, vertDataAddress, GL_DYNAMIC_DRAW);
        // EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, legendLineEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSizeBytes, indicesDataAddress, GL_DYNAMIC_DRAW);

        /* Position Attributes */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, glType, GL_FALSE, strideBytes, (GLvoid *) 0);

        glBindVertexArray(0); // Unbind VAO
    }

    void IShadedLine2D::drawData(int numIndices, bool selected) {
        // Draws the data currently stored in the shaded line corresponding to the given VAO
        // The VAO is bound to the EBO from earlier
        // Select Shader
        std::shared_ptr<Shader> shader = selectShader();
        shader->Use();

        glUniform1f(glGetUniformLocation(shader->Program, "logXBase"), (float)logXBase);
        glUniform1f(glGetUniformLocation(shader->Program, "logYBase"), (float)logYBase);
        glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                           glm::value_ptr(*axesViewportTransform));
        glm::vec4 inColor;
        if (selected) {
            inColor = glm::vec4(colour, 1.3*opacityRatio);
        } else {
            inColor = glm::vec4(colour, opacityRatio);
        }

        GLfloat zDepthVal = getZDepthValue();
        glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
        glUniform1f(glGetUniformLocation(shader->Program, "z"), zDepthVal);
        glBindVertexArray(lineVAO);
        glDrawElements(mode, numIndices, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }


    int GLPL::IShadedLine2D::getHoverCursor() {
        return 0;
    }

    std::shared_ptr<Shader> IShadedLine2D::selectShader() {
        std::shared_ptr<Shader> shader;
        if (logX) {
            if (logY) {
                // Both logX and logY
                shader = shaderSetPt->getPlot2dLogxLogyShader();
            } else {
                // LogX only
                shader = shaderSetPt->getPlot2dLogxShader();
            }
        } else {
            if (logY) {
                // LogY only
                shader = shaderSetPt->getPlot2dLogyShader();
            } else {
                // Both Linear
                shader = shaderSetPt->getPlot2dShader();
            }
        }

        return shader;
    }

    void IShadedLine2D::drawLegendArea(glm::mat4 rectOverallTransform, bool selected) {
        // Select Shader
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();

        glUniform1f(glGetUniformLocation(shader->Program, "logXBase"), (float)logXBase);
        glUniform1f(glGetUniformLocation(shader->Program, "logYBase"), (float)logYBase);
        glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                           glm::value_ptr(rectOverallTransform));
        glm::vec4 inColor;
        if (selected) {
            inColor = glm::vec4(colour, 1.3*opacityRatio);
        } else {
            inColor = glm::vec4(colour, opacityRatio);
        }

        GLfloat zDepthVal = getZDepthValue();
        glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
        glUniform1f(glGetUniformLocation(shader->Program, "z"), zDepthVal);
        glBindVertexArray(legendLineVAO);
        glDrawElements(mode, legendIndices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }


    void IShadedLine2D::drawLegendEntry(glm::mat4 rectOverallTransform) {
        if (isSelected()) {
            drawLegendArea(rectOverallTransform, selected);
        }
        drawLegendArea(rectOverallTransform, false);
    }


}