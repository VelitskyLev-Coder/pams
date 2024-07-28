#pragma once
#include <vector>

#include "Matrix.h"

struct PamResult {
  std::vector<std::vector<size_t>> clusters;
  std::vector<size_t> medoids;
};

std::vector<size_t> initializeMedoids(size_t max_num, size_t k,
                                      size_t seed = 0);

void assignPointsToMedoids(const Matrix& distanceMatrix,
                           const std::vector<size_t>& medoids,
                           std::vector<size_t>& oAssignedPoints);

double computeTotalCost(const Matrix& distanceMatrix,
                        const std::vector<size_t>& oAssignedPoints);

size_t getbestMedoidIndexAtCluster(const Matrix& distanceMatrix,
                                   const std::vector<size_t>& clusters,
                                   size_t clusterMedoidIndex);

PamResult pam(const Matrix& distanceMatrix, int k);
