#pragma once

#include <iomanip>
#include <iostream>
#include <istream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

class Matrix {
 public:
  Matrix(std::istream& stream);

  void PrintMat();

  std::span<const double> operator[](size_t row) const;
  size_t size() const;

 private:
  std::vector<double> _data;
  size_t _size;
};
