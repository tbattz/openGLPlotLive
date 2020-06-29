//
// Created by tbatt on 24/06/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_IPLOTABLE_H
#define OPENGLPLOTLIVE_PROJ_IPLOTABLE_H

// Project Includes
#include "../rendering/IDrawable.h"


namespace GLPL {
    class IPlotable {
    public:
        // Constructor
        IPlotable();

        // Functions
        virtual std::vector<float> getMinMax() = 0;
        void setAxesViewportTransform(std::shared_ptr<glm::mat4> newAxesViewportTransform);

    protected:
        std::shared_ptr<glm::mat4> axesViewportTransform = std::make_shared<glm::mat4>(glm::mat4(1.0f));

    };
}


#endif //OPENGLPLOTLIVE_PROJ_IPLOTABLE_H
