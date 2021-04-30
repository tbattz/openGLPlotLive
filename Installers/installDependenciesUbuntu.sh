#!/bin/bash
# Make script executable with sudo chmod +x installDependencies.sh
# Run script without sudo.     ./installDependencies.

# Get number of cores
NPROC=$(nproc)
echo "Found" $NPROC "cores."     

# Get current directory
cd ..
curDIR=$PWD

# Switch to downloads directory
cd ~/Downloads
echo "Switched to ~/Downloads"

echo -e "\e[44m ================================ Build Dependencies =============================== \e[49m"
sudo apt-get -y install cmake

echo -e "\e[44m ===================== GLFW (multi-platform library for OpenGL) ==================== \e[49m"
if [ ! -d ~/Downloads/glfw/ ]; then
	git clone https://github.com/glfw/glfw.git
else
	echo "glfw directory already exists! Not cloning."
fi
echo "Installing mesa depencies."
sudo apt-get -y install xorg-dev libglu1-mesa-dev
echo "Building glfw."
cd glfw
cmake -G "Unix Makefiles"
make -j$NPROC
sudo make install
cd ..

echo -e "\e[44m ================== GLEW (The OpenGL Extension Wrangler Library) =================== \e[49m"
sudo apt-get -y install libglew-dev

echo -e "\e[44m ======================= GLM (The OpenGL Mathematics Library) ====================== \e[49m"
if [ ! -d ~/Downloads/glm/ ]; then
	git clone https://github.com/g-truc/glm.git
else
	echo "GLM directory exists! Not cloning."
fi
# Make include folder
echo "Copying GLM to" $curDIR"/include/"
sudo cp -r ~/Downloads/glm/glm /usr/local/include

echo -e "\e[44m ======================= FreeType - Font Rendering Library ========================= \e[49m"
if [ ! -f ~/Downloads/freetype.tar.bz2 ]; then
	wget -O ~/Downloads/freetype.tar.bz2 "http://download.savannah.gnu.org/releases/freetype/freetype-2.4.0.tar.bz2"
fi
tar xvjf ~/Downloads/freetype.tar.bz2
cd ~/Downloads/freetype-2.4.0
# Copy include files
sudo cp -r include/* /usr/local/include
