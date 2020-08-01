//
// Created by tbatt on 11/04/2020.
//

#include "Line2D2Vecs.h"

#include <utility>

namespace GLPL {
    Line2D2Vecs::Line2D2Vecs(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                             std::shared_ptr<ParentDimensions> parentDimensions,
                             GLenum mode) : ISingleLine2D(std::move(parentDimensions)) {
        this->dataPtX = dataPtX;
        this->dataPtY = dataPtY;
        this->setMode(mode);

        /* Setup Buffers */
        updateInternalData();
        int dataSizeBytes = internalData.size()*sizeof(internalData[0]);
        createAndSetupBuffers(dataSizeBytes, &internalData[0], 2*sizeof(internalData[0]));

        /* Set number of Points */
        nPts = internalData.size()/2.0;
    }

    Line2D2Vecs::~Line2D2Vecs() {

    }

    void Line2D2Vecs::updateInternalData() {
        /* Creates an internal data store from the current dataVecPt */
        // Get minimum length of both vectors
        int len = std::min(this->dataPtX->size(), this->dataPtY->size());
        // Resize vector to data
        internalData.resize(2*len);
        // Update with new data
        for(int i=0; i<len; i++) {
            internalData[2*i] = (*dataPtX)[i];
            internalData[2*i + 1] = (*dataPtY)[i];
        }
    }

    void Line2D2Vecs::Draw() {
        // Check if the number of points changed
        int newPts = (internalData).size()/2;
        if (newPts != nPts) {
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]), &internalData[0], GL_DYNAMIC_DRAW);
        }

        // Draw plot
        drawData(nPts);
    }

    std::vector<float> Line2D2Vecs::getMinMax() {
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
}