//
// Created by bcub3d-desktop on 26/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_ISCATTERPLOT_H
#define OPENGLPLOTLIVE_PROJ_ISCATTERPLOT_H


// Standard Includes
#include <memory>

// Project Includes
#include "../rendering/IDrawable.h"
#include "../rendering/ConstantXYDrawable.h"
#include "../axes/IPlotable.h"
#include "../lines/lineColours.h"


namespace GLPL {


    class IScatterPlot : public IPlotable, public ConstantXYDrawable {
    public:

        IScatterPlot(std::shared_ptr<ParentDimensions> parentDimensions);

        void setMarkerColour(glm::vec3 newMarkerColour);
        void setOpacityRatio(float newOpacityRatio);
        void setMarkerOutlineColour(glm::vec3 newMarkerOutlineColour);
        void setOutlineOpacityRatio(float newOpacityRatio);
        glm::vec3 getColour();
        int getHoverCursor() override;

        virtual std::vector<float> getMinMax() = 0;
        virtual std::tuple<float, float> getClosestPoint(float xVal) = 0;
        virtual void generateMarkerVerts() = 0;

    protected:
        // Data
        glm::vec3 markerColour = LC_WHITE;
        glm::vec3 markerOutlineColour = LC_WHITE;
        float opacityRatio = 1.0;
        float outlineOpacityRatio = 1.0;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_ISCATTERPLOT_H
