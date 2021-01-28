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

    std::pair<std::vector<unsigned int>, std::vector<unsigned int>> Plotable::genIndicesSortedVector(std::vector<float>* unsortedVector) {
        // Create vector of indices to use for sorting other vectors
        std::vector<unsigned int> indicesForSorting(unsortedVector->size());
        std::iota(indicesForSorting.begin(), indicesForSorting.end(), 0);
        // Sort by values in unsorted vector
        std::stable_sort(indicesForSorting.begin(), indicesForSorting.end(),
                [&](unsigned int A, unsigned int B) -> bool {
                    return (*unsortedVector)[A] < (*unsortedVector)[B];
        });

        // Create a new set of indices for the sorted data, that corresponds to the original order of the data
        std::vector<unsigned int> finalIndices(indicesForSorting.size());
        for(unsigned int i=0; i < indicesForSorting.size(); i++) {
           unsigned int newInd = indicesForSorting[i];
           finalIndices[newInd] = i;
        }

        return std::pair<std::vector<unsigned int>, std::vector<unsigned int>>(indicesForSorting, finalIndices);
    }

    std::vector<float> Plotable::sortVectorByIndices(std::vector<float>* unsortedVector, std::vector<unsigned int> indices) {
        // Sort vector by indices vector
        std::vector<float> sortedVector;
        sortedVector.reserve(unsortedVector->size());
        for(unsigned int i=0; i < indices.size(); i++) {
            sortedVector.push_back((*unsortedVector)[indices[i]]);
        }

        return sortedVector;
    }

    void Plotable::setLogModes(bool newLogX, bool newLogY) {
        logX = newLogX;
        logY = newLogY;
    }

    void Plotable::setLogXBase(unsigned int newLogXBase) {
        logXBase = newLogXBase;
    }

    void Plotable::setLogYBase(unsigned int newLogYBase) {
        logYBase = newLogYBase;
    }


}