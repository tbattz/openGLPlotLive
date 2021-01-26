//
// Created by bcub3d-desktop on 26/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_SCATTER2D2VECS_H
#define OPENGLPLOTLIVE_PROJ_SCATTER2D2VECS_H


#include "IScatterPlot2D.h"

namespace GLPL {


    class Scatter2D2Vecs : public IScatterPlot2D {
        // Two vectors corresponding to an x vector and a y vector
    public:
        // Constructor
        Scatter2D2Vecs(std::vector<float>* dataPtX, std::vector<float>* dataPtY,
                std::shared_ptr<ParentDimensions> parentDimensions);

        // Destructor
        ~Scatter2D2Vecs();

        // Functions
        void updateInternalData();
        void updateIncrementalInternalData();
        void Draw();
        std::string getID();
        void clearData();
        std::vector<float> getMinMax(bool onlyPositiveX, bool onlyPositiveY);
        std::tuple<float, float> getClosestPoint(float xVal);
        std::tuple<float, float> getClosestPoint(float xVal, float xmin, float xmax, float ymin, float ymax);

        std::vector<float>* dataPtX;
        std::vector<float>* dataPtY;

    private:
        // Data
        int nPts = 0;

        std::vector<float> internalData;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_SCATTER2D2VECS_H
