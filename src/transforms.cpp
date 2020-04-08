//
// Created by bcub3d-desktop on 8/4/20.
//

#include "transforms.h"

namespace GLPL {
    // Transform to custom viewports
    glm::mat4 Transforms::viewportTransform(float x, float y, float width, float height) {
        // Creates transform matrix for a custom sized viewport
        // x:			The bottom left corner x coordinate (in 0 to 1)
        // y:			The bottom left corner y coordinate (in 0 to 1)
        // width:		The width of the new viewport (in 0 to 1)
        // height:		The height of the new viewport (in 0 to 1)
        // Output is in [-1 to 1]


        // Calculate Center
        float xc = x + width/2.0; // 0 to 1
        float yc = y + height/2.0; // 0 to 1

        // Convert from [0,1] to [-1,1]
        float xn = 2*xc - 1;
        float yn = 2*yc - 1;

        // Translate by offset
        glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(xn, yn, 0));

        // Scale new viewport
        float scaleX = width;
        float scaleY = height;
        glm::mat4 scale = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));

        return scale;
    };
}