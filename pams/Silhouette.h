#pragma once
#include <vector>

#include "Matrix.h"

std::vector<std::vector<size_t>> createPamClusters(const Matrix& distMatrix);

double calcSilhouette(const Matrix& distMatrix,
                      const std::vector<std::vector<size_t>>& clusters,
                      const std::vector<size_t>& medoids);
