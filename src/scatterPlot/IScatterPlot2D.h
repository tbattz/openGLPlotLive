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

        // Marker Vertices
        std::vector<float> markerVerts = {};    // positions (2)
        std::vector<float> markerOutlineVerts = {};

        // Functions
        std::shared_ptr<Shader> selectShader();
        void generateAllMarkerVerts();
        void generateMarkerEquallySpaced(unsigned int nCirclePoints, float thetaRadStart, float xHalfWidth, float yHalfHeight);
        void generateMarkerSquareVerts(float xHalfWidth, float yHalfHeight);
        void generateMarkerCircleVerts(float xHalfWidth, float yHalfHeight);

    };

}


#endif //OPENGLPLOTLIVE_PROJ_ISCATTERPLOT2D_H
