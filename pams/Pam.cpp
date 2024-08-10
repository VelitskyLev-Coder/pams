#include "Pam.h"

#include <numeric>
#include <set>
#include <unordered_map>
#include <unordered_set>

/*
* Build Phase according to the paper
https://www.cs.umb.edu/cs738/pam1.pdf
*/

std::vector<size_t> PamBuilder::initializeMedoids(size_t k) {
  std::vector<size_t>& resultMedoids = initializeMedoidResultCache;
  std::vector<double>& minDistances = initializeMedoidMinDistanceCache;

  if (resultMedoids.size() >= k) {
    return std::vector<size_t>(resultMedoids.begin(),
                               resultMedoids.begin() + k);
  }

  size_t numPoints = distanceMatrix.size();

  if (resultMedoids.empty()) {
    size_t firstMedoid = 0;
    double minSumDistance = std::numeric_limits<double>::max();
    for (size_t i = 0; i < numPoints; ++i) {
      double sumDistance =
          std::reduce(distanceMatrix[i].begin(), distanceMatrix[i].end());
      if (sumDistance < minSumDistance) {
        minSumDistance = sumDistance;
        firstMedoid = i;
      }
    }

    resultMedoids.push_back(firstMedoid);
    minDistances.resize(numPoints, std::numeric_limits<double>::max());
    for (size_t i = 0; i < numPoints; ++i) {
      minDistances[i] = distanceMatrix[firstMedoid][i];
    }
  }

  std::set<size_t> remainingPoints;  // We use set to keep indexes in order for
                                     // cache locality
  for (size_t i = 0; i < numPoints; ++i) {
    remainingPoints.insert(i);
  }
  for (size_t medoid : resultMedoids) {
    remainingPoints.erase(medoid);
  }

  while (resultMedoids.size() < k) {
    size_t bestCandidate = 0;
    double maxGain = -1;

    for (size_t i : remainingPoints) {
      double gain = 0;

      for (size_t j : remainingPoints) {
        double Dj = minDistances[j];
        double Cji = std::max(Dj - distanceMatrix[i][j], 0.0);
        gain += Cji;
      }

      if (gain > maxGain) {
        maxGain = gain;
        bestCandidate = i;
      }
    }

    resultMedoids.push_back(bestCandidate);
    remainingPoints.erase(bestCandidate);

    // Update minDistances with the new medoid
    for (size_t i = 0; i < numPoints; ++i) {
      minDistances[i] =
          std::min(minDistances[i], distanceMatrix[bestCandidate][i]);
    }
  }

  return std::vector<size_t>(resultMedoids.begin(), resultMedoids.begin() + k);
}

void PamBuilder::assignPointsToMedoids(const std::vector<size_t>& medoids,
                                       std::vector<size_t>& oAssignedPoints) {
  size_t numPoints = distanceMatrix.size();
  oAssignedPoints.resize(numPoints);

  std::fill(oAssignedPoints.begin(), oAssignedPoints.end(), medoids[0]);

  for (size_t medoidIndex : medoids) {
    for (size_t i = 0; i < numPoints; i++) {
      double curDistance = distanceMatrix[medoidIndex][i];
      if (curDistance < distanceMatrix[oAssignedPoints[i]][i]) {
        oAssignedPoints[i] = medoidIndex;
      }
    }
  }
}


size_t PamBuilder::getbestMedoidIndexAtCluster(
    const std::vector<size_t>& clusters, size_t clusterMedoidIndex) {
  size_t resultBestMedoid = clusterMedoidIndex;
  double minDistanceSum = std::numeric_limits<double>::max();
  for (size_t pointIndex = 0; pointIndex < clusters.size(); pointIndex++) {
    if (clusters[pointIndex] != clusterMedoidIndex) {
      continue;
    }
    double curDistanceSum = 0;
    for (int i = 0; i < clusters.size(); i++) {
      if (clusters[i] == clusterMedoidIndex) {
        curDistanceSum += distanceMatrix[pointIndex][i];
      }
    }
    if (curDistanceSum < minDistanceSum) {
      minDistanceSum = curDistanceSum;
      resultBestMedoid = pointIndex;
    }
  }

  return resultBestMedoid;
}

PamResult PamBuilder::pam(const int k) {
  std::vector<size_t> medoids = initializeMedoids(k);
  std::vector<size_t> clusters(distanceMatrix.size());

  assignPointsToMedoids(medoids, clusters);

  while (true) {
    bool improved = false;

    for (size_t i = 0; i < medoids.size(); i++) {
      size_t bestNewMedoidIndexInCurrentCluster =
          getbestMedoidIndexAtCluster(clusters, medoids[i]);
      if (bestNewMedoidIndexInCurrentCluster != medoids[i]) {
        improved = true;
        medoids[i] = bestNewMedoidIndexInCurrentCluster;
      }
    }

    if (!improved) {
      break;  // Exit if no improvement
    }

    assignPointsToMedoids(medoids, clusters);
  }

  std::unordered_map<size_t, std::vector<size_t>> resultMap;
  for (size_t i = 0; i < clusters.size(); ++i) {
    resultMap[clusters[i]].push_back(i);
  }
  std::vector<std::vector<size_t>> result;
  for (auto& [_, points] : resultMap) {
    result.emplace_back(std::move(points));  // Casting to rvalue
  }

  return {result, medoids};
}
