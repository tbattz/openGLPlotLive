/*
 * axes.h
 *
 *  Created on: 12Feb.,2017
 *      Author: bcub3d-laptop
 */

#ifndef P_AXES_H_
#define P_AXES_H_

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Standard Includes
#include <iomanip>
#include <sstream>
#include <math.h>

// GLPL Includes
#include "../src/transforms.h"
#include "../src/window.h"
#include "../src/plot.h"
#include "../src/shader.h"
#include "../src/fonts.h"

namespace GLPL {

	class Axes {
		// Axes contains the axes draw space, axes and tick markings
	public:
		// Position
		float x; // Location of bottom left corner x position of axes in 0 to 1 (in plot area)
		float y; // Location of bottom left corner y position of axes in 0 to 1 (in plot area)
		float width; // Width of axes as a proportion of the current plot width, including tick marks
		float height; // Height of axes as a proportion of the current plot height, including tick marks
		// Axes Limits
		float xmin = -1.0;
		float xmax = 1.0;
		float ymin = -1.0;
		float ymax = 1.0;
		bool autoScaleX = true; // True if the x-axes are to sale to the given data
		bool autoScaleY = true; // True if the y-axes are to sale to the given data
		bool autoScaleRound = true; // True to round auto scale limits to the nearest significant value
		// Axes Ticks
		float majorTickSizeW = 0.03; // Size of major axes ticks (proportional to plot area width)
		float minorTickSizeW = 0.015;// Size of minor axes ticks (proportional to plot area width)
		float majorTickSizeH = 0.03; // Size of major axes ticks (proportional to plot area height)
		float minorTickSizeH = 0.015;// Size of minor axes ticks (proportional to plot area height)
		int majorTickNum = 5; 		 // The number of major tick marks
		int minorTickNum = 3;		 // The number of minor tick marks between each set of major tick marks
		int maxMajorTickNum = 20;	 // The maximum number of major tick marks (used to preallocate buffer)
		int maxMinorTickNum = 5;	 // The maximum number of minor tick marks (used to preallocate buffer)
		// Buffers
		GLuint VAO, VBO;
		GLuint axVAO, axVBO;
		GLuint axtVAO, axtVBO;
		// Axes Box
		vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
		// Axes Lines
		vector<GLfloat> axesVerts = {-1, -1,   1, -1,    1,  1,    -1, 1};
		// Axes Ticks
		vector<GLfloat> axesTicks = {0, 0, -1, 1};
		// Line Data
		vector<Line2DPts*> lines2DPts;
		vector<Line2DVec*> lines2DVec;
		vector<Line2DVecVec*> lines2DVecVec;
		vector<Line2DVecGLMV3*> lines2DVecGLMV3;
		vector<Line2DVecfVecGLMV3*> lines2DVecfVecGLMV3;
		// Window Dimensions
		WinDimensions* winDimPt;
		// Font Shader
		Shader* textShaderPt;
		// Font Pointer
		const GLchar* fontPath = "/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf";
		GLFont axesTicksFont;


		Axes(float x,float y, float width, float height, WinDimensions* winDimPt, Shader* textShaderPt) : axesTicksFont(fontPath) {
			// Setup Position
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->winDimPt = winDimPt;
			this->textShaderPt = textShaderPt;

			// Setup Buffers
			createAndSetupBuffers();
			createAndSetupAxesLineBuffers();
			createAndSetupAxesTickBuffers();

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

		void createAndSetupAxesLineBuffers() {
			/* Create Buffers */
			glGenVertexArrays(1,&axVAO);
			glGenBuffers(1,&axVBO);

			/* Setup Buffers */
			glBindVertexArray(axVAO);
			glBindBuffer(GL_ARRAY_BUFFER,axVBO);
			glBufferData(GL_ARRAY_BUFFER, axesVerts.size()*2*sizeof(GLfloat),&axesVerts[0],GL_DYNAMIC_DRAW);

			/* Position Attributes */
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0,(GLvoid*)0);

			glBindVertexArray(0); // Unbind VAO

		}

		void createAndSetupAxesTickBuffers() {
			/* Create Buffers */
			glGenVertexArrays(1,&axtVAO);
			glGenBuffers(1,&axtVBO);

			/* Setup Buffers */
			glBindVertexArray(axtVAO);
			glBindBuffer(GL_ARRAY_BUFFER,axtVBO);
			glBufferData(GL_ARRAY_BUFFER, maxMajorTickNum*2*sizeof(GLfloat),&axesTicks[0],GL_DYNAMIC_DRAW);

			/* Position Attributes */
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

			glBindVertexArray(0); // Unbind VAO
		}

		void Draw(Shader shader,glm::mat4 plotViewportTrans) {
			// Check for autoscaling of axes
			if(autoScaleX || autoScaleY) {
				updateAxesLimitsAutoscale();
			}

			// Calculate Viewport Transformation
			//glm::mat4 axesAreaViewportTrans = plotViewportTrans * viewportTransform(x, y, width, height);
			glm::mat4 axesViewportTrans = plotViewportTrans * viewportTransform(x+majorTickSizeW, y+majorTickSizeH, width - majorTickSizeW, height - majorTickSizeH);
			glm::mat4 axesLimitsViewportTrans = axesViewportTrans * scale2AxesLimits();

			// Draw Axes Box Outline
			//drawAxesAreaOutline(shader, axesAreaViewportTrans);

			// Draw Axes Box
			drawAxesBox(shader, axesViewportTrans);

			// Scissor Test
			glEnable(GL_SCISSOR_TEST);
			vector<float> xy = calculateScissor(axesLimitsViewportTrans);
			glScissor(xy[0]*(winDimPt->width),xy[1]*(winDimPt->height),(xy[2]-xy[0])*(winDimPt->width),(xy[3]-xy[1])*(winDimPt->height));

			// Draw Axes Lines
			drawAxesLines(shader, axesLimitsViewportTrans);

			// Draw All Lines
			drawLines(shader, axesLimitsViewportTrans);
			// Disable Scissor Testing
			glDisable(GL_SCISSOR_TEST);

			// Draw Tick Marks and Labels
			drawAxesTickMarks(shader,axesViewportTrans);

			// Draw Tick Mark Labels
			drawMajorAxesTickLabels(axesViewportTrans);
		}

		vector<float> calculateScissor(glm::mat4 axesLimitsViewportTrans) {
			// Calculate corners of axes limits area
			glm::vec4 a = axesLimitsViewportTrans * glm::vec4(xmin,ymin,0,1); // -1 to 1
			glm::vec4 b = axesLimitsViewportTrans * glm::vec4(xmax,ymax,0,1); // -1 to 1
			// Transform back to 0 to 1
			float x1 = 0.5*a[0] + 0.5;
			float y1 = 0.5*a[1] + 0.5;
			float x2 = 0.5*b[0] + 0.5;
			float y2 = 0.5*b[1] + 0.5;
			// Form vector
			vector<float> xyVec = {x1,y1,x2,y2};

			return xyVec;
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

		void addLine(Line2DPts* line) {
			// Adds a line containing pt2D to the axes
			lines2DPts.push_back(line);
		}

		void addLine(Line2DVec* line) {
			// Adds a line containing vectors to the axes
			lines2DVec.push_back(line);
		}

		void addLine(Line2DVecVec* line) {
			// Adds a line containing a vector of vectors to the axes
			lines2DVecVec.push_back(line);
		}

		void addLine(Line2DVecGLMV3* line) {
			// Adds a line containing a vector of glm::dvec3 to the axes
			lines2DVecGLMV3.push_back(line);
		}

		void addLine(Line2DVecfVecGLMV3* line) {
			// Adds to the axes a line containing both a vector of floats and vector of glm::dvec3, so that
			// data from the two can be plotted against each other.
			lines2DVecfVecGLMV3.push_back(line);
		}

		void drawLines(Shader shader, glm::mat4 axesLimitsViewportTrans) {
			// Draws the lines of 2Dpts on the axes
			for(unsigned int i=0; i<lines2DPts.size(); i++) {
				lines2DPts[i]->Draw(shader, axesLimitsViewportTrans);
			}
			// Draw the lines of 2D Vecs on the axes
			for(unsigned int i=0; i<lines2DVec.size(); i++) {
				lines2DVec[i]->Draw(shader, axesLimitsViewportTrans);
			}
			// Draw the lines of 2D Vectors of vectors on the axes
			for(unsigned int i=0; i<lines2DVecVec.size(); i++) {
				lines2DVecVec[i]->Draw(shader, axesLimitsViewportTrans);
			}
			// Draw the lines of vectors of glm::dvec3 on the axes
			for(unsigned int i=0; i<lines2DVecGLMV3.size(); i++) {
				lines2DVecGLMV3[i]->Draw(shader, axesLimitsViewportTrans);
			}
			// Draw the lines of vectors of floats against vectors of glm::dvec3 on the axes
			for(unsigned int i=0; i<lines2DVecfVecGLMV3.size(); i++) {
				lines2DVecfVecGLMV3[i]->Draw(shader, axesLimitsViewportTrans);
			}
		}

		void drawAxesLines(Shader shader, glm::mat4 axesLimitsViewportTrans) {
			// Update Line Data
			axesVerts = {xmin, 0.0,   xmax, 0.0,   0.0, ymin,   0.0, ymax};
			glBindBuffer(GL_ARRAY_BUFFER,axVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, axesVerts.size()*sizeof(GLfloat), &axesVerts[0]) ;
			// Draws axes lines in grey
			shader.Use();
			glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesLimitsViewportTrans));
			glm::vec4 inColor = glm::vec4(0.5,0.5,0.5,1.0);
			glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
			glBindVertexArray(axVAO);
			glDrawArrays(GL_LINES,0,4);
			glBindVertexArray(0);

		}

		void drawAxesTickMarks(Shader shader, glm::mat4 axesViewportTrans) {
			// Draws the major and minor axes tick marks
			// Update Tick Marks
			axesTicks = {};
			// x Axes
			for(int i=0; i<majorTickNum; i++) {
				// Calculate in -1 to 1
				float spacing = 2.0/(majorTickNum-1);
				axesTicks.push_back(-1.0 + (i*spacing)); 	// x1
				axesTicks.push_back(-1.0);					// y1
				axesTicks.push_back(-1.0 + (i*spacing));	// x2
				axesTicks.push_back(-1.0 - (2.0/(1.0-(majorTickSizeH)) - 2.0)); // y2
			}
			// y Axes
			for(int i=0; i<majorTickNum; i++) {
				// Calculate in -1 to 1
				float spacing = 2.0/(majorTickNum-1);
				axesTicks.push_back(-1.0); 					// x1
				axesTicks.push_back(-1.0 + (i*spacing));	// y1
				axesTicks.push_back(-1.0 - (2.0/(1.0-(majorTickSizeW)) - 2.0));	// x2
				axesTicks.push_back(-1.0 + (i*spacing)); 	// y2
			}

			// Bind Buffers
			glBindBuffer(GL_ARRAY_BUFFER,axtVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, axesTicks.size()*sizeof(GLfloat), &axesTicks[0]);
			// Draw Tick Lines
			shader.Use();
			glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesViewportTrans));
			glm::vec4 inColor = glm::vec4(1.0,1.0,1.0,1.0);
			glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
			glBindVertexArray(axtVAO);
			glDrawArrays(GL_LINES,0,(int)axesTicks.size()/2);
			glBindVertexArray(0);

		}

		glm::mat4 scale2AxesLimits() {
			// Creates a transformation matrix to scale points to the axes limits
			// Calculate center of current limits
			float xShift = ((xmin+xmax)/2.0)/(xmax-xmin) * 2.0; // xavg/width * 2.0, *2 to take it to -1 to 1
			float yShift = ((ymin+ymax)/2.0)/(ymax-ymin) * 2.0; // yavg/height * 2.0, *2 to take it to -1 to 1

			// Translate by offset
			glm::mat4 trans = glm::translate(glm::mat4(1), glm::vec3(-xShift, -yShift,0));

			// Scale to limits
			float scaleX = 2.0/(xmax-xmin); // Inverted due to -1 to 1 mapping (less than abs(1) region)
			float scaleY = 2.0/(ymax-ymin); // Inverted due to -1 to 1 mapping (less than abs(1) region)
			glm::mat4 scale = glm::scale(trans, glm::vec3(scaleX,scaleY,1));

			return scale;
		}

		void drawMajorAxesTickLabels(glm::mat4 axesViewportTrans) {
			// Draws the number labelling for the major axes ticks
			// Set Uniform
			textShaderPt->Use();
			glm::mat4 textProjection = glm::ortho(0.0f, (float)winDimPt->width, 0.0f, (float)winDimPt->height);
			glUniformMatrix4fv(glGetUniformLocation(textShaderPt->Program,"textProjection"), 1, GL_FALSE, glm::value_ptr(axesViewportTrans*textProjection));
			// Draw Labels
			// x Axes
			for(int i=0; i<majorTickNum; i++) {
				// Calculate Label
				std::stringstream ss;
				float val = (xmin + (i*(xmax-xmin)/(majorTickNum-1.0)));
				ss << std::fixed << std::setprecision(1) << val;
				// Calculate Position
				float x = (axesTicks[i*4]+1)/2.0 * winDimPt->width;
				float y = (axesTicks[i*4+1]+1)/2.0 * winDimPt->height - 60;
				// Draw Label
				axesTicksFont.RenderText(textShaderPt,ss.str(),x,y,1.0f,glm::vec3(1.0f,1.0f,1.0f),2);
			}
			// y Axes
			for(int i=0; i<majorTickNum; i++) {
				int j = i + majorTickNum;
				// Calculate Label
				std::stringstream ss;
				float val = (ymin + (i*(ymax-ymin)/(majorTickNum-1.0)));
				ss << std::fixed << std::setprecision(1) << val;
				// Calculate Position
				float x = (axesTicks[j*4]+1)/2.0 * winDimPt->width - 80;
				float y = (axesTicks[j*4+1]+1)/2.0 * winDimPt->height;
				// Draw Label
				axesTicksFont.RenderText(textShaderPt,ss.str(),x,y,1.0f,glm::vec3(1.0f,1.0f,1.0f),3);
			}
		}

		void updateAxesLimits(float xmin, float xmax, float ymin, float ymax,bool autoChange = true) {
			// Updates the axes limits to those given
			this->xmin = xmin;
			this->xmax = xmax;
			this->ymin = ymin;
			this->ymax = ymax;
			// Set Autoscale Off
			if(autoChange) {
				autoScaleX = false;
				autoScaleY = false;
			}
		}

		void updateAxesLimits(vector<float> minMax, bool autoChange = true) {
			// Updates the axes limits given a vector of {xmin,xmax,ymin,ymax}
			this->xmin = minMax[0];
			this->xmax = minMax[1];
			this->ymin = minMax[2];
			this->ymax = minMax[3];
			// Set Autoscale Off
			if(autoChange) {
				autoScaleX = false;
				autoScaleY = false;
			}
		}

		void updateXAxesLimits(float xmin, float xmax, bool autoChange = true) {
			// Updates the x-axes limits to those given
			this->xmin = xmin;
			this->xmax = xmax;
			// Set autoscale off
			if (autoChange) {
				autoScaleX = false;
			}
		}

		void updateYAxesLimits(float ymin, float ymax, bool autoChange) {
			// Updates the y-axes limits to those given
			this->ymin = ymin;
			this->ymax = ymax;
			// Set autoscale off
			if (autoChange) {
				autoScaleY = false;
			}
		}

		void updateAxesLimitsAutoscale() {
			// Updates the axes limits according to the maximum
			// and minimum values of the current data
			vector<float> dataMinMax = {maxFloat,-maxFloat,maxFloat,-maxFloat};
			// Get data max and mins
			for(unsigned int i=0; i<lines2DPts.size(); i++) {
				vector<float> minMax = lines2DPts[i]->getMinMax();
				compareMinMax(&dataMinMax,minMax);
			}
			for(unsigned int i=0; i<lines2DVec.size(); i++) {
				vector<float> minMax = lines2DVec[i]->getMinMax();
				compareMinMax(&dataMinMax,minMax);
			}
			for(unsigned int i=0; i<lines2DVecVec.size(); i++) {
				vector<float> minMax = lines2DVecVec[i]->getMinMax();
				compareMinMax(&dataMinMax,minMax);
			}
			for(unsigned int i=0; i<lines2DVecGLMV3.size(); i++) {
				vector<float> minMax = lines2DVecGLMV3[i]->getMinMax();
				compareMinMax(&dataMinMax,minMax);
			}
			for(unsigned int i=0; i<lines2DVecfVecGLMV3.size(); i++) {
				vector<float> minMax = lines2DVecfVecGLMV3[i]->getMinMax();
				printf("%f,%f,%f,%f\n",minMax[0],minMax[1],minMax[2],minMax[3]);
				compareMinMax(&dataMinMax,minMax);
			}
			// Adjust to nearest significant number if specified
			if(autoScaleRound) {
				for(unsigned int i =0; i < dataMinMax.size(); i++) {
					float num = dataMinMax[i];
					// Get number of tens
					int tens = -1;
					while(abs(num) > 1) {
						tens += 1;
						num = num/10.0;
					}
					// Odd are mins, even are maxes
					if(i%2==0) {
						// Even, minimum
						dataMinMax[i] = floor(dataMinMax[i]/(pow(10.0,tens)))*(pow(10.0,tens));
					} else {
						// Odd, maximum
						dataMinMax[i] = ceil(dataMinMax[i]/(pow(10.0,tens)))*(pow(10.0,tens));
					}
				}
			}

			// Update min and max values
			if(autoScaleX) {
				updateXAxesLimits(dataMinMax[0],dataMinMax[1],false);
			}
			if(autoScaleY) {
				updateYAxesLimits(dataMinMax[2],dataMinMax[3],false);
			}
		}

		void compareMinMax(vector<float>* dataMinMaxPt,vector<float> minMax) {
			if (minMax[0] < (*dataMinMaxPt)[0]) {
				(*dataMinMaxPt)[0] = minMax[0];
			}
			if (minMax[1] > (*dataMinMaxPt)[1]) {
				(*dataMinMaxPt)[1] = minMax[1];
			}
			if (minMax[2] < (*dataMinMaxPt)[2]) {
				(*dataMinMaxPt)[2] = minMax[2];
			}
			if (minMax[3] > (*dataMinMaxPt)[3]) {
				(*dataMinMaxPt)[3] = minMax[3];
			}
		}
	};
}



#endif /* P_AXES_H_ */
