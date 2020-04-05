//
// Created by bcub3d-desktop on 29/3/20.
//

#include "Line2DVec.h"


namespace GLPL {
    Line2DVec::Line2DVec(std::vector<float>* dataVecPt, GLenum mode) {
        this->dataVecPt = dataVecPt;
        this->setMode(mode);

        /* Setup Buffers */
        int dataSizeBytes = dataVecPt->size()*sizeof((*dataVecPt)[0]);
        createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, dataVecPt->data(), 2*sizeof((*dataVecPt)[0])); // dataVecPt->data() gives the address of the first element of the vector

        /* Set Number of Points */
        nPts = dataVecPt->size()/2;

    }

    void Line2DVec::Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
        // Check if number of points changed
        int newPts = dataVecPt->size()/2;
        if (newPts != nPts) {
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            glBufferData(GL_ARRAY_BUFFER, dataVecPt->size()*sizeof((*dataVecPt)[0]),dataVecPt->data(),GL_DYNAMIC_DRAW);
        }

        // Draw Plot
        drawData(shader, axesLimitViewportTrans, &VAO, getColour(), nPts, getMode());
    }

    void Line2DVec::appendVec(float x, float y) {
        // Appends a point to the current data
        this->dataVecPt->push_back(x);
        this->dataVecPt->push_back(y);
    }

    std::vector<float> Line2DVec::getMinMax() {
        // Gets the minimum and maximum values of both x and y for the data
        float xmin = 0;
        float xmax = 0;
        float ymin = 0;
        float ymax = 0;
        for (unsigned int i = 0; i<dataVecPt->size()/2.0; i++) {
            float xval = (*dataVecPt)[2*i];
            float yval = (*dataVecPt)[2*i+1];
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

