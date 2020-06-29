//
// Created by bcub3d-desktop on 27/3/20.
//

#include "ILine2D.h"



namespace GLPL {

    ILine2D::ILine2D() {

    }

    void ILine2D::createAndSetupBuffers(GLuint *VAOPt, GLuint *VBOPt, int dataSizeBytes, const void *dataAddress,
                                        int strideBytes, int glType) {
        /* Create Buffers */
        glGenVertexArrays(1, VAOPt);
        glGenBuffers(1, VBOPt);

        /* Setup Buffers */
        glBindVertexArray(*VAOPt);
        glBindBuffer(GL_ARRAY_BUFFER, *VBOPt);

        /* Copy data into buffer */
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);

        /* Position Attributes */
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, glType, GL_FALSE, strideBytes, (GLvoid *) 0);

        glBindVertexArray(0); // Unbind VAO

    }

    void
    ILine2D::drawData(Shader shader, glm::mat4 axesLimitViewportTrans, GLuint *VAOPt, glm::vec3 colour, int nPts,
                      GLenum mode) {
        // Draws the data currently stored in the line corresponding to the given VAO
        shader.Use();
        glLineWidth(lineWidth);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "transformViewport"), 1, GL_FALSE,
                           glm::value_ptr(axesLimitViewportTrans));
        glm::vec4 inColor = glm::vec4(colour, 1.0);
        glUniform4fv(glGetUniformLocation(shader.Program, "inColor"), 1, glm::value_ptr(inColor));
        glBindVertexArray(*VAOPt);
        glDrawArrays(mode, 0, nPts);
        glBindVertexArray(0);
        glLineWidth(1);
    }

    void ILine2D::setLineColour(glm::vec3 lineColor) {
        this->colour = lineColor;
    }

    void ILine2D::setMode(GLenum newMode) {
        this->mode = newMode;
    }

    void ILine2D::setLineWidth(unsigned int newLineWidth) {
        this->lineWidth = newLineWidth;
    }

    glm::vec3 ILine2D::getColour() {
        return this->colour;
    }

    GLenum ILine2D::getMode() {
        return this->mode;
    }


}