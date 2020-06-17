//
// Created by tbatt on 17/06/2020.
//

#include "ConstantScaleDrawable.h"


GLPL::ConstantScaleDrawable::ConstantScaleDrawable(float x, float y, float width, float height,
        const ParentDimensions& parentDimensions) : IDrawable() {
    // Width and height are relative to their parent
    // Only set shader and parent attributes if we aren't the top level
    ConstantScaleDrawable::setParentDimensions(parentDimensions);

    // Initialise data
    ConstantScaleDrawable::setPosition(x, y);
    ConstantScaleDrawable::setSize(width, height);
    IDrawable::createAndSetupBuffers();

}

void GLPL::ConstantScaleDrawable::setPosition(float newX, float newY) {
    this->x = newX;
    this->y = newY;
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::ConstantScaleDrawable::setSize(float newWidth, float newHeight) {
    // New width and height relative to their parent
    // Update size
    this->width = newWidth;
    this->height = newHeight;
    // Update in pixels
    this->widthPx = (int) (this->width * (float)parentWidthPx);
    this->heightPx = (int) (this->height * (float)parentHeightPx);
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::ConstantScaleDrawable::setParentDimensions(glm::mat4 newParentTransform,
                                          int newParentWidthPx,
                                          int newParentHeightPx) {
    this->parentWidthPx = newParentWidthPx;
    this->parentHeightPx = newParentHeightPx;
    this->parentTransform = newParentTransform;
    updateTransforms();
}

void GLPL::ConstantScaleDrawable::setParentDimensions(const ParentDimensions& parentDimensions) {
    this->parentTransform = parentDimensions.parentTransform;
    this->parentWidthPx = parentDimensions.parentWidthPx;
    this->parentHeightPx = parentDimensions.parentHeightPx;
    this->shaderSetPt = parentDimensions.shaderSet;
    updateTransforms();
}

