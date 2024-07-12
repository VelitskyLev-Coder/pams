
#include <cmath>

#include "matrix.h"


Matrix::Matrix(std::istream& stream)
{
  std::string line;
  while (std::getline(stream, line)) {
    std::stringstream lineStream(line);
    std::string value;
    while (std::getline(lineStream, value, ',')) {
      data.push_back(std::stod(value));
    }
  }
  size = std::sqrt(data.size());
}

void Matrix::PrintMat()
{
  for (size_t i = 0; i < size; ++i) {
    for (size_t j = 0; j < size; j++) {
      std::cout << std::fixed << std::setprecision(2) << (*this)[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

std::span<const double> Matrix::operator[](size_t row) const
{
  return std::span<const double>(&data[row * size], size);
}

