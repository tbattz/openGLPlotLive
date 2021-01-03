//
// Created by bcub3d-desktop on 3/1/21.
//

#ifndef OPENGLPLOTLIVE_PROJ_TOOLTIP_H
#define OPENGLPLOTLIVE_PROJ_TOOLTIP_H


// Project Includes
#include "../texts/TextString.h"


namespace GLPL {


    class Tooltip : public TextString {
    public:
        // Constructor
        Tooltip(const std::string& textString, float x, float y, float fontSize, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();

    private:
        // Shading
        GLuint EBO;
        std::vector<int> internalIndices = {0, 1, 3, 2, 3, 1};
        glm::vec4 tooltipBackgroundColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);

        // Functions
        void setupBackgroundBuffers();
        void drawBackground();

    };
}


#endif //OPENGLPLOTLIVE_PROJ_TOOLTIP_H
