//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXES2_H
#define OPENGLPLOTLIVE_PROJ_AXES2_H

// Project Includes
#include "../rendering/IDrawable.h"
#include "../texts/textString.h"


namespace GLPL {
    class Axes2 : public IDrawable {
    public:
        // Constructor
        Axes2(float x, float y, float width, float height, ParentPointers parentPointers);

        // Functions
        void addText(const char* textString, float x, float y, float fontSize);
        void Draw();

    private:
        // Data
        std::vector<TextString> textStringList;
    };
}


#endif //OPENGLPLOTLIVE_PROJ_AXES2_H
