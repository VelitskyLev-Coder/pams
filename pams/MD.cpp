#include <limits>
#include <numeric>
#include <iostream>
#include <cmath>


#include "MD.h"
#include "Pam.h"

std::vector<std::vector<size_t>> createPamClusters(const Matrix& distMatrix, int minK, int maxK)
{
	std::vector<std::vector<size_t>> resultClusters;
	double bestSilhouetteValue = -1.0;

	for (int i = minK; i <= maxK; i++) {
		auto [clusers, medoids] = pam(distMatrix, i);
		double curSilhouetteValue = calcSilhouette(distMatrix, clusers, medoids);
    std::cout << curSilhouetteValue << "\n";
		if (curSilhouetteValue > bestSilhouetteValue) {
			bestSilhouetteValue = curSilhouetteValue;
			resultClusters = clusers;
		}
	}
	return resultClusters;
}

double calcSilhouette(const Matrix& distMatrix, const std::vector<std::vector<size_t>>& clusters, const std::vector<size_t>& medoids)
{
  std::vector<double> silhouetteScores;

  for (size_t clusterIndex = 0; clusterIndex < clusters.size(); ++clusterIndex) {
    for (size_t i : clusters[clusterIndex]) {
      // Calculate a'(i) as the average distance from point i to all other points in its own cluster
      double a_prime = 0.0;
      for (size_t j : clusters[clusterIndex]) {
        if (i != j) {
          a_prime += distMatrix[i][j];
        }
      }
      if (clusters[clusterIndex].size() > 1) {
        a_prime /= (clusters[clusterIndex].size() - 1);
      }

      // Calculate b'(i) as the minimum average distance from point i to points in the nearest different cluster
      double b_prime = std::numeric_limits<double>::max();
      for (size_t otherClusterIndex = 0; otherClusterIndex < clusters.size(); ++otherClusterIndex) {
        if (otherClusterIndex != clusterIndex) {
          double avgDistanceToOtherCluster = 0.0;
          for (size_t j : clusters[otherClusterIndex]) {
            avgDistanceToOtherCluster += distMatrix[i][j];
          }
          avgDistanceToOtherCluster /= clusters[otherClusterIndex].size();
          if (avgDistanceToOtherCluster < b_prime) {
            b_prime = avgDistanceToOtherCluster;
          }
        }
      }

      // Calculate the simplified silhouette score for point i
      double s_prime = (b_prime - a_prime) / std::max(a_prime, b_prime);
      silhouetteScores.push_back(s_prime);
    }
  }

  // Calculate the overall simplified silhouette score
  double overallSilhouetteScore = std::accumulate(silhouetteScores.begin(), silhouetteScores.end(), 0.0) / silhouetteScores.size();
  return overallSilhouetteScore;
}