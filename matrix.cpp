#include <iostream>
#include <vector>
#include <assert.h>
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
			std::cout << std::hex << int(matrix_[row_index][col_index]) << " ";
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

// void Matrix::add_column(string col) {
//     assert(vertical_length_ == col.size());
//     for (int i = 0; i < vertical_length_; i++) {
//         matrix_[i].push_back(col[i]);
//     }
    
// }
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

vector<string> key_expansion(string key) {
    vector<string> result;
    result.push_back(key);  // initial unchanged key
    
    // rounds 1-10
        // for position of i = multiples of 4, ie first of each round key
        // 2 = rotWord(shift down 1) of the previous (w i-1)
        // 2 = subBytes from S-box
        // 1 = w i-4
        // 3 = Rcon(4) (constant)
            // first is [01, 00, 00, 00]
            // second is [02, 00, 00, 00]
            // 04, 08, 10, 20, 40, 80, 1b, 36
        // w i = 1 ^ 2 ^ 3
        
        // else for others, ie rows 2, 3, and 4 of round key
        // wi = w i-1 ^ w i-4
    
    assert(result.size() == 11);
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

Matrix initialize_aes_sbox() {
    unsigned char sbox[256];
	uint8_t p = 1, q = 1;
	
    do {
		p = p ^ (p << 1) ^ (p & 0x80 ? 0x1B : 0);

		q ^= q << 1;
		q ^= q << 2;
		q ^= q << 4;
		q ^= q & 0x80 ? 0x09 : 0;

		uint8_t xformed = q ^ ROTL8(q, 1) ^ ROTL8(q, 2) ^ ROTL8(q, 3) ^ ROTL8(q, 4);

		sbox[p] = xformed ^ 0x63;
	} while (p != 1);

	sbox[0] = 0x63;
    
    string sbox_str = "";
	for (auto i : sbox) { sbox_str += i; }
    return Matrix(sbox_str, 16);
}

Matrix sub_bytes(Matrix initial) {
    // initialize sbox into a matrix
    Matrix sbox = initialize_aes_sbox();
    
    // convert each byte into a vector of the hex
    vector<string> hex_vect;
    for (auto c : initial.str()) {
        string temp_ch = "";
        temp_ch += c;
        hex_vect.push_back(ASCII_to_hex(temp_ch));
    }
    
    // substitute each through the S-box
    string result_str = "";
    for (int i = 0; i < hex_vect.size(); i++) {
        unsigned char row = hex_vect[i][0];
        unsigned char col = hex_vect[i][1];
        
        int row_num = 0;
        if (int(row) >= 48 && int(row) <= 57) { row_num = int(row) - 48; }
        else if (int(row) >= 97 && int(row) <= 102) { row_num = int(row) - 87; }
        
        int col_num = 0;
        if (int(col) >= 48 && int(col) <= 57) { col_num = int(col) - 48; }
        else if (int(col) >= 97 && int(col) <= 102) { col_num = int(col) - 87; }
        
        result_str += sbox.pos(row_num, col_num);
    } 
    
    assert(result_str.size() == 16);
    return Matrix(result_str, 4);
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
    
    // Matrix result(horizontal_length, vertical_length);
	string result_str = "";
	int i = 0;
	int j = 0;
	for (int result_index = 0; result_index < horizontal_length * vertical_length; result_index++) {
		unsigned char current = 0x00;
		for (int k = 0; k < constant; k++) { current ^= a.pos(i, k) * b.pos(k, j); }
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

Matrix get_column(Matrix initial, int col) {
    assert(col < initial.horizontal_length());
    string result = "";
    for (int i = 0; i < initial.vertical_length(); i++) { result += initial.pos(i, col); } 
    return Matrix(result, 1);
}

void gmix_column(unsigned char *r) {
    unsigned char a[4];
    unsigned char b[4];
    unsigned char c;
    unsigned char h;
    /* The array 'a' is simply a copy of the input array 'r'
     * The array 'b' is each element of the array 'a' multiplied by 2
     * in Rijndael's Galois field
     * a[n] ^ b[n] is element n multiplied by 3 in Rijndael's Galois field */ 
    for (c = 0; c < 4; c++) {
        a[c] = r[c];
        /* h is 0xff if the high bit of r[c] is set, 0 otherwise */
        h = (r[c] >> 7) & 1; /* arithmetic right shift, thus shifting in either zeros or ones */
        b[c] = r[c] << 1; /* implicitly removes high bit because b[c] is an 8-bit char, so we xor by 0x1b and not 0x11b in the next line */
        b[c] ^= h * 0x1B; /* Rijndael's Galois field */
    }
    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; /* 2 * a0 + a3 + a2 + 3 * a1 */
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; /* 2 * a1 + a0 + a3 + 3 * a2 */
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; /* 2 * a2 + a1 + a0 + 3 * a3 */
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; /* 2 * a3 + a2 + a1 + 3 * a0 */
}

Matrix mix_columns(Matrix initial) {
    // only works for 4x4 matrix
    assert(initial.horizontal_length() == 4);
    assert(initial.vertical_length() == 4);
    
    string col1 = get_column(initial, 0).str();
    string col2 = get_column(initial, 1).str();
    string col3 = get_column(initial, 2).str();
    string col4 = get_column(initial, 3).str();
    
    unsigned char col1_mixed[4] = {(unsigned char)col1[0], (unsigned char)col1[1], (unsigned char)col1[2], (unsigned char)col1[3]};
    gmix_column(col1_mixed);
    unsigned char col2_mixed[4] = {(unsigned char)col2[0], (unsigned char)col2[1], (unsigned char)col2[2], (unsigned char)col2[3]};
    gmix_column(col2_mixed);
    unsigned char col3_mixed[4] = {(unsigned char)col3[0], (unsigned char)col3[1], (unsigned char)col3[2], (unsigned char)col3[3]};
    gmix_column(col3_mixed);
    unsigned char col4_mixed[4] = {(unsigned char)col4[0], (unsigned char)col4[1], (unsigned char)col4[2], (unsigned char)col4[3]};
    gmix_column(col4_mixed);
    
    string result = "";
    result += col1_mixed[0];
    result += col2_mixed[0];
    result += col3_mixed[0];
    result += col4_mixed[0];
    
    result += col1_mixed[1];
    result += col2_mixed[1];
    result += col3_mixed[1];
    result += col4_mixed[1];
    
    result += col1_mixed[2];
    result += col2_mixed[2];
    result += col3_mixed[2];
    result += col4_mixed[2];
    
    result += col1_mixed[3];
    result += col2_mixed[3];
    result += col3_mixed[3];
    result += col4_mixed[3];
    
    return Matrix(result, 4);
}
