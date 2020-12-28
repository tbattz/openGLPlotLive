//
// Created by bcub3d-desktop on 4/7/20.
//

#include "AxesLineTicks.h"


namespace GLPL {

    AxesLineTicks::AxesLineTicks(AxesDirection axesDirection,
                                 std::shared_ptr<ParentDimensions> parentDimensions) :
                                 ConstantXYDrawable(0.0f, 0.0f, 1.0f, 1.0f, CONSTANT_SCALE, CONSTANT_SIZE, std::move(parentDimensions)) {
        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.78f, 0.0f, 1.0f);
        // Set Not Hoverable
        setHoverable(false);

        // Setup Buffers
        AxesLineTicks::createAndSetupAxesLineBuffers();

        // Set Axes Direction
        this->axesDirection = axesDirection;

        // Rescale pixel size for high DPI
        AxesLineTicks::setPixelSpacing(minorTickSpacingPx, minorTickLengthPx, majorTickLengthPx);

        // Generate Axes Line
        AxesLineTicks::generateAllVertices();

        // Set size
        AxesLineTicks::updateSize();

    }

    void AxesLineTicks::generateAllVertices() {
        // Generate the Axes Line
        AxesLineTicks::generateAxesLines();

        // Generate the ticks
        AxesLineTicks::generateTickVerts();

        // Update the buffers
        AxesLineTicks::updateAxesLineBuffers();

        // Generate major tick labels
        AxesLineTicks::generateMajorTickLabels();
        // Check overlapping
        while (currFontSize > 0 && isLabelsOverlapping()) {
            currFontSize -= 1;
            AxesLineTicks::setMajorTickFontSize(currFontSize);
            AxesLineTicks::generateMajorTickLabels();
            // Update size
            AxesLineTicks::updateSize();
        }

        // Update mouse over verts
        IDrawable::calcMouseOverVertsWithChildren();
        IDrawable::calcMouseOverVertsNoChildren();
    }

    void AxesLineTicks::generateAxesLines() {
        switch(axesDirection) {
            case X_AXES_TOP: {
                // Line on bottom
                axesLineVerts = {-1.0, -1.0, 1.0, -1.0};
                setXYScale(CONSTANT_SCALE, CONSTANT_SIZE);
                setAttachLocation(BOTTOM_LEFT);
                setPosition(0.0f, 1.0f);
                break;
            }
            case X_AXES_BOTTOM: {
                // Line on top
                axesLineVerts = {-1.0, 1.0, 1.0, 1.0};
                setXYScale(CONSTANT_SCALE, CONSTANT_SIZE);
                setAttachLocation(TOP_LEFT);
                setPosition(0.0f, 0.0f);
                break;
            }
            case X_AXES_CENTRE: {
                // Line in centre
                // Calculate the position within the window
                float yMid = clip(-yMin/(yMax - yMin), 0.0f, 1.0f);   // In 0 to 1
                axesLineVerts = {-1.0, 0.0, 1.0, 0.0};
                setXYScale(CONSTANT_SCALE, CONSTANT_SIZE);
                setAttachLocation(CENTRE);
                setPosition(0.5f, yMid);
                break;
            }
            case Y_AXES_LEFT: {
                // Line on right
                axesLineVerts = {1.0, -1.0, 1.0, 1.0};
                setXYScale(CONSTANT_SIZE, CONSTANT_SCALE);
                setAttachLocation(BOTTOM_RIGHT);
                setPosition(0.0f, 0.0f);
                break;
            }
            case Y_AXES_RIGHT: {
                // Line on left
                axesLineVerts = {-1.0, -1.0, -1.0, 1.0};
                setXYScale(CONSTANT_SIZE, CONSTANT_SCALE);
                setAttachLocation(BOTTOM_LEFT);
                setPosition(1.0f, 0.0f);
                break;
            }
            case Y_AXES_CENTRE: {
                // Line in centre
                // Calculate the position within the window
                float xMid = clip(-xMin/(xMax - xMin), 0.0f, 1.0f);   // In 0 to 1
                axesLineVerts = {0.0, -1.0, 0.0, 1.0};
                setXYScale(CONSTANT_SIZE, CONSTANT_SCALE);
                setAttachLocation(CENTRE);
                setPosition(xMid, 0.5f);
                break;
            }
            default: {
                std::cout << "Invalid Axes Direction!" << std::endl;
            }
        }
    }

    std::tuple<unsigned int, std::vector<TickSpacingType>, std::vector<float>, std::vector<float>>
    AxesLineTicks::generateEquallySpacingBetweenLimits(float sectionWidthRel,
                                                       float sectionWidthAxesUnits,
                                                       float midRelPos) {
        // Negative values
        float currRelPos = midRelPos;
        float currAxesPos = 0.0f;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;
        unsigned int count = 0;
        while (currRelPos >= -1.0f && currRelPos <= 1.0f) {
            // Store tick type
            if (count % (minorSpacingsPerMajor + 1) == 0) {
                tickSpacingType.push_back(MAJOR_SPACING);
            } else {
                tickSpacingType.push_back(MINOR_SPACING);
            }
            // Store relative position
            relPos.push_back(currRelPos);
            // Store axes position
            axesPos.push_back(currAxesPos);
            // Update position
            currRelPos -= sectionWidthRel;
            currAxesPos -= sectionWidthAxesUnits;
            // Increment count
            count += 1;
        }
        // Store cross over point
        unsigned int crossIndex = relPos.size() - 1;
        // Positive Values
        currRelPos = midRelPos;
        currAxesPos = 0.0f;
        count = 0;
        while (currRelPos <= 1.0f && currRelPos >= -1.0f) {
            // Store tick type
            if (count % (minorSpacingsPerMajor + 1) == 0) {
                tickSpacingType.push_back(MAJOR_SPACING);
            } else {
                tickSpacingType.push_back(MINOR_SPACING);
            }
            // Store relative position
            relPos.push_back(currRelPos);
            // Store axes position
            axesPos.push_back(currAxesPos);
            // Update position
            currRelPos += sectionWidthRel;
            currAxesPos += sectionWidthAxesUnits;
            // Increment count
            count += 1;
        }

        return std::make_tuple(crossIndex, tickSpacingType, relPos, axesPos);
    }

    void AxesLineTicks::generateXTopTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos) {
        if (tickSpacingType == MAJOR_SPACING) {
            // Major
            // Store relative position
            majorTickVerts.push_back(relPos);   // x1
            majorTickVerts.push_back(-1.0f);    // y1
            majorTickVerts.push_back(relPos);   // x2
            majorTickVerts.push_back(1.0f);     // y2
            // Store axes position
            majorTickAxesPos.push_back(axesPos);
        } else if (tickSpacingType == MINOR_SPACING) {
            // Minor
            float relLen = 2.0f * (float)minorTickLengthPx / (float)majorTickLengthPx;
            // Store relative position
            minorTickVerts.push_back(relPos);          // x1
            minorTickVerts.push_back(-1.0f);           // y1
            minorTickVerts.push_back(relPos);          // x2
            minorTickVerts.push_back(-1.0f + relLen);  // y2
            // Store axes position
            minorTickAxesPos.push_back(axesPos);
        }
    }

    void AxesLineTicks::generateXBottomTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos) {
        if (tickSpacingType == MAJOR_SPACING) {
            // Major
            // Store relative position
            majorTickVerts.push_back(relPos);  // x1
            majorTickVerts.push_back(-1.0f);   // y1
            majorTickVerts.push_back(relPos);  // x2
            majorTickVerts.push_back(1.0f);    // y2
            // Store axes position
            majorTickAxesPos.push_back(axesPos);
        } else if (tickSpacingType == MINOR_SPACING) {
            // Minor
            float relLen = 2.0f * (float)minorTickLengthPx / (float)majorTickLengthPx;
            // Store relative position
            minorTickVerts.push_back(relPos);         // x1
            minorTickVerts.push_back(1.0f);           // y1
            minorTickVerts.push_back(relPos);         // x2
            minorTickVerts.push_back(1.0f - relLen);  // y2
            // Store axes position
            minorTickAxesPos.push_back(axesPos);
        }
    }

    void AxesLineTicks::generateXCentreTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos) {
        if (tickSpacingType == MAJOR_SPACING) {
            // Major
            // Store relative position
            majorTickVerts.push_back(relPos);  // x1
            majorTickVerts.push_back(-1.0f);   // y1
            majorTickVerts.push_back(relPos);  // x2
            majorTickVerts.push_back(1.0f);    // y2
            // Store axes position
            majorTickAxesPos.push_back(axesPos);
        } else if (tickSpacingType == MINOR_SPACING) {
            // Minor
            float relLen = (float)minorTickLengthPx / (float)majorTickLengthPx;
            // Store relative position
            minorTickVerts.push_back(relPos);   // x1
            minorTickVerts.push_back(-relLen);  // y1
            minorTickVerts.push_back(relPos);   // x2
            minorTickVerts.push_back(relLen);   // y2
            // Store axes position
            minorTickAxesPos.push_back(axesPos);
        }
    }

    void AxesLineTicks::generateYLeftTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos) {
        if (tickSpacingType == MAJOR_SPACING) {
            // Major
            // Store relative position
            majorTickVerts.push_back(1.0f);     // x1
            majorTickVerts.push_back(relPos);   // y1
            majorTickVerts.push_back(-1.0f);    // x2
            majorTickVerts.push_back(relPos);   // y2
            // Store axes position
            majorTickAxesPos.push_back(axesPos);
        } else if (tickSpacingType == MINOR_SPACING) {
            // Minor
            float relLen = 2.0f * (float)minorTickLengthPx / (float)majorTickLengthPx;
            // Store relative position
            minorTickVerts.push_back(1.0f);           // x1
            minorTickVerts.push_back(relPos);         // y1
            minorTickVerts.push_back(1.0f - relLen);  // x2
            minorTickVerts.push_back(relPos);         // y2
            // Store axes position
            minorTickAxesPos.push_back(axesPos);
        }
    }

    void AxesLineTicks::generateYRightTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos) {
        if (tickSpacingType == MAJOR_SPACING) {
            // Major
            // Store relative position
            majorTickVerts.push_back(-1.0f);   // x1
            majorTickVerts.push_back(relPos);  // y1
            majorTickVerts.push_back(1.0f);    // x2
            majorTickVerts.push_back(relPos);  // y2
            // Store axes position
            majorTickAxesPos.push_back(axesPos);
        } else if (tickSpacingType == MINOR_SPACING) {
            // Minor
            float relLen = 2.0f * (float)minorTickLengthPx / (float)majorTickLengthPx;
            // Store relative position
            minorTickVerts.push_back(-1.0f);          // x1
            minorTickVerts.push_back(relPos);         // y1
            minorTickVerts.push_back(relLen - 1.0f);  // x2
            minorTickVerts.push_back(relPos);         // y2
            // Store axes position
            minorTickAxesPos.push_back(axesPos);
        }
    }

    void AxesLineTicks::generateYCentreTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos) {
        if (tickSpacingType == MAJOR_SPACING) {
            // Major
            // Store relative position
            majorTickVerts.push_back(-1.0f);   // x1
            majorTickVerts.push_back(relPos);  // y1
            majorTickVerts.push_back(1.0f);    // x2
            majorTickVerts.push_back(relPos);  // y2
            // Store axes position
            majorTickAxesPos.push_back(axesPos);
        } else if (tickSpacingType == MINOR_SPACING) {
            // Minor
            float relLen = (float)minorTickLengthPx / (float)majorTickLengthPx;
            // Store relative position
            minorTickVerts.push_back(-relLen);  // x1
            minorTickVerts.push_back(relPos);   // y1
            minorTickVerts.push_back(relLen);   // x2
            minorTickVerts.push_back(relPos);   // y2
            // Store axes position
            minorTickAxesPos.push_back(axesPos);
        }
    }

    void AxesLineTicks::generateXTickVerts() {
        float pixelsPerUnit = (float)getWidthPx() / 2.0f;
        float minorSectionWidthRel = (float)minorTickSpacingPx / pixelsPerUnit; // -1 to 1
        float minorSectionWidthAxesUnits = minorSectionWidthRel * (xMax - xMin) / 2.0f; // xMin to xMax
        float xMidRelPos = (xMax + xMin) / (xMin - xMax); // -1 to 1
        // Generate Sequence
        unsigned int crossIndex;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;
        std::tie(crossIndex, tickSpacingType, relPos, axesPos) = generateEquallySpacingBetweenLimits(minorSectionWidthRel, minorSectionWidthAxesUnits, xMidRelPos);
        // Generate vertices
        for(unsigned int i=0; i < relPos.size(); i++) {
            if (axesPos[i] >= (xMin - abs(0.01 * xMin)) && axesPos[i] <= (xMax + abs(0.01 * xMax))) {
                switch (axesDirection) {
                    case X_AXES_TOP: {
                        AxesLineTicks::generateXTopTickVerts(tickSpacingType[i], relPos[i], axesPos[i]);
                        break;
                    }
                    case X_AXES_BOTTOM: {
                        AxesLineTicks::generateXBottomTickVerts(tickSpacingType[i], relPos[i], axesPos[i]);
                        break;
                    }
                    case X_AXES_CENTRE: {
                        AxesLineTicks::generateXCentreTickVerts(tickSpacingType[i], relPos[i], axesPos[i]);
                        break;
                    }
                    case Y_AXES_LEFT:
                    case Y_AXES_RIGHT:
                    case Y_AXES_CENTRE:
                    default: {
                    }
                }
            }
        }
    }

    void AxesLineTicks::generateYTickVerts() {
        float pixelsPerUnit = (float)getHeightPx() / 2.0f;
        float minorSectionWidthRel = (float)minorTickSpacingPx / pixelsPerUnit; // -1 to 1
        float minorSectionWidthAxesUnits = minorSectionWidthRel * (yMax - yMin) / 2.0f; // yMin to yMax
        float yMidRelPos = (yMax + yMin) / (yMin - yMax); // -1 to 1
        // Generate Sequence
        unsigned int crossIndex;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;
        std::tie(crossIndex, tickSpacingType, relPos, axesPos) = generateEquallySpacingBetweenLimits(minorSectionWidthRel, minorSectionWidthAxesUnits, yMidRelPos);
        // Generate vertices
        for (unsigned int i = 0; i < relPos.size(); i++) {
            if (axesPos[i] >= (yMin - abs(0.01 * yMin)) && axesPos[i] <= (yMax + abs(0.01 * yMax))) {
                switch (axesDirection) {
                    case X_AXES_TOP:
                    case X_AXES_BOTTOM:
                    case X_AXES_CENTRE: {
                        break;
                    }
                    case Y_AXES_LEFT: {
                        AxesLineTicks::generateYLeftTickVerts(tickSpacingType[i], relPos[i], axesPos[i]);
                        break;
                    }
                    case Y_AXES_RIGHT: {
                        AxesLineTicks::generateYRightTickVerts(tickSpacingType[i], relPos[i], axesPos[i]);
                        break;
                    }
                    case Y_AXES_CENTRE: {
                        AxesLineTicks::generateYCentreTickVerts(tickSpacingType[i], relPos[i], axesPos[i]);
                        break;
                    }
                    default: {
                    }
                }
            }
        }
    }

    void AxesLineTicks::generateTickVerts() {
        // Calculate the distribution of ticks
        // Clear previous entries
        minorTickVerts.clear();
        minorTickAxesPos.clear();
        majorTickVerts.clear();
        majorTickAxesPos.clear();
        // Create based around the axes mid position
        switch(axesDirection) {
            case X_AXES_BOTTOM:
            case X_AXES_TOP:
            case X_AXES_CENTRE: {
                AxesLineTicks::generateXTickVerts();
                break;
            }
            case Y_AXES_LEFT:
            case Y_AXES_RIGHT:
            case Y_AXES_CENTRE: {
                AxesLineTicks::generateYTickVerts();
                break;
            }
            default: {}
        }
    }

    std::pair<float, float>  AxesLineTicks::generateTickLabelVerts(float xPos, float yPos) {
        float labelXPos, labelYPos;
        switch(axesDirection) {
            case X_AXES_TOP: {
                labelXPos = xPos;
                labelYPos = yOffsetFactor * abs(yPos);
                break;
            }
            case X_AXES_BOTTOM: {
                labelXPos = xPos;
                labelYPos = -yOffsetFactor * abs(yPos);
                break;
            }
            case X_AXES_CENTRE: {
                labelXPos = xPos;
                labelYPos = -yOffsetFactor * abs(yPos);
                break;
            }
            case Y_AXES_LEFT: {
                labelXPos = -xOffsetFactor * abs(xPos);
                labelYPos = yPos;
                break;
            }
            case Y_AXES_RIGHT: {
                labelXPos = xOffsetFactor * abs(xPos);
                labelYPos = yPos;
                break;
            }
            case Y_AXES_CENTRE: {
                labelXPos = -xOffsetFactor * abs(xPos);
                labelYPos = yPos;
                break;
            }
            default: {
                labelXPos = xPos;
                labelYPos = yPos;
            }
        }
        labelXPos = 0.5f * (labelXPos + 1); // Convert (-1 to 1) to (0 to 1)
        labelYPos = 0.5f * (labelYPos + 1); // Convert (-1 to 1) to (0 to 1)

        return {labelXPos, labelYPos};

    }

    AttachLocation AxesLineTicks::generateMajorTickOffsetAttachLocation() {
        AttachLocation attachLocation;
        switch(axesDirection) {
            case X_AXES_TOP: {
                attachLocation = CENTRE_BOTTOM;
                break;
            }
            case X_AXES_BOTTOM: {
                attachLocation = CENTRE_TOP;
                break;
            }
            case X_AXES_CENTRE: {
                attachLocation = CENTRE_TOP;
                break;
            }
            case Y_AXES_LEFT: {
                attachLocation = CENTRE_RIGHT;
                break;
            }
            case Y_AXES_RIGHT: {
                attachLocation = CENTRE_LEFT;
                break;
            }
            case Y_AXES_CENTRE: {
                attachLocation = CENTRE_RIGHT;
                break;
            }
            default: {
                attachLocation = CENTRE;
            }
        }
        return attachLocation;
    }

    std::string AxesLineTicks::value2NeatStr(float inValue, unsigned int maxChar, unsigned int maxDecimal) {
        // Setup buffer
        char textBuf[10];
        std::string text;
        // Get length
        unsigned int count = 1;
        float divValue = inValue;
        // TODO - Make this cleaner
        if (abs(divValue) > 1e-15) {
            while (abs(divValue) < 1) {
                divValue = divValue * 10;
                count += 1;
            }
            while (abs(divValue) > 10) {
                divValue = divValue / 10;
                count += 1;
            }
            // Check length is greater than max char
            if (count < maxChar) {
                unsigned int decCount = maxChar - count;
                if (decCount > maxDecimal) {
                    decCount = maxDecimal;
                }
                const char *formatStr = ("%." + std::to_string(decCount) + "f").c_str();
                sprintf(textBuf, formatStr, inValue);
                text = std::string(textBuf);
            } else {
                // Exponential format
                int decCount = count - 3;
                if (decCount < 0) {
                    decCount = 0;
                }
                const char *formatStr = ("%." + std::to_string(decCount) + "f").c_str();
                if (abs(inValue) < 1) {
                    sprintf(textBuf, formatStr, inValue * (pow(10.0, count - 1)));
                } else {
                    sprintf(textBuf, formatStr, inValue / (pow(10.0, count - 1)));
                }
                text.append(std::string(textBuf));
                if (inValue < 1) {
                    text.append("-");
                }
                text.append("e");
                text.append(std::to_string(count));
            }
        }

        return text;
    }

    bool AxesLineTicks::isLabelsOverlapping() {
        bool overlapping = false;
        float prevMin = 0;
        float prevMax = 0;
        float currMin = 0;
        float currMax = 0;

        if (!majorTickTextStrings.empty()) {
            for(unsigned int i=0; i < majorTickTextStrings.size() - 1; i++) {
                if ((strcmp(majorTickTextStrings[i + 1]->getTextString().c_str(), "0.00") != 0) &&
                    (strcmp(majorTickTextStrings[i + 1]->getTextString().c_str(), "") != 0)) {
                    switch (axesDirection) {
                        case X_AXES_TOP:
                        case X_AXES_BOTTOM:
                        case X_AXES_CENTRE: {
                            prevMin = majorTickTextStrings[i]->getLeft();
                            prevMax = majorTickTextStrings[i]->getRight();
                            currMin = majorTickTextStrings[i + 1]->getLeft();
                            currMax = majorTickTextStrings[i + 1]->getRight();
                            break;
                        }
                        case Y_AXES_LEFT:
                        case Y_AXES_RIGHT:
                        case Y_AXES_CENTRE: {
                            prevMin = majorTickTextStrings[i]->getBottom();
                            prevMax = majorTickTextStrings[i]->getTop();
                            currMin = majorTickTextStrings[i + 1]->getBottom();
                            currMax = majorTickTextStrings[i + 1]->getTop();
                            break;
                        }
                        default: {
                            std::cout << "Invalid Axes Direction!" << std::endl;
                        }
                    }

                    if (((prevMin < currMin) && (currMin < prevMax)) ||
                        ((prevMin < currMax) && (currMax < prevMax))) {
                        // Value is overlapping
                        overlapping = true;
                        break;
                    }
                }
            }
        }

        return overlapping;
    }

    void AxesLineTicks::setMajorTickFontSize(float fontSize) {
        for(auto & majorTickTextString : majorTickTextStrings) {
            majorTickTextString->setFontSize(fontSize);
        }
        currFontSize = fontSize;
    }

    std::vector<float> AxesLineTicks::getAxesTickPos() {
        return majorTickAxesPos;
    }

    void AxesLineTicks::generateMajorTickLabels() {
        // Generate labels
        unsigned int usedCount = 0;
        for(unsigned int i=0; i < majorTickAxesPos.size(); i++) {
            // Create Parent Dimensions
            std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
            // Create Text String
            std::string text = value2NeatStr(majorTickAxesPos[i], 4, 2);
            // Check that the label should not be the zero label
            if ((axesDirection != X_AXES_CENTRE and axesDirection != Y_AXES_CENTRE) or text != "0.00") {
                // Create text string
                std::pair<float, float> labelPos = AxesLineTicks::generateTickLabelVerts(majorTickVerts[4 * i], majorTickVerts[4 * i + 1]);
                // Check if a text string already exists
                if (majorTickTextStrings.size() < usedCount + 1) {
                    // Create new text string
                    std::shared_ptr<TextString> textStringPt = std::make_shared<TextString>(text, labelPos.first, labelPos.second, baseFontSize, newParentPointers);
                    // Set pin position
                    AttachLocation attachLocation = generateMajorTickOffsetAttachLocation();
                    textStringPt->setAttachLocation(attachLocation);
                    // Register Child
                    AxesLineTicks::registerChild(textStringPt);
                    // Store Text String
                    majorTickTextStrings.push_back(textStringPt);
                } else {
                    // Use existing text string
                    std::shared_ptr<TextString> textStringPt = majorTickTextStrings[usedCount];
                    textStringPt->setTextString(text);
                    textStringPt->setPosition(labelPos.first, labelPos.second);
                }
                // Increment the in use counter
                usedCount += 1;
            }
        }

        // Hide Excess Text Strings
        for(unsigned int i=usedCount; i < majorTickTextStrings.size(); i++) {
            majorTickTextStrings[i]->setTextString("");
        }

    }

    void AxesLineTicks::setPixelSpacing(unsigned int newMinorTickSpacingPx, unsigned int newMinorTickLengthPx, unsigned int newMajorTickLengthPx) {
        if (axesDirection == X_AXES_TOP || axesDirection == X_AXES_BOTTOM || axesDirection == X_AXES_CENTRE) {
            float yScaleDpi = this->shaderSetPt->getYDpiScaling();
            this->minorTickSpacingPx = (int)(yScaleDpi * (float)newMinorTickSpacingPx);
            this->minorTickLengthPx = (int)(yScaleDpi * (float)newMinorTickLengthPx);
            this->majorTickLengthPx = (int)(yScaleDpi * (float)newMajorTickLengthPx);
        } else if (axesDirection == Y_AXES_LEFT || axesDirection == Y_AXES_CENTRE || axesDirection == Y_AXES_RIGHT) {
            float xScaleDpi = this->shaderSetPt->getXDpiScaling();
            this->minorTickSpacingPx = (int)(xScaleDpi * (float)newMinorTickSpacingPx);
            this->minorTickLengthPx = (int)(xScaleDpi * (float)newMinorTickLengthPx);
            this->majorTickLengthPx = (int)(xScaleDpi * (float)newMajorTickLengthPx);
        }

    }


    void AxesLineTicks::updateSize() {
        switch (axesDirection) {
            case X_AXES_TOP:
            case X_AXES_BOTTOM:
            case X_AXES_CENTRE: {
                ConstantXYDrawable::setSize(1.0f, (float)majorTickLengthPx);
                break;
            }
            case Y_AXES_LEFT:
            case Y_AXES_RIGHT:
            case Y_AXES_CENTRE: {
                ConstantXYDrawable::setSize((float)majorTickLengthPx, 1.0f);
                break;
            }
            default: {
                std::cout << "Invalid Axes Direction!" << std::endl;
            }
        }
    }


    void AxesLineTicks::setMinMax(float newXMin, float newXMax, float newYMin, float newYMax) {
        if ((newXMin != newXMax) && (newYMin != newYMax)) {
            xMin = newXMin;
            xMax = newXMax;
            yMin = newYMin;
            yMax = newYMax;
            // Regenerate axes lines
            AxesLineTicks::generateAllVertices();
        }
    }

    float AxesLineTicks::getFontSize() {
        return currFontSize;
    }

    void AxesLineTicks::createAndSetupAxesLineBuffers() {
        // Axes Line
        // Create Buffers
        glGenVertexArrays(1,&axesLineVAO);
        glGenBuffers(1,&axesLineVBO);

        // Setup Buffers
        glBindVertexArray(axesLineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, axesLineVBO);
        glBufferData(GL_ARRAY_BUFFER, axesLineVerts.size() * sizeof(GLfloat), &axesLineVerts[0], GL_STATIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glBindVertexArray(0);

        // Major Tick Buffers
        // Create Buffers
        glGenVertexArrays(1,&majorTickVAO);
        glGenBuffers(1,&majorTickVBO);

        // Setup Buffers
        glBindVertexArray(majorTickVAO);
        glBindBuffer(GL_ARRAY_BUFFER, majorTickVBO);
        glBufferData(GL_ARRAY_BUFFER, majorTickVerts.size()*sizeof(GLfloat),&majorTickVerts[0],GL_STATIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glBindVertexArray(0);

        // Minor Tick Buffers
        // Create Buffers
        glGenVertexArrays(1,&minorTickVAO);
        glGenBuffers(1,&minorTickVBO);

        // Setup Buffers
        glBindVertexArray(minorTickVAO);
        glBindBuffer(GL_ARRAY_BUFFER, minorTickVBO);
        glBufferData(GL_ARRAY_BUFFER, minorTickVerts.size()*sizeof(GLfloat),&minorTickVerts[0],GL_STATIC_DRAW);

        // Position Attributes
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        glBindVertexArray(0);
    }

    void AxesLineTicks::updateAxesLineBuffers() {
        // Axes Line
        glBindBuffer(GL_ARRAY_BUFFER, axesLineVBO);
        glBufferData(GL_ARRAY_BUFFER, axesLineVerts.size()*sizeof(axesLineVerts[0]), &axesLineVerts[0], GL_DYNAMIC_DRAW);
        // Major Ticks
        glBindBuffer(GL_ARRAY_BUFFER, majorTickVBO);
        glBufferData(GL_ARRAY_BUFFER, majorTickVerts.size()*sizeof(majorTickVerts[0]), &majorTickVerts[0], GL_DYNAMIC_DRAW);
        // Minor Ticks
        glBindBuffer(GL_ARRAY_BUFFER, minorTickVBO);
        glBufferData(GL_ARRAY_BUFFER, minorTickVerts.size()*sizeof(minorTickVerts[0]), &minorTickVerts[0], GL_DYNAMIC_DRAW);
    }

    void AxesLineTicks::drawAxesLine() {
        // Draw Axes Line
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(axesLineColor));
        glBindVertexArray(axesLineVAO);
        glDrawArrays(GL_LINES,0,(int)axesLineVerts.size()/2.0);
        glBindVertexArray(0);
    }

    void AxesLineTicks::drawMajorTicks() {
        // Draw the series of major ticks
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(axesLineColor));
        glBindVertexArray(majorTickVAO);
        glDrawArrays(GL_LINES,0,(int)majorTickVerts.size()/2.0);
        glBindVertexArray(0);
    }

    void AxesLineTicks::drawMinorTicks() {
        // Draw the series of minor ticks
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(axesLineColor));
        glBindVertexArray(minorTickVAO);
        glDrawArrays(GL_LINES,0,(int)minorTickVerts.size()/2.0);
        glBindVertexArray(0);
    }


    void AxesLineTicks::drawMajorTickLabels() {
        for(auto & i : majorTickTextStrings) {
            i->Draw();
        }
    }

    void AxesLineTicks::Draw() {
        // Draw Axes line
        AxesLineTicks::drawAxesLine();
        // Draw Major Ticks
        AxesLineTicks::drawMajorTicks();
        // Draw Minor Ticks
        AxesLineTicks::drawMinorTicks();
        // Draw Major Tick Labels
        AxesLineTicks::drawMajorTickLabels();
    }

    std::string AxesLineTicks::getID() {
        return "AxesLineTicks:" + std::to_string(x) + ":" + std::to_string(y);
    }

    void AxesLineTicks::setParentDimensions(glm::mat4 newParentTransform, int newParentXPx, int newParentYPx,
                                            int newParentWidthPx, int newParentHeightPx) {
        ConstantXYDrawable::setParentDimensions(newParentTransform, newParentXPx, newParentYPx, newParentWidthPx,
                                                newParentHeightPx);
        // Update vertices
        AxesLineTicks::generateAllVertices();
    }

    void AxesLineTicks::setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions) {
        ConstantXYDrawable::setParentDimensions(parentDimensions);
        // Update vertices
        AxesLineTicks::generateAllVertices();
    }

}