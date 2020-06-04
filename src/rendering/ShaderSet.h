//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_SHADERSET_H
#define OPENGLPLOTLIVE_PROJ_SHADERSET_H

// Standard Includes
#include <memory>

// Project Includes
#include "shader.h"
#include "../texts/CharacterLoader.h"


namespace GLPL {
    // Define Fonts
    #ifdef _WIN32
        #define FONTPATH "C:/Windows/Fonts/Arial.ttf"
    #elif __linux__
        #define FONTPATH "/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf"
    #endif

    // Contains the shaders for plotting
    class ShaderSet {
    public:
        // Constructor
        ShaderSet();
        // Destructor
        ~ShaderSet();

        // Functions
        std::shared_ptr<Shader> getTextShader();
        std::shared_ptr<Shader> getPlot2dShader();
        std::shared_ptr<Shader> getPlotPosNeg2DShader();
        std::shared_ptr<Shader> getPlotTransparent2dShader();
        std::shared_ptr<CharacterLoader> getCharacterLoader();

    private:
        // Shaders
        Shader textShader = Shader("Shaders/font.vs", "Shaders/font.frag");
        Shader plot2dShader = Shader("Shaders/plot2d.vs","Shaders/plot2d.frag");
        Shader plotPosNeg2DShader = Shader("Shaders/plotPosNeg2d.vs", "Shaders/plotPosNeg2d.frag");
        Shader plotTransparent2dShader = Shader("Shaders/plotTransparent2d.vs", "Shaders/plotTransparent2d.frag");

        // Text
        CharacterLoader characterLoader = CharacterLoader(FONTPATH);


    };

}


#endif //OPENGLPLOTLIVE_PROJ_SHADERSET_H
