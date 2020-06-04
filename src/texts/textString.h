//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_TEXTSTRING_H
#define OPENGLPLOTLIVE_PROJ_TEXTSTRING_H

// Project Includes
#include "../rendering/IDrawable.h"


namespace GLPL {
    enum TextAlignment {
        LEFT_TOP,
        LEFT_BOTTOM,
        H_CENTRE,
        V_CENTRE
    };

    class TextString : public IDrawable {
    public:
        // Constructor
        TextString(std::string textString, float x, float y, float fontSize, ParentPointers parentPointers);

        // Functions
        void Draw();
        void setTextString(std::string newTextString);
        void setTextColor(glm::vec3 textColor);
        void setTextAlignment(TextAlignment newTextAlignment);
        void setFontSize(float fontSize);

    private:
        // Functions
        void createAndSetupFontBuffers();
        void updateTextDimensions();
        void generateVertices();
        void drawLeftTopAligned();
        void drawLeftBottomAligned();
        void drawHCentreAligned();
        void drawVCentreAligned();

        // Buffers
        GLuint textVAO, textVBO;

        // Data
        std::shared_ptr<CharacterLoader> characterLoader;
        // Text Attributes
        std::string textString = "";
        float fontSize = 12;
        glm::vec3 textColor = glm::vec3(1.0f);
        TextAlignment textAlignment = LEFT_TOP;
        // Text Dimensions
        TextFontDimensions textFontDimensions;
        glm::vec2 fontPixelFactor = glm::vec2(1.0f);        // Font space to pixel space conversion factors
        glm::vec2 pixelRelativeFactor = glm::vec2(1.0f);    // Pixel space to axes relative space conversion factors, without origin translate
        // Font Polygon Vertices List
        std::vector<std::array<std::array<GLfloat, 4>, 6>> verticesList;    // In Axes relative space

    };
}

#endif //OPENGLPLOTLIVE_PROJ_TEXTSTRING_H
