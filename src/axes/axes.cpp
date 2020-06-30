//
// Created by tbatt on 19/04/2020.
//

// Standard Includes

// Project Includes
#include "axes.h"



namespace GLPL {

    Axes::Axes(float x, float y, float width, float height,
               std::shared_ptr<ParentDimensions> parentDimensions) :
            ConstantScaleDrawable(x, y, width, height, std::move(parentDimensions)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        // TODO - Remove text string creation
        Axes::addText("Test String", 0.0, 0.0, 10);
        Axes::addText("2nd Test String", 0.25, 0.9, 15);
        Axes::addText("3rd Test String", 0.495, 0.0, 15);

        this->getText(0)->setZDepthValue(0);
        this->getText(1)->setZDepthValue(10);
        this->getText(2)->setZDepthValue(-10);

        // Add axes area
        Axes::createAxesArea();

    }

    void Axes::createAxesArea() {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register Child
        std::shared_ptr<IDrawable> axesAreaObj = std::make_shared<AxesArea>(0.2, 0.2, 0.6, 0.6, newParentPointers);
        std::shared_ptr<AxesArea> axesAreaPt = std::dynamic_pointer_cast<AxesArea>(axesAreaObj);
        Axes::registerChild(axesAreaObj);
        // Store Text String
        this->axesArea = axesAreaPt;
    }

    void Axes::addText(const char* textString, float x, float y, float fontSize) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register Child
        std::shared_ptr<IDrawable> textStringObj = std::make_shared<TextString>(textString, x, y, fontSize, newParentPointers);
        std::shared_ptr<TextString> textStringPt = std::dynamic_pointer_cast<TextString>(textStringObj);
        Axes::registerChild(textStringObj);
        // Store Text String
        textStringMap.insert(std::pair<unsigned int, std::shared_ptr<TextString>>(textStringCount, textStringPt));
        textStringCount += 1;
    }

    std::shared_ptr<TextString> Axes::getText(unsigned int textStringId) {
        if (textStringMap.count(textStringId) > 0) {
            return textStringMap.at(textStringId);
        } else {
            std::cout << "TextString " << textStringId << " does not exist!" << std::endl;
            return nullptr;
        }
    }

    void Axes::removeTextString(unsigned int textStringId) {
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

    void Axes::Draw() {
        // Sort children if required
        IDrawable::sortChildren();
        // Draw children
        for(auto & i : children) {
            i->Draw();
        }
    }

    std::shared_ptr<ILine2D> Axes::addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY, LineType lineType, glm::vec3 colour,
                       float opacityRatio) {
        return axesArea->addLine(dataPtX, dataPtY, lineType, colour, opacityRatio);
    }

}