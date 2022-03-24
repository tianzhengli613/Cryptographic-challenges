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
		// Initialize a X x Y Matrix by the input and X length
		// the Y will be automatically be determined
		Matrix(string input, int horizontal_length);
		
		// Or you can initialize an empty X x Y Matrix with X length and Y length
		Matrix(int horizontal_length, int vertical_length);
		
		void display();
		void display_int();
		string str();
		
		unsigned char pos(int vertical, int horizontal);
		void set(unsigned char val, int vertical, int horizontal);
		
		int horizontal_length();
		int vertical_length();
		 
};

Matrix dot(Matrix a, Matrix b);

Matrix matrix_XOR(Matrix a, Matrix b);

// Step 1: Sub-bytes
Matrix initialize_aes_sbox();			// modified from code provided by Wikipedia: 
										// https://en.wikipedia.org/wiki/Rijndael_S-box
Matrix matrix_sub_bytes(Matrix initial);

// Step 2: Shift rows
Matrix shift_rows(Matrix initial);
Matrix unshift_rows(Matrix initial);

Matrix mix_columns(Matrix initial);