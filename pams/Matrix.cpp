
#include "matrix.h"

#include <cmath>

Matrix::Matrix(std::istream& stream) {
  const size_t buffer_size = 1024 * 1024 * 10;
  char* buffer = new char[buffer_size];
  stream.rdbuf()->pubsetbuf(buffer, buffer_size);

  std::string line;
  while (std::getline(stream, line)) {
    std::stringstream lineStream(line);
    std::string value;
    while (std::getline(lineStream, value, ',')) {
      _data.push_back(std::stod(value));
    }
  }

  // Clean up the buffer
  delete[] buffer;

  // Calculate the matrix size only once
  _size = std::sqrt(_data.size());
}

void Matrix::PrintMat() {
  for (size_t i = 0; i < _size; ++i) {
    for (size_t j = 0; j < _size; j++) {
      std::cout << std::fixed << std::setprecision(2) << (*this)[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << std::endl;
}

std::span<const double> Matrix::operator[](size_t row) const {
  return std::span<const double>(&_data[row * _size], _size);
}

size_t Matrix::size() const { return _size; }
