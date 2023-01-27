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
#include "../textures/TextureManager.h"


namespace GLPL {

    // Contains the shaders for plotting
    class ShaderSet {
    public:
        // Constructor
        ShaderSet();

        // Functions
        const GLchar* getFontPath();
        float getXDpiScaling();
        float getYDpiScaling();
        std::shared_ptr<Shader> getTextShader();
        std::shared_ptr<Shader> getPlot2dShader();
        std::shared_ptr<Shader> getPlot2dLogxShader();
        std::shared_ptr<Shader> getPlot2dLogyShader();
        std::shared_ptr<Shader> getPlot2dLogxLogyShader();
        std::shared_ptr<Shader> getScatter2dShader();
        std::shared_ptr<Shader> getScatter2dLogxShader();
        std::shared_ptr<Shader> getScatter2dLogyShader();
        std::shared_ptr<Shader> getScatter2dLogxLogyShader();
        std::shared_ptr<Shader> getTextureShader();
        std::shared_ptr<Shader> getPlotPosNeg2DShader();
        std::shared_ptr<Shader> getPlotTransparent2dShader();
        std::shared_ptr<CharacterLoader> getCharacterLoader();
        std::shared_ptr<TextureManager> getTextureManager();

    private:
        // DPI Scaling
        float xScaleDpi = 1;
        float yScaleDpi = 1;

        // Shaders
        Shader textShader = Shader("Shaders/font.vs", "Shaders/font.frag");
        Shader plot2dShader = Shader("Shaders/plot2d.vs","Shaders/plot2d.frag");
        Shader plot2dShaderLogx = Shader("Shaders/plot2dLogx.vs","Shaders/plot2d.frag");
        Shader plot2dShaderLogy = Shader("Shaders/plot2dLogy.vs","Shaders/plot2d.frag");
        Shader plot2dShaderLogxLogy = Shader("Shaders/plot2dLogxLogy.vs","Shaders/plot2d.frag");
        Shader scatter2dShader = Shader("Shaders/scatter2d.vs","Shaders/scatter2d.frag");
        Shader scatter2dLogxShader = Shader("Shaders/scatter2dLogx.vs","Shaders/scatter2d.frag");
        Shader scatter2dLogyShader = Shader("Shaders/scatter2dLogy.vs","Shaders/scatter2d.frag");
        Shader scatter2dLogxLogyShader = Shader("Shaders/scatter2dLogxLogy.vs","Shaders/scatter2d.frag");
        Shader textureShader = Shader("Shaders/texture.vs", "Shaders/texture.frag");
        Shader plotPosNeg2DShader = Shader("Shaders/plotPosNeg2d.vs", "Shaders/plotPosNeg2d.frag");
        Shader plotTransparent2dShader = Shader("Shaders/plotTransparent2d.vs", "Shaders/plotTransparent2d.frag");

        // shared_ptr
        std::shared_ptr<Shader> textShaderPt;
        std::shared_ptr<Shader> plot2dShaderPt;
        std::shared_ptr<Shader> plot2dLogxShaderPt;
        std::shared_ptr<Shader> plot2dLogyShaderPt;
        std::shared_ptr<Shader> plot2dLogxLogyShaderPt;
        std::shared_ptr<Shader> scatter2dShaderPt;
        std::shared_ptr<Shader> scatter2dLogxShaderPt;
        std::shared_ptr<Shader> scatter2dLogyShaderPt;
        std::shared_ptr<Shader> scatter2dLogxLogyShaderPt;
        std::shared_ptr<Shader> textureShaderPt;
        std::shared_ptr<Shader> plotPosNeg2DShaderPt;
        std::shared_ptr<Shader> plotTransparent2dShaderPt;
        std::shared_ptr<TextureManager> textureMangerPt;


        // Text
        std::shared_ptr<CharacterLoader> characterLoaderPt;
        // Textures
        TextureManager textureManager = TextureManager();

    };

}


#endif //OPENGLPLOTLIVE_PROJ_SHADERSET_H
