#include <fstream>

#include "Matrix.h"
#include "Pam.h"

int main() {
  //std::ifstream file("..\\distance_matrix.csv");
  //std::ifstream file("..\\distance_matrix2.csv");
  std::ifstream file("..\\dist_matrix(10000x10000).txt");
  if (!file.is_open()) {
    std::cerr << "Unable to open file\n";
    return 0;
  }
  Matrix matrix(file);

  // matrix.PrintMat();
  auto clusters = pam(matrix, 10);

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

  return 0;
}