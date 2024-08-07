#pragma once
#include <vector>

#include "Matrix.h"

struct PamResult {
  std::vector<std::vector<size_t>> clusters;
  std::vector<size_t> medoids;
};

class PamBuilder {
 public:
  PamBuilder(const Matrix& distMatrix) : distanceMatrix(distMatrix) {}
  PamResult pam(int k);

 private:
  void assignPointsToMedoids(const std::vector<size_t>& medoids,
                             std::vector<size_t>& oAssignedPoints);

  size_t getbestMedoidIndexAtCluster(const std::vector<size_t>& clusters,
                                     size_t clusterMedoidIndex);
  std::vector<size_t> initializeMedoids(size_t k);

  const Matrix& distanceMatrix;
  std::vector<size_t> initializeMedoidResultCache;
  std::vector<double> initializeMedoidMinDistanceCache;
};
