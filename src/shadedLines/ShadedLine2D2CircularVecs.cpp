//
// Created by tbatt on 12/04/2020.
//

#include "ShadedLine2D2CircularVecs.h"

//
// Created by tbatt on 11/04/2020.
//

#include "ShadedLine2D2CircularVecs.h"

namespace GLPL {
    ShadedLine2D2CircularVecs::ShadedLine2D2CircularVecs(std::vector<float> *dataPtX, std::vector<float> *dataPtY, GLenum mode) {
        this->dataPtX = dataPtX;
        this->dataPtY = dataPtY;
        this->setMode(mode);

        /* Setup Buffers */
        updateInternalData(0);
        int vertDataSizeBytes = internalData.size()*sizeof(internalData[0]);
        int indicesDataSizeBytes = internalIndices.size()*sizeof(internalIndices[0]);
        createAndSetupBuffers(&VAO, &VBO, &EBO, vertDataSizeBytes, indicesDataSizeBytes,
                &internalData[0], &internalIndices[0], 2*sizeof(internalData[0]));
    }

    ShadedLine2D2CircularVecs::~ShadedLine2D2CircularVecs() {

    }

    void ShadedLine2D2CircularVecs::updateInternalData(unsigned int currIndex) {
        /* Creates an internal data store from the current dataVecPt */
        // Get minimum length of both vectors
        unsigned int totLen = std::min(this->dataPtX->size(), this->dataPtY->size());
        // Resize vector to data
        internalData.resize(4*totLen); // x1, y1, x1, 0 for each point added
        // Update with new data, for each point we add (2 values), add the corresponding point on the x-axes (2 values)
        // First slice
        unsigned int len1 = totLen - currIndex;
        for(unsigned int i=0; i<len1; i++) {
            internalData[4*i] = (*dataPtX)[currIndex + i];
            internalData[4*i + 1] = (*dataPtY)[currIndex + i];
            internalData[4*i + 2] = (*dataPtX)[currIndex + i];
            internalData[4*i + 3] = 0.0;
        }
        // Second slice
        unsigned int len2 = currIndex;
        for(unsigned int i=0; i<len2; i++) {
            internalData[4*i + 4*len1] = (*dataPtX)[i];
            internalData[4*i + 4*len1 + 1] = (*dataPtY)[i];
            internalData[4*i + 4*len1 + 2] = (*dataPtX)[i];
            internalData[4*i + 4*len1 + 3] = 0.0;
        }
        // Generate indices for triangles
        if (totLen > 1) {
            internalIndices.resize(6*(totLen-1)); // 6(n-1) indices added for each point (2 triangles)
            for (unsigned int n = 1; n < internalData.size() / 4; n++) {
                // Current point is pt 2n, current axes pt is 2n+1
                // Old point is 2n-2, old axes point is 2n+1-2 = 2n-1
                // For odd numbered triangles, we use the old point, old axes point and new axes point
                internalIndices[6*(n-1)]     = 2*n - 2;     // Old point
                internalIndices[6*(n-1) + 1] = 2*n - 1;     // Old axes point
                internalIndices[6*(n-1) + 2] = 2*n + 1;     // New axes point
                // For even numbered triangles, we use the new point, the new axes point and the previous point
                internalIndices[6*(n-1) + 3] = 2*n;         // New point
                internalIndices[6*(n-1) + 4] = 2*n + 1;     // New axes point
                internalIndices[6*(n-1) + 5] = 2*n - 2;     // Old point
            }
        } else {
            internalIndices.resize(0);
        }
        nIndices = internalIndices.size();
        updated = true;
    }

    void ShadedLine2D2CircularVecs::Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans) {
        // Check if the number of points changed
        if (updated) {
            nIndices = internalIndices.size();
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]), &internalData[0], GL_DYNAMIC_DRAW);
            updated = false;
        }

        // Draw plot
        drawData(shader, axesLimitViewportTrans, &VAO, getColour(), nIndices, getMode());
    }

    std::vector<float> ShadedLine2D2CircularVecs::getMinMax() {
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