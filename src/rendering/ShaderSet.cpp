//
// Created by tbatt on 19/04/2020.
//

#include "ShaderSet.h"

namespace GLPL {
    GLPL::ShaderSet::ShaderSet() {
        //characterLoader.getStringDimensions("1", 12);

    }

    GLPL::ShaderSet::~ShaderSet() {

    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getTextShader() {
        return std::make_shared<Shader>(textShader);
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlot2dShader() {
        return std::make_shared<Shader>(plot2dShader);
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlotPosNeg2DShader() {
        return std::make_shared<Shader>(plotPosNeg2DShader);
    }

    std::shared_ptr<Shader> GLPL::ShaderSet::getPlotTransparent2dShader() {
        return std::make_shared<Shader>(plotTransparent2dShader);
    }

    std::shared_ptr<CharacterLoader> ShaderSet::getCharacterLoader() {
        return std::make_shared<CharacterLoader>(characterLoader);
    }
}