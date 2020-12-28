//
// Created by bcub3d-laptop-dell on 11/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_UTIL_H
#define OPENGLPLOTLIVE_PROJ_UTIL_H

// Standard Includes
#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

# define M_PI           3.14159265358979323846  /* pi */


namespace GLPL {

    int binarySearch(std::vector<float> data, unsigned int leftInd, unsigned int rightInd, float inVal, unsigned int dataLen=2);
    float clip(float inVal, float minVal, float maxVal);

}


#endif //OPENGLPLOTLIVE_PROJ_UTIL_H
