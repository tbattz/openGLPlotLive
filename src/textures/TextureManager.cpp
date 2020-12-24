//
// Created by bcub3d-laptop-dell on 23/12/20.
//

// Opengl Includes
#include <glad/glad.h>

// Texture Loading Includes
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Includes
#include <iostream>

// Project Includes
#include "TextureManager.h"

#include <unistd.h>



namespace GLPL {


    GLPL::TextureManager::TextureManager() {
        TextureManager::loadTexture("interactor-white", "textures/interactor-white.jpg");
    }

    void TextureManager::loadTexture(std::string textureName, const char* texturePath) {
        // Bind to new texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

        // Cheek Result
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture! - " << texturePath << std::endl;
        }

        // Free the data
        stbi_image_free(data);

        // Store in map
        textureMap.insert(std::pair<std::string, unsigned int>(textureName, texture));

    }

    unsigned int TextureManager::getTextureId(std::string textureName) {
        if (textureMap.count(textureName) > 0) {
            return textureMap.at(textureName);
        } else {
            std::cout << "Texture " << textureName << " not found in texture map!" << std::endl;
            return 0;
        }
    }
}
