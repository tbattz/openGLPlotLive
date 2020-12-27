//
// Created by tbatt on 24/06/2020.
//

#include "Plotable.h"

#include <utility>


namespace GLPL {

    Plotable::Plotable(std::shared_ptr<ParentDimensions> parentDimensions) :
        ConstantXYDrawable(0.0f, 0.0f, 1.0f, 1.0f, CONSTANT_SCALE, CONSTANT_SCALE, std::move(parentDimensions)) {

    }

    void Plotable::setAxesViewportTransform(std::shared_ptr<glm::mat4> newAxesViewportTransform) {
        this->axesViewportTransform = std::move(newAxesViewportTransform);
    }

    void Plotable::setPlotableId(int newPlotableId) {
        plotableId = newPlotableId;
    }

    std::vector<int> Plotable::genIndicesSortedVector(std::vector<float>* unsortedVector) {
        // Create vector of indices
        std::vector<int> indices(unsortedVector->size());
        std::iota(indices.begin(), indices.end(), 0);
        // Sort by values in unsorted vector
        std::sort(indices.begin(), indices.end(),
                [&](int A, int B) -> bool {
                    return (*unsortedVector)[A] < (*unsortedVector)[B];
        });

        return indices;
    }

    std::vector<float> Plotable::sortVectorByIndices(std::vector<float>* unsortedVector, std::vector<int> indices) {
        // Sort vector by indices vector
        std::vector<float> sortedVector;
        sortedVector.reserve(unsortedVector->size());
        for(unsigned int i=0; i < indices.size(); i++) {
            sortedVector.push_back((*unsortedVector)[indices[i]]);
        }

        return sortedVector;
    }


}