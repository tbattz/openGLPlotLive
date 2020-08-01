//
// Created by bcub3d-desktop on 1/8/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_CONSTANTXYDRAWABLE_H
#define OPENGLPLOTLIVE_PROJ_CONSTANTXYDRAWABLE_H

#include "IDrawable.h"


namespace GLPL {

    enum XYScale {
        CONSTANT_SCALE,
        CONSTANT_SIZE
    };


    class ConstantXYDrawable : public IDrawable {
    public:
        // Constructor
        ConstantXYDrawable(float x, float y, float width, float height,
                            XYScale xScale, XYScale yScale,
                            std::shared_ptr<ParentDimensions> parentDimensions,
                            AttachLocation newAttachLocation = BOTTOM_LEFT);

        // Functions
        void setPosition(float newX, float newY);
        void setSize(float newWidth, float newHeight);
        void updateSizePx();
        void setParentDimensions(glm::mat4 newParentTransform,
                                 int newParentXPx,
                                 int newParentYPx,
                                 int newParentWidthPx,
                                 int newParentHeightPx);
        void setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions);

        void setXScale(XYScale newXScale);
        void setYScale(XYScale newYScale);
        void setXYScale(XYScale newXScale, XYScale newYScale);


        // Data
        XYScale xScale = CONSTANT_SCALE;
        XYScale yScale = CONSTANT_SCALE;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_CONSTANTXYDRAWABLE_H
