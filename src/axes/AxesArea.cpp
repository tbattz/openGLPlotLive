//
// Created by tbatt on 19/06/2020.
//

// Standard Includes
#include <utility>

// Project Includes
#include "AxesArea.h"
#include "../shadedLines/ShadedLine2D2CircularVecs.h"
#include "../lines/Line2D2Vecs.h"
#include "../interaction/PressButtonWithImage.h"
#include "../scatterPlot/IScatterPlot.h"
#include "../scatterPlot/Scatter2D2Vecs.h"


namespace GLPL {

    AxesArea::AxesArea(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions) :
        IDrawable() {
        // Set bounding box color
        boundingBoxColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        // Set Not Hoverable
        setHoverable(false);

        AxesArea::setParentDimensions(std::move(parentDimensions));

        // Initialise data
        AxesArea::setPosition(x, y);
        AxesArea::setSize(width, height);
        IDrawable::createAndSetupBuffers();

        // Calculate transform
        AxesArea::updateAxesViewportTransform();

        // Add Title
        AxesArea::addText("Axes Title", "axes-title", 0.5, 1.11, 14, CENTRE_BOTTOM);

        // Add Axes Label
        AxesArea::addText("x label", "x-label", 0.5, -0.05, 12, CENTRE_TOP);
        AxesArea::addText("y label", "y-label", -0.11, 0.5, 12, CENTRE_RIGHT);

        // Add Buttons
        AxesArea::addButtonWithTexture("Interactor", "interactor-white", 1.0, 1.01, 0.08, 0.08, BOTTOM_RIGHT, true, "Toggle the interactor");
        AxesArea::addButtonWithTexture("Y Axes Limits Scaling", "axes-y-limits-white", 0.91, 1.01, 0.08, 0.08, BOTTOM_RIGHT, true, "Enable/disable y-axes auto scaling");
        AxesArea::addButtonWithTexture("X Axes Limits Scaling", "axes-x-limits-white", 0.82, 1.01, 0.08, 0.08, BOTTOM_RIGHT, true, "Enable/disable x-axes auto scaling");
        AxesArea::addButtonWithTexture("Grid", "grid-white", 0.73, 1.01, 0.08, 0.08, BOTTOM_RIGHT, true, "Toggle grid");
        AxesArea::addButtonWithTexture("Axes", "axes-white", 0.64, 1.01, 0.08, 0.08, BOTTOM_RIGHT, true, "Toggle axes");
        AxesArea::addButtonWithTexture("AxesBox", "axes-box-white", 0.55, 1.01, 0.08, 0.08, BOTTOM_RIGHT, true, "Toggle axes box");

        // Create Interactor
        AxesArea::createInteractor();

        // Create Grid
        AxesArea::createGrid();

        // Create Zoom Box
        AxesArea::createZoomBox();

        // Create legend
        AxesArea::createLegend();

    }

    std::string AxesArea::getID() {
        return "AxesArea:" + std::to_string(x) + ":" + std::to_string(y);
    }

    void AxesArea::Draw() {
        // Update Axes Limits
        AxesArea::updateAxesLimits();

        // Update mouse over line
        AxesArea::updateInteractor();

        // Update zoom drag box
        AxesArea::updateZoomDragBox();

        // Draw Axes
        if (buttonMap["Axes"]->isActive()) {
            for (auto &i : axesLines) {
                i.second->Draw();
            }
        }

        // Draw Grid
        AxesArea::drawGrid();

        // Scissor Test
        glEnable(GL_SCISSOR_TEST);
        glScissor(xPx, yPx, widthPx, heightPx);

        // Draw lines, scatter plots
        for(auto & i : plotableMap) {
            i.second->Draw();
        }

        // Disable Scissor Testing
        glDisable(GL_SCISSOR_TEST);

        // Draw attachments
        for(auto & i : axesItems) {
            i->Draw();
        }

        // Draw Text
        for(auto & i : textStringMap) {
            i.second->Draw();
        }

        // Draw Buttons
        for(auto & i : buttonMap) {
            i.second->Draw();
        }

        // Draw Axes box
        if (buttonMap["AxesBox"]->isActive()) {
            AxesArea::drawAxesBox();
        }

        // Draw legend
        if (legendVisible) {
            legend->Draw();
        }

    }

    void AxesArea::onScroll(double xoffset, double yoffset) {
        AxesArea::zoomAxes(yoffset);
    }

    void AxesArea::onRightDrag(bool dragging, double origXPos, double origYPos) {
        // Reset other held buttons
        rightShiftMouseHeld = false;
        // Set new held button
        rightMouseHeld = dragging;
        mouseHeldX = origXPos;
        mouseHeldY = origYPos;

        xMinDrag = xmin;
        xMaxDrag = xmax;
        yMinDrag = ymin;
        yMaxDrag = ymax;
    }

    void AxesArea::onMiddleDrag(bool dragging, double origXPos, double origYPos) {
        // Set new held button
        middleMouseHeld = dragging;
        mouseHeldX = origXPos;
        mouseHeldY = origYPos;

        if (isHovered() && isMouseOver(mouseX, mouseY, false)) {
            xMinDrag = xmin;
            xMaxDrag = xmax;
            yMinDrag = ymin;
            yMaxDrag = ymax;
        }

        // On Release, set the current axes limits to that of the zoom box line
        if (!dragging) {
            float newXMin = std::min(zoomBoxX[0], zoomBoxX[1]);
            float newXMax = std::max(zoomBoxX[0], zoomBoxX[1]);
            float newYMin = std::min(zoomBoxY[0], zoomBoxY[2]);
            float newYMax = std::max(zoomBoxY[0], zoomBoxY[2]);
            setAxesLimits(newXMin, newXMax, newYMin, newYMax);
        }

    }

    void AxesArea::onLeftDrag(bool dragging, double origXPos, double origYPos) {
        // Reset other held buttons
        leftShiftMouseHeld = false;
        // Set new held button
        leftMouseHeld = dragging;
        mouseHeldX = origXPos;
        mouseHeldY = origYPos;

        if (isHovered() && isMouseOver(mouseX, mouseY, false)) {
            xMinDrag = xmin;
            xMaxDrag = xmax;
            yMinDrag = ymin;
            yMaxDrag = ymax;
        }

    }

    void AxesArea::onLeftShiftDrag(bool dragging, double origXPos, double origYPos) {
        // Reset other held buttons
        leftMouseHeld = false;
        // Set new held button
        leftShiftMouseHeld = dragging;
        mouseHeldX = origXPos;
        mouseHeldY = origYPos;

        if (isHovered() && isMouseOver(mouseX, mouseY, false)) {
            xMinDrag = xmin;
            xMaxDrag = xmax;
            yMinDrag = ymin;
            yMaxDrag = ymax;
        }

        // On Release, set the current axes limits to that of the zoom box line
        if (!dragging) {
            float newXMin = std::min(zoomBoxX[0], zoomBoxX[1]);
            float newXMax = std::max(zoomBoxX[0], zoomBoxX[1]);
            float newYMin = std::min(zoomBoxY[0], zoomBoxY[2]);
            float newYMax = std::max(zoomBoxY[0], zoomBoxY[2]);
            setAxesLimits(newXMin, newXMax, newYMin, newYMax);
        }

    }

    void AxesArea::setLastMousePos(double lastMouseX, double lastMouseY) {
        IDrawable::setLastMousePos(lastMouseX, lastMouseY);

        // Change zoom if right mouse is being dragged
        if (rightMouseHeld) {
            zoomAxesByDragging();
        }

        // Drag the axes if shift held with the left mouse being dragged
        if (leftMouseHeld) {
            moveAxesByDragging();
        }
    }

    void AxesArea::setAxesBoxOn(bool axesBoxOnBool) {
        (buttonMap["AxesBox"]->setActive(axesBoxOnBool));
    }

    void AxesArea::setAxesBoxColor(glm::vec4 newAxesBoxColour) {
        axesBoxColor = newAxesBoxColour;
    }

    void AxesArea::addAxesLine(const std::string& axesName, AxesDirection axesDirection) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create Axes
        std::shared_ptr<AxesLineTicks> newAxes = std::make_shared<AxesLineTicks>(axesDirection, newParentPointers);
        // Store Axes
        axesLines.insert(std::pair<std::string, std::shared_ptr<AxesLineTicks>>(axesName, newAxes));
        // Register as a child
        AxesArea::registerChild(newAxes);
    }

    void AxesArea::setXAxesLimits(float newXMin, float newXMax) {
        // Check max limits
        if (std::isinf(newXMin)) {
            newXMin = -1e15;
        }
        if (std::isinf(newXMax)) {
            newXMax = 1e15;
        }

        // Set values
        xmin = newXMin;
        xmax = newXMax;
        // Update Axes Lines
        for(const auto& axesLine : axesLines) {
            axesLine.second->setMinMaxXAxes(newXMin, newXMax);
        }
        // Update Grid
        std::vector<float> xAxesPos = axesLines.at("x")->getAxesTickPos();
        grid->setMinMaxXAxes(newXMin, newXMax);
        grid->setXLines(xAxesPos);
        // Update Axes Area
        AxesArea::updateAxesViewportTransform();
    }

    void AxesArea::setYAxesLimits(float newYMin, float newYMax) {
        // Check max limits
        if (std::isinf(newYMin)) {
            newYMin = -1e15;
        }
        if (std::isinf(newYMax)) {
            newYMax = 1e15;
        }

        // Set values
        ymin = newYMin;
        ymax = newYMax;
        // Update Axes Lines
        for(const auto& axesLine : axesLines) {
            axesLine.second->setMinMaxYAxes(newYMin, newYMax);
        }
        // Update Grid
        std::vector<float> yAxesPos = axesLines.at("y")->getAxesTickPos();
        grid->setMinMaxYAxes(newYMin, newYMax);
        grid->setYLines(yAxesPos);
        // Update Axes Area
        AxesArea::updateAxesViewportTransform();
    }

    void AxesArea::setAxesLimits(float newXMin, float newXMax, float newYMin, float newYMax) {
        // Check max limits
        if (std::isinf(newXMin)) {
            newXMin = -1e15;
        }
        if (std::isinf(newXMax)) {
            newXMax = 1e15;
        }
        if (std::isinf(newYMin)) {
            newYMin = -1e15;
        }
        if (std::isinf(newYMax)) {
            newYMax = 1e15;
        }

        if ((fabs(newXMin - newXMax) > 0) && (fabs(newYMin - newYMax) > 0)) {
            // Set values
            xmin = newXMin;
            xmax = newXMax;
            ymin = newYMin;
            ymax = newYMax;
            // Update Axes Lines
            for (const auto &axesLine : axesLines) {
                axesLine.second->setMinMax(newXMin, newXMax, newYMin, newYMax);
            }
            // Update Grid
            grid->setMinMax(newXMin, newXMax, newYMin, newYMax);
            std::vector<float> xAxesPos = axesLines.at("x")->getAxesTickPos();
            std::vector<float> yAxesPos = axesLines.at("y")->getAxesTickPos();
            grid->setXLines(xAxesPos);
            grid->setYLines(yAxesPos);
            // Update Axes Area
            AxesArea::updateAxesViewportTransform();
        }
    }

    void AxesArea::setLogScale(bool logOn, unsigned int newLogBase, LogAxes logAxes) {
        switch(logAxes) {
            case X_AXES: {
                axesLines.at("x")->setLogScale(logOn, newLogBase);
                break;
            }
            case Y_AXES: {
                axesLines.at("y")->setLogScale(logOn, newLogBase);
                break;
            }
            case BOTH: {
                axesLines.at("x")->setLogScale(logOn, newLogBase);
                axesLines.at("y")->setLogScale(logOn, newLogBase);
                break;
            }
            default: {
                std::cout << "Unknown LogAxes Enum!" << std::endl;
            }
        }

        setPlotableLogModes();
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
            LineType lineType, glm::vec3 colour, float opacityRatio, std::string label) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create Line
        std::shared_ptr<Plotable> lineObj;
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
        linePt->setLabel(std::move(label));

        // Register Children
        AxesArea::registerChild(lineObj);
        // Set axes area transform
        linePt->setAxesViewportTransform(axesViewportTransformation);
        // Store line
        linePt->setPlotableId(nextPlotableId);
        plotableMap.insert(std::pair<unsigned int, std::shared_ptr<ILine2D>>(nextPlotableId, linePt));
        nextPlotableId += 1;

        // Add to legend
        if (!linePt->getLabel().empty()) {
            legend->addPlotableToLegend(linePt->getLabel(), linePt);
        }

        // Update log bools
        setPlotableLogModes();

        // Update limits for axes
        AxesArea::updateAxesLimits();

        return linePt;
    }

    std::shared_ptr<Plotable> AxesArea::getPlotable(int plotableId) {
        if (plotableMap.count(plotableId) > 0) {
            return plotableMap.at(plotableId);
        } else {
            std::cout << "Plotable " << plotableId << " does not exist!" << std::endl;
            return nullptr;
        }
    }

    void AxesArea::removePlotable(int plotableId) {
        if (plotableMap.count(plotableId) > 0) {
            std::shared_ptr<Plotable> line2Remove = plotableMap.at(plotableId);
            std::shared_ptr<IDrawable> drawable2Remove = std::dynamic_pointer_cast<GLPL::IDrawable>(line2Remove);
            // Remove child
            IDrawable::removeChild(drawable2Remove);
            // Remove axes
            plotableMap.erase(plotableId);
        } else {
            std::cout << "Cannot remove Plotable " << plotableId << ", Plotable does not exist!" << std::endl;
        }
    }

    std::shared_ptr<IScatterPlot> AxesArea::addScatterPlot(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                                                           glm::vec3 colour, float opacityRatio, MarkerType markerType, std::string label) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create Scatter Plot
        std::shared_ptr<Plotable> scatterObj;
        std::shared_ptr<IScatterPlot> scatterPt;

        scatterObj = std::make_shared<Scatter2D2Vecs>(dataPtX, dataPtY, newParentPointers);
        scatterPt = std::dynamic_pointer_cast<Scatter2D2Vecs>(scatterObj);

        // Set Attributes
        scatterPt->setMarkerColour(colour);
        scatterPt->setMarkerOutlineColour(colour);
        scatterPt->setOpacityRatio(opacityRatio);
        scatterPt->setOutlineOpacityRatio(1.25f*opacityRatio);
        scatterPt->setMarkerType(markerType);
        scatterPt->setLabel(std::move(label));

        // Register Children
        AxesArea::registerChild(scatterObj);
        // Set axes area transform
        scatterPt->setAxesViewportTransform(axesViewportTransformation);
        // Store line
        scatterPt->setPlotableId(nextPlotableId);
        plotableMap.insert(std::pair<unsigned int, std::shared_ptr<IScatterPlot>>(nextPlotableId, scatterPt));
        nextPlotableId += 1;

        // Add to legend
        if (!scatterPt->getLabel().empty()) {
            legend->addPlotableToLegend(scatterPt->getLabel(), scatterPt);
        }

        // Update log bools
        setPlotableLogModes();

        // Update limits for axes
        AxesArea::updateAxesLimits();

        return scatterPt;
    }

    void AxesArea::addText(std::string textString, std::string stringId, float x, float y, float fontSize, AttachLocation attachLocation) {
        if (textStringMap.count(stringId) == 0) {
            // Create Parent Dimensions
            std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
            // Register Child
            std::shared_ptr<IDrawable> textStringObj = std::make_shared<TextString>(textString, x, y, fontSize,
                                                                                    newParentPointers);
            std::shared_ptr<TextString> textStringPt = std::dynamic_pointer_cast<TextString>(textStringObj);
            // Set pin position
            textStringPt->setAttachLocation(attachLocation);
            // Register Child
            AxesArea::registerChild(textStringObj);
            // Store Text String
            textStringMap.insert(std::pair<std::string, std::shared_ptr<TextString>>(stringId, textStringPt));
        } else {
            std::cout << "String " << stringId << "already exists!" << std::endl;
        }
    }

    std::shared_ptr<TextString> AxesArea::getText(std::string textStringId) {
        if (textStringMap.count(textStringId) > 0) {
            return textStringMap.at(textStringId);
        } else {
            std::cout << "TextString " << textStringId << " does not exist!" << std::endl;
            return nullptr;
        }
    }

    void AxesArea::setText(std::string stringId, std::string newTextString) {
        if (textStringMap.count(stringId) > 0) {
            textStringMap.at(stringId)->setTextString(newTextString);
        } else {
            std::cout << "TextString " << stringId << " does not exist!" << std::endl;
        }
    }

    void AxesArea::setTextRotation(std::string stringId, TextRotation newTextRotation) {
        if (textStringMap.count(stringId) > 0) {
            textStringMap.at(stringId)->setTextRotation(newTextRotation);
        } else {
            std::cout << "TextString " << stringId << " does not exist!" << std::endl;
        }
    }

    float AxesArea::convertMouseX2AxesX(float mouseXVal) {
        // Calculate mouse position in x axes
        // x interpolation
        float mouseXAx = 0;
        float xminVal = 2*getLeft() - 1;
        float xmaxVal = 2*getRight() - 1;
        float sx1 = (parentTransform * glm::vec4(xminVal, 0.0f, 0.5f, 1.0f))[0];;
        float sx2 = (parentTransform * glm::vec4(xmaxVal, 0.0f, 0.5f, 1.0f))[0];
        if (!axesLines.at("x")->getLogState()) {
            float ax1 = xmin;
            float ax2 = xmax;
            float mx = (ax2 - ax1) / (sx2 - sx1);
            float cx = ax2 - (mx * sx2);
            mouseXAx = (mx * (float) mouseXVal) + cx;
        } else {
            // Log Scale
            unsigned int logBase = axesLines.at("x")->getLogBase();
            float ax1 = logWithBase(xmin, logBase);
            float ax2 = logWithBase(xmax, logBase);
            float mx = (ax2 - ax1) / (sx2 - sx1);
            float cx = ax2 - (mx * sx2);

            mouseXAx = std::pow(logBase, (mx * (float) mouseXVal) + cx);
        }
        return mouseXAx;
    }

    float AxesArea::convertMouseY2AxesY(float mouseYVal) {
        // Calculate mouse position in y axes
        // y interpolation
        float mouseYAx = 0;
        float yminVal = 2*getBottom() - 1;
        float ymaxVal = 2*getTop() - 1;
        float sx1 = (parentTransform * glm::vec4(0.0f, yminVal, 0.5f, 1.0f))[1];
        float sx2 = (parentTransform * glm::vec4(0.0f, ymaxVal, 0.5f, 1.0f))[1];
        if (!axesLines.at("y")->getLogState()) {
            float ax1 = ymin;
            float ax2 = ymax;
            float mx = (ax2 - ax1) / (sx2 - sx1);
            float cx = ax2 - (mx * sx2);
            mouseYAx = (mx * (float) mouseYVal) + cx;
        } else {
            // Log Scale
            unsigned int logBase = axesLines.at("y")->getLogBase();
            float ax1 = logWithBase(ymin, logBase);
            float ax2 = logWithBase(ymax, logBase);
            float mx = (ax2 - ax1) / (sx2 - sx1);
            float cx = ax2 - (mx * sx2);
            mouseYAx = std::pow(logBase, (mx * (float) mouseYVal) + cx);
        }

        return mouseYAx;
    }

    float AxesArea::convertMouseX2RelativeX() {
        // Calculate mouse position in relative x
        // x interpolation
        float rx1 = getLeft();
        float rx2 = getRight();
        float ax1 = (overallTransform * glm::vec4(getLeft(), 0.0f, 0.5f, 1.0f))[0];
        float ax2 = (overallTransform * glm::vec4(getRight(), 0.0f, 0.5f, 1.0f))[0];
        float mx = (rx2 - rx1) / (ax2 - ax1);
        float cx = rx2 - (mx*ax2);
        float mouseRelX = (mx*(float)mouseX) + cx;
        mouseRelX = (mouseRelX + 1) / 2.0f;

        return mouseRelX;
    }

    float AxesArea::convertMouseY2RelativeY() {
        // Calculate mouse position in relative y
        // y interpolation
        float ry1 = getLeft();
        float ry2 = getRight();
        float ay1 = (overallTransform * glm::vec4(0.0f,getBottom(), 0.5f, 1.0f))[1];
        float ay2 = (overallTransform * glm::vec4(0.0f,getTop(), 0.5f, 1.0f))[1];
        float mx = (ry2 - ry1) / (ay2 - ay1);
        float cx = ry2 - (mx*ay2);
        float mouseRelY = (mx*(float)mouseY) + cx;
        mouseRelY = (mouseRelY + 1) / 2.0f;

        return mouseRelY;
    }

    int AxesArea::getHoverCursor() {
        if (buttonMap["Interactor"]->isActive()) {
            return GLFW_CROSSHAIR_CURSOR;
        } else {
            return 0;
        }
    }

    void AxesArea::showLegend(bool legendVisibility = true) {
        legendVisible = legendVisibility;
    }

    void AxesArea::setLegendAttachLocation(AttachLocation newAttachLocation) {
        legend->setAttachLocation(newAttachLocation);
    }

    std::shared_ptr<std::unordered_map<int, std::shared_ptr<Plotable>>> AxesArea::getPlotableMap() {
        return std::make_shared<std::unordered_map<int, std::shared_ptr<Plotable>>>(plotableMap);
    }

    void AxesArea::removeTextString(std::string textStringId) {
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

    void AxesArea::addButton(const std::string& buttonName, float x, float y, float width, float height,
                             AttachLocation attachLocation, bool activeState) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register Child
        std::shared_ptr<IDrawable> buttonObj = std::make_shared<PressButton>(buttonName, x, y, width, height, newParentPointers);
        std::shared_ptr<PressButton> buttonObjPt = std::dynamic_pointer_cast<PressButton>(buttonObj);
        // Set pin position
        buttonObjPt->setAttachLocation(attachLocation);
        // Set state
        buttonObjPt->setActive(activeState);
        // Register Child
        AxesArea::registerChild(buttonObj);
        // Store button
        buttonMap.insert(std::pair<std::string, std::shared_ptr<PressButton>>(buttonName, buttonObjPt));
    }

    void AxesArea::addButtonWithTexture(const std::string& buttonName, const std::string& textureName, float x, float y, float width, float height,
                             AttachLocation attachLocation, bool activeState, const std::string& tooltipText) {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register Child
        std::shared_ptr<IDrawable> buttonObj = std::make_shared<PressButtonWithImage>(buttonName, x, y, width, height, textureName, newParentPointers, tooltipText);
        std::shared_ptr<PressButton> buttonObjPt = std::dynamic_pointer_cast<PressButton>(buttonObj);
        // Set pin position
        buttonObjPt->setAttachLocation(attachLocation);
        // Set state
        buttonObjPt->setActive(activeState);
        // Register Child
        AxesArea::registerChild(buttonObj);
        // Store button
        buttonMap.insert(std::pair<std::string, std::shared_ptr<PressButton>>(buttonName, buttonObjPt));
    }

    void AxesArea::setButtonState(const std::string& buttonName, bool activeState) {
        if (buttonMap.count(buttonName) > 0) {
            buttonMap.at(buttonName).get()->setActive(activeState);
        } else {
            std::cout << "PressButton " << buttonName << " does not exist!" << std::endl;
        }
    }

    void AxesArea::updateAxesViewportTransform() {
        // Update Transform
        glm::mat4 viewportTransform = GLPL::Transforms::viewportTransform(x, y, width, height);
        glm::mat4 axesLimitsTransform = AxesArea::scale2AxesLimits();
        axesViewportTransformation = std::make_shared<glm::mat4>(parentTransform * viewportTransform * axesLimitsTransform);
        // Update Children Lines, Scatter Plots
        for(auto & i : plotableMap) {
            i.second->setAxesViewportTransform(axesViewportTransformation);
        }

        // Update Grid
        if (grid != nullptr) {
            grid->setAxesViewportTransform(axesViewportTransformation);
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
        // Check log states
        bool logX = (axesLines.find("x") == axesLines.end()) ? false : axesLines.at("x")->getLogState();
        bool logY = (axesLines.find("y") == axesLines.end()) ? false : axesLines.at("y")->getLogState();

        // Calculate x transform
        float xShift, scaleX;
        if (logX) {
            // Get log base
            unsigned int logBase = axesLines.at("x")->getLogBase();
            // Calculate center of current limits
            xShift = ((logWithBase(xmin, logBase) + logWithBase(xmax, logBase)) / 2.0f) / (logWithBase(xmax, logBase) - logWithBase(xmin, logBase)) * 2.0f; // xavg/width * 2.0, *2 to take it to -1 to 1
            // Scale to limits
            scaleX = 2.0f/(logWithBase(xmax, logBase) - logWithBase(xmin, logBase)); // Inverted due to -1 to 1 mapping (less than abs(1) region)
        } else {
            // Calculate center of current limits
            xShift = (float)((xmin + xmax) / 2.0) / (xmax - xmin) * 2.0f; // xavg/width * 2.0, *2 to take it to -1 to 1
            // Scale to limits
            scaleX = 2.0f/(xmax-xmin); // Inverted due to -1 to 1 mapping (less than abs(1) region)
        }

        // Calculate y transform
        float yShift, scaleY;
        if (logY) {
            // Get log base
            unsigned int logBase = axesLines.at("y")->getLogBase();
            // Calculate center of current limits
            yShift = ((logWithBase(ymin, logBase) + logWithBase(ymax, logBase))/2.0f)/(logWithBase(ymax, logBase) - logWithBase(ymin, logBase)) * 2.0f; // yavg/height * 2.0, *2 to take it to -1 to 1
            // Scale to limits
            scaleY = 2.0f/(logWithBase(ymax, logBase) - logWithBase(ymin, logBase)); // Inverted due to -1 to 1 mapping (less than abs(1) region)
        } else {
            // Calculate center of current limits
            yShift = ((ymin+ymax)/2.0f)/(ymax-ymin) * 2.0f; // yavg/height * 2.0, *2 to take it to -1 to 1
            // Scale to limits
            scaleY = 2.0f/(ymax-ymin); // Inverted due to -1 to 1 mapping (less than abs(1) region)
        }

        // Translate by offset and scale
        glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(-xShift, -yShift,0));
        glm::mat4 scale = glm::scale(trans, glm::vec3(scaleX,scaleY,1));

        return scale;
    }

    void GLPL::AxesArea::drawAxesBox() {
        // Draw bounding box
        std::shared_ptr<Shader> shader = shaderSetPt->getPlot2dShader();
        shader->Use();
        glUniformMatrix4fv(glGetUniformLocation(shader->Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(overallTransform));
        glUniform4fv(glGetUniformLocation(shader->Program,"inColor"),1,glm::value_ptr(axesBoxColor));
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP,0,4);
        glBindVertexArray(0);
    }

    void GLPL::AxesArea::drawGrid() {
        if (buttonMap["Grid"]->isActive()) {
            grid->Draw();
        }
    }

    void GLPL::AxesArea::updateInteractor() {
        if (buttonMap["Interactor"]->isActive()) {
            if (isHovered() && isMouseOver(mouseX, mouseY, false)) {
                // Calculate mouse position in x
                float mouseXAx = convertMouseX2AxesX((float)mouseX);
                float mouseYAx = convertMouseY2AxesY((float)mouseY);
                float mouseRelX = convertMouseX2RelativeX();
                float mouseRelY = convertMouseY2RelativeY();

                for (auto &i : plotableMap) {
                    if (i.second->isSelected()) {
                        std::tuple<float, float> pt = i.second->getClosestPoint(mouseXAx, xmin, xmax, ymin, ymax);
                        if (pt != std::make_tuple(0.0, 0.0)) {
                            float x1 = std::get<0>(pt);
                            float y1 = std::get<1>(pt);
                            float x2 = mouseXAx;
                            float y2 = mouseYAx;
                            // Update line
                            interactorLine->clearData();
                            interactorLine->dataPtX->push_back(x1);
                            interactorLine->dataPtX->push_back(x2);
                            interactorLine->dataPtY->push_back(y1);
                            interactorLine->dataPtY->push_back(y2);
                            interactorLine->updateInternalData();
                            // Update Text String
                            char textBuf[50];
                            const char *formatStr;
                            if (abs(x) > 1000 || abs(y) > 1000) {
                                formatStr = "(%.2e, %.2e)";
                            } else {
                                formatStr = "(%.2f, %.2f)";
                            }
                            sprintf(textBuf, formatStr, x, y1);
                            interactorText->setTextString(textBuf);
                            // Check if the text string should be above or below
                            if (y1 > y2) {
                                // Require offset to account for the height of the cursor
                                float mouseRelYOffset = convertVerticalPx2ObjRel(mouseHeightPx);
                                interactorText->setPosition((float) mouseRelX, (float) mouseRelY - mouseRelYOffset);
                                interactorText->setAttachLocation(CENTRE_TOP);
                            } else {
                                interactorText->setPosition((float) mouseRelX, (float) mouseRelY);
                                interactorText->setAttachLocation(CENTRE_BOTTOM);
                            }
                        }
                    }
                }
            } else {
                interactorLine->clearData();
                interactorText->setTextString("");
            }
        } else {
            interactorLine->clearData();
            interactorText->setTextString("");
        }
    }

    void GLPL::AxesArea::updateZoomDragBox() {
        if (leftShiftMouseHeld || middleMouseHeld) {
            // Calculate mouse position in x
            float dragXAx = convertMouseX2AxesX((float)mouseHeldX);
            float dragYAx = convertMouseY2AxesY((float)mouseHeldY);
            float mouseXAx = convertMouseX2AxesX((float)mouseX);
            float mouseYAx = convertMouseY2AxesY((float)mouseY);
            // Clip mouse to axes limits
            mouseXAx = clip(mouseXAx, xmin, xmax);
            mouseYAx = clip(mouseYAx, ymin, ymax);


            // Update line
            zoomBoxLine->clearData();
            // Pt 1 - (dragX, dragY)
            zoomBoxLine->dataPtX->push_back(dragXAx);
            zoomBoxLine->dataPtY->push_back(dragYAx);
            // Pt 2 - (mouseX, dragY)
            zoomBoxLine->dataPtX->push_back(mouseXAx);
            zoomBoxLine->dataPtY->push_back(dragYAx);
            // Pt 3 - (mouseX, mouseY)
            zoomBoxLine->dataPtX->push_back(mouseXAx);
            zoomBoxLine->dataPtY->push_back(mouseYAx);
            // Pt 4 - (dragX, mouseY)
            zoomBoxLine->dataPtX->push_back(dragXAx);
            zoomBoxLine->dataPtY->push_back(mouseYAx);
            // Pt 1 - (dragX, dragY)
            zoomBoxLine->dataPtX->push_back(dragXAx);
            zoomBoxLine->dataPtY->push_back(dragYAx);

            zoomBoxLine->updateInternalData();
        } else {
            zoomBoxLine->clearData();
            zoomBoxLine->dataPtX->push_back(0.0f);
            zoomBoxLine->dataPtY->push_back(0.0f);
        }
    }

    void GLPL::AxesArea::updateXAxesLimits() {
        if (buttonMap["X Axes Limits Scaling"]->isActive()) {
            // Get the overall maximum and minimum from all lines
            float newXmin = -0.0;
            float newXmax = 0.0;
            if (axesLines.at("x")->getLogState()) {
                newXmin = 1.0;
                newXmax = 1.0;
            }
            for (std::pair<unsigned int, std::shared_ptr<Plotable>> lineInfo : plotableMap) {
                std::vector<float> minMax = lineInfo.second->getMinMax(true);
                if (minMax.size() == 4) {
                    if (minMax[0] < newXmin) { newXmin = minMax[0]; };
                    if (minMax[1] > newXmax) { newXmax = minMax[1]; };
                }
            }

            // Set axes limits
            float absLim = 1e-10;
            if ((abs(xmin - newXmin) > absLim) ||
                (abs(xmax - newXmax) > absLim)) {
                AxesArea::setXAxesLimits(newXmin, newXmax);
            }

        }

        // Match axes lines sizing
        float xFontSize = axesLines.at("x")->getFontSize();
        float yFontSize = axesLines.at("y")->getFontSize();
        float minFontSize = std::min(xFontSize, yFontSize);
        if (xFontSize < yFontSize) {
            axesLines.at("y")->setMajorTickFontSize(minFontSize);
        } else if (yFontSize < xFontSize) {
            axesLines.at("x")->setMajorTickFontSize(minFontSize);
        }
    }

    void GLPL::AxesArea::updateYAxesLimits() {
        if (buttonMap["Y Axes Limits Scaling"]->isActive()) {
            // Get the overall maximum and minimum from all lines
            float newYmin = -0.0;
            float newYmax = 0.0;
            if (axesLines.at("y")->getLogState()) {
                newYmin = 1.0;
                newYmax = 1.0;
            }
            for (std::pair<unsigned int, std::shared_ptr<Plotable>> lineInfo : plotableMap) {
                std::vector<float> minMax = lineInfo.second->getMinMax(true);
                if (minMax.size() == 4) {
                    if (minMax[2] < newYmin) { newYmin = minMax[2]; };
                    if (minMax[3] > newYmax) { newYmax = minMax[3]; };
                }
            }

            // Set axes limits
            float absLim = 1e-10;
            if ((abs(ymin - newYmin) > absLim) ||
                (abs(ymax - newYmax) > absLim)) {
                AxesArea::setYAxesLimits(newYmin, newYmax);
            }

        }

        // Match axes lines sizing
        float xFontSize = axesLines.at("x")->getFontSize();
        float yFontSize = axesLines.at("y")->getFontSize();
        float minFontSize = std::min(xFontSize, yFontSize);
        if (xFontSize < yFontSize) {
            axesLines.at("y")->setMajorTickFontSize(minFontSize);
        } else if (yFontSize < xFontSize) {
            axesLines.at("x")->setMajorTickFontSize(minFontSize);
        }
    }

    void GLPL::AxesArea::updateAxesLimits() {
        AxesArea::updateXAxesLimits();
        AxesArea::updateYAxesLimits();
    }

    std::pair<float, float>
    GLPL::AxesArea::calcScrolledVals(float minVal, float maxVal, float currVal, float zoomFrac, bool dir) {
        // a, b are before scroll
        // c, d are after scroll
        // a + b = Ro
        // c + d = Rn
        // Ro = xmax - xmin
        // Rn = (1+r)*Ro
        // a/(a+b) = c/(c+d) => c = a*Rn/(a+b)
        // d = Rn - c
        float oldRange = maxVal - minVal;
        float a = currVal - minVal;
        float b = maxVal - currVal;

        float newRange;
        if (dir) {
            newRange = (1 + zoomFrac) * oldRange;
        } else {
            newRange = (1 - zoomFrac) * oldRange;
        }
        // Cap max range
        float newMin, newMax;
        if (abs(a) < 1e15) {
            // Calculate new limits
            float c = a * newRange / (a + b);
            float d = newRange - c;
            newMin = currVal - c;
            newMax = currVal + d;
        } else {
            newMin = minVal;
            newMax = maxVal;
        }

        return std::pair<float, float>(newMin, newMax);
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomLinearX(float zoomDir) {
        // Zoom centered around the current mouse position
        float mouseXAx = convertMouseX2AxesX(mouseX);
        std::pair<float, float> newX = calcScrolledVals(xmin, xmax, mouseXAx, zoomRatio, zoomDir > 0);

        return newX;
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomLinearY(float zoomDir) {
        // Zoom centered around the current mouse position
        float mouseYAx = convertMouseY2AxesY(mouseY);
        std::pair<float, float> newY = calcScrolledVals(ymin, ymax, mouseYAx, zoomRatio, zoomDir > 0);

        return newY;
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomLogX(float zoomDir) {
        // Zoom centered around the current mouse position
        float mouseXAx = convertMouseX2AxesX(mouseX);

        // Account for log values
        unsigned int logBase = axesLines.at("x")->getLogBase();
        float inXmin = logWithBase(xmin, logBase);
        float inXmax = logWithBase(xmax, logBase);
        mouseXAx = logWithBase(mouseXAx, logBase);

        std::pair<float, float> newX = calcScrolledVals(inXmin, inXmax, mouseXAx, zoomRatio, zoomDir > 0);

        // Undo log
        newX.first = std::pow(logBase, newX.first);
        newX.second = std::pow(logBase, newX.second);

        return newX;
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomLogY(float zoomDir) {
        // Zoom centered around the current mouse position
        float mouseYAx = convertMouseY2AxesY(mouseY);

        // Account for log values
        unsigned int logBase = axesLines.at("y")->getLogBase();
        float inYmin = logWithBase(ymin, logBase);
        float inYmax = logWithBase(ymax, logBase);
        mouseYAx = logWithBase(mouseYAx, logBase);

        std::pair<float, float> newY = calcScrolledVals(inYmin, inYmax, mouseYAx, zoomRatio, zoomDir > 0);

        // Undo log
        newY.first = std::pow(logBase, newY.first);
        newY.second = std::pow(logBase, newY.second);

        return newY;
    }

    void GLPL::AxesArea::zoomAxes(float zoomDir) {
        // Zoom centered around the current mouse position
        std::pair<float, float> newX, newY;
        newX = axesLines.at("x")->getLogState() ? calcAxesZoomLogX(zoomDir) : calcAxesZoomLinearX(zoomDir);
        newY = axesLines.at("y")->getLogState() ? calcAxesZoomLogY(zoomDir) : calcAxesZoomLinearY(zoomDir);

        AxesArea::setAxesLimits(newX.first, newX.second, newY.first, newY.second);
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomDragLinearX() {
        // Zoom centered around the current mouse position by dragging the mouse
        // The change in zoom is proportional to the drag distance in x and y
        float xDiff = mouseX - mouseHeldX;
        float xFrac = abs(dragZoomFactor * xDiff / 2.0f);
        float mouseXAx = convertMouseX2AxesX(mouseX);

        std::pair<float, float> newX = calcScrolledVals(xMinDrag, xMaxDrag, mouseXAx, xFrac, xDiff < 0);

        return newX;
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomDragLinearY() {
        // Zoom centered around the current mouse position by dragging the mouse
        // The change in zoom is proportional to the drag distance in x and y
        float yDiff = mouseY - mouseHeldY;
        float yFrac = abs(dragZoomFactor * yDiff / 2.0f);
        float mouseYAx = convertMouseY2AxesY(mouseY);

        std::pair<float, float> newY = calcScrolledVals(yMinDrag, yMaxDrag, mouseYAx, yFrac, yDiff < 0);

        return newY;
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomDragLogX() {
        // Zoom centered around the current mouse position by dragging the mouse
        // The change in zoom is proportional to the drag distance in x and y
        float xDiff = mouseX - mouseHeldX;
        float xFrac = abs(dragZoomFactor * xDiff / 2.0f);

        // Account for log values
        unsigned int logBase = axesLines.at("x")->getLogBase();
        float inXmin = logWithBase(xMinDrag, logBase);
        float inXmax = logWithBase(xMaxDrag, logBase);
        float mouseXAx = logWithBase(convertMouseX2AxesX(mouseX), logBase);

        std::pair<float, float> newX = calcScrolledVals(inXmin, inXmax, mouseXAx, xFrac, xDiff < 0);

        // Undo log
        newX.first = std::pow(logBase, newX.first);
        newX.second = std::pow(logBase, newX.second);

        return newX;
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesZoomDragLogY() {
        // Zoom centered around the current mouse position by dragging the mouse
        // The change in zoom is proportional to the drag distance in x and y
        float yDiff = mouseY - mouseHeldY;
        float yFrac = abs(dragZoomFactor * yDiff / 2.0f);

        // Account for log values
        unsigned int logBase = axesLines.at("y")->getLogBase();
        float inYmin = logWithBase(yMinDrag, logBase);
        float inYmax = logWithBase(yMaxDrag, logBase);
        float mouseYAx = logWithBase(convertMouseY2AxesY(mouseY), logBase);

        std::pair<float, float> newY = calcScrolledVals(inYmin, inYmax, mouseYAx, yFrac, yDiff < 0);

        // Undo log
        newY.first = std::pow(logBase, newY.first);
        newY.second = std::pow(logBase, newY.second);

        return newY;
    }

    void GLPL::AxesArea::zoomAxesByDragging() {
        // Zoom centered around the current mouse position by dragging the mouse
        // The change in zoom is proportional to the drag distance in x and y
        std::pair<float, float> newX = axesLines.at("x")->getLogState() ? calcAxesZoomDragLogX() : calcAxesZoomDragLinearX();
        std::pair<float, float> newY = axesLines.at("y")->getLogState() ? calcAxesZoomDragLogY() : calcAxesZoomDragLinearY();

        AxesArea::setAxesLimits(newX.first, newX.second, newY.first, newY.second);
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesDragLinearX() {
        // Move the axes proportional to the distance the mouse is dragged
        float newXMin, newXMax;
        float mouseXAx = convertMouseX2AxesX(mouseX);
        float mouseHeldXAx = convertMouseX2AxesX(mouseHeldX);
        float xDiff = mouseXAx - mouseHeldXAx;
        float xRange = xMaxDrag - xMinDrag;
        if (abs(xRange) > 0) {
            float xRatio = xDiff / xRange;
            newXMin = xMinDrag + (xRatio * xRange);
            newXMax = xMaxDrag + (xRatio * xRange);
        } else {
            newXMin = xMinDrag;
            newXMax = xMaxDrag;
        }

        return {newXMin, newXMax};
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesDragLinearY() {
        // Move the axes proportional to the distance the mouse is dragged
        float newYMin, newYMax;
        float mouseYAx = convertMouseY2AxesY(mouseY);
        float mouseHeldYAx = convertMouseY2AxesY(mouseHeldY);
        float yDiff = mouseYAx - mouseHeldYAx;
        float yRange = yMaxDrag - yMinDrag;
        if (abs(yRange) > 0) {
            float yRatio = yDiff / yRange;
            newYMin = yMinDrag + (yRatio * yRange);
            newYMax = yMaxDrag + (yRatio * yRange);
        } else {
            newYMin = yMinDrag;
            newYMax = yMaxDrag;
        }


        return {newYMin, newYMax};
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesDragLogX() {
        // Move the axes proportional to the distance the mouse is dragged
        float mouseXAx = convertMouseX2AxesX(mouseX);
        float mouseHeldXAx = convertMouseX2AxesX(mouseHeldX);

        // Account for log values
        float newXMin, newXMax;
        unsigned int logBase = axesLines.at("x")->getLogBase();
        if (abs(xMinDrag) > 0 && abs(xMaxDrag) > 0) {
            float inXmin = logWithBase(xMinDrag, logBase);
            float inXmax = logWithBase(xMaxDrag, logBase);
            float xDiff = logWithBase(mouseXAx, logBase) - logWithBase(mouseHeldXAx, logBase);

            float xRange = inXmax - inXmin;
            float xRatio = xDiff / xRange;
            newXMin = inXmin + (xRatio * xRange);
            newXMax = inXmax + (xRatio * xRange);

            // Undo log
            newXMin = std::pow(logBase, newXMin);
            newXMax = std::pow(logBase, newXMax);

        } else {
            newXMin = xMinDrag;
            newXMax = xMaxDrag;
        }

        return {newXMin, newXMax};
    }

    std::pair<float, float> GLPL::AxesArea::calcAxesDragLogY() {
        // Move the axes proportional to the distance the mouse is dragged
        float mouseYAx = convertMouseY2AxesY(mouseY);
        float mouseHeldYAx = convertMouseY2AxesY(mouseHeldY);

        // Account for log values
        float newYMin, newYMax;
        unsigned int logBase = axesLines.at("y")->getLogBase();
        if (abs(yMinDrag) > 0 && abs(yMaxDrag) > 0) {
            float inYmin = logWithBase(yMinDrag, logBase);
            float inYmax = logWithBase(yMaxDrag, logBase);
            float yDiff = logWithBase(mouseYAx, logBase) - logWithBase(mouseHeldYAx, logBase);

            float yRange = inYmax - inYmin;
            float yRatio = yDiff / yRange;
            newYMin = inYmin + (yRatio * yRange);
            newYMax = inYmax + (yRatio * yRange);

            newYMin = std::pow(logBase, newYMin);
            newYMax = std::pow(logBase, newYMax);
        } else {
            newYMin = yMinDrag;
            newYMax = yMaxDrag;
        }


        return {newYMin, newYMax};
    }

    void GLPL::AxesArea::moveAxesByDragging() {
        // Move the axes proportional to the distance the mouse is dragged
        std::pair<float, float> newX = axesLines.at("x")->getLogState() ? calcAxesDragLogX() : calcAxesDragLinearX();
        std::pair<float, float> newY = axesLines.at("y")->getLogState() ? calcAxesDragLogY() : calcAxesDragLinearY();

        AxesArea::setAxesLimits(newX.first, newX.second, newY.first, newY.second);
    }

    void GLPL::AxesArea::createInteractor() {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();

        // Create Line
        interactorLine = std::make_shared<Line2D2Vecs>(&interactorDataX, &interactorDataY, newParentPointers);
        // Register Children
        AxesArea::registerChild(interactorLine);
        // Set axes area transform
        interactorLine->setAxesViewportTransform(axesViewportTransformation);
        // Set not hoverable
        interactorLine->setHoverable(false);
        // Store line
        interactorLine->setPlotableId(nextPlotableId);
        plotableMap.insert(std::pair<unsigned int, std::shared_ptr<ILine2D>>(nextPlotableId, interactorLine));
        nextPlotableId += 1;

        // Create text label
        // Create label
        interactorText = std::make_shared<TextString>("", x, y, 8, newParentPointers);
        interactorText->setAttachLocation(CENTRE_BOTTOM);
        interactorText->setHoverable(false);
        // Register Child
        AxesArea::registerChild(interactorText);
        // Store Text String
        textStringMap.insert(std::pair<std::string, std::shared_ptr<TextString>>("interactor-text", interactorText));

    }

    void GLPL::AxesArea::createGrid() {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create grid
        grid = std::make_shared<Grid>(newParentPointers);
        // Register Children
        AxesArea::registerChild(grid);
        // Set axes area transform
        grid->setAxesViewportTransform(axesViewportTransformation);
        // Set not hoverable
        grid->setHoverable(false);
    }

    void GLPL::AxesArea::createZoomBox() {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create Line
        zoomBoxLine = std::make_shared<Line2D2Vecs>(&zoomBoxX, &zoomBoxY, newParentPointers);
        // Set Properties
        zoomBoxLine->setLineColour(glm::vec3(0.0f, 1.0f, 0.0f));
        zoomBoxLine->setLineWidth(2);
        // Register Children
        AxesArea::registerChild(zoomBoxLine);
        // Set axes area transform
        zoomBoxLine->setAxesViewportTransform(axesViewportTransformation);
        // Set not hoverable
        zoomBoxLine->setHoverable(false);
        // Store line
        zoomBoxLine->setPlotableId(nextPlotableId);
        plotableMap.insert(std::pair<unsigned int, std::shared_ptr<ILine2D>>(nextPlotableId, zoomBoxLine));
        nextPlotableId += 1;

    }

    void AxesArea::createLegend() {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create legend
        legend = std::make_shared<Legend>(0.98, 0.05, newParentPointers);
        legend->setAttachLocation(BOTTOM_RIGHT);
        legend->setHoverable(false);
        // Register Children
        AxesArea::registerChild(legend);

    }

    void AxesArea::setPlotableLogModes() {
        // Set the current log modes for all the lines
        bool logX = axesLines.at("x")->getLogState();
        bool logY = axesLines.at("y")->getLogState();
        unsigned int logXBase = axesLines.at("x")->getLogBase();
        unsigned int logYBase = axesLines.at("y")->getLogBase();
        for(auto & i : plotableMap) {
            i.second->setLogModes(logX, logY);
            i.second->setLogXBase(logXBase);
            i.second->setLogYBase(logYBase);
        }
        grid->setLogModes(logX, logY);
    }
}