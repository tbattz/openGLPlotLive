//
// Created by tbatt on 11/04/2020.
//

#include "Line2D2CircularVecs.h"
#include "GLTypes.h"

namespace GLPL {
    template <typename T, typename U>
    Line2D2CircularVecs<T, U>::Line2D2CircularVecs(std::vector<T> *dataPtX, std::vector<U> *dataPtY, GLenum mode) {
        this->dataPtX = dataPtX;
        this->dataPtY = dataPtY;
        this->setMode(mode);

        /* Setup Buffers */
        updateInternalData(0);
        int dataSizeBytes = internalData.size()*sizeof(internalData[0]);
        int glType = GLPL::getGLType<largerType>();
        createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, &internalData[0], 2*sizeof(internalData[0]), glType);

        /* Set number of Points */
        nPts = internalData.size()/2.0;
    }

    template <typename T, typename U>
    void Line2D2CircularVecs<T, U>::updateInternalData(unsigned int currIndex) {
        /* Creates an internal data store from the current dataVecPt */
        // Get minimum length of both vectors
        unsigned int totLen = std::min(this->dataPtX->size(), this->dataPtY->size());
        // Resize vector to data
        internalData.resize(2*totLen);
        // Update with new data
        // First slice
        unsigned int len1 = totLen - currIndex;
        for(unsigned int i=0; i<len1; i++) {
            internalData[2*i] = (largerType)(*dataPtX)[currIndex + i];
            internalData[2*i + 1] = (largerType)(*dataPtY)[currIndex + i];
        }
        // Second slice
        unsigned int len2 = currIndex;
        for(unsigned int i=0; i<len2; i++) {
            internalData[2*i + 2*len1] = (largerType)(*dataPtX)[i];
            internalData[2*i + 2*len1 + 1] = (largerType)(*dataPtY)[i];
        }
        updated = true;
    }

    template <typename T, typename U>
    void Line2D2CircularVecs<T, U>::Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans) {
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

    template <typename T, typename U>
    std::vector<float> Line2D2CircularVecs<T, U>::getMinMax() {
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