//
// Created by bcub3d-desktop on 20/11/22.
//

// Standard Includes
#include <utility>

// Project Includes
#include "Legend.h"
#include "../shadedLines/ShadedLine2D2CircularVecs.h"
#include "../lines/Line2D2Vecs.h"
#include "../scatterPlot/IScatterPlot.h"
#include "../scatterPlot/Scatter2D2Vecs.h"


namespace GLPL {

    Legend::Legend(float x, float y, std::shared_ptr<ParentDimensions> parentDimensions, AttachLocation attachLocation) :
        ConstantXYDrawable(x, y, 0.275f, 0.35f, CONSTANT_SCALE, CONSTANT_SCALE, std::move(parentDimensions), attachLocation) {

        // Create background rectangle
        std::shared_ptr<ParentDimensions> backgroundParentDimensions = createParentDimensions();
        rect = std::make_shared<Rectangle>(0.0f, 0.0f, 1.0f, 1.0f, backgroundParentDimensions);
        rect->setBackgroundColor(legendBackgroundColor);
        rect->setOutlineColor(legendOutlineColor);
        Legend::registerChild(rect);

    }

    std::string Legend::getID() {
        return "Legend: " + std::to_string(x) + ":" + std::to_string(y);
    }

    void Legend::Draw() {
        createRow();

        rect->Draw();

        // Draw children
        for(auto & i : children) {
            i->Draw();
        }

        // Draw line/markers
        for(auto & plotablePair : legendItemMap) {
            std::shared_ptr<Rectangle> currRect = std::get<2>(plotablePair.second);
            std::shared_ptr<Plotable> plotable = std::get<0>(plotablePair.second);
            std::shared_ptr<ParentDimensions> parentDimensions = currRect->createParentDimensions();
            plotable->drawLegendEntry(parentDimensions);
        }

    }

    void Legend::createRow() {
        // TODO - Handle removable of plot lines from legend entry
        // Adjust position of text labels and resize legend box
        if (textChanged) {
            // Determine size of box
            float heightOffset = 0.01f;
            float currHeight = heightOffset;
            float maxWidth = 0.0f;

            for (std::pair<unsigned int, std::tuple<std::shared_ptr<Plotable>, std::shared_ptr<TextString>, std::shared_ptr<Rectangle>>> textEntry : legendItemMap) {
                std::shared_ptr<TextString> textStringPt = std::get<1>(textEntry.second);
                if (!textStringPt->getTextString().empty()) {
                    // Determine maximum width
                    maxWidth = std::max(maxWidth, textStringPt->getWidth()*1.02f);

                    // Get total Height
                    currHeight += textStringPt->getHeight() + heightOffset;
                }
            }

            // Set box size
            float newWidth = (maxWidth + 0.3f)*this->getWidth();
            float newHeight = currHeight*this->getHeight();
            this->setSize(newWidth, newHeight);

            // Set text label vertical position
            currHeight = heightOffset;
            for (std::pair<unsigned int, std::tuple<std::shared_ptr<Plotable>, std::shared_ptr<TextString>, std::shared_ptr<Rectangle>>> textEntry : legendItemMap) {
                std::shared_ptr<TextString> textStringPt = std::get<1>(textEntry.second);
                std::shared_ptr<Rectangle> currRect = std::get<2>(textEntry.second);
                if (!textStringPt->getTextString().empty()) {
                    // Position text string
                    std::array<float, 2> oldPosition = textStringPt->getPosition();
                    textStringPt->setPosition(oldPosition[0]-0.05f, currHeight + 0.05f);

                    // Position marker box
                    currRect->setPosition(0.025f, currHeight + 0.05f);
                    currRect->setSize(0.2f, textStringPt->getHeight() - 0.05f);

                    std::cout << currHeight << std::endl;

                    currHeight += textStringPt->getHeight() + heightOffset;

                    std::cout << textStringPt->getPosition()[1] << " : " << currRect->getPosition()[1] << std::endl;
                }
            }
            textChanged = false;
        }

    }

    void Legend::addPlotableToLegend(std::string label, std::shared_ptr<Plotable> plotable) {
        // Create rectangle and text entry to match
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = Legend::createParentDimensions();
        // Create Text String
        std::shared_ptr<IDrawable> textStringObj = std::make_shared<TextString>(label, 0.3f, 0.0f, 10.0f, newParentPointers);
        std::shared_ptr<TextString> textStringPt = std::dynamic_pointer_cast<TextString>(textStringObj);
        // Set pin position
        textStringPt->setAttachLocation(BOTTOM_LEFT);

        // Register Child
        Legend::registerChild(textStringObj);

        // Create rectangle to store marker outline
        //std::shared_ptr<ParentDimensions> newParentPointers2 = rect->createParentDimensions();
        std::shared_ptr<Rectangle> newRect = std::make_shared<Rectangle>(-1.0f, -1.0f, 1.0f, 1.0f, newParentPointers);
        newRect->setBackgroundColor(glm::vec4(0.0f));
        newRect->setOutlineColor(glm::vec4(0.0f));
        newRect->setAttachLocation(BOTTOM_LEFT);
        // Register Child
        Legend::registerChild(newRect);

        // Create tuple
        std::tuple<std::shared_ptr<Plotable>, std::shared_ptr<TextString>, std::shared_ptr<Rectangle>> legendItem = std::make_tuple(plotable, textStringPt, newRect);
        std::pair<unsigned int, std::tuple<std::shared_ptr<Plotable>, std::shared_ptr<TextString>, std::shared_ptr<Rectangle>>> legendEntry = std::make_pair(legendItemCount, legendItem);
        legendItemMap.insert(legendEntry);
        legendItemCount += 1;

        textChanged = true;
    }


}

