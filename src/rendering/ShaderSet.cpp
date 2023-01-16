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
        plot2dLogxShaderPt = std::make_shared<Shader>(plot2dShaderLogx);
        plot2dLogyShaderPt = std::make_shared<Shader>(plot2dShaderLogy);
        plot2dLogxLogyShaderPt = std::make_shared<Shader>(plot2dShaderLogxLogy);
        scatter2dShaderPt = std::make_shared<Shader>(scatter2dShader);
        scatter2dLogxShaderPt = std::make_shared<Shader>(scatter2dLogxShader);
        scatter2dLogyShaderPt = std::make_shared<Shader>(scatter2dLogyShader);
        scatter2dLogxLogyShaderPt = std::make_shared<Shader>(scatter2dLogxLogyShader);
        textureShaderPt = std::make_shared<Shader>(textureShader);
        plotPosNeg2DShaderPt = std::make_shared<Shader>(plotPosNeg2DShader);
        plotTransparent2dShaderPt = std::make_shared<Shader>(plotTransparent2dShader);
        characterLoaderPt = std::make_shared<CharacterLoader>(characterLoader);
        textureMangerPt = std::make_shared<TextureManager>(textureManager);
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

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlot2dLogxShader() {
        return plot2dLogxShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlot2dLogyShader() {
        return plot2dLogyShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlot2dLogxLogyShader() {
        return plot2dLogxLogyShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getScatter2dShader() {
        return scatter2dShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getScatter2dLogxShader() {
        return scatter2dLogxShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getScatter2dLogyShader() {
        return scatter2dLogyShaderPt;
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getScatter2dLogxLogyShader() {
        return scatter2dLogxLogyShaderPt;
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