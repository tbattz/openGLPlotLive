/*
 * plot.h
 *
 *  Created on: 12Feb.,2017
 *      Author: bcub3d-laptop
 */

#ifndef P_PLOT_H_
#define P_PLOT_H_

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Standard Includes
#include <memory>

// Project Includes
#include "../rendering/shader.h"
#include "../axes/axes.h"



namespace GLPL {
	class Plot {
		// Plots contain sets of axes, labels and titles
    public:
	    // Constructor
        Plot(float x, float y, float width, float height, std::shared_ptr<IWindow> windowPt, Shader* textShaderPt);
	    // Destructor
	    ~Plot();

        // Functions
        void Draw(Shader shader);
        void addLine(std::shared_ptr<ILine2D> linePt);
        void addShadedLine(std::shared_ptr<IShadedLine2D> shadedLinePt);
        std::shared_ptr<GLPL::Axes> getAxes();
        void setPlotOutlineOn(bool plotOutlineOn);

	private:
	    // Functions
        void createAndSetupBuffers();
        void setupFontShader(GLuint screenWidth, GLuint screenHeight);
        void drawPlotOutline(Shader shader, glm::mat4 plotViewportTrans);

	    // Data
		float x;			// Location of bottom left corner x position of plot in current window in 0 to 1
		float y;			// Location of bottom left corner y position of plot in current window in 0 to 1
		float width;  		// Width of plot as a proportion of the current window width
		float height;		// Height of plot as a proportion of the current window height
        std::shared_ptr<Axes> axes;

        // Outlines
        bool plotOutlineOn = true;

		// Buffers
		GLuint VAO, VBO;
		// Area
		std::vector<GLfloat> boxVerts = { -1, -1,    1, -1,    1,  1,    -1, 1};
		// Window Dimensions
        std::shared_ptr<IWindow> windowPt;

	};
}


#endif /* P_PLOT_H_ */
