//
// Created by bcub3d-desktop on 26/12/20.
//

#include "Scatter2D2Vecs.h"
#include "../util/util.h"


namespace GLPL {

    Scatter2D2Vecs::Scatter2D2Vecs(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                                   std::shared_ptr<ParentDimensions> parentDimensions) :
            IScatterPlot2D(std::move(parentDimensions)) {
        this->dataPtX = dataPtX;
        this->dataPtY = dataPtY;

        // Generate Marker Vertices
        generateAllMarkerVerts();

        // Setup Buffers
        updateInternalData();
        int dataSizeBytes = internalData.size() * sizeof(internalData[0]);
        int markerVertsSizeBytes = markerVerts.size() * sizeof(markerVerts[0]);
        int markerOutlineVertsSizeBytes = markerOutlineVerts.size() * sizeof(markerOutlineVerts[0]);
        createAndSetupBuffers(dataSizeBytes, &internalData[0],
                              markerVertsSizeBytes, &markerVerts[0],
                              markerOutlineVertsSizeBytes, &markerOutlineVerts[0]);

        // Set number of Points
        nPts = internalData.size() / 2.0;

    }

    Scatter2D2Vecs::~Scatter2D2Vecs() {

    }

    void Scatter2D2Vecs::updateInternalData() {
        /* Creates an internal data store from the current dataVecPt */
        // Get minimum length of both vectors
        int len = std::min(this->dataPtX->size(), this->dataPtY->size());
        // Resize vector to data
        internalData.resize(2*len);
        // Sort by x value
        std::vector<int> sortedIndices = genIndicesSortedVector(dataPtX);
        // Sort data by these indices
        std::vector<float> sortedX = sortVectorByIndices(dataPtX, sortedIndices);
        std::vector<float> sortedY = sortVectorByIndices(dataPtY, sortedIndices);

        // Update with new data
        for(int i=0; i<len; i++) {
            internalData[2*i] = sortedX[i];
            internalData[2*i + 1] = sortedY[i];
        }
    }

    void Scatter2D2Vecs::updateIncrementalInternalData() {

    }

    void Scatter2D2Vecs::Draw() {
        // Check if the number of points changed
        int newPts = (internalData).size()/2;
        if (newPts != nPts) {
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER, scatterVBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]), &internalData[0], GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, scatterOutlineVBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]), &internalData[0], GL_DYNAMIC_DRAW);
        }

        // Draw plot
        if (isSelected()) {
            drawData(nPts, selected);
        }
        drawData(nPts, false);
    }

    std::string Scatter2D2Vecs::getID() {
        return "Scatter2D2Vecs:" + std::to_string(x) + ":" + std::to_string(y);
    }

    void Scatter2D2Vecs::clearData() {
        this->dataPtX->clear();
        this->dataPtY->clear();
        this->updateInternalData();
        nPts = 0;
    }

    std::vector<float> Scatter2D2Vecs::getMinMax() {
        // Gets the minimum and maximum values of both x and y for the data
        float maxFloat = std::numeric_limits<float>::max();
        float xmin = maxFloat;
        float xmax = -maxFloat;
        float ymin = maxFloat;
        float ymax = -maxFloat;
        for (unsigned int i = 0; i<internalData.size()/2.0; i++) {
            float xval = (internalData)[2*i];
            float yval = (internalData)[2*i+1];
            if (xval > xmax) {
                xmax = xval;
            }
            if (xval < xmin) {
                xmin = xval;
            }
            if (yval > ymax) {
                ymax = yval;
            }
            if (yval < ymin) {
                ymin = yval;
            }
        }

        return std::vector<float> {xmin,xmax,ymin,ymax};
    }

    std::tuple<float, float> Scatter2D2Vecs::getClosestPoint(float xVal) {
        unsigned int ind = binarySearch(internalData, 0, (internalData.size()/2) - 1, xVal);
        if (ind < internalData.size()/2) {
            // Check which point is closer
            if (ind > 1 && ind < internalData.size()/2 - 1) {
                float diffLeft = abs(internalData[2 * ind] - xVal);
                float diffRight = abs(internalData[2 * (ind + 1)] - xVal);
                if (diffRight < diffLeft) {
                    // Use the right index
                    ind += 1;
                }
            }
            return std::make_tuple(internalData[2 * ind], internalData[2 * ind + 1]);
        } else {
            return std::make_tuple(0,0);
        }
    }




}