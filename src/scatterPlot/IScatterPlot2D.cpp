//
// Created by bcub3d-desktop on 26/12/20.
//

// Standard Includes
#include <utility>

// Project Includes
#include "IScatterPlot2D.h"
#include "../src/util/util.h"


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
        glGenVertexArrays(1, &scatterVAO);
        glGenBuffers(1, &scatterVBO);
        glBindVertexArray(scatterVAO);

        // Setup Buffers
        glBindBuffer(GL_ARRAY_BUFFER, markerVBO);
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Data Pts
        // Setup Buffers
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

        glBindVertexArray(0);
    }

    void IScatterPlot2D::createAndSetupBuffersMarkerOutline(int dataSizeBytes, const void *dataAddress,
                                                            int markerOutlineIndicesDataSizeBytes, const void *markerOutlineIndicesDataAddress) {
        // Marker Instance
        // Create Buffers
        glGenBuffers(1, &markerOutlineVBO);
        glGenVertexArrays(1, &scatterOutlineVAO);
        glGenBuffers(1, &scatterOutlineVBO);
        glBindVertexArray(scatterOutlineVAO);

        // Setup Buffers
        glBindBuffer(GL_ARRAY_BUFFER, markerOutlineVBO);
        glBufferData(GL_ARRAY_BUFFER, dataSizeBytes, dataAddress, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Data Pts
        // Setup Buffers
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

        glBindVertexArray(0);

    }

    void IScatterPlot2D::drawData(int nPts, bool selected) {
        // Draws the data currently stored in the scatter plot corresponding to the given VAO
        if (nPts > 0) {
            std::shared_ptr<Shader> shader = selectShader();
            shader->Use();
            glm::vec4 inColor = glm::vec4(markerColour, opacityRatio);

            // Marker Polygons
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                               glm::value_ptr(*axesViewportTransform));
            glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inColor));
            glBindVertexArray(scatterVAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, (int)markerVerts.size()/2.0, nPts);
            glBindVertexArray(0);

            // Marker Outlines
            glm::vec4 inOutlineColor = glm::vec4(markerOutlineColour, outlineOpacityRatio);
            glUniformMatrix4fv(glGetUniformLocation(shader->Program, "transformViewport"), 1, GL_FALSE,
                               glm::value_ptr(*axesViewportTransform));
            glUniform4fv(glGetUniformLocation(shader->Program, "inColor"), 1, glm::value_ptr(inOutlineColor));
            glBindVertexArray(scatterOutlineVAO);
            glDrawArraysInstanced(GL_LINE_LOOP, 0, (int)markerOutlineVerts.size()/2.0, nPts);
            glBindVertexArray(0);

        }
    }

    std::shared_ptr<Shader> IScatterPlot2D::selectShader() {
        std::shared_ptr<Shader> shader;
        if (logX) {
            if (logY) {
                // Both logX and logY
                shader = shaderSetPt->getScatter2dLogxLogyShader();
            } else {
                // LogX only
                shader = shaderSetPt->getScatter2dLogxShader();
            }
        } else {
            if (logY) {
                // LogY only
                shader = shaderSetPt->getScatter2dLogyShader();
            } else {
                // Both Linear
                shader = shaderSetPt->getScatter2dShader();
            }
        }

        return shader;
    }

    void IScatterPlot2D::generateAllMarkerVerts() {
        // Generate the marker verts
        float xHalfWidth = convertHorizontalPx2ObjRel(markerSizePx);
        float yHalfHeight = convertVerticalPx2ObjRel(markerSizePx);

        switch(markerType) {
            case SQUARE: {
                IScatterPlot2D::generateMarkerSquareVerts(xHalfWidth, yHalfHeight);
                break;
            }
            case CIRCLE: {
                IScatterPlot2D::generateMarkerCircleVerts(xHalfWidth, yHalfHeight);
                break;
            }
            case DIAMOND: {
                IScatterPlot2D::generateMarkerEquallySpaced(4, M_PI/2.0, xHalfWidth, yHalfHeight);
                break;
            }
            case TRIANGLE_DOWN: {
                IScatterPlot2D::generateMarkerEquallySpaced(3, -M_PI/2.0, xHalfWidth, yHalfHeight);
                break;
            }
            case TRIANGLE_UP: {
                IScatterPlot2D::generateMarkerEquallySpaced(3, M_PI/2.0, xHalfWidth, yHalfHeight);
                break;
            }
            case TRIANGLE_LEFT: {
                IScatterPlot2D::generateMarkerEquallySpaced(3, M_PI, xHalfWidth, yHalfHeight);
                break;
            }
            case TRIANGLE_RIGHT: {
                IScatterPlot2D::generateMarkerEquallySpaced(3, 0.0, xHalfWidth, yHalfHeight);
                break;
            }
            default: {
                IScatterPlot2D::generateMarkerSquareVerts(xHalfWidth, yHalfHeight);
            }
        }


        // Update buffer
        glBindBuffer(GL_ARRAY_BUFFER, scatterVBO);
        int markerVertsSizeBytes = (int)markerVerts.size() * sizeof(markerVerts[0]);
        glBufferData(GL_ARRAY_BUFFER, markerVertsSizeBytes, &markerVerts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Update buffer
        glBindBuffer(GL_ARRAY_BUFFER, scatterOutlineVBO);
        int markerOutlineVertsSizeBytes = (int)markerOutlineVerts.size() * sizeof(markerOutlineVerts[0]);
        glBufferData(GL_ARRAY_BUFFER, markerOutlineVertsSizeBytes, &markerOutlineVerts[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

    }

    void IScatterPlot2D::generateMarkerEquallySpaced(unsigned int nCirclePoints, float thetaRadStart, float xHalfWidth, float yHalfHeight) {
        float thetaIncr = 2*M_PI / nCirclePoints;
        markerVerts.clear();
        markerOutlineVerts.clear();
        for (unsigned int i=0; i < nCirclePoints; i++) {
            // TODO - Use an EBO and make this more efficient
            // Create 3 points for the polygon
            markerVerts.push_back(xHalfWidth * std::cos((float)i*thetaIncr + thetaRadStart));
            markerVerts.push_back(yHalfHeight * std::sin((float)i*thetaIncr + thetaRadStart));
            markerVerts.push_back(xHalfWidth * std::cos((float)(i+1)*thetaIncr + thetaRadStart));
            markerVerts.push_back(yHalfHeight * std::sin((float)(i+1)*thetaIncr + thetaRadStart));
            markerVerts.push_back(0.0);
            markerVerts.push_back(0.0);

            // Create the point for the outline
            markerOutlineVerts.push_back(xHalfWidth * std::cos(i*thetaIncr + thetaRadStart));
            markerOutlineVerts.push_back(yHalfHeight * std::sin(i*thetaIncr + thetaRadStart));
        }
    }

    void IScatterPlot2D::generateMarkerSquareVerts(float xHalfWidth, float yHalfHeight) {
        // Update marker polygon verts
        markerVerts = {-xHalfWidth, yHalfHeight,
                       xHalfWidth,  -yHalfHeight,
                       -xHalfWidth, -yHalfHeight,

                       -xHalfWidth, yHalfHeight,
                       xHalfWidth,  -yHalfHeight,
                       xHalfWidth,  yHalfHeight};

        // Update marker outline verts
        markerOutlineVerts = {-xHalfWidth,  yHalfHeight,
                              xHalfWidth,  yHalfHeight,
                              xHalfWidth,  -yHalfHeight,
                              -xHalfWidth, -yHalfHeight};

    }

    void IScatterPlot2D::generateMarkerCircleVerts(float xHalfWidth, float yHalfHeight) {
        unsigned int nCirclePoints;
        if (markerSizePx < 10) {
            nCirclePoints = 10;
        } else {
            nCirclePoints = ceil((50.0/6.0) + (markerSizePx / 6.0));
        }

        IScatterPlot2D::generateMarkerEquallySpaced(nCirclePoints, 0.0, xHalfWidth, yHalfHeight);
    }


}