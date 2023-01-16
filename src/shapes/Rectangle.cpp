//
// Created by bcub3d-desktop on 20/11/22.
//

#include "Rectangle.h"


namespace GLPL {

    Rectangle::Rectangle(float x, float y, float width, float height,
                         std::shared_ptr<ParentDimensions> parentDimensions) :
                     ConstantXYDrawable(x,  y, width, height,
                     CONSTANT_SCALE, CONSTANT_SCALE,
                     std::move(parentDimensions)) {

        // Set bounding box color
        boundingBoxColor = glm::vec4(0.6f, 0.6f, 1.0f, 1.0f);


        // Setup Buffers
        createAndSetupRectBuffers();


    }



    void Rectangle::Draw() {
        // Draw rectangle box
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();

        // Draw background
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(backgroundColor));
        glBindVertexArray(rectSolidVAO);
        glDrawArrays(GL_TRIANGLES,0,6);
        glBindVertexArray(0);

        // Draw outline
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(outlineColor));
        glBindVertexArray(rectOutlineVAO);
        glDrawArrays(GL_LINE_LOOP,0,4);
        glBindVertexArray(0);


        // Draw bounding box of children
        for(auto & i : children) {
            i->drawBoundingBox();
        }

    }

    std::string Rectangle::getID() {
        return std::string();
    }

    void Rectangle::createAndSetupRectBuffers() {
        // Background shape
        // Create Buffers
        glGenVertexArrays(1,&rectSolidVAO);
        glGenBuffers(1,&rectSolidVBO);

        // Setup Buffers
        glBindVertexArray(rectSolidVAO);
        glBindBuffer(GL_ARRAY_BUFFER,rectSolidVBO);
        glBufferData(GL_ARRAY_BUFFER, rectSolidVerts.size()*sizeof(GLfloat),&rectSolidVerts[0],GL_STATIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glBindVertexArray(0);

        // Outline shape
        // Create Buffers
        glGenVertexArrays(1,&rectOutlineVAO);
        glGenBuffers(1,&rectOutlineVBO);

        // Setup Buffers
        glBindVertexArray(rectOutlineVAO);
        glBindBuffer(GL_ARRAY_BUFFER,rectOutlineVBO);
        glBufferData(GL_ARRAY_BUFFER, rectOutlineVerts.size()*sizeof(GLfloat),&rectOutlineVerts[0],GL_STATIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glBindVertexArray(0);

    }

    void Rectangle::setOutlineColor(glm::vec4 newColor) {
        outlineColor = newColor;
    }

    void Rectangle::setBackgroundColor(glm::vec4 newColor) {
        backgroundColor = newColor;
    }


}









