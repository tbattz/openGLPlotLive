//
// Created by tbatt on 17/06/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_CONSTANTSCALEDRAWABLE_H
#define OPENGLPLOTLIVE_PROJ_CONSTANTSCALEDRAWABLE_H

#include "IDrawable.h"


namespace GLPL {
    class ConstantScaleDrawable : public IDrawable {
    public:
        // Constructor
        ConstantScaleDrawable(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);

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

    };
}


#endif //OPENGLPLOTLIVE_PROJ_CONSTANTSCALEDRAWABLE_H
