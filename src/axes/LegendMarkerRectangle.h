//
// Created by bcub3d-ryzen on 11/1/23.
//

#ifndef OPENGLPLOTLIVE_PROJ_LEGENDMARKERRECTANGLE_H
#define OPENGLPLOTLIVE_PROJ_LEGENDMARKERRECTANGLE_H


// Project Includes
#include "../texts/TextString.h"
#include "../rendering/ConstantXYDrawable.h"
#include "../shadedLines/ShadedLine2D2CircularVecs.h"
#include "../shapes/Rectangle.h"


namespace GLPL {

    class LegendMarkerRectangle : public Rectangle {
    public:
        // Constructor
        LegendMarkerRectangle(float x, float y, float width, float height,
                              std::shared_ptr<ParentDimensions> parentDimensions,
                              std::shared_ptr<Plotable> plotable);

        void Draw() override;


    private:
        std::shared_ptr<Plotable> plotable;
    };

}


#endif //OPENGLPLOTLIVE_PROJ_LEGENDMARKERRECTANGLE_H
