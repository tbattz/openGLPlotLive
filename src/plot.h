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
	// Plots contain sets of axes, labels and titles
public:
	float x;			// Location of bottom left corner x position of plot in current window in 0 to 1
	float y;			// Location of bottom left corner y position of plot in current window in 0 to 1
	float width;  		// Width of plot as a proportion of the current window width
	float height;		// Height of plot as a proportion of the current window height
	//Axes* axesPt;
	Axes axes;
	// Buffers
	GLuint VAO, VBO;
	// Area
	vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
	// Window Dimensions
	WinDimensions* winDimPt;

	Plot(float x, float y, float width, float height, WinDimensions* winDimPt) : axes(0.15,0.15,0.80,0.75,winDimPt) {
		// Set Size and Position
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->winDimPt = winDimPt;

		// Setup Buffers
		createAndSetupBuffers();

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

	void Draw(Shader shader) {
		// Calculate Viewport Transformation
		glm::mat4 plotViewportTrans = viewportTransform(x, y, width, height);

		// Draw Plot Box Obutline
		drawPlotOutline(shader,plotViewportTrans);

		// Draw Axes
		axes.Draw(shader, plotViewportTrans);

	}

	void drawPlotOutline(Shader shader, glm::mat4 plotViewportTrans) {
		// Draw temp box for plot limits
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(plotViewportTrans));
		glm::vec4 inColor = glm::vec4(1.0,0.0,0.0,1.0);
		glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP,0,4);
		glBindVertexArray(0);

	}
};



#endif /* PLOT_H_ */
