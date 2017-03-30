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

/* ----------------------- */
/* Functions for all lines */
/* ----------------------- */
void createAndSetupBuffers(GLuint* VAOPt,GLuint* VBOPt,int dataSizeBytes,const void* dataAddress,int strideBytes) {
	/* Create Buffers */
	glGenVertexArrays(1,VAOPt);
	glGenBuffers(1,VBOPt);

	/* Setup Buffers */
	glBindVertexArray(*VAOPt);
	glBindBuffer(GL_ARRAY_BUFFER,*VBOPt);

	glBufferData(GL_ARRAY_BUFFER, dataSizeBytes,dataAddress,GL_DYNAMIC_DRAW);

	/* Position Attributes */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, strideBytes,(GLvoid*)0);

	glBindVertexArray(0); // Unbind VAO

}

void drawData(Shader shader, glm::mat4 axesLimitViewportTrans,GLuint* VAOPt, int nPts) {
	// Draws the data currently stored in the line corresponding to the given VAO
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesLimitViewportTrans));
	glm::vec4 inColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
	glBindVertexArray(*VAOPt);
	glDrawArrays(GL_LINE_STRIP,0,nPts);
	glBindVertexArray(0);
}

/* ------------------------ */
/* 	    Line Functions 	    */
/* ------------------------ */
/* ====================================================================== */
/*						 Line of pt2D Structures						  */
/* ====================================================================== */
class Line2DPts {
public:
	/* Buffers */
	GLuint VAO, VBO;
	int nPts = 0;

	/* Data */
	vector<pt2D>* dataPtPt;

	Line2DPts(vector<pt2D>* dataPtPt) {
		this->dataPtPt = dataPtPt;

		/* Setup Buffers */
		int dataSizeBytes = dataPtPt->size()*2*sizeof(GLfloat);
		createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, dataPtPt->data(), sizeof(pt2D)); // dataPtPt->data() gives the address of the first element of the vector

		/* Set Number of Points */
		nPts = dataPtPt->size();

	}

	void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
		// Check if number of points changed
		int newPts = dataPtPt->size();
		if (newPts != nPts) {
			nPts = newPts;
			// Update buffer and attributes
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferData(GL_ARRAY_BUFFER, dataPtPt->size()*2*sizeof(GLfloat),dataPtPt->data(),GL_DYNAMIC_DRAW);
		}

		// Draw Plot
		drawData(shader, axesLimitViewportTrans, &VAO, nPts);
	}

	void appendPt(pt2D pt) {
		// Appends a point to the current data
		this->dataPtPt->push_back(pt);
	}

};

/* ====================================================================== */
/*						 Line of 2-length Vectors						  */
/* ====================================================================== */
class Line2DVec {
public:
	/* Buffers */
	GLuint VAO, VBO;
	int nPts = 0;

	/* Data */
	vector<float>* dataVecPt;

	Line2DVec(vector<float>* dataVecPt) {
		this->dataVecPt = dataVecPt;

		/* Setup Buffers */
		int dataSizeBytes = dataVecPt->size()*sizeof((*dataVecPt)[0]);
		createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, dataVecPt->data(), 2*sizeof((*dataVecPt)[0])); // dataVecPt->data() gives the address of the first element of the vector

		/* Set Number of Points */
		nPts = dataVecPt->size()/2;

	}

	void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
		// Check if number of points changed
		int newPts = dataVecPt->size()/2;
		if (newPts != nPts) {
			nPts = newPts;
			// Update buffer and attributes
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferData(GL_ARRAY_BUFFER, dataVecPt->size()*sizeof((*dataVecPt)[0]),dataVecPt->data(),GL_DYNAMIC_DRAW);
		}

		// Draw Plot
		drawData(shader, axesLimitViewportTrans, &VAO, nPts);
	}

	void appendVec(float x, float y) {
		// Appends a point to the current data
		this->dataVecPt->push_back(x);
		this->dataVecPt->push_back(y);
	}

};

/* ====================================================================== */
/*						 Line of vector of vectors						  */
/* ====================================================================== */
class Line2DVecVec {
public:
	/* Buffers */
	GLuint VAO, VBO;
	int nPts = 0;

	/* Data */
	vector<vector<float>>* dataVecPt;
	vector<float> internalData;

	Line2DVecVec(vector<vector<float>>* dataVecPt) {
		this->dataVecPt = dataVecPt;

		/* Setup Buffers */
		updateInternalData();
		int dataSizeBytes = internalData.size()*sizeof(internalData[0]);
		createAndSetupBuffers(&VAO, &VBO, dataSizeBytes, &internalData[0], 2*sizeof(internalData[0]));

		/* Set Number of Points */
		nPts = dataVecPt->size()/2.0;

	}

	void updateInternalData() {
		/* Creates an internal data store from the current dataVecPt */
		// Clear Previous Data
		internalData.clear();
		// Update With New Data
		for(unsigned int i=0; i<dataVecPt->size(); i++) {
			internalData.push_back((*dataVecPt)[i][0]);
			internalData.push_back((*dataVecPt)[i][1]);
		}
	}

	void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) {
		// Check if number of points changed
		int newPts = (internalData).size()/2;
		if (newPts != nPts) {
			nPts = newPts;
			// Update buffer and attributes
			glBindBuffer(GL_ARRAY_BUFFER,VBO);
			glBufferData(GL_ARRAY_BUFFER, internalData.size()*sizeof(internalData[0]),&internalData[0],GL_DYNAMIC_DRAW);
		}

		// Draw Plot
		drawData(shader, axesLimitViewportTrans, &VAO, nPts);
	}

};

#endif /* LINE2D_H_ */
