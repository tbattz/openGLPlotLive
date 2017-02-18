/*
 * plot.h
 *
 *  Created on: 12Feb.,2017
 *      Author: bcub3d-laptop
 */

#ifndef PLOT_H_
#define PLOT_H_

#include "../src/axes.h"

class Plot {
public:
	float x;		// Location of x position of plot in current window in 0 to 1
	float y;		// Location of y position of plot in current window in 0 to 1
	float width;  // Width of plot as a proportion of the current window width
	float height;	// Height of plot as a proportion of the current window height
	vector<Axes> axesVector;	// Vector containing all axes

	Plot(float x, float y, float width, float height) {
		// Set Size and Position
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		// Create Axes
		axesVector.push_back(Axes(0.1,0.1,0.8,0.8));
	}

	void Draw(Shader shader, glm::mat4 viewportTrans) {
		for(unsigned int i=0; i<axesVector.size(); i++) {
			axesVector[i].Draw(shader, viewportTrans);
		}
	}
};



#endif /* PLOT_H_ */
