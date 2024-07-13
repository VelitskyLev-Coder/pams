#include <chrono>
#include <fstream>

#include "MD.h"
#include "Matrix.h"

int main() {
  // std::ifstream file("..\\distance_matrix.csv");
  // std::ifstream file("..\\distance_matrix2.csv");
  std::ifstream file("..\\dist_matrix(10000x10000).txt");
  if (!file.is_open()) {
    std::cerr << "Unable to open file\n";
    return 0;
  }
  Matrix matrix(file);

  // matrix.PrintMat();
  std::vector<std::vector<size_t>> clusters;

  {
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    start = std::chrono::high_resolution_clock::now();
    clusters = createPamClusters(matrix);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "\n\nElapsed time:\t "
              << ((std::chrono::duration<double>)(end - start)).count()
              << " seconds\n";
  }
  std::cout << "clusters = {\n";
  int cluster_id = 0;
  for (auto& vec : clusters) {
    std::cout << "    " << cluster_id << ": [";
    for (size_t i = 0; i < vec.size(); ++i) {
      std::cout << vec[i];
      if (i != vec.size() - 1) std::cout << ", ";
    }
    std::cout << "],\n";
    ++cluster_id;
  }
  std::cout << "}\n";
  std::cout << "Number of Clusters: " << clusters.size() << std::endl;
  return 0;
}
