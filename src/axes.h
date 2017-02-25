/*
 * axes.h
 *
 *  Created on: 12Feb.,2017
 *      Author: bcub3d-laptop
 */

#ifndef AXES_H_
#define AXES_H_

#include "../src/transforms.h"
#include "../src/plot.h"

class Axes {
	// Axes contains the axes draw space, axes and tick markings
public:
	// Position
	float x; // Location of bottom left corner x position of axes in 0 to 1 (in plot area)
	float y; // Location of bottom left corner y position of axes in 0 to 1 (in plot area)
	float width; // Width of axes as a proportion of the current plot width, including tick marks
	float height; // Height of axes as a proportion of the current plot height, including tick marks
	// Axes Ticks
	float majorTickSizeW = 0.02; // Size of major axes ticks (proportional to plot area width)
	float minorTickSizeW = 0.01; // Size of minor axes ticks (proportional to plot area width)
	float majorTickSizeH = 0.02; // Size of major axes ticks (proportional to plot area height)
	float minorTickSizeH = 0.01; // Size of minor axes ticks (proportional to plot area height)
	// Buffers
	GLuint VAO, VBO;
	// Axes Box
	vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
	// Plot Properties
	float plotWidth;	// In pixels
	float plotHeight;	// In pixels


	Axes(float x,float y, float width, float height, float plotWidth, float plotHeight) {
		// Setup Position
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

		// Store Plot Properties
		this->plotWidth = plotWidth;
		this->plotHeight = plotHeight;

		// Setup Buffers
		createAndSetupBuffers();
	}

	void createAndSetupBuffers() {
		vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
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

	void Draw(Shader shader,glm::mat4 plotViewportTrans) {
		// Calculate Viewport Transformation
		glm::mat4 axesAreaViewportTrans = plotViewportTrans * viewportTransform(x, y, width, height);
		glm::mat4 axesViewportTrans = plotViewportTrans * viewportTransform(x+majorTickSizeW, y+majorTickSizeH, width - majorTickSizeW, height - majorTickSizeH);

		// Draw Axes Box Outline
		drawAxesAreaOutline(shader,axesAreaViewportTrans);

		// Draw Axes Box
		drawAxesBox(shader,axesViewportTrans);
	}

	void drawAxesAreaOutline(Shader shader, glm::mat4 axesAreaViewportTrans) {
		// Draw outline of plot area
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesAreaViewportTrans));
		glm::vec4 inColor = glm::vec4(0.0,0.0,1.0,1.0);
		glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP,0,4);
		glBindVertexArray(0);
	}

	void drawAxesBox(Shader shader, glm::mat4 axesViewportTrans) {
		// Draw Axes box
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesViewportTrans));
		glm::vec4 inColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
		glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP,0,4);
		glBindVertexArray(0);
	}

};



#endif /* AXES_H_ */
