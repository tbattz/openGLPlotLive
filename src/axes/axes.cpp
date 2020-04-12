//
// Created by bcub3d-desktop on 28/3/20.
//

#include "axes.h"

namespace GLPL {

    Axes::Axes(float x,float y, float width, float height, std::shared_ptr<IWindow> windowPt, Shader* textShaderPt) : axesTicksFont(fontPath) {
        // Setup Position
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->windowPt = windowPt;
        this->textShaderPt = textShaderPt;

        // Setup Buffers
        Axes::createAndSetupBuffers();
        Axes::createAndSetupAxesLineBuffers();
        Axes::createAndSetupAxesTickBuffers();
    }

    void Axes::createAndSetupBuffers() {
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

    void Axes::createAndSetupAxesLineBuffers() {
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

    void Axes::createAndSetupAxesTickBuffers() {
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

    void Axes::Draw(Shader shader,glm::mat4 plotViewportTrans) {
        // Check for autoscaling of axes
        if(autoScaleX || autoScaleY) {
            updateAxesLimitsAutoscale();
        }

        // Calculate Viewport Transformation
        //glm::mat4 axesAreaViewportTrans = plotViewportTrans * viewportTransform(x, y, width, height);
        glm::mat4 axesViewportTrans = plotViewportTrans * GLPL::Transforms::viewportTransform(x+majorTickSizeW, y+majorTickSizeH, width - majorTickSizeW, height - majorTickSizeH);
        glm::mat4 axesLimitsViewportTrans = axesViewportTrans * scale2AxesLimits();

        // Draw Axes Box Outline
        //drawAxesAreaOutline(shader, axesAreaViewportTrans);

        // Draw Axes Box
        if (axesAreaOutlineOn) {
            drawAxesBox(shader, axesViewportTrans);
        }

        // Scissor Test
        glEnable(GL_SCISSOR_TEST);
        std::vector<float> xy = calculateScissor(axesLimitsViewportTrans);
        glScissor(xy[0]*(windowPt->getWidth()),xy[1]*(windowPt->getHeight()),(xy[2]-xy[0])*(windowPt->getWidth()),(xy[3]-xy[1])*(windowPt->getHeight()));

        // Draw Axes Lines
        if(xyAxesOn) {
            drawAxesLines(shader, axesLimitsViewportTrans);
        }

        // Draw All Lines
        drawLines(shader, axesLimitsViewportTrans);
        // Draw All Shaded Lines
        drawShadedLines(shader, axesLimitsViewportTrans);
        // Disable Scissor Testing
        glDisable(GL_SCISSOR_TEST);

        // Draw Tick Marks and Labels
        drawAxesTickMarks(shader,axesViewportTrans);

        // Draw Tick Mark Labels
        if (axesLabelsOn) {
            drawMajorAxesTickLabels(axesViewportTrans);
        }
    }

    std::vector<float> Axes::calculateScissor(glm::mat4 axesLimitsViewportTrans) {
        // Calculate corners of axes limits area
        glm::vec4 a = axesLimitsViewportTrans * glm::vec4(xmin,ymin,0,1); // -1 to 1
        glm::vec4 b = axesLimitsViewportTrans * glm::vec4(xmax,ymax,0,1); // -1 to 1
        // Transform back to 0 to 1
        float x1 = 0.5*a[0] + 0.5;
        float y1 = 0.5*a[1] + 0.5;
        float x2 = 0.5*b[0] + 0.5;
        float y2 = 0.5*b[1] + 0.5;
        // Form vector
        std::vector<float> xyVec = {x1,y1,x2,y2};

        return xyVec;
    }

    void Axes::drawAxesAreaOutline(Shader shader, glm::mat4 axesAreaViewportTrans) {
        // Draw outline of plot area
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesAreaViewportTrans));
        glm::vec4 inColor = glm::vec4(0.0,0.0,1.0,1.0);
        glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP,0,4);
        glBindVertexArray(0);
    }

    void Axes::drawAxesBox(Shader shader, glm::mat4 axesViewportTrans) {
        // Draw Axes box
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program,"transformViewport"), 1, GL_FALSE, glm::value_ptr(axesViewportTrans));
        glm::vec4 inColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
        glUniform4fv(glGetUniformLocation(shader.Program,"inColor"),1,glm::value_ptr(inColor));
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_LOOP,0,4);
        glBindVertexArray(0);
    }

    void Axes::drawAxesLines(Shader shader, glm::mat4 axesLimitsViewportTrans) {
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

    void Axes::drawLines(Shader shader, glm::mat4 axesLimitsViewportTrans) {
        // Draw the lines on the axes
        for(unsigned int i=0; i<lines2D.size(); i++) {
            lines2D[i]->Draw(shader, axesLimitsViewportTrans);
        }
    }

    void Axes::drawShadedLines(Shader shader, glm::mat4 axesLimitsViewportTrans) {
        // Draw the shaded lines on the axes
        for(unsigned int i=0; i<shadedLines2D.size(); i++) {
            shadedLines2D[i]->Draw(shader, axesLimitsViewportTrans);
        }
    }

    void Axes::drawAxesTickMarks(Shader shader, glm::mat4 axesViewportTrans) {
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


    glm::mat4 Axes::scale2AxesLimits() {
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

    void Axes::drawMajorAxesTickLabels(glm::mat4 axesViewportTrans) {
        // Draws the number labelling for the major axes ticks
        // Set Uniform
        textShaderPt->Use();
        glm::mat4 textProjection = glm::ortho(0.0f, (float)windowPt->getWidth(), 0.0f, (float)windowPt->getHeight());
        glUniformMatrix4fv(glGetUniformLocation(textShaderPt->Program,"textProjection"), 1, GL_FALSE, glm::value_ptr(axesViewportTrans*textProjection));
        // Draw Labels
        // x Axes
        for(int i=0; i<majorTickNum; i++) {
            // Calculate Label
            std::stringstream ss;
            float val = (xmin + (i*(xmax-xmin)/(majorTickNum-1.0)));
            ss << std::fixed << std::setprecision(1) << val;
            // Calculate Position
            float x = (axesTicks[i*4]+1)/2.0 * windowPt->getWidth();
            float y = (axesTicks[i*4+1]+1)/2.0 * windowPt->getHeight() - 60;
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
            float x = (axesTicks[j*4]+1)/2.0 * windowPt->getWidth() - 80;
            float y = (axesTicks[j*4+1]+1)/2.0 * windowPt->getHeight();
            // Draw Label
            axesTicksFont.RenderText(textShaderPt,ss.str(),x,y,1.0f,glm::vec3(1.0f,1.0f,1.0f),3);
        }
    }

    void Axes::updateAxesLimits(float xmin, float xmax, float ymin, float ymax,bool autoChange) {
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

    void Axes::updateAxesLimits(std::vector<float> minMax, bool autoChange) {
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

    void Axes::updateXAxesLimits(float xmin, float xmax, bool autoChange) {
        // Updates the x-axes limits to those given
        this->xmin = xmin;
        this->xmax = xmax;
        // Set autoscale off
        if (autoChange) {
            autoScaleX = false;
        }
    }

    void Axes::updateYAxesLimits(float ymin, float ymax, bool autoChange) {
        // Updates the y-axes limits to those given
        this->ymin = ymin;
        this->ymax = ymax;
        // Set autoscale off
        if (autoChange) {
            autoScaleY = false;
        }
    }

    void Axes::updateAxesLimitsAutoscale() {
        // Updates the axes limits according to the maximum
        // and minimum values of the current data
        std::vector<float> dataMinMax = getOverallMinMax();

        // Adjust to nearest significant number if specified
        if(autoScaleRound) {
            adjustSigFig(&dataMinMax);
        }

        // Make axes equal if required
        if (equalAxes) {
            makeAxesEqual(&dataMinMax);
        }

        // Update min and max values
        if(autoScaleX) {
            updateXAxesLimits(dataMinMax[0],dataMinMax[1],false);
        }
        if(autoScaleY) {
            updateYAxesLimits(dataMinMax[2],dataMinMax[3],false);
        }
    }

    void Axes::compareMinMax(std::vector<float>* dataMinMaxPt,std::vector<float> minMax) {
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

    Shader *Axes::getTextShaderPt() {
        return textShaderPt;
    }

    void Axes::addLine(std::shared_ptr<ILine2D> linePt) {
        this->lines2D.push_back(linePt);
    }

    void Axes::addShadedLine(std::shared_ptr<IShadedLine2D> shadedLinePt) {
        this->shadedLines2D.push_back(shadedLinePt);
    }

    void Axes::setAutoScaleRound(bool newAutoScaleRound) {
        this->autoScaleRound = newAutoScaleRound;
    }

    void Axes::setEqualAxes(bool equalAxesBool) {
        this->equalAxes = equalAxesBool;
        this->autoScaleX = true;
        this->autoScaleY = true;
    }

    std::vector<float> Axes::getOverallMinMax() {
        // Updates the axes limits according to the maximum
        // and minimum values of the current data
        float maxFloat = std::numeric_limits<float>::max();
        std::vector<float> dataMinMax = {maxFloat,-maxFloat,maxFloat,-maxFloat};
        // Get data max and mins
        for(unsigned int i=0; i<lines2D.size(); i++) {
            std::vector<float> minMax = lines2D[i]->getMinMax();
            compareMinMax(&dataMinMax, minMax);
        }

        return dataMinMax;
    }

    void Axes::adjustSigFig(std::vector<float>* dataMinMaxPt) {
        for(unsigned int i =0; i < dataMinMaxPt->size(); i++) {
            float num = (*dataMinMaxPt)[i];
            // Get number of tens
            int tens = -1;
            while(abs(num) > 1) {
                tens += 1;
                num = num/10.0;
            }
            // Odd are mins, even are maxes
            if(i%2==0) {
                // Even, minimum
                (*dataMinMaxPt)[i] = floor((*dataMinMaxPt)[i]/(pow(10.0,tens)))*(pow(10.0,tens));
            } else {
                // Odd, maximum
                (*dataMinMaxPt)[i] = ceil((*dataMinMaxPt)[i]/(pow(10.0,tens)))*(pow(10.0,tens));
            }
        }
    }

    void Axes::makeAxesEqual(std::vector<float> *dataMinMaxPt) {
        // Get biggest range
        float xRange = (*dataMinMaxPt)[1] - (*dataMinMaxPt)[0];
        float yRange = (*dataMinMaxPt)[3] - (*dataMinMaxPt)[2];
        float w = width*(float)(windowPt->getWidth());
        float h = height*(float)(windowPt->getHeight());
        if (xRange/w < yRange/h) {
            // Adjust xRange
            float maxRange = (h/w)*yRange;
            double midX = ((*dataMinMaxPt)[0]+(*dataMinMaxPt)[1])/2.0;
            (*dataMinMaxPt)[0] = midX - (maxRange/2.0);
            (*dataMinMaxPt)[1] = midX + (maxRange/2.0);
        } else {
            // Adjust yRange
            float maxRange = (w/h)*xRange;
            double midY = ((*dataMinMaxPt)[2]+(*dataMinMaxPt)[3])/2.0;
            (*dataMinMaxPt)[2] = midY - (maxRange/2.0);
            (*dataMinMaxPt)[3] = midY + (maxRange/2.0);
        }

    }

    void Axes::setMinorTickSize(float newMinorTickSize) {
        this->minorTickSizeW = newMinorTickSize;
        this->minorTickSizeH = newMinorTickSize;
    }

    void Axes::setMajorTickSize(float newMajorTickSize) {
        this->majorTickSizeW = newMajorTickSize;
        this->majorTickSizeH = newMajorTickSize;
    }

    void Axes::setAxesAreaOutlineOn(bool axesAreaOutlineOn) {
        this->axesAreaOutlineOn = axesAreaOutlineOn;
    }

    void Axes::setXYAxesOn(bool showXYAxes) {
        this->xyAxesOn = showXYAxes;
    }

    void Axes::setPositionSize(float x, float y, float width, float height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void Axes::setAutoScale(bool autoScaleOn) {
        this->autoScaleX = autoScaleOn;
        this->autoScaleY = autoScaleOn;
    }

    void Axes::setAxesLabelsOn(bool axesLabelsOn) {
        this->axesLabelsOn = axesLabelsOn;
    }

}