#include <iostream>
#include <vector>
#include <assert.h>
#include <sstream>
#include "matrix.h"
#include "functions.h"

using std::string;
using std::vector;

#define ROTL8(x,shift) ((uint8_t) ((x) << (shift)) | ((x) >> (8 - (shift))))

// Initializers

Matrix::Matrix(string input, int horizontal_length) {
	int vertical_length = input.size() / horizontal_length;
	assert(horizontal_length * vertical_length == input.size());
	horizontal_length_ = horizontal_length;
	vertical_length_ = vertical_length;
	
	int index = 0;
	for (int i = 0; i < vertical_length; i++) {
		vector<unsigned char> curr_row;
		for (int j = 0; j < horizontal_length; j++) {
			curr_row.push_back(input[index++]);
		}
		matrix_.push_back(curr_row);
	}
}

Matrix::Matrix(unsigned char * input, int size, int horizontal_length) {
	int vertical_length = size / horizontal_length;
	assert(horizontal_length * vertical_length == size);
	horizontal_length_ = horizontal_length;
	vertical_length_ = vertical_length;
	
	int index = 0;
	for (int i = 0; i < vertical_length; i++) {
		vector<unsigned char> curr_row;
		for (int j = 0; j < horizontal_length; j++) {
			curr_row.push_back(input[index++]);
		}
		matrix_.push_back(curr_row);
	}
}

Matrix::Matrix(int horizontal_length, int vertical_length) {
	horizontal_length_ = horizontal_length;
	vertical_length_ = vertical_length;
	vector<unsigned char> rows(horizontal_length, ' ');
	for (int i = 0; i < vertical_length; i++) { matrix_.push_back(rows); }
}
// ==================================================
// Class functions

void Matrix::display() {
	for (int row_index = 0; row_index < vertical_length_; row_index++) {
		for (int col_index = 0; col_index < horizontal_length_; col_index++) {
			std::cout << matrix_[row_index][col_index] << " ";
		}
		std::cout << "\n";
	}
}

void Matrix::display_int() {
	for (int row_index = 0; row_index < vertical_length_; row_index++) {
		for (int col_index = 0; col_index < horizontal_length_; col_index++) {
			std::cout << int(matrix_[row_index][col_index]) << " ";
		}
		std::cout << "\n";
	}
}

void Matrix::display_hex() {
	for (int row_index = 0; row_index < vertical_length_; row_index++) {
		for (int col_index = 0; col_index < horizontal_length_; col_index++) {
			// convert
			unsigned char ch = matrix_[row_index][col_index];
			std::stringstream stream;
			stream << std::hex << int(ch);
			string temp = stream.str();
		
			// pad with 0
			if (temp.size() < 2) { temp = "0" + temp; }
			std::cout << std::hex << temp << " ";
		}
		std::cout << "\n";
	}
}

string Matrix::str() {
	string result = ""; 
	for (int i = 0; i < vertical_length_; i++) {
		for (int j = 0; j < horizontal_length_; j++) { result += matrix_[i][j]; }
	}
	return result;
}

unsigned char Matrix::pos(int vertical, int horizontal) { return matrix_[vertical][horizontal]; }
void Matrix::set(unsigned char val, int vertical, int horizontal) { matrix_[vertical][horizontal] = val; }

int Matrix::horizontal_length() { return horizontal_length_; }
int Matrix::vertical_length() { return vertical_length_; }

Matrix set_column(Matrix initial, Matrix col, int col_num) {
	assert(initial.vertical_length() == col.vertical_length());
	assert(col.horizontal_length() == 1);
	assert(col_num < initial.horizontal_length());
	
	Matrix result = initial;
	for (int i = 0; i < initial.vertical_length(); i++) {
		result.set(col.pos(i, 0), i, col_num);
	}
	return result;
}

Matrix get_column(Matrix initial, int col) {
	assert(col < initial.horizontal_length());
	string result = "";
	for (int i = 0; i < initial.vertical_length(); i++) { result += initial.pos(i, col); } 
	return Matrix(result, 1);
}

Matrix dot(Matrix a, Matrix b) {
	assert(a.horizontal_length() == b.vertical_length());
	int constant = a.horizontal_length();
	int horizontal_length = b.horizontal_length();
	int vertical_length = a.vertical_length();
	
	string result_str = "";
	int i = 0;
	int j = 0;
	for (int result_index = 0; result_index < horizontal_length * vertical_length; result_index++) {
		unsigned char current = 0x00;
		for (int k = 0; k < constant; k++) { current += a.pos(i, k) * b.pos(k, j); }
		result_str += current;
		if (++j >= horizontal_length) {
			i++;
			j = 0;
		}
	}
	
	assert(result_str.size() % horizontal_length == 0);
	return Matrix(result_str, horizontal_length);
}

Matrix transpose(Matrix initial) {
	Matrix result(initial.vertical_length(), initial.horizontal_length());
	for (int i = 0; i < initial.vertical_length(); i++) {
		for (int j = 0; j < initial.horizontal_length(); j++) {
			result.set(initial.pos(i, j), j, i);
		}
	}
	return result;
}

Matrix matrix_XOR(Matrix a, Matrix b) {
	assert(a.horizontal_length() == b.horizontal_length());
	assert(a.vertical_length() == b.vertical_length());
	int horizontal_length = a.horizontal_length();
	int vertical_length = a.vertical_length();
	
	string result = "";
	for (int i = 0; i < vertical_length; i++) {
		for (int j = 0; j < horizontal_length; j++) {
			result += a.pos(i, j) ^ b.pos(i, j);
		}
	}
	return Matrix(result, horizontal_length);
}

Matrix rot_word(Matrix initial) {
	// 1x4 matrix
	assert(initial.horizontal_length() == 1);
	assert(initial.vertical_length() == 4);
	string result = "";
	result += initial.pos(1, 0);
	result += initial.pos(2, 0);
	result += initial.pos(3, 0);
	result += initial.pos(0, 0);
	return Matrix(result, 1);
}

vector<string> key_expansion(string key) {
	assert(key.size() == 16);
	vector<string> result;
	result.push_back(key);  // initial unchanged key
	
	// initialize rcon
	unsigned char rcon0[4] = {0x01, 0x00, 0x00, 0x00};
	unsigned char rcon1[4] = {0x02, 0x00, 0x00, 0x00};
	unsigned char rcon2[4] = {0x04, 0x00, 0x00, 0x00};
	unsigned char rcon3[4] = {0x08, 0x00, 0x00, 0x00};
	unsigned char rcon4[4] = {0x10, 0x00, 0x00, 0x00};
	unsigned char rcon5[4] = {0x20, 0x00, 0x00, 0x00};
	unsigned char rcon6[4] = {0x40, 0x00, 0x00, 0x00};
	unsigned char rcon7[4] = {0x80, 0x00, 0x00, 0x00};
	unsigned char rcon8[4] = {0x1b, 0x00, 0x00, 0x00};
	unsigned char rcon9[4] = {0x36, 0x00, 0x00, 0x00};
	vector<Matrix> rcon {
		Matrix(rcon0, 4, 1), Matrix(rcon1, 4, 1), Matrix(rcon2, 4, 1), Matrix(rcon3, 4, 1),
		Matrix(rcon4, 4, 1), Matrix(rcon5, 4, 1), Matrix(rcon6, 4, 1), Matrix(rcon7, 4, 1),
		Matrix(rcon8, 4, 1), Matrix(rcon9, 4, 1)
	};
	
	// rounds 1-10
	Matrix prev_key(key, 4);
	for (int round = 0; round < 10; round++) {
		Matrix current_key(4, 4);
		
		// first word of 4 in byte of currount round
		Matrix i_minus_4_word = get_column(prev_key, 0);	// i - 4 word
		Matrix prev_word = get_column(prev_key, 3);			// i - 1 word
		prev_word = rot_word(prev_word);					// rotate last word of the previous key
		prev_word = sub_bytes(prev_word);					// sub-bytes
		
		Matrix first_word = matrix_XOR(i_minus_4_word, prev_word);
		first_word = matrix_XOR(first_word, rcon[round]);
		current_key = set_column(current_key, first_word, 0);
		prev_word = first_word;
		
		// words 2, 3, and 4
		for(int i = 1; i <= 3; i++) {
			i_minus_4_word = get_column(prev_key, i);
			Matrix current_word = matrix_XOR(i_minus_4_word, prev_word);
			current_key = set_column(current_key, current_word, i);
			prev_word = current_word;
		}
		assert((current_key.str()).size() == 16);
		result.push_back(current_key.str());
		prev_key = current_key;
	}
	
	assert(result.size() == 11);
	return result;
}

// S-box table
unsigned char sbox[256] = {
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// inverse S-box table
unsigned char inverse_sbox[256] = {
	0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

Matrix sub_bytes(Matrix initial) {
	// convert each byte into a vector of the hex
	vector<string> hex_vect;
	for (auto & c : initial.str()) {
		string temp_ch = "";
		temp_ch += c;
		hex_vect.push_back(ASCII_to_hex(temp_ch));
	}
	
	// substitute each through the S-box
	string result_str = "";
	for (int i = 0; i < hex_vect.size(); i++) {
		unsigned char vertical = hex_vect[i][0];
		unsigned char horizontal = hex_vect[i][1];
		
		int vertical_index = 0;
		if (int(vertical) >= 48 && int(vertical) <= 57) { vertical_index = int(vertical) - 48; }
		else if (int(vertical) >= 97 && int(vertical) <= 102) { vertical_index = int(vertical) - 87; }
		
		int horizontal_index = 0;
		if (int(horizontal) >= 48 && int(horizontal) <= 57) { horizontal_index = int(horizontal) - 48; }
		else if (int(horizontal) >= 97 && int(horizontal) <= 102) { horizontal_index = int(horizontal) - 87; }
		
		int index = vertical_index * 16 + horizontal_index;
		result_str += sbox[index];
	} 
	
	assert(result_str.size() == (initial.str()).size());
	return Matrix(result_str, initial.horizontal_length());
}


Matrix inverse_sub_bytes(Matrix initial) {
	// convert each byte into a vector of the hex
	vector<string> hex_vect;
	for (auto & c : initial.str()) {
		string temp_ch = "";
		temp_ch += c;
		hex_vect.push_back(ASCII_to_hex(temp_ch));
	}
	
	// substitute each through the S-box
	string result_str = "";
	for (int i = 0; i < hex_vect.size(); i++) {
		unsigned char vertical = hex_vect[i][0];
		unsigned char horizontal = hex_vect[i][1];
		
		int vertical_index = 0;
		if (int(vertical) >= 48 && int(vertical) <= 57) { vertical_index = int(vertical) - 48; }
		else if (int(vertical) >= 97 && int(vertical) <= 102) { vertical_index = int(vertical) - 87; }
		
		int horizontal_index = 0;
		if (int(horizontal) >= 48 && int(horizontal) <= 57) { horizontal_index = int(horizontal) - 48; }
		else if (int(horizontal) >= 97 && int(horizontal) <= 102) { horizontal_index = int(horizontal) - 87; }
		
		int index = vertical_index * 16 + horizontal_index;
		result_str += inverse_sbox[index];
	} 
	
	assert(result_str.size() == (initial.str()).size());
	return Matrix(result_str, initial.horizontal_length());
}

Matrix shift_rows(Matrix initial) {
	// only works for 4x4 matrix
	assert(initial.horizontal_length() == 4);
	assert(initial.vertical_length() == 4);
	Matrix result(4, 4);
	
	// first row is unchanged
	result.set(initial.pos(0, 0), 0, 0);
	result.set(initial.pos(0, 1), 0, 1);
	result.set(initial.pos(0, 2), 0, 2);
	result.set(initial.pos(0, 3), 0, 3);
	
	// second row rotated left once
	result.set(initial.pos(1, 1), 1, 0);
	result.set(initial.pos(1, 2), 1, 1);
	result.set(initial.pos(1, 3), 1, 2);
	result.set(initial.pos(1, 0), 1, 3);
	
	// third row rotated left twice
	result.set(initial.pos(2, 2), 2, 0);
	result.set(initial.pos(2, 3), 2, 1);
	result.set(initial.pos(2, 0), 2, 2);
	result.set(initial.pos(2, 1), 2, 3);
	
	// fourth row rotated left thrice
	result.set(initial.pos(3, 3), 3, 0);
	result.set(initial.pos(3, 0), 3, 1);
	result.set(initial.pos(3, 1), 3, 2);
	result.set(initial.pos(3, 2), 3, 3);
	
	return result;
}

Matrix unshift_rows(Matrix initial) {
	// only works for 4x4 matrix
	assert(initial.horizontal_length() == 4);
	assert(initial.vertical_length() == 4);
	Matrix result(4, 4);
	
	// first row is unchanged
	result.set(initial.pos(0, 0), 0, 0);
	result.set(initial.pos(0, 1), 0, 1);
	result.set(initial.pos(0, 2), 0, 2);
	result.set(initial.pos(0, 3), 0, 3);
	
	// second row rotated right once
	result.set(initial.pos(1, 3), 1, 0);
	result.set(initial.pos(1, 0), 1, 1);
	result.set(initial.pos(1, 1), 1, 2);
	result.set(initial.pos(1, 2), 1, 3);
	
	// third row rotated right twice
	result.set(initial.pos(2, 2), 2, 0);
	result.set(initial.pos(2, 3), 2, 1);
	result.set(initial.pos(2, 0), 2, 2);
	result.set(initial.pos(2, 1), 2, 3);
	
	// fourth row rotated right thrice
	result.set(initial.pos(3, 1), 3, 0);
	result.set(initial.pos(3, 2), 3, 1);
	result.set(initial.pos(3, 3), 3, 2);
	result.set(initial.pos(3, 0), 3, 3);
	
	return result;
}

Matrix byte_multiplication(Matrix a, Matrix b) {
	assert(a.horizontal_length() == b.vertical_length());
	int constant = a.horizontal_length();
	int horizontal_length = b.horizontal_length();
	int vertical_length = a.vertical_length();
	
	string result_str = "";
	int i = 0;
	int j = 0;
	for (int result_index = 0; result_index < horizontal_length * vertical_length; result_index++) {
		unsigned char current = 0x00;
		unsigned char bin = 0b10000000;
		for (int k = 0; k < constant; k++) {
			if (a.pos(i, k) == 0x02) {
				unsigned char temp = b.pos(k, j) << 1;
				if ((b.pos(k, j) | bin) == b.pos(k, j)) { temp ^= 0x1b; }	// if leftmost bit is 1
				current ^= temp;
			}
			if (a.pos(i, k) == 0x03) {
				unsigned char temp = b.pos(k, j) << 1;
				if ((b.pos(k, j) | bin) == b.pos(k, j)) { temp ^= 0x1b; }	// if leftmost bit is 1
				temp ^= b.pos(k, j);
				current ^= temp;
			}
			if (a.pos(i, k) == 0x01) { current ^= b.pos(k, j); }
		}
		result_str += current;
		if (++j >= horizontal_length) {
			i++;
			j = 0;
		}
	}
	
	assert(result_str.size() % horizontal_length == 0);
	return Matrix(result_str, horizontal_length);
}

void gmix_column(unsigned char *r) {
	unsigned char a[4];
	unsigned char b[4];
	unsigned char c;
	unsigned char h;
	for (c = 0; c < 4; c++) {
		a[c] = r[c];
		h = (r[c] >> 7) & 1;
		b[c] = r[c] << 1; 
		b[c] ^= h * 0x1B;
	}
	r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
	r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
	r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; 
	r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; 
}

// revised
Matrix mix_columns(Matrix initial) {
	// only works for 4x4 matrix
	assert(initial.horizontal_length() == 4);
	assert(initial.vertical_length() == 4);
	
	unsigned char constants_ch[16] = {
		0x02, 0x03, 0x01, 0x01, 
		0x01, 0x02, 0x03, 0x01,
		0x01, 0x01, 0x02, 0x03,
		0x03, 0x01, 0x01, 0x02
	};
	Matrix constants(constants_ch, 16, 4);
	
	return byte_multiplication(constants, initial);
}

// Matrix inverse_mix_columns(Matrix initial) {
// 	// only works for 4x4 matrix
// 	assert(initial.horizontal_length() == 4);
// 	assert(initial.vertical_length() == 4);
// }