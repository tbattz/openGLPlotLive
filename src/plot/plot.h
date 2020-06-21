//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_PLOT_H
#define OPENGLPLOTLIVE_PROJ_PLOT_H

// Standard Includese
#include <memory>

// Project Includes
#include "../axes/axes.h"
#include "../rendering/ConstantScaleDrawable.h"

namespace GLPL {
    class Plot : public ConstantScaleDrawable {
        // Plot contains sets of axes, labels and titles
    public:
        // Constructor
        Plot(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);
        // Destructor
        ~Plot();

        // Functions
        void addAxes(float x, float y, float width, float height);
        void Draw();

    //private:
        // Axes
        std::vector<std::shared_ptr<Axes>> axesList;



    };

}


#endif //OPENGLPLOTLIVE_PROJ_PLOT_H
