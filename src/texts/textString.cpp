//
// Created by tbatt on 19/04/2020.
//

#include "textString.h"

#include <utility>

namespace GLPL {

    TextString::TextString(std::string textString, float x, float y, float fontSize,
                           ParentPointers parentPointers) :
            IDrawable(x, y, 0.0f, 0.0f, std::move(parentPointers)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

        // Set character loader
        characterLoader = this->shaderSetPt->getCharacterLoader();

        // Store string
        TextString::setTextString(textString);
        TextString::setFontSize(fontSize);

        // Calculate position, offset to account for glyphs that go below the line, in font coordinates
        textFontDimensions = characterLoader->getStringFontDimensions(textString);

        // Calculate text dimensions in pixels
        IDrawable::setSizePx(fontPixelFactor[0] * textFontDimensions.width, fontPixelFactor[1] * textFontDimensions.height);

        // Setup Buffers
        TextString::createAndSetupFontBuffers();
    }

    void TextString::Draw() {
        // Use text shader
        std::shared_ptr<Shader> shader = this->shaderSetPt->getTextShader();
        shader->Use();
        // Setup Uniforms
        glUniform3f(glGetUniformLocation(shader->Program, "textColor"), textColor.x, textColor.y, textColor.z);
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(this->textVAO);

        // Iterate through each character
        switch (textAlignment) {
            case LEFT_TOP: {
                drawLeftTopAligned();
                break;
            }
            case LEFT_BOTTOM: {
                drawLeftBottomAligned();
                break;
            }
            case H_CENTRE: {
                drawHCentreAligned();
                break;
            }
            case V_CENTRE: {
                drawVCentreAligned();
                break;
            }
            default: {
                std::cout << "Unknown Text Alignment Enum!" << std::endl;
            }
        }

        // Unbind Arrays
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    void TextString::setTextString(std::string newTextString) {
        this->textString = std::move(newTextString);

        // Update text dimensions
        TextString::updateTextDimensions();
    }

    void TextString::setTextColor(glm::vec3 newTextColor) {
        this->textColor = newTextColor;
    }

    void TextString::setTextAlignment(TextAlignment newTextAlignment) {
        this->textAlignment = newTextAlignment;
    }

    void TextString::setFontSize(float newFontSize) {
        this->fontSize = newFontSize;

        // Update text dimensions
        TextString::updateTextDimensions();
    }

    void TextString::createAndSetupFontBuffers() {
        // Configure VAO and VBO for quads
        glGenVertexArrays(1,&this->textVAO);
        glGenBuffers(1,&this->textVBO);
        glBindVertexArray(this->textVAO);
        glBindBuffer(GL_ARRAY_BUFFER,this->textVBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*6*4,NULL,GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    void TextString::updateTextDimensions() {
        // Calculate new dimensions for string
        textFontDimensions = characterLoader->getStringFontDimensions(textString);

        // Convert font space to pixel space
        fontPixelFactor = characterLoader->getFontSpaceToPixelSpaceFactor(this->fontSize);
        IDrawable::setSizePx((int)(fontPixelFactor[0] * (float)textFontDimensions.width),
                             (int)(fontPixelFactor[1] * (float)textFontDimensions.height));

        // Convert pixel space to axes relative space
        pixelRelativeFactor = glm::vec2(1./(*parentWidthPx), 1./(*parentHeightPx));

        // Generate Vertices
        TextString::generateVertices();
    }

    void TextString::generateVertices() {
        // Generate vertices for top left alignment
        int xOrigin = 0;
        int yOrigin = 0;
        float xPos = 0;
        float yPos = 0;
        float w = 0;
        float h = 0;

        // Clear previous vertices
        verticesList.clear();

        // Get largest negative y offset
        yOrigin = -textFontDimensions.yOffset;




        // Draw glyphs
        for(unsigned int i=0; i < textString.size(); i++) {
            // Get the corresponding character
            const char c = (textString[i]);
            Character3 ch = characterLoader->getCharacter(c);
            // Calculate Top Left Position
            xPos = (float)xOrigin + (float)ch.bearingX;
            yPos = (float)yOrigin - (float)(ch.height - ch.bearingY);
            w = (float)(ch.width);
            h = (float)(ch.height);

            // Conversions - Normalise by font coordinate width and height
            /*float xPosRel = (pixelRelativeFactor[0] * fontPixelFactor[0] * xPos) + x;
            float wRel = pixelRelativeFactor[0] * fontPixelFactor[0] * w;
            float yPosRel = (pixelRelativeFactor[1] * fontPixelFactor[1] * yPos) + y;
            float hRel = pixelRelativeFactor[1] * fontPixelFactor[1] * h;*/
            float xPosRel = xPos/textFontDimensions.width;
            float wRel = w/textFontDimensions.width;
            float yPosRel = yPos/textFontDimensions.height;
            float hRel = h/textFontDimensions.height;

            if (i < 1) {
                std::cout << xPosRel << ", " << yPosRel << ", " << wRel << ", " << hRel << std::endl;
            }

            // Calculate vertices for current character
            std::array<std::array<GLfloat, 4>, 6> vertices = {{
                    {xPosRel,           yPosRel + hRel,     0.0, 0.0},
                    {xPosRel,           yPosRel,            0.0, 1.0},
                    {xPosRel + wRel,    yPosRel,            1.0, 1.0},
                    {xPosRel,           yPosRel + hRel,     0.0, 0.0},
                    {xPosRel + wRel,    yPosRel,            1.0, 1.0},
                    {xPosRel + wRel,    yPosRel + hRel,     1.0, 0.0}
            }};

            // Store vertices
            verticesList.push_back(vertices);

            // Update origin
            xOrigin += ch.advance;
        }
    }

    void TextString::drawLeftTopAligned() {
        // Draw glyphs
        for(unsigned int i=0; i < textString.size(); i++) {
            // Get the corresponding character
            const char c = (textString[i]);
            Character3 ch = characterLoader->getCharacter(c);

            // Render glyph texture over quad face
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update VBO memory
            glBindBuffer(GL_ARRAY_BUFFER,this->textVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesList[i]), &verticesList[i], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            // Render quad face
            glDrawArrays(GL_TRIANGLES,0,6);
        }
    }

    void TextString::drawLeftBottomAligned() {

    }

    void TextString::drawHCentreAligned() {

    }

    void TextString::drawVCentreAligned() {

    }

}