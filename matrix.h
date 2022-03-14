#include <iostream>

using std::string;

class Matrix {
	private:
		unsigned char matrix_[4][4];
		
	public:
		Matrix(string input);
		unsigned char pos(int row, int col);
		 
};

Matrix matrix_XOR(Matrix a, Matrix b);