//
// Created by tbatt on 19/04/2020.
//

#include "ShaderSet.h"

namespace GLPL {
    GLPL::ShaderSet::ShaderSet() {
        // Get DPI Scaling
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        glfwGetMonitorContentScale(monitor, &xScaleDpi, &yScaleDpi);

        // Make shared_ptrs
        textShaderPt = std::make_shared<Shader>(textShader);
        plot2dShaderPt = std::make_shared<Shader>(plot2dShader);
        scatter2dShaderPt = std::make_shared<Shader>(scatter2dShader);
        textureShaderPt = std::make_shared<Shader>(textureShader);
        plotPosNeg2DShaderPt = std::make_shared<Shader>(plotPosNeg2DShader);
        plotTransparent2dShaderPt = std::make_shared<Shader>(plotTransparent2dShader);
        characterLoaderPt = std::make_shared<CharacterLoader>(characterLoader);
        textureMangerPt = std::make_shared<TextureManager>(textureManager);
    }

    GLPL::ShaderSet::~ShaderSet() {

    }


    float ShaderSet::getXDpiScaling() {
        return xScaleDpi;
    }


    float ShaderSet::getYDpiScaling() {
        return yScaleDpi;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getTextShader() {
        return textShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlot2dShader() {
        return plot2dShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getScatter2dShader() {
        return scatter2dShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getTextureShader() {
        return textureShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlotPosNeg2DShader() {
        return plotPosNeg2DShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlotTransparent2dShader() {
        return plotTransparent2dShaderPt;
    }

    std::shared_ptr<CharacterLoader> ShaderSet::getCharacterLoader() {
        return characterLoaderPt;
    }

    std::shared_ptr<TextureManager> ShaderSet::getTextureManager() {
        return textureMangerPt;
    }

}