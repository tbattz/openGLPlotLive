//
// Created by tbatt on 19/04/2020.
//

#include "axes.h"



namespace GLPL {

    Axes::Axes(float x, float y, float width, float height,
               const ParentDimensions& parentDimensions) :
            ConstantScaleDrawable(x, y, width, height, parentDimensions) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        // TODO - Remove text string creation
        //Axes2::addText("Test String", 0.1, 0.8, 50);
        Axes::addText("Test String", 0.0, 0.0, 50);

    }

    void Axes::addText(const char* textString, float x, float y, float fontSize) {
        // Create Text String
        ParentDimensions newParentPointers = IDrawable::createParentDimensions();
        std::shared_ptr<IDrawable> textStringPt = std::make_shared<TextString>(textString, x, y, fontSize, newParentPointers);
        // Register Child
        Axes::registerChild(textStringPt);
        // Store Text String
        std::shared_ptr<TextString> textStringPt2 = std::dynamic_pointer_cast<TextString>(textStringPt);
        textStringList.push_back(textStringPt2);
    }

    void Axes::Draw() {
        for(auto & i : children) {
            i->Draw();
        }

    }
}