//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_CHARACTERLOADER_H
#define OPENGLPLOTLIVE_PROJ_CHARACTERLOADER_H

// Standard Includes
#include <map>

// GLAD - Multi Language GL Loader-Generator
#include <glad/glad.h>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Free Type Library
#include <ft2build.h>
#include FT_FREETYPE_H


namespace GLPL {
    // Character Structure
    struct Character {
        GLuint      TextureID; // ID handle of the glyph texture
        glm::ivec2  Size;      // Size of glyph
        glm::ivec2  Bearing;   // Offset from baseline to left/top of glyph
        GLuint      Advance;   // Offset to advance to next glyph
    };

    struct Character3 {
        // See https://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
        GLuint      TextureID;      // ID handle of the glyph texture
        int         width;          // Width of glyph
        int         height;         // Height of glyph
        int         bearingX;       // Horizontal distance to the left of the glyph from the origin
        int         bearingY;       // Vertical distance to the top of the glyph from the origin
        int         advance;        // Distance from the origin to the right edge of the area surrounding the glyph
    };

    struct TextFontDimensions {
        int width;    // Total width
        int height;   // Total height
        int yOffset;  // The offset from the origin line to the bottom left hand corner
    };

    struct TextPixelDimensions {
        float width;    // Total width
        float height;   // Total height
        float yOffset;  // The offset from the origin line to the bottom left hand corner
    };


    struct StringFontDimensions {
        int width;         // Total width in font space
        int height;        // Total height in font space
        int yOffset;       // Vertical offset from top edge to the origin line, in font space
    };

    struct StringPixelDimensions {
        float width;        // Total width in pixels
        float height;       // Total height in pixels
        float yOffset;      // Vertical offset in pixel space
    };

    struct StringDimensions {
        StringFontDimensions fontDims;
        StringPixelDimensions pixelDims;
    };


    class CharacterLoader {
    public:
        // Constructor
        CharacterLoader(const GLchar* fontPath);
        Character3 getCharacter(const char reqCharacter);
        glm::vec2 calcPixelsPerEmSquare(int fontSize);
        glm::ivec2 calcFontPixelWidthHeight(int fontSize, int fontWidth, int fontHeight);
        glm::vec2 getFontSpaceToPixelSpaceFactor(int fontSize);
        glm::ivec2 calcStringWidthHeightInFontCoordinates(std::string newString, int fontSize);
        glm::ivec2 getStringDimensions(std::string newString, int fontSize);
        TextFontDimensions getStringFontDimensions(std::string newString);

        StringFontDimensions calcStringFontDimensions(std::string textString, float fontSize);
        StringDimensions calcStringDimensions(std::string textString, float fontSize);

    private:
        // Data
        std::map<GLchar, Character3> Characters;
        GLuint VAO, VBO;
        // Freetype
        FT_Library ft;
        FT_Face face;
        // Font Size
        float ptInPixels = 1;
        float ptInPerHeight = 1;
        float unitsPerEm = 1;
        float dpiX = 1;
        float dpiY = 1;
        const float inch2Mm = 25.4;

        // Functions
        void loadFont(const GLchar *fontPath);
        void loadGlyphs();
        void createAndSetupBuffers();
        void determineScaling();
    };
}

#endif //OPENGLPLOTLIVE_PROJ_CHARACTERLOADER_H
