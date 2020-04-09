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
#include <memory>
#include <math.h>

// GLPL Includes
#include "../rendering/transforms.h"
#include "../window/window.h"
#include "../rendering/shader.h"
#include "../rendering/fonts.h"

// Project Includes
#include "../lines/Line2DPts.h"

namespace GLPL {

	class Axes {
		// Axes contains the axes draw space, axes and tick markings
	public:
	    // Constructor
        Axes(float x,float y, float width, float height, std::shared_ptr<IWindow> windowPt, Shader* textShaderPt);

	    // Functions
        void Draw(Shader shader,glm::mat4 plotViewportTrans);
        void updateAxesLimits(float xmin, float xmax, float ymin, float ymax,bool autoChange = true);
        void updateAxesLimits(std::vector<float> minMax, bool autoChange = true);
        void updateXAxesLimits(float xmin, float xmax, bool autoChange = true);
        void updateYAxesLimits(float ymin, float ymax, bool autoChange);
        void updateAxesLimitsAutoscale();
        Shader* getTextShaderPt();
        void addLine(std::shared_ptr<ILine2D> linePt);
        void setAutoScaleRound(bool newAutoScaleRond);
        void setEqualAxes(bool equalAxesBool);

    private:
	    // Functions
        void createAndSetupBuffers();
        void createAndSetupAxesLineBuffers();
        void createAndSetupAxesTickBuffers();
        std::vector<float> calculateScissor(glm::mat4 axesLimitsViewportTrans);
        void drawAxesAreaOutline(Shader shader, glm::mat4 axesAreaViewportTrans);
        void drawAxesBox(Shader shader, glm::mat4 axesViewportTrans);
        void drawAxesLines(Shader shader, glm::mat4 axesLimitsViewportTrans);
        void drawLines(Shader shader, glm::mat4 axesLimitsViewportTrans);;
        void drawAxesTickMarks(Shader shader, glm::mat4 axesViewportTrans);
        glm::mat4 scale2AxesLimits();
        void drawMajorAxesTickLabels(glm::mat4 axesViewportTrans);
        void compareMinMax(std::vector<float>* dataMinMaxPt,std::vector<float> minMax);
        std::vector<float> getOverallMinMax();
        void adjustSigFig(std::vector<float>* dataMinMaxPt);
        void makeAxesEqual(std::vector<float>* dataMinMaxPt);



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
		bool equalAxes = false; // Will adjust the plot limits so that the xrange and yrange are the same
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
		std::vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
		// Axes Lines
        std::vector<GLfloat> axesVerts = {-1, -1,   1, -1,    1,  1,    -1, 1};
		// Axes Ticks
        std::vector<GLfloat> axesTicks = {0, 0, -1, 1};
		// Line Data
		std::vector<std::shared_ptr<ILine2D>> lines2D;
		// Window Dimensions
		std::shared_ptr<IWindow> windowPt;
		// Font Shader
		Shader* textShaderPt;
		// Font Pointer
		const GLchar* fontPath = FONTPATH;
		GLFont axesTicksFont;
	};
}



#endif /* P_AXES_H_ */
