//
// Created by bcub3d-desktop on 27/3/20.
//

#include "ISingleLine2D.h"



namespace GLPL {

    ISingleLine2D::ISingleLine2D(std::shared_ptr<ParentDimensions> parentDimensions) :
            ILine2D(std::move(parentDimensions)) {

    }

    void ISingleLine2D::createAndSetupBuffers(int dataSizeBytes, const void *dataAddress,
                                              int strideBytes, int glType) {
        /* Create Buffers */
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineEBO);

        /* Setup Buffers */
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineEBO);

        /* Copy data into buffer */
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);

        /* Position Attributes */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, glType, GL_FALSE, strideBytes, (GLvoid *) 0);

        glBindVertexArray(0); // Unbind VAO

    }

    void
    ISingleLine2D::drawData(int nPts) {
        // Draws the data currently stored in the line corresponding to the given VAO
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();
        glLineWidth(lineWidth);
        glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                           glm::value_ptr(*axesViewportTransform));
        glm::vec4 inColor = glm::vec4(colour, 1.0);
        glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
        glBindVertexArray(lineVAO);
        glDrawArrays(mode, 0, nPts);
        glBindVertexArray(0);
        glLineWidth(1);
    }




}