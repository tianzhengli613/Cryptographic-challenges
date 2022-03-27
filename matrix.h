#include <iostream>
#include <vector>

using std::string;
using std::vector;

class Matrix {
	private:
		int horizontal_length_;
		int vertical_length_;
		vector<vector<unsigned char>> matrix_;
		
	public:
		// Initialize a X x Y Matrix by the input and horizontal X length
		// the Y will be automatically be determined
		Matrix(string input, int horizontal_length);
		
		// or instead with unsigned char array for an X x Y matrix
		// by including the size and desired horizontal X length
		Matrix(unsigned char * input, int size, int horizontal_length);
		
		// or you can initialize an empty X x Y Matrix with 
		// horizontal X length and vertical Y length
		Matrix(int horizontal_length, int vertical_length);
		
		void display();
		void display_int();
		void display_hex();
		string str();
		
		unsigned char pos(int vertical, int horizontal);
		void set(unsigned char val, int vertical, int horizontal);
		
		int horizontal_length();
		int vertical_length();
		 
};

Matrix set_column(Matrix initial, Matrix col, int col_num);
Matrix get_column(Matrix initial, int col);

Matrix dot(Matrix a, Matrix b);
Matrix transpose(Matrix initial);
Matrix matrix_XOR(Matrix a, Matrix b);

Matrix rot_word(Matrix initial);
vector<string> key_expansion(string key);

// Step 1: Sub-bytes
Matrix sub_bytes(Matrix initial);
Matrix inverse_sub_bytes(Matrix initial);

// Step 2: Shift rows
Matrix shift_rows(Matrix initial);
Matrix unshift_rows(Matrix initial);

// Step 3: Mix columns
Matrix byte_multiplication(Matrix a, Matrix b);
Matrix mix_columns(Matrix initial);
Matrix inverse_mix_columns(Matrix initial);