//
// Created by tbatt on 17/06/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_CONSTANTSIZEDRAWABLE_H
#define OPENGLPLOTLIVE_PROJ_CONSTANTSIZEDRAWABLE_H

#include "IDrawable.h"


namespace GLPL {
    class ConstantSizeDrawable : public IDrawable {
    public:
        // Constructor
        ConstantSizeDrawable(float x, float y, float width, float height, const ParentDimensions& parentDimensions);

        // Functions
        void setPosition(float newX, float newY);
        void setSize(float newWidth, float newHeight);
        void setParentDimensions(glm::mat4 newParentTransform,
                                 int newParentWidthPx,
                                 int newParentHeightPx);
        void setParentDimensions(const ParentDimensions& parentDimensions);
    };
}


#endif //OPENGLPLOTLIVE_PROJ_CONSTANTSIZEDRAWABLE_H
