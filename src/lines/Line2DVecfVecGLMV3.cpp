//
// Created by bcub3d-desktop on 29/3/20.
//

#include "Line2DVecfVecGLMV3.h"


namespace GLPL {
    Line2DVecfVecGLMV3::Line2DVecfVecGLMV3(std::vector<float>* dataVecfPt, std::vector<glm::dvec3>* dataVecGLMV3Pt, int indexGLM, GLenum mode) {
        this->dataVecfPt = dataVecfPt;
        this->setMode(mode);
        this->dataVecGLMV3Pt = dataVecGLMV3Pt;
        this->indexGLM = indexGLM;

        /* Setup Buffers */
        updateInternalData();
        int dataSizeBytes = internalData.size()*sizeof(internalData[0]);
        createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, &internalData[0], 2*sizeof(internalData[0]));

        /* Set Number of Points */
        nPts = internalData.size()/2.0;

    }

    void Line2DVecfVecGLMV3::updateInternalData() {
        /* Creates an internal data store from the current dataVecPt */
                // Clear Previous Data
                internalData.clear();
        // Get maximum length of both vectors
        int len = std::min(dataVecfPt->size(),dataVecGLMV3Pt->size());
        // Update With New Data
        for(int i=0; i<len; i++) {
            internalData.push_back((*dataVecfPt)[i]);
            internalData.push_back((*dataVecGLMV3Pt)[i][indexGLM]);
        }
    }

    void Line2DVecfVecGLMV3::Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
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

    std::vector<float> Line2DVecfVecGLMV3::getMinMax() {
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