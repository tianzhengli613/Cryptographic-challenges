#include <iostream>
#include <vector>
#include <assert.h>
#include "matrix.h"

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
// ================================================================
// Matrix functions

Matrix dot(Matrix a, Matrix b) {
    assert(a.horizontal_length() == b.vertical_length());
	int constant = a.horizontal_length();
    int horizontal_length = b.horizontal_length();
    int vertical_length = a.vertical_length();
    
    // Matrix result(horizontal_length, vertical_length);
	string result_str = "";
	int i = 0;
	int j = 0;
	for (int result_index = 0; result_index < horizontal_length * vertical_length; result_index++) {
		unsigned char current = 0x00;
		for (int k = 0; k < constant; k++) { current += a.pos(i, k) * b.pos(k, j); }
		result_str += current;
		j++;
		if (j >= horizontal_length) {
			i++;
			j = 0;
		}
	}
    
	assert(result_str.size() % horizontal_length == 0);
    return Matrix(result_str, horizontal_length);
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

Matrix initialize_aes_sbox() {
    unsigned char sbox[256];
	uint8_t p = 1, q = 1;
	
	/* loop invariant: p * q == 1 in the Galois field */
	do {
		/* multiply p by 3 */
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		/* divide q by 3 (equals multiplication by 0xf6) */
		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		/* compute the affine transformation */
		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	/* 0 is a special case since it has no inverse */
	sbox[0] = 0x63;
    
    string sbox_str = "";
	for (auto i : sbox) { sbox_str += i; }
    return Matrix(sbox_str, 16);
}

Matrix matrix_sub_bytes(Matrix initial) {
    // initialize sbox into a matrix
    Matrix sbox = initialize_aes_sbox();
    
    // convert each byte into a vector of the hex
    
    // substitute each through the S-box
    return sbox;
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

// Matrix mix_columns(Matrix initial) {
//     // only works for 4x4 matrix
//     assert(initial.x() == 4);
//     assert(initial.y() == 4);
    
//     int dim = 4;
//     Matrix result(dim, dim);
    
    
    
//     return result;
// }
