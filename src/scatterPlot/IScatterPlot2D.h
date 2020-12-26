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
                                   int markerVertSizeBytes, const void *markerVertsAddress,
                                   int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress);
        void createAndSetupBuffersMarkerPolygons(int dataSizeBytes, const void *dataAddress,
                                   int markerVertSizeBytes, const void *markerVertsAddress);
        void createAndSetupBuffersMarkerOutline(int dataSizeBytes, const void *dataAddress,
                                   int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress);


        void drawData(int nPts, bool selected);

    protected:
        // Line Buffers
        GLuint scatterVAO, scatterVBO;
        GLuint markerVBO;
        GLuint scatterOutlineVAO, scatterOutlineVBO;
        GLuint markerOutlineVBO;

        // Marker Size
        float markerSizePx = 10; // Width/Height of the marker in pixels

    };

}


#endif //OPENGLPLOTLIVE_PROJ_ISCATTERPLOT2D_H
