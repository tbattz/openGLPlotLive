//
// Created by bcub3d-laptop-dell on 23/12/20.
//

#include "PressButtonWithImage.h"

#include <utility>


namespace GLPL {


    PressButtonWithImage::PressButtonWithImage(std::string newButtonName, float x, float y, float width, float height,
                                               std::string newTextureName,
                                               std::shared_ptr<ParentDimensions> parentDimensions,
                                               std::string tooltipText) : PressButton(
            std::move(newButtonName), x, y, width, height, std::move(parentDimensions), std::move(tooltipText)) {

        textureName = std::move(newTextureName);
        textureManager = shaderSetPt->getTextureManager();

        createAndSetupLogoBuffers();
    }

    void PressButtonWithImage::Draw() {
        PressButton::Draw();

        // Bind Texture
        glBindTexture(GL_TEXTURE_2D, textureManager->getTextureId(textureName));

        // Use texture shader
        std::shared_ptr<Shader> shader = shaderSetPt->getTextureShader();
        shader->Use();

        // Setup uniforms
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(logoVAO);

        // Draw logo
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind Arrays
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    std::string PressButtonWithImage::getID() {
        return PressButton::getID();
    }

    void PressButtonWithImage::createAndSetupLogoBuffers() {
        // Create Buffers
        glGenVertexArrays(1, &logoVAO);
        glGenBuffers(1, &logoVBO);
        glGenBuffers(1, &logoEBO);

        // Setup Buffers
        glBindVertexArray(logoVAO);
        glBindBuffer(GL_ARRAY_BUFFER, logoVBO);
        glBufferData(GL_ARRAY_BUFFER, logoVerts.size()*sizeof(logoVerts[0]), &logoVerts[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, logoEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, logoIndices.size()*sizeof(logoIndices[0]), &logoIndices[0], GL_STATIC_DRAW);

        // Position Attributes
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture Coord Attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
        glEnableVertexAttribArray(1);
    }
}