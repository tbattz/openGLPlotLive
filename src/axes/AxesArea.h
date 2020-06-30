//
// Created by tbatt on 19/06/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXESAREA_H
#define OPENGLPLOTLIVE_PROJ_AXESAREA_H

// Project Includes
#include "../rendering/ConstantScaleDrawable.h"
#include "../lines/ILine2D_OLD.h"
#include "IPlotable.h"
#include "../shadedLines/IShadedLine2D.h"
#include "../lines/LineType.h"


namespace GLPL {

    class AxesArea : public IDrawable {
    public:
        // Constructor
        AxesArea(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();
        void addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                LineType lineType=SINGLE_LINE, glm::vec3 colour=LC_WHITE, float opacityRatio=1.0);
        std::shared_ptr<IPlotable> getLine(unsigned int lineId);
        void removeLine(unsigned int lineId);
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
        // Axes
        float xmin = -1.0;
        float xmax = 1.0;
        float ymin = -1.0;
        float ymax = 1.0;

        // Functions
        std::vector<float> calculateScissor(glm::mat4 axesLimitsViewportTrans);
        glm::mat4 scale2AxesLimits();

    };


}


#endif //OPENGLPLOTLIVE_PROJ_AXESAREA_H
