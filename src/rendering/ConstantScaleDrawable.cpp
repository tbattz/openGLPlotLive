//
// Created by tbatt on 17/06/2020.
//

// Standard Includes
#include <utility>

#include "ConstantScaleDrawable.h"


GLPL::ConstantScaleDrawable::ConstantScaleDrawable(float x, float y, float width, float height,
                                                   std::shared_ptr<ParentDimensions> parentDimensions) : IDrawable() {
    // Width and height are relative to their parent
    // Only set shader and parent attributes if we aren't the top level
    ConstantScaleDrawable::setParentDimensions(std::move(parentDimensions));

    // Initialise data
    ConstantScaleDrawable::setPosition(x, y);
    ConstantScaleDrawable::setSize(width, height);
    IDrawable::createAndSetupBuffers();

}

void GLPL::ConstantScaleDrawable::setPosition(float newX, float newY) {
    // Set position
    this->x = newX;
    this->y = newY;
    // Set position in pixels
    IDrawable::updatePositionPx();
    // Update Transforms
    IDrawable::updateTransforms();
    // Update Children
    updateChildren();
}

void GLPL::ConstantScaleDrawable::setSize(float newWidth, float newHeight) {
    // New width and height relative to their parent
    // Update size
    this->width = newWidth;
    this->height = newHeight;
    // Update in pixels
    ConstantScaleDrawable::updateSizePx();
    // Update Transforms
    IDrawable::updateTransforms();
    // Update Children
    updateChildren();
}

void GLPL::ConstantScaleDrawable::updateSizePx() {
    this->widthPx = (int) (this->width * (float)parentWidthPx);
    this->heightPx = (int) (this->height * (float)parentHeightPx);
}

void GLPL::ConstantScaleDrawable::setParentDimensions(glm::mat4 newParentTransform,
                                          int newParentXPx,
                                          int newParentYPx,
                                          int newParentWidthPx,
                                          int newParentHeightPx) {
    this->parentXPx = newParentXPx;
    this->parentYPx = newParentYPx;
    this->parentWidthPx = newParentWidthPx;
    this->parentHeightPx = newParentHeightPx;
    this->parentTransform = newParentTransform;
    updatePositionPx();
    updateSizePx();
    updateTransforms();
    // Update Children
    updateChildren();
}

void GLPL::ConstantScaleDrawable::setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions) {
    this->parentTransform = parentDimensions->parentTransform;
    this->parentXPx = parentDimensions->parentXPx;
    this->parentYPx = parentDimensions->parentYPx;
    this->parentWidthPx = parentDimensions->parentWidthPx;
    this->parentHeightPx = parentDimensions->parentHeightPx;
    this->shaderSetPt = parentDimensions->shaderSet;
    updatePositionPx();
    updateSizePx();
    updateTransforms();
    // Update Children
    updateChildren();
}

