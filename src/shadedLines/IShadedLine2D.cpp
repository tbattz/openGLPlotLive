//
// Created by tbatt on 12/04/2020.
//

#include "IShadedLine2D.h"


namespace GLPL {

    IShadedLine2D::IShadedLine2D(std::shared_ptr<ParentDimensions> parentDimensions) :
        ConstantScaleDrawable(0.0f, 0.0f, 1.0f, 1.0f, std::move(parentDimensions)) {

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

    void IShadedLine2D::drawData(int numIndices) {
        // Draws the data currently stored in the shaded line corresponding to the given VAO
        // The VAO is bound to the EBO from earlier
        // Select Shader
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();

        glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                glm::value_ptr(*axesViewportTransform));
        glm::vec4 inColor = glm::vec4(colour, opacityRatio);
        GLfloat zDepthVal = getZDepthValue();
        glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
        glUniform1f(glGetUniformLocation(shader->Program, "z"), zDepthVal);
        glBindVertexArray(lineVAO);
        glDrawElements(mode, numIndices, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    void IShadedLine2D::setShadeColour(glm::vec3 shadeColour) {
        this->colour = shadeColour;
    }

    void IShadedLine2D::setMode(GLenum newMode) {
        this->mode = newMode;
    }

    void IShadedLine2D::setOpacityRatio(float newOpacityRatio) {
        this->opacityRatio = newOpacityRatio;
    }

    glm::vec3 IShadedLine2D::getColour() {
        return this->colour;
    }

    GLenum IShadedLine2D::getMode() {
        return this->mode;
    }


}