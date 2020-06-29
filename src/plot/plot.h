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
        std::shared_ptr<Axes> getAxes(unsigned int axesId);
        void removeAxes(unsigned int axesId);
        void Draw();

    private:
        // Axes
        unsigned int axesCount = 0;
        std::unordered_map<unsigned int, std::shared_ptr<Axes>> axesMap;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_PLOT_H
