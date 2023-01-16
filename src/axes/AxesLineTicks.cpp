//
// Created by bcub3d-desktop on 4/7/20.
//

#include <cmath>
#include <iomanip>
#include "AxesLineTicks.h"


namespace GLPL {

    AxesLineTicks::AxesLineTicks(AxesDirection axesDirection,
                                 std::shared_ptr<ParentDimensions> parentDimensions) :
                                 ConstantXYDrawable(0.0f, 0.0f, 1.0f, 1.0f, CONSTANT_SCALE, CONSTANT_SIZE, std::move(parentDimensions)) {
        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.78f, 0.0f, 1.0f);
        // Set Not Hoverable
        setHoverable(false);

        // Fill Log Width Vector
        AxesLineTicks::fillLogWidths();

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
    AxesLineTicks::generateEqualSpacingBetweenLimitsLinear(float sectionWidthRel,
                                            float sectionWidthAxesUnits,
                                            float midRelPos) {
        // Negative values
        float currRelPos = midRelPos;
        float currAxesPos = 0.0f;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;
        unsigned int count = 0;
        while (currRelPos >= -1.0f) {
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
        while (currRelPos <= 1.0f) {
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

    std::tuple<unsigned int, std::vector<TickSpacingType>, std::vector<float>, std::vector<float>>
    AxesLineTicks::generateEqualSpacingBetweenLimitsLog(float majorSectionWidthRel, float midRelPos) {

        float logXMin = logWithBase(xMin, logBase);
        float logXMax = logWithBase(xMax, logBase);

        // Convert log values to rel values
        float m = 2.0f / (logXMax - logXMin);
        float c = (logXMin + logXMax) / (logXMin - logXMax);

        // Determine the distance between major levels
        // If too large, then skip labelling n major levels
        unsigned int n = 0;
        float relPosDiff = 0;
        while (relPosDiff < majorSectionWidthRel) {
            n += 1;
            float currBase = std::pow(logBase, n);
            float relPos1 = (m * logWithBase(1.0f, logBase) + c);
            float relPos2 = (m * logWithBase(currBase, logBase) + c);
            relPosDiff = relPos2 - relPos1;
        }

        // Values greater than 1
        float currRelPos = midRelPos;
        float currAxesPos = 1.0f;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;
        unsigned int i = -1;

        while (currRelPos <= 1.0f) {
            float currBaseVal = currAxesPos;
            i += 1;

            // Set major line first
            if (i % n == 0) {
                tickSpacingType.push_back(MAJOR_SPACING);
            } else {
                tickSpacingType.push_back(MINOR_SPACING);
            }
            // Store relative position
            relPos.push_back(currRelPos);
            // Store axes position
            axesPos.push_back(currAxesPos);
            // Update position
            currAxesPos = currAxesPos + currBaseVal;
            currRelPos = (m * logWithBase(currAxesPos, logBase)) + c;

            for (unsigned int i=0; i < logBase - 2; i++) {
                // Set major line first
                tickSpacingType.push_back(MINOR_SPACING);
                // Store relative position
                relPos.push_back(currRelPos);
                // Store axes position
                axesPos.push_back(currAxesPos);
                // Update position
                currAxesPos = currAxesPos + currBaseVal;
                currRelPos = (m * logWithBase(currAxesPos, logBase)) + c;
            }

        }

        // Store cross over point
        unsigned int crossIndex = (unsigned int)(relPos.size() - 1);
        // Positive Values
        currRelPos = midRelPos;
        currAxesPos = 1.0f;
        float currBaseVal = currAxesPos;
        i = 0;

        // Values less than 1
        while (currRelPos >= -1.0f) {
            i += 1;

            // Set major line first
            if (i % n == 0) {
                tickSpacingType.push_back(MAJOR_SPACING);
            } else {
                tickSpacingType.push_back(MINOR_SPACING);
            }
            // Update position
            currAxesPos = currBaseVal / (float)logBase;
            currRelPos = (m*logWithBase(currAxesPos, logBase)) + c;
            // Store relative position
            relPos.push_back(currRelPos);
            // Store axes position
            axesPos.push_back(currAxesPos);

            currBaseVal /= logBase;

            for (unsigned int i=0; i < logBase - 2; i++) {
                // Update position
                currAxesPos = currAxesPos + currBaseVal;
                currRelPos = (m * logWithBase(currAxesPos, logBase)) + c;
                // Set major line first
                tickSpacingType.push_back(MINOR_SPACING);
                // Store relative position
                relPos.push_back(currRelPos);
                // Store axes position
                axesPos.push_back(currAxesPos);

            }

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
        // Generate Sequence
        unsigned int crossIndex;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;
        if (!logScale) {
            float pixelsPerUnit = (float)getWidthPx() / 2.0f;
            float minorSectionWidthRel = (float)minorTickSpacingPx / pixelsPerUnit; // -1 to 1
            float minorSectionWidthAxesUnits = minorSectionWidthRel * (xMax - xMin) / 2.0f; // xMin to xMax
            float xMidRelPos = (xMax + xMin) / (xMin - xMax); // -1 to 1
            std::tie(crossIndex, tickSpacingType, relPos, axesPos) = generateEqualSpacingBetweenLimitsLinear(minorSectionWidthRel,
                                                                                                       minorSectionWidthAxesUnits,
                                                                                                       xMidRelPos);
        } else {
            // Adjust max for positive values only
            float logXMin = logWithBase(xMin, logBase);
            float logXMax = logWithBase(xMax, logBase);

            float pixelsPerUnit = (float)getWidthPx() / 2.0f;
            float majorSectionWidthRel = (float)minorTickSpacingPx * (float)minorSpacingsPerMajor / pixelsPerUnit / 3.0f; // -1 to 1
            float xMidRelPos = (logXMax + logXMin) / (logXMin - logXMax); // -1 to 1
            std::tie(crossIndex, tickSpacingType, relPos, axesPos) = generateEqualSpacingBetweenLimitsLog(majorSectionWidthRel,
                                                                                                          xMidRelPos);
        }

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
        // Generate Sequence
        unsigned int crossIndex;
        std::vector<TickSpacingType> tickSpacingType;
        std::vector<float> relPos;
        std::vector<float> axesPos;

        if (!logScale) {
            float pixelsPerUnit = (float)getHeightPx() / 2.0f;
            float minorSectionWidthRel = (float)minorTickSpacingPx / pixelsPerUnit; // -1 to 1

            float minorSectionWidthAxesUnits = minorSectionWidthRel * (yMax - yMin) / 2.0f; // yMin to yMax
            float yMidRelPos = (yMax + yMin) / (yMin - yMax); // -1 to 1
            std::tie(crossIndex, tickSpacingType, relPos, axesPos) = generateEqualSpacingBetweenLimitsLinear(minorSectionWidthRel,
                                                                                                       minorSectionWidthAxesUnits,
                                                                                                       yMidRelPos);
        } else {
            // Adjust max for positive values only
            float logYMin = logWithBase(yMin, logBase);
            float logYMax = logWithBase(yMax, logBase);

            float pixelsPerUnit = (float)getHeightPx() / 2.0f;
            float majorSectionWidthRel = (float)minorTickSpacingPx * (float)minorSpacingsPerMajor / pixelsPerUnit; // -1 to 1
            float yMidRelPos = (logYMax + logYMin) / (logYMin - logYMax); // -1 to 1
            std::tie(crossIndex, tickSpacingType, relPos, axesPos) = generateEqualSpacingBetweenLimitsLog(majorSectionWidthRel,
                                                                                                          yMidRelPos);
        }


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

    void AxesLineTicks::adjustMinMaxLogScale() {
        if (logScale) {
            switch (axesDirection) {
                case X_AXES_CENTRE:
                case X_AXES_BOTTOM:
                case X_AXES_TOP: {
                    if (xMin < 1e-15) {
                        xMin = 1e-15;
                        std::cout << "X Log Scale, but xMin is negative! Setting to 1e-15" << std::endl;
                    }
                    if (xMax < 1e-15) {
                        xMax = 1e-15;
                        std::cout << "X Log Scale, but xMax is negative! Setting to 1e-15" << std::endl;
                    }
                    break;
                }
                case Y_AXES_CENTRE:
                case Y_AXES_LEFT:
                case Y_AXES_RIGHT: {
                    if (yMin < 1e-15) {
                        yMin = 1e-15;
                        std::cout << "Y Log Scale, but yMin is negative! Setting to 1e-15" << std::endl;
                    }
                    if (yMax < 1e-15) {
                        yMax = 1e-15;
                        std::cout << "Y Log Scale, but yMax is negative! Setting to 1e-15" << std::endl;
                    }
                    break;
                }
                default: {
                    std::cout << "Invalid Axes Direction!" << std::endl;
                }
            }
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

    std::string AxesLineTicks::value2NeatStr(double inValue, unsigned int maxCharSuggestion, double expSwapover,
                                             unsigned int minDecimal) {
        // Setup buffer
        unsigned int maxLen = 20;
        char textBuf[maxLen];
        std::string text;

        if ((fabs(inValue) < expSwapover && fabs(inValue) >= 1.0/pow(10,maxCharSuggestion - 2)) || fabs(inValue) < 1e-15) {
            // Calculate the number of digits before the decimal point in standard format
            int mainDigits;
            if (fabs(inValue) > 1e-15 and fabs(inValue) >= 1) {
                mainDigits = std::ceil(fabs(logWithBase(inValue, logBase)) + 1);
            } else {
                mainDigits = 1;
            }

            int remainDigits = std::max((int)maxCharSuggestion - abs(mainDigits), (int)minDecimal);
            if (fabs(inValue) < 1) {
                remainDigits -= 1;
            }

            // Use standard format
            const char *formatStr = ("%." + std::to_string(remainDigits) + "f").c_str();
            sprintf(textBuf, formatStr, inValue);
            text = std::string(textBuf);
        } else {
            // Calculate length with zero decimals but with decimal point
            int sizeExp = ceil(logWithBase(fabs(logWithBase(fabs(inValue), logBase)), logBase));
            int lenNoDecimal = 3 + sizeExp;
            if (inValue < 0) {
                lenNoDecimal += 1;
            }
            if (fabs(inValue) < 1) {
                lenNoDecimal += 1;
            }
            int numDecimals = (int)maxCharSuggestion - lenNoDecimal;
            if (numDecimals < (int)minDecimal) {
                numDecimals = (int)minDecimal;
            }
            const char *formatStr = ("%." + std::to_string(numDecimals) + "e").c_str();
            snprintf(textBuf, maxLen, formatStr, inValue);

            // Find exponent, skip non-digit chars
            char *exponent = strchr(textBuf, 'e') + 2;

            // If we have an exponent starting with 0, drop it
            if(exponent != NULL && exponent[0] == '0')
            {
                exponent[0] = exponent[1];
                exponent[1] = '\0';
            }


            text = std::string(textBuf);
        }

        return text;
    }

    bool AxesLineTicks::isLabelsOverlapping() {
        bool overlapping = false;
        float prevMin = 0;
        float prevMax = 0;
        float currMin = 0;
        float currMax = 0;
        float lowerOverlap = 1 - overlapFactor;
        float upperOverlap = 1 + overlapFactor;

        if (!majorTickTextStrings.empty()) {
            for(unsigned int i=0; i < majorTickTextStrings.size() - 1; i++) {
                if ((strcmp(majorTickTextStrings[i + 1]->getTextString().c_str(), "0.00") != 0) &&
                    (strcmp(majorTickTextStrings[i + 1]->getTextString().c_str(), "") != 0)) {
                    switch (axesDirection) {
                        case X_AXES_TOP:
                        case X_AXES_BOTTOM:
                        case X_AXES_CENTRE: {
                            prevMin = lowerOverlap * majorTickTextStrings[i]->getLeft();
                            prevMax = upperOverlap * majorTickTextStrings[i]->getRight();
                            currMin = lowerOverlap * majorTickTextStrings[i + 1]->getLeft();
                            currMax = upperOverlap * majorTickTextStrings[i + 1]->getRight();
                            break;
                        }
                        case Y_AXES_LEFT:
                        case Y_AXES_RIGHT:
                        case Y_AXES_CENTRE: {
                            prevMin = lowerOverlap * majorTickTextStrings[i]->getBottom();
                            prevMax = upperOverlap * majorTickTextStrings[i]->getTop();
                            currMin = lowerOverlap * majorTickTextStrings[i + 1]->getBottom();
                            currMax = upperOverlap * majorTickTextStrings[i + 1]->getTop();
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
        if (!logScale || !extraLogLines) {
            return majorTickAxesPos;
        } else {
            std::vector<float> allTickAxesPos(majorTickAxesPos);
            allTickAxesPos.insert(allTickAxesPos.end(), minorTickAxesPos.begin(), minorTickAxesPos.end());

            return allTickAxesPos;
        }
    }

    void AxesLineTicks::setLogScale(bool logOn, unsigned int newLogBase) {
        logScale = logOn;
        logBase = newLogBase;

        // Set minor spacing amount to the log base
        if (logScale) {
            minorSpacingsPerMajor = std::max(logBase,(unsigned int)4);
        } else {
            minorSpacingsPerMajor = 3;
        }

        // Regenerate axes lines
        AxesLineTicks::generateAllVertices();
    }

    bool AxesLineTicks::getLogState() {
        return logScale;
    }

    unsigned int AxesLineTicks::getLogBase() {
        return logBase;
    };

    void AxesLineTicks::generateMajorTickLabels() {
        // Generate labels
        unsigned int usedCount = 0;
        for(unsigned int i=0; i < majorTickAxesPos.size(); i++) {
            // Create Parent Dimensions
            std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
            // Create Text String
            std::string text = value2NeatStr(majorTickAxesPos[i], 4, 1000, 1);
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

    void AxesLineTicks::setMinMaxXAxes(float newXMin, float newXMax) {
        if (newXMin != newXMax) {
            xMin = newXMin;
            xMax = newXMax;
            // Adjust if we have a log scale
            if (logScale) {
                AxesLineTicks::adjustMinMaxLogScale();
            }
            // Regenerate axes lines
            AxesLineTicks::generateAllVertices();
        }
    }

    void AxesLineTicks::setMinMaxYAxes(float newYMin, float newYMax) {
        if (newYMin != newYMax) {
            yMin = newYMin;
            yMax = newYMax;
            // Adjust if we have a log scale
            if (logScale) {
                AxesLineTicks::adjustMinMaxLogScale();
            }
            // Regenerate axes lines
            AxesLineTicks::generateAllVertices();
        }
    }

    void AxesLineTicks::setMinMax(float newXMin, float newXMax, float newYMin, float newYMax) {
        if ((newXMin != newXMax) && (newYMin != newYMax)) {
            xMin = newXMin;
            xMax = newXMax;
            yMin = newYMin;
            yMax = newYMax;
            // Adjust if we have a log scale
            if (logScale) {
                AxesLineTicks::adjustMinMaxLogScale();
            }
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

    void AxesLineTicks::fillLogWidths() {
        // Calculate proportional log widths
        // Sum of widths = 1, no need to divide by the total width
        std::vector<float> logVals;
        // Calculate log values
        for(unsigned int i=1; i < 11; i++) {
            float logVal = logWithBase(i, logBase);
            logVals.push_back(logVal);
        }
        // Calculate log widths
        for(unsigned int i=1; i < 11; i++) {
            float logWidth = logVals[i] - logVals[i-1];
            unsigned int xVal = i + 1;
            logWidths[xVal] = logWidth;
        }

    }

}