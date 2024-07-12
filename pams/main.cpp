#include<fstream>

#include "matrix.h"


int main() {
	std::ifstream file("D:\\CPP\\pams\\distance_matrix.csv");
	if (!file.is_open()) {
		std::cerr << "Unable to open file\n";
		return 0;
	}
	Matrix matrix(file);

	matrix.PrintMat();

	return 0;
}