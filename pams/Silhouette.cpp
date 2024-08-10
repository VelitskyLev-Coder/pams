
#include <cmath>
#include <execution>
#include <iostream>
#include <limits>
#include <numeric>

#include "Pam.h"
#include "Silhouette.h"

std::vector<std::vector<size_t>> createPamClusters(const Matrix& distMatrix){
  const int DECREASE_THRESHOLD = 5;
 

  std::vector<PamResult> pamResults();


  std::vector<std::vector<size_t>> resultClusters;
  double bestSilhouetteValue = -1.0;
  double prevSilhouetteValue = std::numeric_limits<double>::lowest();
  int decreaseCount = 0;
  int localMaxK = 1;

  PamBuilder pamBuilder(distMatrix);

  for (int k = 2;; ++k) {
    auto [clusters, medoids] = pamBuilder.pam(k);
    double curSilhouetteValue = calcSilhouette(distMatrix, clusters, medoids);
    std::cout << "Silhouette for k=" << k << ": " << curSilhouetteValue << "\n";

    if (curSilhouetteValue > bestSilhouetteValue) {
      bestSilhouetteValue = curSilhouetteValue;
      resultClusters = clusters;
      localMaxK = k;
    }

    if (curSilhouetteValue < prevSilhouetteValue) {
      decreaseCount++;
    } else {
      decreaseCount = 0;
    }

    if (decreaseCount == DECREASE_THRESHOLD) {
      std::cout << "Stopping at k=" << k << " due to " << DECREASE_THRESHOLD
                << " consecutive decreases in silhouette value.\n";
      break;
    }

    prevSilhouetteValue = curSilhouetteValue;
  }

  std::cout << "Best k found: " << localMaxK
            << " with silhouette: " << bestSilhouetteValue << "\n";
  return resultClusters;
}


// Impelemtation acording to https://en.wikipedia.org/wiki/Silhouette_(clustering)
double calcSilhouette(const Matrix& distMatrix,
                      const std::vector<std::vector<size_t>>& clusters,
                      const std::vector<size_t>& medoids) {
  size_t numPoints = distMatrix.size();
  std::vector<double> silhouetteVals(numPoints, 0.0);

  for (size_t i = 0; i < clusters.size(); ++i) {
    for (size_t j = 0; j < clusters[i].size(); ++j) {
      size_t point = clusters[i][j];

      // Calculate a(i)
      double a = 0.0;
      if (clusters[i].size() > 1) {
        for (size_t k = 0; k < clusters[i].size(); ++k) {
          if (point != clusters[i][k]) {
            a += distMatrix[point][clusters[i][k]];
          }
        }
        a /= (clusters[i].size() - 1);
      }

      // Calculate b(i)
      double b = std::numeric_limits<double>::max();
      for (size_t m = 0; m < clusters.size(); ++m) {
        if (m != i) {
          double avgDist = 0.0;
          for (size_t k = 0; k < clusters[m].size(); ++k) {
            avgDist += distMatrix[point][clusters[m][k]];
          }
          avgDist /= clusters[m].size();
          if (avgDist < b) {
            b = avgDist;
          }
        }
      }

      // Calculate s(i)
      double s = (b - a) / std::max(a, b);
      silhouetteVals[point] = s;
    }
  }

  // Calculate the mean silhouette score
  double silhouetteScore =
      std::reduce(silhouetteVals.begin(), silhouetteVals.end()) / numPoints;

  return silhouetteScore;
}
