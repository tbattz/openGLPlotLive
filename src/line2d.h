/*
 * line2d.h
 *
 *  Created on: 3Feb.,2017
 *      Author: bcub3d-desktop
 */

#ifndef LINE2D_H_
#define LINE2D_H_

// GLEW (OpenGL Extension Wrangler Library)
#define GLEW_STATIC
#include <GL/glew.h>

// Standard Includes
#include <vector>
using std::vector;

struct pt2D {
	GLfloat x;
	GLfloat y;
};


class Line2D {
public:
	/* Buffers */
	GLuint VAO, VBO;
	int nPts = 0;

	/* Data */
	vector<pt2D> dataPt;

	Line2D(vector<pt2D> dataPt) {
		this->dataPt = dataPt;

		/* Setup Buffers */
		createAndSetupBuffers();

		/* Set Number of Points */
		nPts = (dataPt).size();

	}

	void createAndSetupBuffers() {
		/* Create Buffers */
		glGenVertexArrays(1,&VAO);
		glGenBuffers(1,&VBO);

		/* Setup Buffers */
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER, dataPt.size()*2*sizeof(GLfloat),&dataPt[0],GL_DYNAMIC_DRAW);

		/* Position Attributes */
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,(GLvoid*)0);

		glBindVertexArray(0); // Unbind VAO

	}

	void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
		// Check if number of points changed
		int newPts = (dataPt).size();
		if (newPts != nPts) {
			nPts = newPts;
			// Update buffer and attributes
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferData(GL_ARRAY_BUFFER, dataPt.size()*2*sizeof(GLfloat),&dataPt[0],GL_DYNAMIC_DRAW);
		}

		// Draw Plot
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesLimitViewportTrans));
		glm::vec4 inColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
		glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP,0,nPts);
		glBindVertexArray(0);
	}

	void appendPt(pt2D pt) {
		// Appends a point to the current data
		this->dataPt.push_back(pt);
	}

};



#endif /* LINE2D_H_ */
