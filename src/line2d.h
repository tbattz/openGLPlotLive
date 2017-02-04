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

	/* Axes */


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

	void Draw(Shader shader) {
		// Check if number of points changed

		// Update buffer and attributes

		// Draw Plot
		shader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP,0,nPts);
		glBindVertexArray(0);
	}

};



#endif /* LINE2D_H_ */
