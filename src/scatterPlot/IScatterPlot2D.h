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

        void createAndSetupLegendBuffers(int dataSizeBytes, const void *dataAddress,
                                         int markerVertSizeBytes, const void *markerVertsAddress,
                                         int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress);
        void createAndSetupLegendBuffersMarkerPolygons(int dataSizeBytes, const void *dataAddress,
                                                       int markerVertSizeBytes, const void *markerVertsAddress);
        void createAndSetupLegendBuffersMarkerOutline(int dataSizeBytes, const void *dataAddress,
                                                      int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress);

        void drawData(int nPts, bool selected);

        void drawLegendEntry(glm::mat4 rectOverallTransform);

    protected:
        // Line Buffers
        GLuint scatterVAO, scatterVBO;
        GLuint markerVBO;
        GLuint scatterOutlineVAO, scatterOutlineVBO;
        GLuint markerOutlineVBO;

        GLuint legendScatterVAO, legendScatterVBO;
        GLuint legendMarkerVBO;
        GLuint legendScatterOutlineVAO, legendScatterOutlineVBO;
        GLuint legendMarkerOutlineVBO;

        // Marker Vertices
        std::vector<float> markerVerts = {};    // positions (2)
        std::vector<float> markerOutlineVerts = {};

        std::vector<float> legendMarkerData = {0.0f, 0.0f};

        // Functions
        std::shared_ptr<Shader> selectShader();
        void generateAllMarkerVerts();
        void generateMarkerEquallySpaced(unsigned int nCirclePoints, float thetaRadStart, float xHalfWidth, float yHalfHeight);
        void generateMarkerSquareVerts(float xHalfWidth, float yHalfHeight);
        void generateMarkerCircleVerts(float xHalfWidth, float yHalfHeight);

        void drawLegendMarker(glm::mat4 rectOverallTransform, bool selected);


    };

}


#endif //OPENGLPLOTLIVE_PROJ_ISCATTERPLOT2D_H
