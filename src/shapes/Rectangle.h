//
// Created by bcub3d-desktop on 20/11/22.
//

#ifndef OPENGLPLOTLIVE_PROJ_RECTANGLE_H
#define OPENGLPLOTLIVE_PROJ_RECTANGLE_H


#include "../rendering/ConstantXYDrawable.h"



namespace GLPL {

    class Rectangle : public ConstantXYDrawable {
    public:
        // Constructor
        Rectangle(float x, float y, float width, float height,
                  std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();
        std::string getID();
        void setBackgroundColor(glm::vec4 newColor);
        void setOutlineColor(glm::vec4 newColor);


    private:
        // Functions
        void createAndSetupRectBuffers();

        // Data
        int nPts = 0;
        glm::vec4 backgroundColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec4 outlineColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

        // Line Buffers
        GLuint rectSolidVAO, rectSolidVBO;
        GLuint rectOutlineVAO, rectOutlineVBO;

        // Vertices
        std::vector<float> rectSolidVerts = {-1.0f,   -1.0f,
                                            -1.0f,   1.0f,
                                             1.0f,   1.0f,
                                             -1.0f,  -1.0f,
                                             1.0f,   1.0f,
                                             1.0f,   -1.0f};
        std::vector<float> rectOutlineVerts = {-1.0f, -1.0f,
                                               -1.0f, 1.0f,
                                               1.0f, 1.0f,
                                               1.0f, -1.0f};


    };


}

#endif //OPENGLPLOTLIVE_PROJ_RECTANGLE_H
