#include <iostream>
#include <assert.h>

using std::string;

class Matrix {
	private:
		unsigned char matrix[4][4];
		
	public:
		Matrix(string input) {
			assert(input.size() == 16);
			// string new_matrix[4][4];
			int index = 0;
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					matrix[row][col] = input[index];
					index++;
				}
			}
		}
		
		unsigned char pos(int row, int col) { return matrix[row][col]; }
		 
};