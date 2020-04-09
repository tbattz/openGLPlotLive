/*
 * tranforms.h
 *
 *  Created on: 25Feb.,2017
 *      Author: bcub3d-desktop
 */

#ifndef P_TRANSFORMS_H_
#define P_TRANSFORMS_H_

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace GLPL {

    class Transforms {
    public:
        static glm::mat4 viewportTransform(float x, float y, float width, float height);
    };

}



#endif /* P_TRANSFORMS_H_ */
