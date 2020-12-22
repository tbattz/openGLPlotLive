//
// Created by tbatt on 11/04/2020.
//

#include "Line2D2CircularVecs.h"

namespace GLPL {
    Line2D2CircularVecs::Line2D2CircularVecs(std::vector<float> *dataPtX, std::vector<float> *dataPtY, GLenum mode) {
        this->dataPtX = dataPtX;
        this->dataPtY = dataPtY;
        this->setMode(mode);

        /* Setup Buffers */
        updateInternalData(0);
        int dataSizeBytes = internalData.size()*sizeof(internalData[0]);
        createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, &internalData[0], 2*sizeof(internalData[0]));

        /* Set number of Points */
        nPts = internalData.size()/2.0;
    }

    Line2D2CircularVecs::~Line2D2CircularVecs() {

    }

    void Line2D2CircularVecs::updateInternalData(unsigned int currIndex) {
        /* Creates an internal data store from the current dataVecPt */
        // Get minimum length of both vectors
        unsigned int totLen = std::min(this->dataPtX->size(), this->dataPtY->size());
        // Resize vector to data
        internalData.resize(2*totLen);
        // Update with new data
        // First slice
        unsigned int len1 = totLen - currIndex;
        for(unsigned int i=0; i<len1; i++) {
            internalData[2*i] = (*dataPtX)[currIndex + i];
            internalData[2*i + 1] = (*dataPtY)[currIndex + i];
        }
        // Second slice
        unsigned int len2 = currIndex;
        for(unsigned int i=0; i<len2; i++) {
            internalData[2*i + 2*len1] = (*dataPtX)[i];
            internalData[2*i + 2*len1 + 1] = (*dataPtY)[i];
        }
        updated = true;
    }

    void Line2D2CircularVecs::Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans) {
        // Check if the number of points changed
        if (updated) {
            int newPts = (internalData).size()/2;
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]), &internalData[0], GL_DYNAMIC_DRAW);
            updated = false;
        }

        // Draw plot
        drawData(shader, axesLimitViewportTrans, &VAO, getColour(), nPts, getMode());
    }

    std::vector<float> Line2D2CircularVecs::getMinMax() {
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