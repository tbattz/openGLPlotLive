//
// Created by bcub3d-laptop-dell on 23/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_PRESSBUTTONWITHIMAGE_H
#define OPENGLPLOTLIVE_PROJ_PRESSBUTTONWITHIMAGE_H

// Project Includes
#include "PressButton.h"
#include "../textures/TextureManager.h"


namespace GLPL {

    class PressButtonWithImage : public PressButton {
    public:
        // Constructor
        PressButtonWithImage(std::string newButtonName, float x, float y, float width, float height,
                             std::string textureName,
                             std::shared_ptr<ParentDimensions> parentDimensions,
                             std::string tooltipText = "");

        // Functions
        void Draw() override;
        std::string getID() override;

    private:
        // Buffers
        unsigned int logoVAO, logoVBO, logoEBO;

        // Image                          // Positions        // Texture Coords
        std::vector<GLfloat> logoVerts = { -1.0f, -1.0f,      0.0f, 0.0f,  // bottom left
                                           1.0f, -1.0f,       1.0f, 0.0f,  // bottom right
                                           1.0f,  1.0f,       1.0f, 1.0f,  // top right
                                           -1.0f, 1.0f,       0.0f, 1.0f}; // top left
        std::vector<unsigned int> logoIndices = {0, 1, 3,   // First triangle
                                                 1, 2, 3};  // Second triangle
        std::string textureName;

        // Texture Manager
        std::shared_ptr<GLPL::TextureManager> textureManager;

        // Functions
        void createAndSetupLogoBuffers();

    };

}


#endif //OPENGLPLOTLIVE_PROJ_PRESSBUTTONWITHIMAGE_H
