//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXES_H
#define OPENGLPLOTLIVE_PROJ_AXES_H

// Project Includes
#include "../texts/textString.h"
#include "../rendering/ConstantScaleDrawable.h"


namespace GLPL {
    class Axes : public ConstantScaleDrawable {
    public:
        // Constructor
        Axes(float x, float y, float width, float height, const ParentDimensions& parentDimensions);

        // Functions
        void addText(const char* textString, float x, float y, float fontSize);
        void Draw();

    private:
        // Data
        std::vector<std::shared_ptr<TextString>> textStringList;
    };
}


#endif //OPENGLPLOTLIVE_PROJ_AXES_H
