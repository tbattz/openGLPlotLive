# openGLPlotLive
openGLPlotter provides a means to plot large amounts of quickly updated data in an OpenGL context. This can be a window created specifically for the plotter, or to embed in another OpenGL window. Originally this was created to enable debugging of another OpenGL project, where the bug required visualising the path of a vehicle in 60 fps. It may be useful for similar purposes, or for those wanting to embed a graphing library in their code, using c++. 

# What can it do
Plot simple data in realtime.

Current method of operation.
* Takes pointers to a vector of x and a vector of y data.
* Internally, it copies the data from the vectors and stores it internally, in a format suitable for passing to OpenGL shaders.
* The data is then plotted on an axes (that may be changing dynamically), with the program managing the display of the data and OpenGL context.

A few interactive tools are provided
* The ability to toggle auto scaling through a button
* Ability to turn off/on the display of the grid lines and outer axes box
* Zooming using the scroll wheel
* Scaling by dragging and holding the right mouse button
* Panning with the left mouse button
* Zoom to box by dragging the middle mouse button
* An interactor that displays the current point of the highlighted group. The current group is iterated through by pressing the space bar


<img src="https://github.com/tbattz/openGLPlotLive/raw/master/Other/plotGif.gif" width="830" height="430">

# Simple Plots
## Window Setup
To create a window for plotting, you will need the following
```c++
// Window Size
int windowWidth  = 800;
int windowHeight = 800;

// Init GLFW
std::shared_ptr<GLPL::IWindow> window = std::shared_ptr<GLPL::IWindow>(new GLPL::Window(windowWidth, windowHeight));
std::shared_ptr<GLPL::Window> window2 = std::dynamic_pointer_cast<GLPL::Window>(window);
```

## Create Data
You will need some data to plot, which now can be plotting by providing pointers to x and y vectors.
```c++
// Graph 2
std::vector<float> xVec9;
std::vector<float> yVec9;
xVec9.reserve(2500);
yVec9.reserve(2500);
for(int i=-1000; i<1500; i++) {
    xVec9.push_back(i/500.0);
    yVec9.push_back(0.75*sin(i/500.0));
}
```

You can also initialise data, and create data as the plot is being draw.
```c++
// Graph 12 - Damping
std::vector<float> xVec12 = {};
std::vector<float> yVec12 = {};
xVec12.reserve(2000);
yVec12.reserve(2000);
```
Adding new data is then provided by obtaining a vector to the line, when the line is created and added to the plot. This will be shown below.

## Create Plot
You will need to create a plot to add the lines to. Creating a plot that starts at x=0.0, y=0.25 (proportion of the window), and has width and height of 0.75 the respective lengths. The position and size of this can be updated in realtime.
```c++
// Create Plot
std::shared_ptr<GLPL::Plot> myplot = std::make_shared<GLPL::Plot>(0.0, 0.25, 0.5, 0.75, window2->getParentDimensions(), 2, 2);
std::shared_ptr<GLPL::IDrawable> myPlotPt = std::dynamic_pointer_cast<GLPL::IDrawable>(myplot);
window2->addPlot(myPlotPt);
```
Lines are then added to the plot as below.
```c++
std::shared_ptr<GLPL::ILine2D> line12 = axesPt->addLine(&xVec12, &yVec12, GLPL::SINGLE_LINE, LC_YELLOW, 0.5);
```
A pointer to the line data can be obtained, if the user wishes to change or add more data to the line. The plot will be updated on the draw draw call.
```c++
std::shared_ptr<GLPL::Line2D2Vecs> line12b = std::dynamic_pointer_cast<GLPL::Line2D2Vecs>(line12);
```
The axes configuration can be adjusted.
```c++
axesPt->setAxesBoxOn(false);
axesPt->setButtonState("Grid", false);
axesPt->setXLabel("Time (s)");
axesPt->setYLabel("Displacement (m)");
axesPt->setTitle("Spring Damping Over Time");
axesPt->setYLabelRotation(GLPL::SIDEWAYS_RIGHT);
```

## The Drawing Loop
To draw in real time you will require a drawing loop.
The following draws the plot, axes and lines, updates the plot data for plot 5 and prepares the plot for the next draw call.
```c++
while(!glfwWindowShouldClose(window->getWindow())) {
    // Pre-loop draw
    window2->preLoopDraw(true);

    // Update Plot Data
    line12b->dataPtX->push_back(i);
    yVal12 = cos((i) / (25*M_PI)) * exp(-(i)/(25*8*M_PI));
    line12b->dataPtY->push_back(yVal12);
    line12b->updateInternalData();

	i += 1;
    
    // Draw Plot
    myplot->Draw();

	// Post-loop draw
	window2->postLoopDraw();
}
```
## After the draw loop, handle closing the window
```c++
// Close Window
glfwTerminate();
```

## Shader Directory
When including in another project, you'll need to copy the Shader directory to the root of your project. Provided the openGLPlotLive directory is in the top level of your project, this can be done in cmake with the following,
```cmake
add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/../openGLPlotLive/Shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders)
```

# Dependencies
##  Automated Install
A few scripts have been created to automate the installation process. In the event these fail, you may have to compile the dependencies manually.

### Linux (Ubuntu)
On Ubuntu these dependencies can be installed by running
```
sudo chmod +x installDependencies.sh
./installDependenciesUbuntu.txt
```
## Manual Install
### Linux (Ubuntu)
* Building Dependencies
	* cmake
	```
	sudo apt-get install cmake
	```
* GLFW (multi-platform library for OpenGL)
	* Download from http://www.glfw.org/download.html and extract.
	* Install dependencies
	```
	sudo apt-get install xorg-dev libglu1-mesa-dev
	```
	* Navigate to the glfw root directory and use cmake
	```
	cmake -G "Unix Makefiles"
	make -j4
	sudo make install
	```

* GLM (The OpenGL Mathematics Library)
	* Download a release from Github and extract, https://github.com/g-truc/glm/tags
	* Copy the glm folder inside the glm release into the include directory
	```
	cd <glm_root_dir>
	sudo cp -r glm /usr/local/include/
	```

* FreeType - Font Rendering Library
	* Download and extract from https://www.freetype.org/index.html
	* Naigate to the root directory and run
	* Copy include files to /usr/local/include
	```
	sudo cp -r include/* /usr/local/include 
	```
	Please DO NOT run ./configure on a system running the Unity window manager, as this will most likely break Unity.


# Compiling
A CMakeLists.txt file is included for compiling with Cmake. This should work for systems running Ubuntu. Navigate to the build directory and remove any old CMakeFiles
```
cd build
rm -r *
```
Run cmake to generate a makefile.
```
cmake ../                           # Linux
cmake -G "MinGW Makefiles" ../      # Windows
```
Compile the program.
```
make -j4
```

There are a number of different make targets.

| Target         | Description                                     | Output (build directory)                                |
| -------------- | ----------------------------------------------- | ------------------------------------------------------- |
| all            | Make all the targets below                      |                                                         |
| examplePlot    | Make the example plot window                    | apps/examplePlot                                        |
| openGLPlotLive | Make the shared library libopenGLPlotLive.so    | src/libOpenGLPlotLive.so                                |
| docs           | Makes both the doxygen and sphinx documentation | docs/doxygen/html/index.html,<br>docs/sphinx/index.html |
| tests          | Make the test suite runner using gtest          | tests/openGLPlotLive-proj_tests                         |


## Debugging
To enable debugging, when running cmake, enable the debugging flag.
```
cmake -DCMAKE_BUILD_TYPE=Debug
```

## Documentation
There are two ways to generate documentation, with the latter requiring the former.
* Using Doxygen
* Using Sphinx with Breathe and Doxygen

### Doxygen
To generate doxygen documentation, provided doxygen is in your path, run the following
```
cd build
rm -rf *

cmake ../                           # Linux
cmake -G "MinGW Makefiles" ../      # Windows

make doxygen
```
The main page is then generated at <project_root>/build/docs/html/index.html.

### Sphinx
Sphinx documentation requires first generating Doxygen documentation, then using the outputted xml with Breathe, to generated the sphinx documentation.
```
# First generate doxygen documentation as above
cd build
make sphinx
```

# Runnning an Example
An example binary is created, examplePlot. To run this
```
cd build/apps
./examplePlot
```

# Controls
When autoscaling is disabled, the following controls allows the user to manipulate the axes area:

| Control Input                     | Description                                                          |
| --------------------------------- | -------------------------------------------------------------------- |
| Scroll Wheel                      | Zooms the axes in and out                                            |
| Right Click & Drag                | Zoom the x & y axes at different scales, based on the drag distance  |
| Left Click & Drag                 | Pan the axes left, right, up or down                                 |
| Left Click, hold Shift Key & Drag | Drag a zoom box, that will zoom to the extents of the box on release |
| Middle Click & Drag               | Drag a zoom box, that will zoom to the extents of the box on release |
| Spacebar                          | Iterates through plotables in the view, when the view is hovered     |

# Compiling an Eclipse Project
To generate an Eclipse project, from the root directory,
```
cd build
rm -r CMakeFiles/
cmake -G "Eclipse CDT4 - Unix Makefiles" ../src
```
Then import the project into Eclipse using File >> Import >> General >> Existing Projects into Workspace. Click next and set the root directory to <workspace>/openGLMap/build. Click Finish. The project can now be built with Eclipse using the 'all' Build Target. The source files will be inside the "[Source Directory]" and are linked to their actual counterpats.

# Debugging
A useful tool for debugging anything at the opengl level is qrenderdoc - https://github.com/baldurk/renderdoc

# Tests with googletest
Some simple tests have been written for debugging purposes. These use the googletest framework. To compile and run the tests,
```
cd build
make tests -j4
cd tests
./openGLPlotLive-proj_tests
```

