//
// Created by bcub3d-desktop on 27/3/20.
//

#include "Line2DPts.h"


namespace GLPL {
    Line2DPts::Line2DPts(std::vector<GLPL::pt2D>* dataPtPt, GLenum mode) : ILine2D_OLD() {
        this->dataPtPt = dataPtPt;
        this->setMode(mode);

        /* Setup Buffers */
        int dataSizeBytes = dataPtPt->size() * 2 * sizeof(GLfloat);
        createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, dataPtPt->data(),
                              sizeof(GLPL::pt2D)); // dataPtPt->data() gives the address of the first element of the vector

        /* Set Number of Points */
        nPts = dataPtPt->size();
    }

    Line2DPts::~Line2DPts() {
        std::cout << "Destructed Line2DPts." << std::endl;
    }

    void Line2DPts::Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans) {
        // Check if number of points changed
        int newPts = dataPtPt->size();
        if (newPts != nPts) {
            nPts = newPts;
            // Update buffer and attributes
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, dataPtPt->size() * 2 * sizeof(GLfloat), dataPtPt->data(), GL_DYNAMIC_DRAW);
        }

        // Draw Plot
        drawData(shader, axesLimitViewportTrans, &VAO, getColour(), nPts, getMode());
    }


    std::vector<float> Line2DPts::getMinMax() {
        // Gets the minimum and maximum values of both x and y for the data
        float xmin = 0;
        float xmax = 0;
        float ymin = 0;
        float ymax = 0;
        for (unsigned int i = 0; i < dataPtPt->size(); i++) {
            float xval = (*dataPtPt)[i].x;
            float yval = (*dataPtPt)[i].y;
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

        return std::vector<float>{xmin, xmax, ymin, ymax};
    }
}