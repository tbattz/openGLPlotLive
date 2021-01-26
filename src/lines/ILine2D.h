//
// Created by bcub3d-desktop on 30/6/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_ILINE2D_H
#define OPENGLPLOTLIVE_PROJ_ILINE2D_H

// Project Includes
#include "../axes/Plotable.h"
#include "lineColours.h"
#include "../util/util.h"


namespace GLPL {
    class ILine2D : public Plotable {
    public:
        ILine2D(std::shared_ptr<ParentDimensions> parentDimensions);

        void setLineColour(glm::vec3 lineColor);
        void setMode(GLenum newMode);
        void setLineWidth(unsigned int newLineWidth);
        glm::vec3 getColour();
        GLenum getMode();
        void setOpacityRatio(float newOpacityRatio);
        int getHoverCursor() override;

        virtual std::vector<float> getMinMax(bool onlyPositiveX, bool onlyPositiveY) = 0;

    protected:
        // Data
        unsigned int lineWidth = 1;
        glm::vec3 colour = LC_WHITE;
        GLenum mode; // Mode, line or points
        float opacityRatio = 1.0;
    };
}


#endif //OPENGLPLOTLIVE_PROJ_ILINE2D_H
