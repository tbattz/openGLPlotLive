//
// Created by bcub3d-ryzen on 11/1/23.
//

#include "LegendMarkerRectangle.h"

#include <utility>



namespace GLPL {


    LegendMarkerRectangle::LegendMarkerRectangle(float x, float y, float width, float height,
                                                   std::shared_ptr<ParentDimensions> parentDimensions,
                                                   std::shared_ptr<Plotable> plotable) : Rectangle(x, y,
                                                                                                   width,
                                                                                                   height,
                                                                                                   std::move(parentDimensions)) {

        this->plotable = std::move(plotable);

    }

    void LegendMarkerRectangle::Draw() {
        plotable->drawLegendEntry(overallTransform);
    }


}



