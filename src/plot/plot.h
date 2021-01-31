//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_PLOT_H
#define OPENGLPLOTLIVE_PROJ_PLOT_H

// Standard Includese
#include <memory>

// Project Includes
#include "../axes/axes.h"
#include "../rendering/ConstantXYDrawable.h"
#include "../axes/axes2D.h"
#include "../axes/axes3D.h"


namespace GLPL {

    class Plot : public ConstantXYDrawable {
        // Plot contains sets of axes, labels and titles
    public:
        // Constructor
        Plot(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);
        // Destructor
        ~Plot();

        // Functions
        std::shared_ptr<GLPL::Axes2D> add2DAxes(float x, float y, float width, float height);
        std::shared_ptr<GLPL::Axes3D> add3DAxes(float x, float y, float width, float height);
        std::shared_ptr<GLPL::Axes> addAxes(float x, float y, float width, float height, AxesType axesType);
        std::shared_ptr<Axes> getAxes(unsigned int axesId);
        void removeAxes(unsigned int axesId);
        void Draw();
        std::string getID();

    private:
        // Axes
        unsigned int axesCount = 0;
        std::unordered_map<unsigned int, std::shared_ptr<Axes>> axesMap;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_PLOT_H
