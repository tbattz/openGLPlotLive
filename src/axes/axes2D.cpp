//
// Created by bcub3d-desktop on 30/1/21.
//

#include "axes2D.h"


namespace GLPL {


    Axes2D::Axes2D(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions)
            : Axes(x, y, width, height, parentDimensions) {

        // Create Axes
        Axes2D::createAxesLines();

    }

    void Axes2D::createAxesLines() {
        // Get parent pointers
        std::shared_ptr<ParentDimensions> ourParentDimensions = createParentDimensions();
        // Create axes
        axesArea->addAxesLine("x", X_AXES_CENTRE);
        axesArea->addAxesLine("y", Y_AXES_CENTRE);
    }

    void Axes2D::addText(const char* textString, float x, float y, float fontSize, AttachLocation attachLocation) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register Child
        std::shared_ptr<IDrawable> textStringObj = std::make_shared<TextString>(textString, x, y, fontSize, newParentPointers);
        std::shared_ptr<TextString> textStringPt = std::dynamic_pointer_cast<TextString>(textStringObj);
        // Set pin position
        textStringPt->setAttachLocation(attachLocation);
        // Register Child
        Axes::registerChild(textStringObj);
        // Store Text String
        textStringMap.insert(std::pair<unsigned int, std::shared_ptr<TextString>>(textStringCount, textStringPt));
        textStringCount += 1;
    }

    std::shared_ptr<TextString> Axes2D::getText(unsigned int textStringId) {
        if (textStringMap.count(textStringId) > 0) {
            return textStringMap.at(textStringId);
        } else {
            std::cout << "TextString " << textStringId << " does not exist!" << std::endl;
            return nullptr;
        }
    }

    void Axes2D::removeTextString(unsigned int textStringId) {
        if (textStringMap.count(textStringId) > 0) {
            std::shared_ptr<TextString> textString2Remove = textStringMap.at(textStringId);
            // Remove child
            IDrawable::removeChild(textString2Remove);
            // Remove axes
            textStringMap.erase(textStringId);
        } else {
            std::cout << "Cannot remove TextString " << textStringId << ", TextString does not exist!" << std::endl;
        }
    }

    std::shared_ptr<ILine2D> Axes2D::addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY, LineType lineType, glm::vec3 colour,
                                           float opacityRatio) {
        return axesArea->addLine(dataPtX, dataPtY, lineType, colour, opacityRatio);
    }

    std::shared_ptr<IScatterPlot> Axes2D::addScatterPlot(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                                                       glm::vec3 colour, float opacityRatio, MarkerType markerType) {
        return axesArea->addScatterPlot(dataPtX, dataPtY, colour, opacityRatio, markerType);
    }

    void Axes2D::setTitle(std::string newTitle) {
        axesArea->setText("axes-title", std::move(newTitle));
    }

    void Axes2D::setXLabel(std::string newXLabel) {
        axesArea->setText("x-label", std::move(newXLabel));
    }

    void Axes2D::setYLabel(std::string newYLabel) {
        axesArea->setText("y-label", std::move(newYLabel));
    }

    void Axes2D::setXLabelRotation(TextRotation newTextRotation) {
        axesArea->setTextRotation("x-label", newTextRotation);
    }

    void Axes2D::setYLabelRotation(TextRotation newTextRotation) {
        axesArea->setTextRotation("y-label", newTextRotation);
    }


}