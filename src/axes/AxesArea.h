//
// Created by tbatt on 19/06/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXESAREA_H
#define OPENGLPLOTLIVE_PROJ_AXESAREA_H

// Project Includes
#include "../lines/ILine2D.h"
#include "AxesLineTicks.h"
#include "../lines/LineType.h"
#include "../interaction/IButton.h"
#include "../lines/Line2D2Vecs.h"
#include "Grid.h"


namespace GLPL {

    class AxesArea : public IDrawable {
    public:
        // Constructor
        AxesArea(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        std::string getID();
        void Draw();
        // Axes
        void setAxesBoxOn(bool axesBoxOnBool);
        void setAxesBoxColor(glm::vec4 newAxesBoxColour);
        void addAxesLine(const std::string& axesName, AxesDirection axesDirection);
        void setAxesLimits(float newXMin, float newXMax, float newYMin, float newYMax);
        // Lines
        std::shared_ptr<ILine2D> addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                LineType lineType=SINGLE_LINE, glm::vec3 colour=LC_WHITE, float opacityRatio=1.0);
        std::shared_ptr<IPlotable> getLine(unsigned int lineId);
        void removeLine(unsigned int lineId);
        // Text
        void addText(const char* textString, float x, float y, float fontSize, AttachLocation attachLocation=BOTTOM_LEFT);
        std::shared_ptr<TextString> getText(unsigned int textStringId);
        void removeTextString(unsigned int textStringId);
        // Button
        void addButton(const std::string& buttonName, float x, float y, float width, float height, AttachLocation attachLocation, bool activeState = true);
        void setButtonState(const std::string& buttonName, bool activeState);
        // Point Interactor
        float convertMouseX2AxesX();
        float convertMouseY2AxesY();
        float convertMouseX2RelativeX();
        float convertMouseY2RelativeY();
        int getHoverCursor();
        // Other
        void updateAxesViewportTransform();
        void setPosition(float newX, float newY);
        void setSize(float newWidth, float newHeight);
        void updateSizePx();
        void setParentDimensions(glm::mat4 newParentTransform,
                                 int newParentXPx,
                                 int newParentYPx,
                                 int newParentWidthPx,
                                 int newParentHeightPx);
        void setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions);


    private:
        // Data
        std::shared_ptr<glm::mat4> axesViewportTransformation = std::make_shared<glm::mat4>(1.0f);
        unsigned int lineCount = 0;
        std::unordered_map<unsigned int, std::shared_ptr<ILine2D>> lineMap;
        std::vector<std::shared_ptr<IDrawable>> axesItems;
        // Axes Limits
        float xmin = -1.0;
        float xmax = 1.0;
        float ymin = -1.0;
        float ymax = 1.0;
        // Axes Settings
        bool axesBoxOn = true;
        glm::vec4 axesBoxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        // Text String
        unsigned int textStringCount = 0;
        std::unordered_map<unsigned int, std::shared_ptr<TextString>> textStringMap;
        // Axes Lines
        std::unordered_map<std::string, std::shared_ptr<AxesLineTicks>> axesLines;
        // Buttons
        std::unordered_map<std::string, std::shared_ptr<IButton>> buttonMap;
        // Interactor
        std::vector<float> interactorDataX = {};
        std::vector<float> interactorDataY = {};
        std::shared_ptr<Line2D2Vecs> interactorLine;
        std::shared_ptr<TextString> interactorText;
        // Grid
        std::shared_ptr<Grid> grid;


        // Functions
        std::vector<float> calculateScissor(glm::mat4 axesLimitsViewportTrans);
        glm::mat4 scale2AxesLimits();
        void drawAxesBox();
        void drawGrid();
        void drawInteractor();
        void updateAxesLimits();
        void createInteractor();
        void createGrid();

    };


}


#endif //OPENGLPLOTLIVE_PROJ_AXESAREA_H
