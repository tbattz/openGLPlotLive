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
	// Transform to custom viewports
	static glm::mat4 viewportTransform(float x, float y, float width, float height) {
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



#endif /* P_TRANSFORMS_H_ */
