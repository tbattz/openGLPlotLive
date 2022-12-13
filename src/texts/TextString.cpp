//
// Created by tbatt on 19/04/2020.
//

// Standard Includes
#include <utility>

// Project Includes
#include "TextString.h"


namespace GLPL {

    TextString::TextString(const std::string& textString, float x, float y, float fontSize,
                           std::shared_ptr<ParentDimensions> parentDimensions) :
            ConstantXYDrawable(x, y, 0.0f, 0.0f, CONSTANT_SIZE, CONSTANT_SIZE, std::move(parentDimensions)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

        // Set character loader
        characterLoader = this->shaderSetPt->getCharacterLoader();

        // Store string
        TextString::setTextString(textString);
        TextString::setFontSize(fontSize);

        // Calculate text sizing
        TextString::updateTextDimensions();

        // Setup Buffers
        TextString::createAndSetupFontBuffers();
    }

    void TextString::Draw() {
        // Update text attributes
        TextString::updateTextDimensions();
        // Use text shader
        std::shared_ptr<Shader> shader = this->shaderSetPt->getTextShader();
        shader->Use();
        // Setup Uniforms
        glBindVertexArray(this->textVAO);
        glUniform3f(glGetUniformLocation(shader->Program, "textColor"), textColor.x, textColor.y, textColor.z);
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glActiveTexture(GL_TEXTURE0);

        // Iterate through each character
        drawText();

        // Unbind Arrays
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    std::string TextString::getID() {
        return "TextString:" + std::to_string(x) + ":" + std::to_string(y) + ":" + textString;
    }

    void TextString::setTextString(std::string newTextString) {
        this->textString = std::move(newTextString);

        // Update text dimensions
        TextString::updateTextDimensions();
    }

    std::string TextString::getTextString() {
        return textString;
    }

    void TextString::setTextColor(glm::vec3 newTextColor) {
        this->textColor = newTextColor;
    }

    void TextString::setFontSize(float newFontSize) {
        this->fontSize = newFontSize;

        // Update text dimensions
        TextString::updateTextDimensions();
    }

    void TextString::setTextRotation(TextRotation newTextRotation) {
        textRotation = newTextRotation;

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

        // Get number of pixels per em square
        glm::vec2 pixelPerEm = characterLoader->calcPixelsPerEmSquare((int)this->fontSize);

        // Calculate the number of em squares for the total text size
        auto emSquareSize = (double)characterLoader->getEmSquareSize();
        double emWidth = textFontDimensions.width / emSquareSize;
        double emHeight = (textFontDimensions.height) / emSquareSize;

        // Calculate the size of the text in pixels
        float xScaleDpi = this->shaderSetPt->getXDpiScaling();
        float yScaleDpi = this->shaderSetPt->getYDpiScaling();
        auto pixelWidth = (float)(pixelPerEm[0] * emWidth / xScaleDpi);
        auto pixelHeight = (float)(pixelPerEm[1] * emHeight / yScaleDpi);
        switch (textRotation) {
            case HORIZONTAL:
            case UPSIDE_DOWN: {
                ConstantXYDrawable::setSize(pixelWidth, pixelHeight);
                break;
            }
            case SIDEWAYS_LEFT:
            case SIDEWAYS_RIGHT: {
                ConstantXYDrawable::setSize(pixelHeight, pixelWidth);
                break;
            }
            default: {
                std::cout << "Incorrect text rotation specified!" << std::endl;
            }
        }

        // Generate Vertices
        TextString::generateVertices();
    }

    void TextString::generateVertices() {
        // Generate vertices for top left alignment
        switch (textRotation) {
            case HORIZONTAL: {
                TextString::generateVerticesHorizontal();

                break;
            }
            case UPSIDE_DOWN: {
                TextString::generateVerticesUpsideDown();

                break;
            }
            case SIDEWAYS_RIGHT: {
                TextString::generateVerticesSidewaysRight();

                break;
            }
            case SIDEWAYS_LEFT: {
                TextString::generateVerticesSidewaysLeft();

                break;
            }
            default: {
                std::cout << "Incorrect text rotation specified!" << std::endl;
            }
        }
    }

    void TextString::generateVerticesHorizontal() {
        // Clear previous vertices
        verticesList.clear();

        int xOrigin = 0;
        int yOrigin = 0;
        float xPos = 0;
        float yPos = 0;
        float w = 0;
        float h = 0;

        // Setup Origin
        xOrigin = 0;
        yOrigin = textFontDimensions.yOffset;

        // Draw glyphs
        for(char c : textString) {
            // Get the corresponding character
            std::shared_ptr<Character3> ch = characterLoader->getCharacter(c);
            // Calculate Top Left Position
            xPos = (float)xOrigin + (float)ch->bearingX;
            yPos = (float)yOrigin - (float)(ch->height - ch->bearingY);
            w = (float)(ch->width);
            h = (float)(ch->height);

            // Conversions - Normalise by font coordinate width and height
            float xPosRel = xPos/(float)textFontDimensions.width;
            float wRel = w/(float)textFontDimensions.width;
            float yPosRel = yPos/(float)textFontDimensions.height;
            float hRel = h/(float)textFontDimensions.height;

            // Calculate vertices for current character
            // Scale from (0 to 1) to (-1 to 1)
            std::array<std::array<GLfloat, 4>, 6> vertices{};
            vertices = {{
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel + hRel)-1.0f,     0.0f, 0.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel)-1.0f,            0.0f, 1.0f},
                                {2.0f*(xPosRel + wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            1.0f, 1.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel + hRel)-1.0f,     0.0f, 0.0f},
                                {2.0f*(xPosRel + wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            1.0f, 1.0f},
                                {2.0f*(xPosRel + wRel)-1.0f,    2.0f*(yPosRel + hRel)-1.0f,     1.0f, 0.0f}
                        }};

            // Store vertices
            verticesList.push_back(vertices);

            // Update origin
            xOrigin += ch->advance;
        }
    }


    void TextString::generateVerticesUpsideDown() {
        // Clear previous vertices
        verticesList.clear();

        int xOrigin = 0;
        int yOrigin = 0;
        float xPos = 0;
        float yPos = 0;
        float w = 0;
        float h = 0;

        // Setup Origin
        xOrigin = textFontDimensions.width;
        yOrigin = textFontDimensions.height - textFontDimensions.yOffset;

        // Draw glyphs
        for(char c : textString) {
            // Get the corresponding character
            std::shared_ptr<Character3> ch = characterLoader->getCharacter(c);
            // Calculate Top Left Position
            xPos = (float)xOrigin - (float)ch->bearingX;
            yPos = (float)yOrigin + (float)(ch->height - ch->bearingY);
            w = (float)(ch->width);
            h = (float)(ch->height);

            // Conversions - Normalise by font coordinate width and height
            float xPosRel = xPos/(float)textFontDimensions.width;
            float wRel = w/(float)textFontDimensions.width;
            float yPosRel = yPos/(float)textFontDimensions.height;
            float hRel = h/(float)textFontDimensions.height;

            // Calculate vertices for current character
            // Scale from (0 to 1) to (-1 to 1)
            std::array<std::array<GLfloat, 4>, 6> vertices{};
            vertices = {{
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel - hRel)-1.0f,     0.0f, 0.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel)-1.0f,            0.0f, 1.0f},
                                {2.0f*(xPosRel - wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            1.0f, 1.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel - hRel)-1.0f,     0.0f, 0.0f},
                                {2.0f*(xPosRel - wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            1.0f, 1.0f},
                                {2.0f*(xPosRel - wRel)-1.0f,    2.0f*(yPosRel - hRel)-1.0f,     1.0f, 0.0f}
                        }};

            // Store vertices
            verticesList.push_back(vertices);

            // Update origin
            xOrigin -= ch->advance;
        }
    }

    void TextString::generateVerticesSidewaysRight() {
        // Clear previous vertices
        verticesList.clear();

        int xOrigin = 0;
        int yOrigin = 0;
        float xPos = 0;
        float yPos = 0;
        float w = 0;
        float h = 0;

        // Setup Origin
        xOrigin = textFontDimensions.height - textFontDimensions.yOffset;
        yOrigin = 0;

        // Draw glyphs
        for(char c : textString) {
            // Get the corresponding character
            std::shared_ptr<Character3> ch = characterLoader->getCharacter(c);
            // Calculate Top Left Position
            xPos = (float)xOrigin - (float)ch->bearingY;
            yPos = (float)yOrigin + (float)ch->bearingX;
            w = (float)(ch->height);
            h = (float)(ch->width);

            // Conversions - Normalise by font coordinate width and height
            float xPosRel = xPos/(float)textFontDimensions.height;
            float wRel = w/(float)textFontDimensions.height;
            float yPosRel = yPos/(float)textFontDimensions.width;
            float hRel = h/(float)textFontDimensions.width;

            // Calculate vertices for current character
            // Scale from (0 to 1) to (-1 to 1)
            std::array<std::array<GLfloat, 4>, 6> vertices{};
            vertices = {{
                                {2.0f*(xPosRel + wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            0.0f, 1.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel)-1.0f,            0.0f, 0.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel + hRel)-1.0f,     1.0f, 0.0f},
                                {2.0f*(xPosRel + wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            0.0f, 1.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel + hRel)-1.0f,     1.0f, 0.0f},
                                {2.0f*(xPosRel + wRel)-1.0f,    2.0f*(yPosRel + hRel)-1.0f,     1.0f, 1.0f}
                        }};

            // Store vertices
            verticesList.push_back(vertices);

            // Update origin
            yOrigin += ch->advance;
        }
    }

    void TextString::generateVerticesSidewaysLeft() {
        // Clear previous vertices
        verticesList.clear();

        int xOrigin = 0;
        int yOrigin = 0;
        float xPos = 0;
        float yPos = 0;
        float w = 0;
        float h = 0;

        // Setup Origin
        xOrigin = textFontDimensions.yOffset;
        yOrigin = textFontDimensions.width;

        // Draw glyphs
        for(char c : textString) {
            // Get the corresponding character
            std::shared_ptr<Character3> ch = characterLoader->getCharacter(c);
            // Calculate Top Left Position
            xPos = (float)xOrigin + (float)ch->bearingY;
            yPos = (float)yOrigin - (float)ch->bearingX;
            w = (float)(ch->height);
            h = (float)(ch->width);

            // Conversions - Normalise by font coordinate width and height
            float xPosRel = xPos/(float)textFontDimensions.height;
            float wRel = w/(float)textFontDimensions.height;
            float yPosRel = yPos/(float)textFontDimensions.width;
            float hRel = h/(float)textFontDimensions.width;

            // Calculate vertices for current character
            // Scale from (0 to 1) to (-1 to 1)
            std::array<std::array<GLfloat, 4>, 6> vertices{};
            vertices = {{
                                {2.0f*(xPosRel - wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            0.0f, 1.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel)-1.0f,            0.0f, 0.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel - hRel)-1.0f,     1.0f, 0.0f},
                                {2.0f*(xPosRel - wRel)-1.0f,    2.0f*(yPosRel)-1.0f,            0.0f, 1.0f},
                                {2.0f*(xPosRel)-1.0f,           2.0f*(yPosRel - hRel)-1.0f,     1.0f, 0.0f},
                                {2.0f*(xPosRel - wRel)-1.0f,    2.0f*(yPosRel - hRel)-1.0f,     1.0f, 1.0f}
                        }};

            // Store vertices
            verticesList.push_back(vertices);

            // Update origin
            yOrigin -= ch->advance;
        }
    }


    void TextString::drawText() {
        // Draw glyphs
        for(unsigned int i=0; i < textString.size(); i++) {
            // Get the corresponding character
            const char c = (textString[i]);
            std::shared_ptr<Character3> ch = characterLoader->getCharacter(c);

            // Render glyph texture over quad face
            glBindVertexArray(textVAO);
            glBindTexture(GL_TEXTURE_2D, ch->TextureID);
            // Update VBO memory
            glBindBuffer(GL_ARRAY_BUFFER,this->textVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesList[i]), &verticesList[i], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            // Render quad face
            glDrawArrays(GL_TRIANGLES,0,6);
            glBindVertexArray(0);
        }

        if (isSelected()) {
            drawMouseOverBox();
        }
    }

}