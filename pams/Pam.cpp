#include "Pam.h"

#include <random>
#include <unordered_set>
#include <unordered_map>

std::vector<size_t> initializeMedoids(size_t max_num, size_t k, size_t seed) {
  {
    std::vector<size_t> result;

    std::mt19937 rng(seed);  // Mersenne Twister random number generator
    std::uniform_int_distribution<size_t> dist(0, max_num - 1);

    std::unordered_set<size_t> distinctNumbers;
    while (distinctNumbers.size() < k) {
      size_t number = dist(rng);
      distinctNumbers.insert(number);
    }

    // Copy the distinct numbers from the set to the vector
    result.assign(distinctNumbers.begin(), distinctNumbers.end());
    std::sort(result.begin(), result.end());
    return result;
  }
}

void assignPointsToMedoids(const Matrix& distanceMatrix,
                           const std::vector<size_t>& medoids,
                           std::vector<size_t>& oAssignedPoints) {
  oAssignedPoints.clear();
  for (size_t i = 0; i < distanceMatrix.size(); i++) {
    double minDistance = std::numeric_limits<double>::max();
    size_t bestMedoid = 0;
    for (size_t medoidIndex : medoids) {
      double curDistance = distanceMatrix[i][medoidIndex];
      if (curDistance < minDistance) {
        minDistance = curDistance;
        bestMedoid = medoidIndex;
      }
    }
    oAssignedPoints.push_back(bestMedoid);
  }
}

double computeTotalCost(const Matrix& distanceMatrix,
                        std::vector<size_t>& clusters) {
  double result = 0;
  for (int i = 0; i < clusters.size(); i++) {
    result += distanceMatrix[clusters[i]][i];
  }
  return result;
}

std::vector<std::vector<size_t>> pam(const Matrix& distanceMatrix, int k) {
  std::vector<size_t> medoids = initializeMedoids(distanceMatrix.size(), k);
  std::vector<size_t> clusters(distanceMatrix.size());
  assignPointsToMedoids(distanceMatrix, medoids, clusters);
  double curCost = computeTotalCost(distanceMatrix, clusters);

  while (true) {
    bool improved = false;
    std::vector<size_t> bestMedoids = medoids;
    std::cout << "Iter!"
              << " " << curCost << '\n';
    for (size_t medoidIndex = 0; medoidIndex < medoids.size(); ++medoidIndex) {
      for (size_t pointIndex = 0; pointIndex < distanceMatrix.size();
           ++pointIndex) {
        if (std::find(medoids.begin(), medoids.end(), pointIndex) !=
            medoids.end()) {
          continue;  // Point is already a medoid
        }

        std::vector<size_t> newMedoids = medoids;
        newMedoids[medoidIndex] = pointIndex;  // Try swapping medoids
        std::vector<size_t> newClusters(distanceMatrix.size());
        assignPointsToMedoids(distanceMatrix, newMedoids, newClusters);
        double newCost = computeTotalCost(distanceMatrix, newClusters);

        if (newCost < curCost) {
          curCost = newCost;
          bestMedoids = newMedoids;
          clusters = newClusters;  // Update clusters
          improved = true;
        }
      }
    }

    if (!improved) {
      break;  // Exit if no improvement
    }

    medoids = bestMedoids;  // Update medoids to the best found
  }

  std::unordered_map<size_t, std::vector<size_t>> resultMap;
  for (size_t i = 0; i < clusters.size(); ++i) {
    resultMap[clusters[i]].push_back(i);
  }

  std::vector<std::vector<size_t>> result;
  for (auto& [_, points] : resultMap) {
    result.emplace_back(std::move(points));
  }

  return result;
}
