//
// Created by bcub3d-desktop on 3/1/21.
//

#include "Tooltip.h"


namespace GLPL {


    Tooltip::Tooltip(const std::string &textString, float x, float y, float fontSize,
                     std::shared_ptr<ParentDimensions> parentDimensions) : TextString(textString, x, y, fontSize,
                                                                                      std::move(parentDimensions)) {

        // Setup background buffers
        Tooltip::setupBackgroundBuffers();
    }

    void Tooltip::Draw() {
        // Draw background first
        Tooltip::drawBackground();

        // Draw text
        TextString::Draw();

    }

    void Tooltip::setupBackgroundBuffers() {
        unsigned int indicesDataSizeBytes = internalIndices.size()*sizeof(internalIndices[0]);

        glGenBuffers(1, &EBO);

        // Setup Buffers
        // Bind to existing VAO
        glBindVertexArray(VAO);
        // EBO - Send Indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSizeBytes, &internalIndices[0], GL_STATIC_DRAW);

        glBindVertexArray(0); // Unbind VAO
    }

    void Tooltip::drawBackground() {
        // Draws the data currently stored in the shaded line corresponding to the given VAO
        // The VAO is bound to the EBO from earlier
        // Select Shader
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();

        glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        GLfloat zDepthVal = getZDepthValue();
        glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(tooltipBackgroundColor));
        glUniform1f(glGetUniformLocation(shader->Program, "z"), zDepthVal);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, internalIndices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}