//
// Created by bcub3d-laptop-dell on 23/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_TEXTUREMANAGER_H
#define OPENGLPLOTLIVE_PROJ_TEXTUREMANAGER_H

// Standard Includes
#include <unordered_map>
#include <vector>


namespace GLPL {

    class TextureManager {
    public:
        // Constructor
        TextureManager();

        // Functions
        void loadTexture(const std::string& textureName, const char* texturePath);
        unsigned int getTextureId(std::string textureName);

    private:
        // Data
        std::unordered_map<std::string, unsigned int> textureMap;

        // Functions
        std::vector<std::string> getFilesInDir();
        bool hasExt(const std::string &str, const std::string &suffix);
        void loadPngTextures();
    };

}

#endif //OPENGLPLOTLIVE_PROJ_TEXTUREMANAGER_H
