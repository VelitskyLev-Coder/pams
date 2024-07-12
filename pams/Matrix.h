#pragma once

#include <vector>
#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <span>

class Matrix
{

public:

	Matrix(std::istream& stream);

	void PrintMat();

	std::span<const double> operator[](size_t row) const;

private:

	std::vector<double> data;
	size_t size;


};

