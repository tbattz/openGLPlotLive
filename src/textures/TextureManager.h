//
// Created by bcub3d-laptop-dell on 23/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_TEXTUREMANAGER_H
#define OPENGLPLOTLIVE_PROJ_TEXTUREMANAGER_H

// Standard Includes
#include <unordered_map>



namespace GLPL {

    class TextureManager {
    public:
        // Constructor
        TextureManager();

        // Functions
        void loadTexture(std::string textureName, const char* texturePath);
        unsigned int getTextureId(std::string textureName);

    private:
        std::unordered_map<std::string, unsigned int> textureMap;
    };

}

#endif //OPENGLPLOTLIVE_PROJ_TEXTUREMANAGER_H
