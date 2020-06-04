//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_PLOT2_H
#define OPENGLPLOTLIVE_PROJ_PLOT2_H

// Standard Includes
#include <memory>

// Project Includes
#include "../window/IWindow.h"
#include "../rendering/IDrawable.h"
#include "../rendering/shader.h"
#include "../rendering/ShaderSet.h"
#include "../axes/axes2.h"

namespace GLPL {
    class Plot2 : public IDrawable {
        // Plot contains sets of axes, labels and titles
    public:
        // Constructor
        Plot2(float x, float y, float width, float height, ParentPointers parentPointers);
        // Destructor
        ~Plot2();

        // Functions
        void addAxes(float x, float y, float width, float height);
        void Draw();

    //private:
        // Axes
        std::vector<std::shared_ptr<Axes2>> axesList;



    };

}


#endif //OPENGLPLOTLIVE_PROJ_PLOT2_H
