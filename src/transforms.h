/*
 * tranforms.h
 *
 *  Created on: 25Feb.,2017
 *      Author: bcub3d-desktop
 */

#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_


// Transform to custom viewports
glm::mat4 viewportTransform(float xc, float yc, float width, float height, float winWidth, float winHeight) {
	// Creates transform matrix for a custom sized viewport
	// xc:			The center x coordinate (in -1 to 1)
	// yc:			The center y coordinate (in -1 to 1)
	// width:		The width of the new viewport
	// height:		The height of the new viewport
	// winWidth: 	The width of the original window
	// winHeight:	The height of the original window

	// Translate by offset
	glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(xc, yc, 0));

	// Scale new viewport
	float scaleX = width/winWidth;
	float scaleY = height/winHeight;
	glm::mat4 scale = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));

	return scale;
}

class WindowProperties {
public:
		// Window size
		float winHeight;
		float winWidth;

	WindowProperties(float winHeight, float winWidth) {
		// Store Window Properties
		this->winHeight = winHeight;
		this->winWidth = winWidth;
	}

	void updateWindowSize(float winHeight, float winWidth) {
		// Update Window Properties
		this->winHeight = winHeight;
		this->winWidth = winWidth;
	}
};

#endif /* TRANSFORMS_H_ */
