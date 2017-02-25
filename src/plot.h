/*
 * plot.h
 *
 *  Created on: 12Feb.,2017
 *      Author: bcub3d-laptop
 */

#ifndef PLOT_H_
#define PLOT_H_

#include "../src/axes.h"
#include "../src/transforms.h"

class Plot {
public:
	float x;			// Location of x position of plot in current window in 0 to 1
	float y;			// Location of y position of plot in current window in 0 to 1
	float width;  		// Width of plot as a proportion of the current window width
	float height;		// Height of plot as a proportion of the current window height
	vector<Axes> axesVector;	// Vector containing all axes

	// Window Properties
	WindowProperties* winProp;

	// Buffers
	GLuint VAO, VBO;
	// Area
	vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};

	Plot(float x, float y, float width, float height, WindowProperties* winProp) {
		// Set Size and Position
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		// Store Window Dimensions
		this->winProp = winProp;

		// Setup Buffers
		createAndSetupBuffers();

		// Create Axes
		//axesVector.push_back(Axes(0.1,0.1,0.8,0.8));
	}

	void createAndSetupBuffers() {
			/* Create Buffers */
			glGenVertexArrays(1,&VAO);
			glGenBuffers(1,&VBO);

			/* Setup Buffers */
			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferData(GL_ARRAY_BUFFER, boxVerts.size()*sizeof(GLfloat),&boxVerts[0],GL_STATIC_DRAW);

			/* Position Attributes */
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

			glBindVertexArray(0);
		}

	void Draw(Shader shader, glm::mat4 viewportTrans) {
		// Draw temp box for plot limits
		shader.Use();
		viewportTrans = viewportTransform(2*x-1, 2*y-1, width*(winProp->winWidth), height*(winProp->winHeight), winProp->winWidth, winProp->winHeight);
		glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(viewportTrans));
		glm::vec4 inColor = glm::vec4(1.0,0.0,0.0,1.0);
		glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP,0,4);
		glBindVertexArray(0);

		// Draw Axes
		for(unsigned int i=0; i<axesVector.size(); i++) {
			axesVector[i].Draw(shader, viewportTrans);
		}


	}
};



#endif /* PLOT_H_ */
