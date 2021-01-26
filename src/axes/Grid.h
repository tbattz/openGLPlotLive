//
// Created by bcub3d-desktop on 4/7/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_GRID_H
#define OPENGLPLOTLIVE_PROJ_GRID_H


// Standard Includes
#include <cmath>

// Project Includes
#include "Plotable.h"


namespace GLPL {

    class Grid : public Plotable {
    public:
        // Constructor
        Grid(std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw() override;
        std::vector<float> getMinMax(bool onlyPositiveX = false, bool onlyPositiveY = false);
        std::string getID() override;
        void setMinMax(float newXMin, float newXMax, float newYMin, float newYMax);
        void setXLines(std::vector<float> xVals);
        void setYLines(std::vector<float> yVals);
        void genLines();

        std::tuple<float, float> getClosestPoint(float xVal);
        std::tuple<float, float> getClosestPoint(float xVal, float xmin, float xmax, float ymin, float ymax);

    private:

        void createAndSetupAxesLineBuffers();
        void updateAxesLineBuffers();
        std::shared_ptr<Shader> selectShader();

        // Major Tick Buffers
        GLuint vertsVAO, vertsVBO;
        std::vector<GLfloat> verts;     // Holds the vertices (-1 to 1) to plot
        std::vector<float> xAxesPos;    // Holds the graph value at this vertex for the x axes
        std::vector<float> yAxesPos;    // Holds the graph value at this vertex for the y axes

        // Settings
        float xMin = 0;
        float xMax = 0;
        float yMin = 0;
        float yMax = 0;
        glm::vec4 axesLineColor = glm::vec4(0.75f, 0.75f, 0.75f, 0.75f);

    };

}


#endif //OPENGLPLOTLIVE_PROJ_GRID_H
