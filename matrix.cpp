#include <iostream>
#include <assert.h>
#include "matrix.h"

using std::string;

Matrix::Matrix(string input) {
	assert(input.size() == 16);
	// string new_matrix[4][4];
	int index = 0;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			matrix_[row][col] = input[index];
			index++;
		}
	}
}

unsigned char Matrix::pos(int row, int col) { return matrix_[row][col]; }

Matrix matrix_XOR(Matrix a, Matrix b) {
	string result = "";
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col ++) {
			result += a.pos(row, col) ^ b.pos(row, col);
		}
	}
	return Matrix(result);
}