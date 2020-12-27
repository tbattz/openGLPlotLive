//
// Created by bcub3d-desktop on 27/3/20.
//

#include <glm/gtx/string_cast.hpp>
#include "ISingleLine2D.h"



namespace GLPL {

    ISingleLine2D::ISingleLine2D(std::shared_ptr<ParentDimensions> parentDimensions) :
            ILine2D(std::move(parentDimensions)) {

    }

    void ISingleLine2D::createAndSetupBuffers(int dataSizeBytes, const void *dataAddress,
                                              int strideBytes, int glType) {
        /* Create Buffers */
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);

        /* Setup Buffers */
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        // Copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);

        /* Position Attributes */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, glType, GL_FALSE, strideBytes, (GLvoid *) 0);

        glBindVertexArray(0); // Unbind VAO

    }

    void ISingleLine2D::createAndSetupBuffers(int vertDataSizeBytes, int indicesDataSizeBytes,
                                              const void *vertDataAddress, const void *indicesDataAddress,
                                              int strideBytes, int glType) {
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


    void ISingleLine2D::drawData(int nPts, bool selected) {
        // Draws the data currently stored in the line corresponding to the given VAO
        if (nPts > 0) {
            std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
            shader->Use();
            glm::vec4 inColor;
            if (!selected) {
                glLineWidth(lineWidth);
                inColor = glm::vec4(colour, 1.0);
            } else {
                glLineWidth(10 * lineWidth);
                inColor = glm::vec4(colour, 0.3);
            }
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                               glm::value_ptr(*axesViewportTransform));
            glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
            glBindVertexArray(lineVAO);
            glDrawElements(mode, nPts, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
            glLineWidth(1);
        }
    }




}