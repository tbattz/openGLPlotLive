//
// Created by tbatt on 19/04/2020.
//

// Free Type Library
#include <ft2build.h>
#include FT_FREETYPE_H

// Standard Includes
#include <cmath>
#include <iostream>
#include <utility>

// Project Includes
#include "CharacterLoader.h"


namespace GLPL {

    CharacterLoader::CharacterLoader(const GLchar *fontPath) {
        // Initialise a font
        // Enable Blending for transparent text background
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Initialise Font
        CharacterLoader::loadFont(fontPath);

        // Load Glyphs
        CharacterLoader::loadGlyphs();

        // Setup the Buffers
        // TODO - Remove?
        CharacterLoader::createAndSetupBuffers();

        // Determine the font scaling
        CharacterLoader::determineScaling();
    }

    Character3 CharacterLoader::getCharacter(const char reqCharacter) {
        return this->Characters[reqCharacter];
    }

    void CharacterLoader::loadFont(const GLchar *fontPath) {
        // Initialise Font
        if(FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << '\n';
        }

        if(FT_New_Face(ft,fontPath,0,&face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << '\n';
        }
        // Set Font Size
        FT_Set_Pixel_Sizes(face,0,48);
    }

    void CharacterLoader::loadGlyphs() {
        // Load first 128 Glyphs
        for(GLubyte c = 0; c < 128; c++) {
            // Load Single glyph
            if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cout << "ERROR:FREETYPE: Failed to load Glyph" << '\n';
                continue;
            }
            // Generate Texture
            GLuint texture;
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Enable text widths that are not multiples of 4
            glGenTextures(1,&texture);
            glBindTexture(GL_TEXTURE_2D,texture);
            glTexImage2D(
                    GL_TEXTURE_2D,0,GL_RED,face->glyph->bitmap.width,face->glyph->bitmap.rows,0,GL_RED,GL_UNSIGNED_BYTE,face->glyph->bitmap.buffer);
            // Set Texture Options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Store characters
            Character3 character3 = {texture,
                                     face->glyph->metrics.width,
                                     face->glyph->metrics.height,
                                     face->glyph->metrics.horiBearingX,
                                     face->glyph->metrics.horiBearingY,
                                     face->glyph->metrics.horiAdvance};

            this->Characters.insert(std::pair<GLchar, Character3>(c, character3));

        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // Clear Resources
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    void CharacterLoader::createAndSetupBuffers() {
        // Configure VAO and VBO for quads
        // TODO - Remove?
        glGenVertexArrays(1,&this->VAO);
        glGenBuffers(1,&this->VBO);
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
        glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*6*4,NULL,GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,4*sizeof(GLfloat),0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    void CharacterLoader::determineScaling() {
        // Get the first monitor resolution
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        auto monitorWidthPixel = (float)mode->width;
        auto monitorHeightPixel = (float)mode->height;
        // Get the physical size of the monitor
        int count;
        int widthMm, heightMm;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        glfwGetMonitorPhysicalSize(monitors[0], &widthMm, &heightMm);
        // 1 pt = 1/72inch = 25.4/72mm
        ptInPixels = (float)(25.4/72) * (monitorHeightPixel / (float)heightMm);
        ptInPerHeight = ptInPixels / monitorHeightPixel;
        // Calculate dpi
        float widthInch = widthMm / inch2Mm;
        float heightInch = heightMm / inch2Mm;
        dpiX = monitorWidthPixel / widthInch;
        dpiY = monitorHeightPixel / heightInch;
    }

    int CharacterLoader::getEmSquareSize() {
        return emSquareSize;
    }

    glm::vec2 CharacterLoader::calcPixelsPerEmSquare(int fontSize) {
        // Calculate the number of pixels required to represent a single em square for this font size
        float pixelSizeX = (float)fontSize * dpiX / 72.;
        float pixelSizeY = (float)fontSize * dpiY / 72.;

        return glm::vec2(pixelSizeX, pixelSizeY);
    }

    glm::ivec2 CharacterLoader::calcFontPixelWidthHeight(int fontSize, int fontWidth, int fontHeight) {
        // Calculate pixel width, height
        glm::vec2 pixelsPerEmSquare = CharacterLoader::calcPixelsPerEmSquare(fontSize);
        int pixelWidth = std::round((float)fontWidth * pixelsPerEmSquare[0] / unitsPerEm);
        int pixelHeight = std::round((float)fontHeight * pixelsPerEmSquare[1] / unitsPerEm);

        return glm::ivec2(pixelWidth, pixelHeight);
    }

    glm::vec2 CharacterLoader::getFontSpaceToPixelSpaceFactor(int fontSize) {
        // Calculate scaling factor from font space to pixel space
        glm::vec2 pixelsPerEmSquare = CharacterLoader::calcPixelsPerEmSquare(fontSize);
        float factorX = pixelsPerEmSquare[0] / unitsPerEm;
        float factorY = pixelsPerEmSquare[1] / unitsPerEm;

        return glm::vec2(factorX, factorY);
    }

    glm::ivec2 CharacterLoader::calcStringWidthHeightInFontCoordinates(std::string newString, int fontSize) {
        // Calculate width and height in font coordinates
        int rowHeight = 0;
        int rowWidth = 0;
        int totalHeight = 0;
        int totalWidth = 0;
        for(unsigned int i=0; i<newString.size(); i++) {
            // Get character
            const char currChar = newString.at(i);
            Character3 ch = this->getCharacter(currChar);
            // Account for multiple lines
            if(currChar == '\n') {
                totalHeight += rowHeight;
                if (rowWidth > totalWidth) {
                    totalWidth = rowWidth;
                }
                rowHeight = 0;
                rowWidth = 0;
            }
            // Increment row dimensions
            if (ch.height > rowHeight) {
                rowHeight = ch.height;
            }
            rowWidth += ch.width;
        }
        // Add last row
        totalHeight += rowHeight;
        if (rowWidth > totalWidth) {
            totalWidth = rowWidth;
        }

        return glm::ivec2(totalWidth, totalHeight);
    }

    /*glm::ivec2 CharacterLoader::getStringDimensions(std::string newString, int fontSize) {
        // Get standard string size
        // Height based maximum height of character in row
        // Width based on width of longest row
        // Calculate width and height in font coordinates
        glm::ivec2 stringFontCoords = CharacterLoader::calcStringWidthHeightInFontCoordinates(std::move(newString), fontSize);

        // Calculate pixel width, height for the given font size
        glm::ivec2 fontPixelSize = CharacterLoader::calcFontPixelWidthHeight(fontSize, stringFontCoords[0], stringFontCoords[1]);

        return fontPixelSize;
    }*/

    TextFontDimensions CharacterLoader::getStringFontDimensions(std::string newString) {
        // Calculate width and height in font coordinates
        int rowHeight = 0;
        int rowWidth = 0;
        int totalHeight = 0;
        int totalWidth = 0;
        int yOffset = 0;
        for(unsigned int i=0; i<newString.size(); i++) {
            // Get character
            const char currChar = newString.at(i);
            Character3 ch = this->getCharacter(currChar);
            // Account for multiple lines
            if(currChar == '\n') {
                totalHeight += rowHeight;
                if (rowWidth > totalWidth) {
                    totalWidth = rowWidth;
                }
                rowHeight = 0;
                rowWidth = 0;
            }
            // Increment row dimensions
            if (ch.height > rowHeight) {
                rowHeight = ch.height;
            }
            rowWidth += ch.advance;
            // Check for larger yOffset
            int charOffset = ch.height - ch.bearingY;
            if (yOffset < charOffset) {
                yOffset = charOffset;
            }
        }
        // Add last row
        totalHeight += rowHeight;
        if (rowWidth > totalWidth) {
            totalWidth = rowWidth;
        }

        // Add offset to total height
        totalHeight += yOffset;

        return TextFontDimensions({totalWidth, totalHeight, yOffset});
    }




    StringDimensions CharacterLoader::calcStringDimensions(std::string textString, float fontSize) {
        // Calculate width and height in font coordinates
        int rowHeight = 0;
        int rowWidth = 0;
        int totalHeight = 0;
        int totalWidth = 0;
        int yOffset = 0;
        for(unsigned int i=0; i<textString.size(); i++) {
            // Get character
            const char currChar = textString.at(i);
            Character3 ch = this->getCharacter(currChar);
            // Account for multiple lines
            if(currChar == '\n') {
                totalHeight += rowHeight;
                if (rowWidth > totalWidth) {
                    totalWidth = rowWidth;
                }
                rowHeight = 0;
                rowWidth = 0;
            }
            // Increment row dimensions
            if (ch.height > rowHeight) {
                rowHeight = ch.height;
            }
            rowWidth += ch.advance;
            // Check for larger yOffset
            int charOffset = ch.height - ch.bearingY;
            if (yOffset < charOffset) {
                yOffset = charOffset;
            }
        }
        // Add last row
        totalHeight += rowHeight;
        if (rowWidth > totalWidth) {
            totalWidth = rowWidth;
        }
        int fontWidth = totalWidth;
        int fontHeight = totalHeight;
        int fontYOffset = yOffset;

        // Calculate scaling factor from font space to pixel space
        glm::vec2 pixelsPerEmSquare = CharacterLoader::calcPixelsPerEmSquare(fontSize);
        float factorX = pixelsPerEmSquare[0] / unitsPerEm;
        float factorY = pixelsPerEmSquare[1] / unitsPerEm;

        float pixelWidth = factorX * (float)fontHeight;
        float pixelHeight = factorY * (float)fontHeight;
        float pixelYOffset = factorY * (float)(fontYOffset);

    }



}