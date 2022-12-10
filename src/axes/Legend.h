//
// Created by bcub3d-desktop on 20/11/22.
//

#ifndef OPENGLPLOTLIVE_PROJ_LEGEND_H
#define OPENGLPLOTLIVE_PROJ_LEGEND_H


// Project Includes
#include "../texts/TextString.h"
#include "../rendering/ConstantXYDrawable.h"
#include "../shadedLines/ShadedLine2D2CircularVecs.h"
#include "../shapes/Rectangle.h"


namespace GLPL {

    class Legend : public ConstantXYDrawable {
    public:
        // Constructor
        Legend(float x, float y, std::shared_ptr<ParentDimensions> parentDimensions, AttachLocation attachLocation = BOTTOM_LEFT);

        // Functions
        std::string getID();
        void Draw();

        void addPlotableToLegend(std::string label, std::shared_ptr<Plotable> plotable);

    protected:
        // Functions
        void createRow();

        // Data
        glm::vec4 legendBackgroundColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        glm::vec4 legendOutlineColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        std::shared_ptr<Rectangle> rect;

        bool textChanged = false;
        unsigned legendItemCount = 0;
         std::unordered_map<unsigned int, std::tuple<std::shared_ptr<Plotable>, std::shared_ptr<TextString>, std::shared_ptr<Rectangle>>> legendItemMap;

        // Can't do it this way because it shades to the axes, which isn't what we want
        std::vector<float> vertsX = {0.0f, 1.0f};
        std::vector<float> vertsY = {-1.0f, 1.0f};

    };


}

#endif //OPENGLPLOTLIVE_PROJ_LEGEND_H
