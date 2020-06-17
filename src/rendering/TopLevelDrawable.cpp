//
// Created by tbatt on 17/06/2020.
//

#include "TopLevelDrawable.h"


GLPL::TopLevelDrawable::TopLevelDrawable(float x, float y, int widthPx, int heightPx) : IDrawable() {
    // Set 1-to-1 transform for top level
    parentTransform = glm::mat4(1.0f);

    // Initialise data
    TopLevelDrawable::setPosition(x, y);
    TopLevelDrawable::setSize((float)widthPx, (float)heightPx);
}

void GLPL::TopLevelDrawable::setPosition(float newX, float newY) {
    this->x = newX;
    this->y = newY;
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::TopLevelDrawable::setSize(float newWidth, float newHeight) {
    // Overwrite width with max ratio
    this->width = 1.0f;
    this->height = 1.0f;
    // Set size in pixels
    this->widthPx = (int)newWidth;
    this->heightPx = (int)newHeight;
    // Update Transforms;
    IDrawable::updateTransforms();
}

void GLPL::TopLevelDrawable::setParentDimensions(glm::mat4 newParentTransform,
                                          int newParentWidthPx,
                                          int newParentHeightPx) {
    // This is already the top level
    parentTransform = glm::mat4(1.0f);
    parentWidthPx = getWidthPx();
    parentHeightPx = getHeightPx();
    updateTransforms();
}

void GLPL::TopLevelDrawable::setParentDimensions(const ParentDimensions& parentDimensions) {
    // This is already the top level
    parentTransform = glm::mat4(1.0f);
    parentWidthPx = getWidthPx();
    parentHeightPx = getHeightPx();
    updateTransforms();
}