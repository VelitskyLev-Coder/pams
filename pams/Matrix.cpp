#include "matrix.h"

#include <cmath>
#include <fstream>
#include <sstream>

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

Matrix::Matrix(const std::string& binaryFile) {
  std::ifstream inputFile(binaryFile, std::ios::binary);
  if (!inputFile) {
    throw std::runtime_error("Cannot open file");
  }

  inputFile.read(reinterpret_cast<char*>(&_size), sizeof(_size));
  _data.resize(_size * _size);
  inputFile.read(reinterpret_cast<char*>(_data.data()),
                 _data.size() * sizeof(double));

  inputFile.close();
}

void Matrix::DumpBinary(const std::string& binaryFile) const {
  std::ofstream outputFile(binaryFile, std::ios::binary);
  if (!outputFile) {
    throw std::runtime_error("Cannot open file");
  }

  outputFile.write(reinterpret_cast<const char*>(&_size), sizeof(_size));
  outputFile.write(reinterpret_cast<const char*>(_data.data()),
                   _data.size() * sizeof(double));

  outputFile.close();
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
