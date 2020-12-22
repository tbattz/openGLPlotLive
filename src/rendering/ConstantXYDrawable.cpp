//
// Created by bcub3d-desktop on 1/8/20.
//

#include "ConstantXYDrawable.h"

namespace GLPL {
    ConstantXYDrawable::ConstantXYDrawable(float x, float y, float width, float height,
                                            XYScale xScale, XYScale yScale,
                                            std::shared_ptr<ParentDimensions> parentDimensions,
                                            AttachLocation newAttachLocation): IDrawable() {
        // Width and height are dependent on xScale and yScale respectively.
        // CONSTANT_SCALE - Input is relative to the parent, 0.0 to 1.0
        // CONSTANT_SIZE - Input is in pixels

        // Set attach location
        IDrawable::setAttachLocation(newAttachLocation);

        // Set Parent Dimensions
        ConstantXYDrawable::setParentDimensions(std::move(parentDimensions));

        // Set XYScales
        this->xScale = xScale;
        this->yScale = yScale;

        // Initialise Data
        ConstantXYDrawable::setPosition(x, y);
        ConstantXYDrawable::setSize(width, height);
        IDrawable::createAndSetupBuffers();

    }

    void ConstantXYDrawable::setPosition(float newX, float newY) {
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

    void ConstantXYDrawable::setSize(float newWidth, float newHeight) {
        // New width and height dependent on xScale, yScale
        // Update width
        switch(xScale) {
            case CONSTANT_SIZE: {
                widthPx = (int)newWidth;
                width = (float)widthPx / float(parentWidthPx);
                break;
            }
            case CONSTANT_SCALE: {
                width = newWidth;
                widthPx = (int)(width * (float)parentWidthPx);
                break;
            }
            default: {
                std::cout << "Incorrect scale specified for xScale!" << std::endl;
            }
        }
        // Update height
        switch(yScale) {
            case CONSTANT_SIZE: {
                heightPx = (int)newHeight;
                height = (float)heightPx / (float)parentHeightPx;
                break;
            }
            case CONSTANT_SCALE: {
                height = newHeight;
                heightPx = (int)(height * (float)parentHeightPx);
                break;
            }
            default: {
                std::cout << "Incorrect scale specified for yScale!" << std::endl;
            }
        }
        // Update Transforms
        IDrawable::updateTransforms();
        // Update Children
        updateChildren();
    }

    void ConstantXYDrawable::updateSizePx() {
        if (xScale == CONSTANT_SIZE && yScale == CONSTANT_SIZE) {
            ConstantXYDrawable::setSize((float)getWidthPx(), (float)getHeightPx());
        } else if (xScale == CONSTANT_SIZE && yScale == CONSTANT_SCALE) {
            ConstantXYDrawable::setSize((float)getWidthPx(), getHeight());
        } else if (xScale == CONSTANT_SCALE && yScale == CONSTANT_SIZE) {
            ConstantXYDrawable::setSize(getWidth(), (float)getHeightPx());
        } else if (xScale == CONSTANT_SCALE && yScale == CONSTANT_SCALE) {
            ConstantXYDrawable::setSize(getWidth(), getHeight());
        }
    }

    void ConstantXYDrawable::setParentDimensions(glm::mat4 newParentTransform,
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

    void ConstantXYDrawable::setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions) {
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

    void ConstantXYDrawable::setXScale(XYScale newXScale) {
        xScale = newXScale;
        ConstantXYDrawable::updateSizePx();
    }

    void ConstantXYDrawable::setYScale(XYScale newYScale) {
        yScale = newYScale;
        ConstantXYDrawable::updateSizePx();
    }

    void ConstantXYDrawable::setXYScale(XYScale newXScale, XYScale newYScale) {
        xScale = newXScale;
        yScale = newYScale;
        ConstantXYDrawable::updateSizePx();
    }


}