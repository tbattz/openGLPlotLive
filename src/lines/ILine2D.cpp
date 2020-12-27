//
// Created by bcub3d-desktop on 30/6/20.
//

#include "ILine2D.h"


namespace GLPL {

    ILine2D::ILine2D(std::shared_ptr<ParentDimensions> parentDimensions) : Plotable(std::move(parentDimensions)) {};

    void ILine2D::setLineColour(glm::vec3 lineColor) {
        this->colour = lineColor;
    }

    void ILine2D::setMode(GLenum newMode) {
        this->mode = newMode;
    }

    void ILine2D::setLineWidth(unsigned int newLineWidth) {
        this->lineWidth = newLineWidth;
    }

    glm::vec3 ILine2D::getColour() {
        return this->colour;
    }

    GLenum ILine2D::getMode() {
        return this->mode;
    }

    void ILine2D::setOpacityRatio(float newOpacityRatio) {
        this->opacityRatio = newOpacityRatio;
    }

    int GLPL::ILine2D::getHoverCursor() {
        return 0;
    }

}
