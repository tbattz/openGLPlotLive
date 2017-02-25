/*
 * axes.h
 *
 *  Created on: 12Feb.,2017
 *      Author: bcub3d-laptop
 */

#ifndef AXES_H_
#define AXES_H_

class Axes {
public:
	// Position
	float x; // Location of x position of axes in 0 to 1 (in plot area)
	float y; // Location of y position of axes in 0 to 1 (in plot area)
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


	Axes(float x,float y, float width, float height) {
		// Setup Position
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;

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

	void Draw(Shader shader,glm::mat4 viewportTrans) {
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(viewportTrans));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_LOOP,0,4);
		glBindVertexArray(0);
	}

};



#endif /* AXES_H_ */
