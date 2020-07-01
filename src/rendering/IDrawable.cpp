//
// Created by tbatt on 19/04/2020.
//

// GLM Mathematics
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Includes
#include "IDrawable.h"
#include "transforms.h"


GLPL::IDrawable::IDrawable() {

};

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
    for(auto & i : children) {
        i->drawBoundingBox();
    }
}

int GLPL::IDrawable::getZDepthValue() {
    return zDepthValue;
}

void GLPL::IDrawable::setZDepthValue(int newZDepthValue) {
    // Set value
    zDepthValue = newZDepthValue;
}

std::shared_ptr<GLPL::ParentDimensions> GLPL::IDrawable::createParentDimensions() {
    std::shared_ptr<ParentDimensions> newParentDimensions = std::make_shared<GLPL::ParentDimensions>(ParentDimensions{overallTransform, xPx, yPx, widthPx, heightPx, shaderSetPt});

    return newParentDimensions;
}

void GLPL::IDrawable::registerChild(const std::shared_ptr<IDrawable>& newChildPt) {
    // Update parent information for child
    std::shared_ptr<ParentDimensions> newParentDimensions = IDrawable::createParentDimensions();
    newChildPt->setParentDimensions(newParentDimensions);
    // Store child
    children.push_back(newChildPt);
    // Resort children
    IDrawable::sortChildren();
}

void GLPL::IDrawable::removeChild(const std::shared_ptr<IDrawable>& childPt) {
    // Remove a child by value
    children.erase(std::remove(children.begin(), children.end(), childPt), children.end());
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
    std::array<float, 2> xy = GLPL::IDrawable::generateXYPositionFromPin();
    this->viewportTransform = GLPL::Transforms::viewportTransform(xy[0], xy[1], width, height);
    this->overallTransform = parentTransform * viewportTransform;
}

void GLPL::IDrawable::updatePositionPx() {
    // Update the size in pixels
    this->xPx = (int)((float)parentXPx + (x*(float)parentWidthPx));
    this->yPx = (int)((float)parentYPx + (y*(float)parentHeightPx));
}

void GLPL::IDrawable::updateChildren() {
    for(auto & i : children) {
        // Update parent information for child
        i->setParentDimensions(overallTransform, xPx, yPx, widthPx, heightPx);
        i->updateChildren();
    }
}

void GLPL::IDrawable::sortChildren() {
    if (!std::is_sorted(children.begin(), children.end(), IDrawable::compareZDepthValue)) {
        std::sort(children.begin(), children.end(), IDrawable::compareZDepthValue);
    }
}

bool GLPL::IDrawable::compareZDepthValue(const std::shared_ptr<IDrawable>& left, const std::shared_ptr<IDrawable>& right) {
    return left->getZDepthValue() < right->getZDepthValue();
}

void GLPL::IDrawable::setPinPosition(GLPL::PinPosition newPinPosition) {
    this->pinPosition = newPinPosition;
    // Update transforms
    GLPL::IDrawable::updateTransforms();
    GLPL::IDrawable::updateChildren();
}

std::array<float, 2> GLPL::IDrawable::generateXYPositionFromPin() {
    // Generate the bottom left xy position from the current position and pin position
    std::array<float, 2> xyPos = {x, y};

    // Handle each pin position mode
    switch(pinPosition) {
        case BOTTOM_LEFT: {
            xyPos = {x, y};
            break;
        }
        case BOTTOM_RIGHT: {
            xyPos = {x - width, y};
            break;
        }
        case TOP_LEFT: {
            xyPos = {x, y - height};
            break;
        }
        case TOP_RIGHT: {
            xyPos = {x - width, y - height};
            break;
        }
        case CENTRE_TOP: {
            xyPos = {x - (width/2.0), y - height};
            break;
        }
        case CENTRE_BOTTOM: {
            xyPos = {x - (width/2.0), y};
            break;
        }
        case CENTRE_LEFT: {
            xyPos = {x, y - (height/2.0)};
            break;
        }
        case CENTRE_RIGHT: {
            xyPos = {x - width, y - (height/2.0)};
            break;
        }
        case CENTRE: {
            xyPos = {x - (width/2.0), y - (height/2.0)};
            break;
        }
        default: {
            std::cout << "Invalid PinPosition! Defaulting to BOTTOM_LEFT." << std::endl;
            xyPos = {x, y};
            break;
        }
    }

    return xyPos;
}




