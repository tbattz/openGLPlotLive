//
// Created by bcub3d-laptop-dell on 11/12/20.
//

#include "util.h"

namespace GLPL {


    int binarySearch(std::vector<float> data, unsigned int leftInd, unsigned int rightInd,
                     float inVal, unsigned int dataLen) {
        // Format is [x1, y1, ..., n1, x2, y2, ..., n2, ...]
        // Take every second first index
        // dataLen is the distance between successive x values
        int m = 0;
        while (leftInd <= rightInd) {
            m = (int)floor((double)(leftInd + rightInd) / 2.0f);
            if (leftInd == rightInd - 1 || leftInd == rightInd) {
                return leftInd;
            } else if (data[dataLen*m] < inVal) {
                leftInd = m ;
            } else if (data[dataLen*m] > inVal) {
                rightInd = m;
            } else {
                return m;
            }

        }
        return -1;
    }

    float clip(float inVal, float minVal, float maxVal) {
        return std::max(minVal, std::min(inVal, maxVal));
    }

}
