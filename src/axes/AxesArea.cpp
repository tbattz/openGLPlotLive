//
// Created by tbatt on 19/06/2020.
//

#include "AxesArea.h"
#include "../shadedLines/ShadedLine2D2CircularVecs.h"
#include "../lines/Line2D2Vecs.h"

#include <utility>

namespace GLPL {

    AxesArea::AxesArea(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions) :
        IDrawable() {
        // Set bounding box color
        boundingBoxColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

        AxesArea::setParentDimensions(std::move(parentDimensions));

        // Initialise data
        AxesArea::setPosition(x, y);
        AxesArea::setSize(width, height);
        IDrawable::createAndSetupBuffers();

        // Calculate transform
        AxesArea::updateAxesViewportTransform();
    }

    void AxesArea::Draw() {
        // Scissor Test
        glEnable(GL_SCISSOR_TEST);
        glScissor(xPx, yPx, widthPx, heightPx);

        // Draw lines
        for(auto & i : lineMap) {
            i.second->Draw();
        }

        // Disable Scissor Testing
        glDisable(GL_SCISSOR_TEST);

    }

    std::vector<float> AxesArea::calculateScissor(glm::mat4 axesLimitsViewportTrans) {
        // Calculate corners of axes limits area
        glm::vec4 a = axesLimitsViewportTrans * glm::vec4(xmin,ymin,0,1); // -1 to 1
        glm::vec4 b = axesLimitsViewportTrans * glm::vec4(xmax,ymax,0,1); // -1 to 1
        // Transform back to 0 to 1
        float x1 = 0.5*a[0] + 0.5;
        float y1 = 0.5*a[1] + 0.5;
        float x2 = 0.5*b[0] + 0.5;
        float y2 = 0.5*b[1] + 0.5;
        // Form vector
        std::vector<float> xyVec = {x1,y1,x2,y2};

        return xyVec;
    }

    std::shared_ptr<ILine2D> AxesArea::addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
            LineType lineType, glm::vec3 colour, float opacityRatio) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create Line
        std::shared_ptr<IDrawable> lineObj;
        std::shared_ptr<ILine2D> linePt;

        switch(lineType) {
            case SINGLE_LINE: {
                lineObj = std::make_shared<Line2D2Vecs>(dataPtX, dataPtY, newParentPointers);
                linePt = std::dynamic_pointer_cast<Line2D2Vecs>(lineObj);
                break;
            }
            case SHADED_LINE: {
                lineObj = std::make_shared<ShadedLine2D2CircularVecs>(dataPtX, dataPtY, newParentPointers);
                linePt = std::dynamic_pointer_cast<ShadedLine2D2CircularVecs>(lineObj);
                break;
            }
            default: {
                lineObj = std::make_shared<ShadedLine2D2CircularVecs>(dataPtX, dataPtY, newParentPointers);
                linePt = std::dynamic_pointer_cast<ShadedLine2D2CircularVecs>(lineObj);
            }
        }
        // Set Attributes
        linePt->setLineColour(colour);
        linePt->setOpacityRatio(opacityRatio);
        // Register Children
        AxesArea::registerChild(lineObj);
        // Set axes area transform
        linePt->setAxesViewportTransform(axesViewportTransformation);
        // Store line
        lineMap.insert(std::pair<unsigned int, std::shared_ptr<ILine2D>>(lineCount, linePt));
        lineCount += 1;

        return linePt;
    }

    std::shared_ptr<IPlotable> AxesArea::getLine(unsigned int lineId) {
        if (lineMap.count(lineId) > 0) {
            return lineMap.at(lineId);
        } else {
            std::cout << "Line " << lineId << " does not exist!" << std::endl;
            return nullptr;
        }
    }

    void AxesArea::removeLine(unsigned int lineId) {
        if (lineMap.count(lineId) > 0) {
            std::shared_ptr<IPlotable> line2Remove = lineMap.at(lineId);
            std::shared_ptr<IDrawable> drawable2Remove = std::dynamic_pointer_cast<GLPL::IDrawable>(line2Remove);
            // Remove child
            IDrawable::removeChild(drawable2Remove);
            // Remove axes
            lineMap.erase(lineId);
        } else {
            std::cout << "Cannot remove Line " << lineId << ", Line does not exist!" << std::endl;
        }
    }

    void AxesArea::updateAxesViewportTransform() {
        // Update Transform
        glm::mat4 viewportTransform = GLPL::Transforms::viewportTransform(x, y, width, height);
        glm::mat4 axesLimitsTransform = AxesArea::scale2AxesLimits();
        axesViewportTransformation = std::make_shared<glm::mat4>(parentTransform * viewportTransform * axesLimitsTransform);
        // Update Children Lines
        for(auto & i : lineMap) {
            i.second->setAxesViewportTransform(axesViewportTransformation);
        }
    }

    void AxesArea::setPosition(float newX, float newY) {
        // Set position
        this->x = newX;
        this->y = newY;
        // Set position in pixels
        IDrawable::updatePositionPx();
        // Update Transforms
        IDrawable::updateTransforms();
        AxesArea::updateAxesViewportTransform();
        // Update Children
        IDrawable::updateChildren();
    }

    void AxesArea::setSize(float newWidth, float newHeight) {
        // New width and height relative to their parent
        // Update size
        this->width = newWidth;
        this->height = newHeight;
        // Update in pixels
        updateSizePx();
        // Update Transforms
        IDrawable::updateTransforms();
        AxesArea::updateAxesViewportTransform();
        // Update Children
        this->updateChildren();
    }

    void GLPL::AxesArea::updateSizePx() {
        this->widthPx = (int) (this->width * (float)parentWidthPx);
        this->heightPx = (int) (this->height * (float)parentHeightPx);
    }

    void GLPL::AxesArea::setParentDimensions(glm::mat4 newParentTransform,
                                                          int newParentXPx,
                                                          int newParentYPx,
                                                          int newParentWidthPx,
                                                          int newParentHeightPx) {
        this->parentXPx = newParentXPx;
        this->parentYPx = newParentYPx;
        this->parentWidthPx = newParentWidthPx;
        this->parentHeightPx = newParentHeightPx;
        this->parentTransform = newParentTransform;
        updatePositionPx();
        updateSizePx();
        updateTransforms();
        AxesArea::updateAxesViewportTransform();
        // Update Children
        updateChildren();
    }

    void GLPL::AxesArea::setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions) {
        this->parentTransform = parentDimensions->parentTransform;
        this->parentXPx = parentDimensions->parentXPx;
        this->parentYPx = parentDimensions->parentYPx;
        this->parentWidthPx = parentDimensions->parentWidthPx;
        this->parentHeightPx = parentDimensions->parentHeightPx;
        this->shaderSetPt = parentDimensions->shaderSet;
        updatePositionPx();
        updateSizePx();
        updateTransforms();
        AxesArea::updateAxesViewportTransform();
        // Update Children
        updateChildren();
    }

    glm::mat4 AxesArea::scale2AxesLimits() {
        // Creates a transformation matrix to scale points to the axes limits
        // Calculate center of current limits
        float xShift = ((xmin+xmax)/2.0)/(xmax-xmin) * 2.0; // xavg/width * 2.0, *2 to take it to -1 to 1
        float yShift = ((ymin+ymax)/2.0)/(ymax-ymin) * 2.0; // yavg/height * 2.0, *2 to take it to -1 to 1

        // Translate by offset
        glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(-xShift, -yShift,0));

        // Scale to limits
        float scaleX = 2.0/(xmax-xmin); // Inverted due to -1 to 1 mapping (less than abs(1) region)
        float scaleY = 2.0/(ymax-ymin); // Inverted due to -1 to 1 mapping (less than abs(1) region)
        glm::mat4 scale = glm::scale(trans, glm::vec3(scaleX,scaleY,1));

        return scale;
    }



}