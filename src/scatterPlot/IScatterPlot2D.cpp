//
// Created by bcub3d-desktop on 26/12/20.
//

#include "IScatterPlot2D.h"

#include <utility>

namespace GLPL {


    IScatterPlot2D::IScatterPlot2D(std::shared_ptr<ParentDimensions> parentDimensions) : IScatterPlot(
            std::move(parentDimensions)) {

    }

    void IScatterPlot2D::createAndSetupBuffers(int dataSizeBytes, const void *dataAddress,
                               int markerVertSizeBytes, const void *markerVertsAddress,
                               int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress) {
        IScatterPlot2D::createAndSetupBuffersMarkerPolygons(dataSizeBytes, dataAddress, markerVertSizeBytes, markerVertsAddress);
        IScatterPlot2D::createAndSetupBuffersMarkerOutline(dataSizeBytes, dataAddress, markerOutlineIndicesDataSizeBytes, markerOutlineIndicesDataAddress);
    }

    void IScatterPlot2D::createAndSetupBuffersMarkerPolygons(int dataSizeBytes, const void *dataAddress,
                                               int markerVertsSizeBytes, const void *markerVertsAddress) {
        // Marker Instance
        // Create Buffers
        glGenBuffers(1, &markerVBO);

        // Setup Buffers
        glBindBuffer(GL_ARRAY_BUFFER, markerVBO);
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Data Pts
        // Create Buffers
        glGenVertexArrays(1, &scatterVAO);
        glGenBuffers(1, &scatterVBO);

        // Setup Buffers
        glBindVertexArray(scatterVAO);
        glBindBuffer(GL_ARRAY_BUFFER, scatterVBO);
        // Copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, markerVertsSizeBytes, markerVertsAddress, GL_DYNAMIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        // Set Instance Data
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, markerVBO);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(1, 1); // Tell OpenGL this is an instance vertex attribute

    }

    void IScatterPlot2D::createAndSetupBuffersMarkerOutline(int dataSizeBytes, const void *dataAddress,
                                                            int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress) {
        // Marker Instance
        // Create Buffers
        glGenBuffers(1, &markerOutlineVBO);

        // Setup Buffers
        glBindBuffer(GL_ARRAY_BUFFER, markerOutlineVBO);
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Data Pts
        // Create Buffers
        glGenVertexArrays(1, &scatterOutlineVAO);
        glGenBuffers(1, &scatterOutlineVBO);

        // Setup Buffers
        glBindVertexArray(scatterOutlineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, scatterOutlineVBO);
        // Copy data into buffer
        glBufferData(GL_ARRAY_BUFFER, markerOutlineIndicesDataSizeBytes, markerOutlineIndicesDataAddress, GL_DYNAMIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        // Set Instance Data
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, markerOutlineVBO);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(1, 1); // Tell OpenGL this is an instance vertex attribute

    }


    void IScatterPlot2D::drawData(int nPts, bool selected) {
        // Draws the data currently stored in the scatter plot corresponding to the given VAO
        if (nPts > 0) {
            std::shared_ptr<Shader> shader = shaderSetPt->getScatter2dShader();
            shader->Use();
            glm::vec4 inColor = glm::vec4(markerColour, opacityRatio);

            // Marker Polygons
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                               glm::value_ptr(*axesViewportTransform));
            glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
            glBindVertexArray(scatterVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nPts);
            glBindVertexArray(0);

            // Marker Outlines
            glm::vec4 inOutlineColor = glm::vec4(markerOutlineColour, outlineOpacityRatio);
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                               glm::value_ptr(*axesViewportTransform));
            glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inOutlineColor));
            glBindVertexArray(scatterOutlineVAO);
            glDrawArraysInstanced(GL_LINE_LOOP, 0, 4, nPts);
            glBindVertexArray(0);

        }
    }
}