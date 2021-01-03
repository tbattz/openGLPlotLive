//
// Created by bcub3d-laptop-dell on 12/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_PRESSBUTTON_H
#define OPENGLPLOTLIVE_PROJ_PRESSBUTTON_H

#include "../rendering/IDrawable.h"
#include "../rendering/ConstantXYDrawable.h"
#include "IClickable.h"
#include "Tooltip.h"


namespace GLPL {

    class PressButton : public ConstantXYDrawable {
    public:
        // Constructor
        PressButton(std::string newButtonName, float x, float y, float width, float height,
                    std::shared_ptr<ParentDimensions> parentDimensions,
                    std::string tooltipText = "");

        // Functions
        void Draw();
        void onLeftClick() override;
        std::string getID();
        void setHovered(bool newHovered);


    private:
        std::string buttonName;

        // Shading
        GLuint EBO;
        std::vector<int> internalIndices = {0, 1, 3, 2, 3, 1};

        // Functions
        void setupShadingBuffers();
        void drawButtonShading();
        void drawButtonOutline();
        void drawTooltip();

        // PressButton Appearance
        glm::vec4 buttonInactiveOutlineColor      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 buttonInactiveOutlineHoverColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 buttonInactiveShadingColor      = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        glm::vec4 buttonInactiveShadingHoverColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        glm::vec4 buttonActiveOutlineColor        = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 buttonActiveOutlineHoverColor   = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 buttonActiveShadingColor        = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);
        glm::vec4 buttonActiveShadingHoverColor   = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

        // Tooltip
        float tooltipHoverTimeS = 0.5; // The hover time until the tooltip is shown, in seconds
        float hoverBeginTime = -1;          // The time at which the button was first hovered for the current hover period
        std::shared_ptr<Tooltip> tooltipPt = nullptr;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_PRESSBUTTON_H
