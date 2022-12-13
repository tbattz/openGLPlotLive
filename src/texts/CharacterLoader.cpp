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

    std::shared_ptr<Character3> CharacterLoader::getCharacter(const char reqCharacter) {
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
            std::shared_ptr<Character3> character3b = std::make_shared<Character3>(Character3{texture,
                                     (int)face->glyph->metrics.width,
                                     (int)face->glyph->metrics.height,
                                     (int)face->glyph->metrics.horiBearingX,
                                     (int)face->glyph->metrics.horiBearingY,
                                     (int)face->glyph->metrics.horiAdvance});

            this->Characters.insert(std::pair<GLchar, std::shared_ptr<Character3>>(c, character3b));

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
        float widthInch = (float)widthMm / inch2Mm;
        float heightInch = (float)heightMm / inch2Mm;
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
            std::shared_ptr<Character3> ch = this->getCharacter(currChar);
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
            if (ch->height > rowHeight) {
                rowHeight = ch->height;
            }
            rowWidth += ch->advance;
            // Check for larger yOffset
            int charOffset = ch->height - ch->bearingY;
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

}