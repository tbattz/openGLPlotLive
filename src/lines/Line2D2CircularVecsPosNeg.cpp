//
// Created by tbatt on 11/04/2020.
//

#include "Line2D2CircularVecsPosNeg.h"

namespace GLPL {
    Line2D2CircularVecsPosNeg::Line2D2CircularVecsPosNeg(std::vector<float>* dataPtX, std::vector<float>* dataPtY,
                                                         glm::vec4 posColor, glm::vec4 negColor, GLenum mode) :
            Line2D2CircularVecs(dataPtX, dataPtY, mode) {
        this->colPos = posColor;
        this->colNeg = negColor;
    }

    Line2D2CircularVecsPosNeg::~Line2D2CircularVecsPosNeg() {

    }

    void Line2D2CircularVecsPosNeg::Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans) {
        // Check if the number of points changed
        if (updated) {
            int newPts = (internalData).size()/2;
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]), &internalData[0], GL_DYNAMIC_DRAW);
            updated = false;
        }

        // Draw plot
        // Draws the data currently stored in the line corresponding to the given VAO
        shader.Use();
        glLineWidth(lineWidth);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "transformViewport"), 1, GL_FALSE,
                           glm::value_ptr(axesLimitViewportTrans));
        glUniform4fv(glGetUniformLocation(shader.Program, "colPos"), 1, glm::value_ptr(colPos));
        glUniform4fv(glGetUniformLocation(shader.Program, "colNeg"), 1, glm::value_ptr(colNeg));
        glBindVertexArray(VAO);
        glDrawArrays(mode, 0, nPts);
        glBindVertexArray(0);
        glLineWidth(1);
    }

}