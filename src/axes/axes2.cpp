//
// Created by tbatt on 19/04/2020.
//

#include "axes2.h"



namespace GLPL {

    Axes2::Axes2(float x, float y, float width, float height,
                 ParentPointers parentPointers) :
            IDrawable(x, y, width, height, parentPointers) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

        // Setup Buffers

        // TODO - Remove text string creation
        //Axes2::addText("Test String", 0.1, 0.8, 50);
        Axes2::addText("Test String", 0.0, 0.0, 50);


    }

    void Axes2::addText(const char* textString, float x, float y, float fontSize) {
        // Create Text String
        ParentPointers newParentPointers = IDrawable::createParentPointers();
        TextString newTextString = TextString(textString, x, y, fontSize, newParentPointers);
        // Register Child
        std::shared_ptr<IDrawable> textStringPt = std::make_shared<TextString>(newTextString);
        Axes2::registerChild(textStringPt);
        // Store Text String
        textStringList.push_back(newTextString);
    }

    void Axes2::Draw() {

        for(unsigned int i=0; i < children.size(); i++) {
            children[i]->Draw();
        }

    }
}