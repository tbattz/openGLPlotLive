//
// Created by tbatt on 19/04/2020.
//

// GLM Mathematics
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Includes
#include "IDrawable.h"
#include "transforms.h"


GLPL::IDrawable::IDrawable() : IClickable() {

};

std::array<float, 2> GLPL::IDrawable::getPosition() {
    return std::array<float, 2> {x, y};
}

float GLPL::IDrawable::getX() {
    return x;
}

float GLPL::IDrawable::getY() {
    return y;
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

void GLPL::IDrawable::onLeftClick() {

}

void GLPL::IDrawable::onLeftShiftClick() {

}

void GLPL::IDrawable::onMiddleClick() {

}

void GLPL::IDrawable::onRightClick() {

}

void GLPL::IDrawable::onRightShiftClick() {

}

void GLPL::IDrawable::onScroll(double xoffset, double yoffset) {

}

void GLPL::IDrawable::onRightDrag(bool dragging, double origXPos, double origYPos) {

}

void GLPL::IDrawable::onMiddleDrag(bool dragging, double origXPos, double origYPos) {

}

void GLPL::IDrawable::onLeftDrag(bool dragging, double origXPos, double origYPos) {

}

void GLPL::IDrawable::onRightShiftDrag(bool dragging, double origXPos, double origYPos) {

}

void GLPL::IDrawable::onLeftShiftDrag(bool dragging, double origXPos, double origYPos) {

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
    glBufferData(GL_ARRAY_BUFFER, mouseOverVertsWithChildren.size() * sizeof(GLfloat), &mouseOverVertsWithChildren[0], GL_STATIC_DRAW);

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

float GLPL::IDrawable::convertHorizontalPx2ObjRel(float horSizePx) {
    // Check Scaling
    float overallWidthRel = (overallTransform * glm::vec4(getRight(), 0.0f, 0.5f, 1.0f))[0] - (overallTransform * glm::vec4(getLeft(), 0.0f, 0.5f, 1.0f))[0];
    float widthRelOnPx = overallWidthRel / (float)getWidthPx();
    // Generate the marker verts
    float horRelSize = widthRelOnPx * horSizePx;

    return horRelSize;
}

float GLPL::IDrawable::convertVerticalPx2ObjRel(float vertSizePx) {
    // Check Scaling
    float overallHeightRel = (overallTransform * glm::vec4(0.0f, getTop(), 0.5f, 1.0f))[1] - (overallTransform * glm::vec4(0.0f, getBottom(), 0.5f, 1.0f))[1];
    float heightRelOnPx = overallHeightRel / (float)getHeightPx();
    // Generate the marker verts
    float vertRelSize = heightRelOnPx * vertSizePx;

    return vertRelSize;
}


bool GLPL::IDrawable::canMouseOver() {
    return mouseOverable;
}

bool GLPL::IDrawable::isHoverable() {
    return hoverable;
}

void GLPL::IDrawable::setHoverable(bool isHoverable) {
    hoverable = isHoverable;
}

bool GLPL::IDrawable::isHovered() {
    return hovered;
}

void GLPL::IDrawable::setHovered(bool newHovered) {
    hovered = newHovered;
}

bool GLPL::IDrawable::isSelected() {
    return selected;
}

void GLPL::IDrawable::setSelected(bool isSelected) {
    selected = isSelected;
    mouseX = 0;
    mouseY = 0;
};

std::vector<GLfloat> GLPL::IDrawable::calcMouseOverVertsWithChildren() {
    // Some children can lie outside the area -1 to 1, so to calculate if the mouse is over the particular area,
    // create a bounding box that includes all the children.
    // Note, that the parent should still only be selected, if the mouse is contained within the parents original verts.
    // Calculate without children first
    calcMouseOverVertsNoChildren();
    float xmin = mouseOverVertsNoChildren[0];
    float xmax = mouseOverVertsNoChildren[2];
    float ymin = mouseOverVertsNoChildren[1];
    float ymax = mouseOverVertsNoChildren[5];

    // Check children
    for(auto & child : children) {
        if (child->isHoverable()) {
            // Child verts are not relative at the moment, they should be
            std::vector<GLfloat> childMouseVerts = child->calcMouseOverVertsWithChildren();
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

    // Update stored values (result is in -1 to 1)
    mouseOverVertsWithChildren = {xmin, ymin, xmax, ymin, xmax, ymax, xmin, ymax};

    return mouseOverVertsWithChildren;
}

std::vector<GLfloat> GLPL::IDrawable::calcMouseOverVertsNoChildren() {
    // Calculate mouse over verts, ignoring the children
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

    // Update stored values (result is in -1 to 1)
    mouseOverVertsNoChildren = {xmin, ymin, xmax, ymin, xmax, ymax, xmin, ymax};

    return mouseOverVertsNoChildren;
}

bool GLPL::IDrawable::isMouseOver(double xpos, double ypos, bool withChildren) {
    // Check if mouses is over common bounding box (-1 to 1)
    if (canMouseOver()) {
        float xmin, xmax, ymin, ymax;
        if (withChildren) {
            xmin = mouseOverVertsWithChildren[0];
            xmax = mouseOverVertsWithChildren[2];
            ymin = mouseOverVertsWithChildren[1];
            ymax = mouseOverVertsWithChildren[5];
        } else {
            xmin = mouseOverVertsNoChildren[0];
            xmax = mouseOverVertsNoChildren[2];
            ymin = mouseOverVertsNoChildren[1];
            ymax = mouseOverVertsNoChildren[5];
        }
        // TODO - Make this an inside polygon check, instead of just checking the square bounding box
        if (xmin < xpos && xpos < xmax) {
            if (ymin < ypos && ypos < ymax) {
                return true;
            }
        }
    }

    return false;
}

void GLPL::IDrawable::getMousedOverChildren(double xpos, double ypos,
        const std::shared_ptr<std::vector<std::shared_ptr<GLPL::IDrawable>>>& mousedOverObjs) {
    for(auto & i : children) {
        if (i->isMouseOver(xpos, ypos)) {
            mousedOverObjs->push_back(i);
            // Check children of children
            i->getMousedOverChildren(xpos, ypos, mousedOverObjs);
        }
    }
}

void GLPL::IDrawable::setLastMousePos(double lastMouseX, double lastMouseY) {
    mouseX = lastMouseX;
    mouseY = lastMouseY;
}

std::tuple<double, double> GLPL::IDrawable::getLastMousePos() {
    return std::make_tuple(mouseX, mouseY);
}

int GLPL::IDrawable::getHoverCursor() {
    return 0;
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
    IDrawable::calcMouseOverVertsWithChildren();
}

void GLPL::IDrawable::removeChild(const std::shared_ptr<IDrawable>& childPt) {
    // Remove a child by value
    children.erase(std::remove(children.begin(), children.end(), childPt), children.end());
    // Calculate mouse over verts
    IDrawable::calcMouseOverVertsWithChildren();
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
    glBufferData(GL_ARRAY_BUFFER, mouseOverVertsWithChildren.size()*sizeof(GLfloat),&mouseOverVertsWithChildren[0],GL_STATIC_DRAW);

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

    IDrawable::calcMouseOverVertsWithChildren();
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




