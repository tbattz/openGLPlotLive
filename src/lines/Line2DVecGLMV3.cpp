//
// Created by bcub3d-desktop on 29/3/20.
//

#include "Line2DVecGLMV3.h"


namespace GLPL {
    Line2DVecGLMV3::Line2DVecGLMV3(std::vector<glm::dvec3>* dataVecPt, int indexX, int indexY, GLenum mode) {
        this->dataVecPt = dataVecPt;
        this->setMode(mode);
        this->indexX = indexX;
        this->indexY = indexY;

        /* Setup Buffers */
        updateInternalData();
        int dataSizeBytes = internalData.size()*sizeof(internalData[0]);
        createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, &internalData[0], 2*sizeof(internalData[0]));

        /* Set Number of Points */
        nPts = dataVecPt->size()/2.0;

    }

    void Line2DVecGLMV3::updateInternalData() {
        /* Creates an internal data store from the current dataVecPt */
        // Resize vector to data
        internalData.resize(2*dataVecPt->size());
        // Update With New Data
        for(unsigned int i=0; i<dataVecPt->size(); i++) {
            internalData[2*i] = (*dataVecPt)[i][indexX];
            internalData[2*i + 1] = (*dataVecPt)[i][indexY];
        }
    }

    void Line2DVecGLMV3::Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
        // Check if number of points changed
        int newPts = (internalData).size()/2;
        if (newPts != nPts) {
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]),&internalData[0],GL_DYNAMIC_DRAW);
        }

        // Draw Plot
        drawData(shader, axesLimitViewportTrans, &VAO, getColour(), nPts, getMode());
    }

    std::vector<float> Line2DVecGLMV3::getMinMax() {
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

};

