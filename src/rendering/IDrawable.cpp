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
    std::array<float, 2> xyPos = IDrawable::generateXYPositionFromPin();
    return xyPos[0];
}

float GLPL::IDrawable::getTop() {
    std::array<float, 2> xyPos = IDrawable::generateXYPositionFromPin();
    return xyPos[1] + height;
}

float GLPL::IDrawable::getRight() {
    std::array<float, 2> xyPos = IDrawable::generateXYPositionFromPin();
    return xyPos[0] + width;
}

float GLPL::IDrawable::getBottom() {
    std::array<float, 2> xyPos = IDrawable::generateXYPositionFromPin();
    return xyPos[1];
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

    // Draw bounding box of children
    for(auto & i : children) {
        i->drawBoundingBox();
    }
}



void GLPL::IDrawable::drawMouseOverBox() {
    // Reupdate buffers
    glBindBuffer(GL_ARRAY_BUFFER, mouseVBO);
    glBufferData(GL_ARRAY_BUFFER, mouseOverVerts.size() * sizeof(GLfloat), &mouseOverVerts[0], GL_STATIC_DRAW);

    // Draw bounding box
    std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
    shader->Use();
    glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(boundingBoxColor));
    glBindVertexArray(mouseVAO);
    glDrawArrays(GL_LINE_LOOP,0,4);
    glBindVertexArray(0);

    // Draw mouse over box of children
    for(auto & i : children) {
        i->drawMouseOverBox();
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


bool GLPL::IDrawable::canMouseOver() {
    return mouseOverable;
}

bool GLPL::IDrawable::isHoverable() {
    return hoverable;
}

std::vector<GLfloat> GLPL::IDrawable::calcMouseOverVerts() {
    // Some children can lie outside the area -1 to 1, so to calculate if the mouse is over the particular area,
    // create a bounding box that includes all the children.
    // Note, that the parent should still only be selected, if the mouse is contained within the parents original verts.
    float xmin = getLeft();
    float xmax = getRight();
    float ymin = getBottom();
    float ymax = getTop();

    // Scale to -1 to 1
    xmin = 2*xmin - 1;
    xmax = 2*xmax - 1;
    ymin = 2*ymin - 1;
    ymax = 2*ymax - 1;

    // Scale to global space
    xmin = (parentTransform * glm::vec4(xmin, 0.0f, 0.5f, 1.0f))[0];
    xmax = (parentTransform * glm::vec4(xmax, 0.0f, 0.5f, 1.0f))[0];
    ymin = (parentTransform * glm::vec4(0.0f, ymin, 0.5f, 1.0f))[1];
    ymax = (parentTransform * glm::vec4(0.0f, ymax, 0.5f, 1.0f))[1];

    for(auto & child : children) {
        if (child->isHoverable()) {
            // Child verts are not relative at the moment, they should be
            std::vector<GLfloat> childMouseVerts = child->calcMouseOverVerts();
            float childXmin = childMouseVerts[0];
            float childXmax = childMouseVerts[2];
            float childYmin = childMouseVerts[1];
            float childYmax = childMouseVerts[5];

            // Compare to previous values
            if (childXmin < xmin) { xmin = childXmin; }
            if (childXmax > xmax) { xmax = childXmax; }
            if (childYmin < ymin) { ymin = childYmin; }
            if (childYmax > ymax) { ymax = childYmax; }
        }
    }

    // Update stored values
    mouseOverVerts = {xmin, ymin, xmax, ymin, xmax, ymax, xmin, ymax};

    return mouseOverVerts;
}

std::vector<std::shared_ptr<GLPL::IDrawable>> GLPL::IDrawable::isMouseOver(double xpos, double ypos) {

    return std::vector<std::shared_ptr<IDrawable>>();
}

void GLPL::IDrawable::registerChild(const std::shared_ptr<IDrawable>& newChildPt) {
    // Update parent information for child
    std::shared_ptr<ParentDimensions> newParentDimensions = IDrawable::createParentDimensions();
    newChildPt->setParentDimensions(newParentDimensions);
    // Store child
    children.push_back(newChildPt);
    // Resort children
    IDrawable::sortChildren();
    // Calculate mouse over verts
    IDrawable::calcMouseOverVerts();
}

void GLPL::IDrawable::removeChild(const std::shared_ptr<IDrawable>& childPt) {
    // Remove a child by value
    children.erase(std::remove(children.begin(), children.end(), childPt), children.end());
    // Calculate mouse over verts
    IDrawable::calcMouseOverVerts();
}

void GLPL::IDrawable::createAndSetupBuffers() {
    // Bounding Box
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

    // Bounding Box With Children
    // Create Buffers
    glGenVertexArrays(1,&mouseVAO);
    glGenBuffers(1,&mouseVBO);

    // Setup Buffers
    glBindVertexArray(mouseVAO);
    glBindBuffer(GL_ARRAY_BUFFER,mouseVBO);
    glBufferData(GL_ARRAY_BUFFER, mouseOverVerts.size()*sizeof(GLfloat),&mouseOverVerts[0],GL_STATIC_DRAW);

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

    IDrawable::calcMouseOverVerts();
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

void GLPL::IDrawable::setAttachLocation(GLPL::AttachLocation newAttachLocation) {
    this->attachLocation = newAttachLocation;
    // Update transforms
    GLPL::IDrawable::updateTransforms();
    GLPL::IDrawable::updateChildren();
}

std::array<float, 2> GLPL::IDrawable::generateXYPositionFromPin() {
    // Generate the bottom left xy position from the current position and pin position
    std::array<float, 2> xyPos = {x, y};

    // Handle each pin position mode
    switch(attachLocation) {
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
            std::cout << "Invalid AttachLocation! Defaulting to BOTTOM_LEFT." << std::endl;
            xyPos = {x, y};
            break;
        }
    }

    return xyPos;
}




