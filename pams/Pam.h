#pragma once
#include <vector>
#include "Matrix.h"


std::vector<size_t> initializeMedoids(size_t max_num, size_t k,
                                      size_t seed = 0);

void assignPointsToMedoids(const Matrix& distanceMatrix,
                           const std::vector<size_t>& medoids,
                           std::vector<size_t>& oAssignedPoints);


double computeTotalCost(const Matrix& distanceMatrix,
                        std::vector<size_t>& oAssignedPoints);

std::vector<std::vector<size_t>> pam(const Matrix& distanceMatrix, int k);