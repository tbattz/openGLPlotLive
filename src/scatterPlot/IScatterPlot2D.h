//
// Created by bcub3d-desktop on 26/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_ISCATTERPLOT2D_H
#define OPENGLPLOTLIVE_PROJ_ISCATTERPLOT2D_H


// Project Includes
#include "IScatterPlot.h"


namespace GLPL {


    class IScatterPlot2D : public IScatterPlot {
    public:
        IScatterPlot2D(std::shared_ptr<ParentDimensions> parentDimensions);

        void createAndSetupBuffers(int dataSizeBytes, const void *dataAddress,
                                   int markerVertsSizeBytes, const void *markerVertsAddress);

        void drawData(int nPts, bool selected);

    protected:
        // Line Buffers
        GLuint scatterVAO, scatterVBO, scatterEBO;
        GLuint markerVBO;

        // Marker Size
        float markerSizePx = 10; // Width/Height of the marker in pixels

    };

}


#endif //OPENGLPLOTLIVE_PROJ_ISCATTERPLOT2D_H
