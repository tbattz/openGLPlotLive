//
// Created by tbatt on 17/06/2020.
//

#include "ConstantSizeDrawable.h"

GLPL::ConstantSizeDrawable::ConstantSizeDrawable(float x, float y, float width, float height,
                                                 const GLPL::ParentDimensions &parentDimensions) :
                                                 IDrawable() {
    // Width and height are in pixels

    // Only set shader and parent attributes if we aren't the top level
    ConstantSizeDrawable::setParentDimensions(parentDimensions);

    // Initialise data
    ConstantSizeDrawable::setPosition(x, y);
    ConstantSizeDrawable::setSize(width, height);
    IDrawable::createAndSetupBuffers();

}

void GLPL::ConstantSizeDrawable::setPosition(float newX, float newY) {
    this->x = newX;
    this->y = newY;
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::ConstantSizeDrawable::setSize(float newWidth, float newHeight) {
    // New width and height in pixels
    // Update in pixels
    this->widthPx = (int)newWidth;
    this->heightPx = (int)newHeight;
    // Update relative width, height to parent
    this->width = (float)widthPx / (float)parentWidthPx;
    this->height = (float)heightPx / (float)parentHeightPx;
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::ConstantSizeDrawable::setParentDimensions(glm::mat4 newParentTransform,
                                                      int newParentWidthPx,
                                                      int newParentHeightPx) {
    this->parentWidthPx = newParentWidthPx;
    this->parentHeightPx = newParentHeightPx;
    this->parentTransform = newParentTransform;
    // When the parent size changes, the relative size of this widget needs to be recalculated, to keep the same pixel size
    ConstantSizeDrawable::setSize((float)getWidthPx(), (float)getHeightPx());
    updateTransforms();
}

void GLPL::ConstantSizeDrawable::setParentDimensions(const ParentDimensions& parentDimensions) {
    this->parentTransform = parentDimensions.parentTransform;
    this->parentWidthPx = parentDimensions.parentWidthPx;
    this->parentHeightPx = parentDimensions.parentHeightPx;
    this->shaderSetPt = parentDimensions.shaderSet;
    // When the parent size changes, the relative size of this widget needs to be recalculated, to keep the same pixel size
    ConstantSizeDrawable::setSize((float)getWidthPx(), (float)getHeightPx());
    updateTransforms();
}