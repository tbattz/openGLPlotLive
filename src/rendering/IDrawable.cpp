//
// Created by tbatt on 19/04/2020.
//

// GLM Mathematics
#include <utility>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Includes
#include "IDrawable.h"
#include "transforms.h"


GLPL::IDrawable::IDrawable(float x, float y, int widthPx, int heightPx) {
    // Assuming the widget is the top level, no parent
    this->isTopLevel = true;

    // Set 1-to-1 transform for top level
    parentTransform = std::make_shared<glm::mat4>(1.0f);

    // Initialise data
    IDrawable::setPosition(x, y);
    IDrawable::setSizePx(widthPx, heightPx);
    IDrawable::createAndSetupBuffers();
}

GLPL::IDrawable::IDrawable(float x, float y, float width, float height, ParentPointers parentPointers) {
    // Assuming the widget is a child of another widget
    this->isTopLevel = false;

    // Only set shader and parent attributes if we aren't the top level
    IDrawable::setParentInformation(parentPointers);

    // Initialise data
    IDrawable::setPosition(x, y);
    IDrawable::setSize(width, height);
    IDrawable::createAndSetupBuffers();
}

std::array<float, 2> GLPL::IDrawable::getPosition() {
    return std::array<float, 2> {x, y};
}

float GLPL::IDrawable::getWidth() {
    return width;
}

float GLPL::IDrawable::getHeight() {
    return height;
}

int GLPL::IDrawable::getWidthPx() {
    return widthPx;
}

int GLPL::IDrawable::getHeightPx() {
    return heightPx;
}

float GLPL::IDrawable::getLeft() {
    return x;
}

float GLPL::IDrawable::getTop() {
    return y + height;
}

float GLPL::IDrawable::getRight() {
    return x + width;
}

float GLPL::IDrawable::getBottom() {
    return y;
}

void GLPL::IDrawable::drawBoundingBox() {
    // Draw bounding box
    std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
    shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
    glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(boundingBoxColor));
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glBindVertexArray(0);
    //std::cout << overallTransform[0][0] << ", " << overallTransform[1][1] << ", " << overallTransform[2][2] << ", " << overallTransform[3][3] << std::endl;

    // Draw bounding box of children
    for(unsigned int i=0; i < children.size(); i++) {
        children[i]->drawBoundingBox();
    }
}

void GLPL::IDrawable::setPosition(float newX, float newY) {
    this->x = newX;
    this->y = newY;
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::IDrawable::setSize(float newWidth, float newHeight) {
    if (isTopLevel) {
        // Overwrite
        this->width = 1.0f;
        this->height = 1.0f;
        std::cout << "IDrawable - This is the top level! Ignoring setting size" << std::endl;
    } else {
        // Update Size
        this->width = newWidth;
        this->height = newHeight;
        // Update in pixels
        this->widthPx = (int) this->width * (*parentWidthPx);
        this->heightPx = (int) this->height * (*parentHeightPx);
    }
    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::IDrawable::setSizePx(int newWidthPx, int newHeightPx) {
    this->widthPx = newWidthPx;
    this->heightPx = newHeightPx;
    if (!isTopLevel) {
        // Update relative width, height to parent
        this->width = (float)widthPx / (*parentWidthPx);
        this->height = (float)heightPx / (*parentHeightPx);
    }

    // Update Transforms
    IDrawable::updateTransforms();
}

void GLPL::IDrawable::setParentInformation(std::shared_ptr<glm::mat4> newParentTransform,
                                           std::shared_ptr<int> newParentWidthPx,
                                           std::shared_ptr<int> newParentHeightPx) {
    this->parentWidthPx = std::move(newParentWidthPx);
    this->parentHeightPx = std::move(newParentHeightPx);
    this->parentTransform = std::move(newParentTransform);
    IDrawable::updateTransforms();
}

void GLPL::IDrawable::setParentInformation(ParentPointers parentPointers) {
    this->parentTransform = parentPointers.parentTransform;
    this->parentWidthPx = parentPointers.parentWidthPx;
    this->parentHeightPx = parentPointers.parentHeightPx;
    this->shaderSetPt = parentPointers.shaderSet;
    IDrawable::updateTransforms();
    IDrawable::updateChildren();
}

GLPL::ParentPointers GLPL::IDrawable::createParentPointers() {
    std::shared_ptr<int> newWidthPx = std::shared_ptr<int>(parentWidthPx);
    std::shared_ptr<int> newHeightPx = std::shared_ptr<int>(parentHeightPx);
    std::shared_ptr<glm::mat4> newOverallTransform = std::make_shared<glm::mat4>(overallTransform);
    ParentPointers newParentPointers({newOverallTransform, parentWidthPx, parentHeightPx, shaderSetPt});

    return newParentPointers;
}

void GLPL::IDrawable::registerChild(std::shared_ptr<IDrawable> newChildPt) {
    children.push_back(newChildPt);
    // Update parent information for child
    ParentPointers newParentPointers = IDrawable::createParentPointers();
    children.back()->setParentInformation(newParentPointers);
}

void GLPL::IDrawable::createAndSetupBuffers() {
    // Create Buffers
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    // Setup Buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, boxVerts.size()*sizeof(GLfloat),&boxVerts[0],GL_STATIC_DRAW);

    // Position Attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    glBindVertexArray(0);
}

void GLPL::IDrawable::updateTransforms() {
    // Update the transforms
    this->viewportTransform = GLPL::Transforms::viewportTransform(x, y, width, height);
    this->overallTransform = (*parentTransform) * viewportTransform;
    std::cout << overallTransform[0][0] << ", " << overallTransform[1][1] << ", " << overallTransform[2][2] << ", " << overallTransform[3][3] << std::endl;
}

void GLPL::IDrawable::updateChildren() {
    for(auto & i : children) {
        // Update parent information for child
        std::shared_ptr<int> newWidthPx = std::shared_ptr<int>(parentWidthPx);
        std::shared_ptr<int> newHeightPx = std::shared_ptr<int>(parentHeightPx);
        std::shared_ptr<glm::mat4> newOverallTransform = std::make_shared<glm::mat4>(overallTransform);
        i->setParentInformation(newOverallTransform, newWidthPx, newHeightPx);
    }
}











