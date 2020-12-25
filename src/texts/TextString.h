//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_TEXTSTRING_H
#define OPENGLPLOTLIVE_PROJ_TEXTSTRING_H

// Project Includes
#include "../rendering/ConstantXYDrawable.h"


namespace GLPL {;

    enum TextRotation {
        HORIZONTAL,
        SIDEWAYS_RIGHT,
        UPSIDE_DOWN,
        SIDEWAYS_LEFT
    };

    class TextString : public ConstantXYDrawable {
    public:
        // Constructor
        TextString(const std::string& textString, float x, float y, float fontSize, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();
        std::string getID();
        void setTextString(std::string newTextString);
        std::string getTextString();
        void setTextColor(glm::vec3 textColor);
        void setFontSize(float fontSize);
        void setTextRotation(TextRotation newTextRotation);


    private:
        // Functions
        void createAndSetupFontBuffers();
        void updateTextDimensions();
        void generateVertices();
        void generateVerticesHorizontal();
        void generateVerticesUpsideDown();
        void generateVerticesSidewaysRight();
        void generateVerticesSidewaysLeft();
        void drawText();

        // Buffers
        GLuint textVAO, textVBO;

        // Data
        std::shared_ptr<CharacterLoader> characterLoader;
        // Text Attributes
        std::string textString = "";
        float fontSize = 12;
        glm::vec3 textColor = glm::vec3(1.0f);
        // Text Dimensions
        TextFontDimensions textFontDimensions;
        glm::vec2 fontPixelFactor = glm::vec2(1.0f);        // Font space to pixel space conversion factors
        glm::vec2 pixelRelativeFactor = glm::vec2(1.0f);    // Pixel space to axes relative space conversion factors, without origin translate
        // Font Polygon Vertices List
        std::vector<std::array<std::array<GLfloat, 4>, 6>> verticesList;    // In Axes relative space
        // Rotation
        TextRotation textRotation = HORIZONTAL;

    };
}

#endif //OPENGLPLOTLIVE_PROJ_TEXTSTRING_H
