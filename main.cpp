// clang++ main.cpp -o run functions.cpp matrix.cpp && ./run

#include <iostream>
#include <assert.h>
#include "functions.h"
#include "matrix.h"

using std::string;

void challenge1();
void challenge2();
void challenge3();
void challenge4();
void challenge5();
void challenge6();
void challenge7();

void test_matrices();

int main() {
	// challenge1();
	// challenge2();
	// challenge3();
	// challenge4();
	// challenge5();
	// challenge6();
	challenge7();	// incomplete
	
	// test_matrices();
	
	std::cout << "\n============================\n";
	std::cout << "All assertions passed.\n";
	
	return 0;
}

void test_matrices() {
	// http://etutorials.org/Networking/802.11+security.+wi-fi+protected+access+and+802.11i/Appendixes/Appendix+A.+Overview+of+the+AES+Block+Cipher/Steps+in+the+AES+Encryption+Process/#:~:text=AES%20defines%20a%20table%20of,value%20from%20the%20substitution%20table.
	// https://www.simplilearn.com/tutorials/cryptography-tutorial/aes-encryption
	
	// 1234567812345678
	// thisisakeyofkeys
	// YELLOW SUBMARINE
	
	// test all Matrix class functions
	Matrix empty(3, 4);
	assert(empty.horizontal_length() == 3);
	assert(empty.vertical_length() == 4);
	assert(empty.str() == "            ");
	assert((empty.str()).size() == 12);
	
	Matrix a("thisisakeyofkeys", 8);
	assert(a.str() == "thisisakeyofkeys");
	assert(a.pos(1, 2) == 'o');
	a.set('K', 0, 7);
	a.set('O', 1, 2);
	assert(a.str() == "thisisaKeyOfkeys");
	
	// test matrix set column
	Matrix set_col_result("o1l2d3c4", 2);
	Matrix new_column("NEWC", 1);
	set_col_result = set_column(set_col_result, new_column, 0);
	string set_col_expected = "N1E2W3C4";
	assert(set_col_result.str() == set_col_expected);
	// =====================================
	
	// Matrix XOR
	Matrix b("YELLOW SUBMARINE", 8);
	Matrix c = matrix_XOR(b, a);
	c = matrix_XOR(c, a);
	assert(c.str() == b.str());
	
	// test matrix dot
	string e1_str = "";
	unsigned char c1 = 1;
	unsigned char c2 = 2;
	unsigned char c3 = 3;
	unsigned char c4 = 4;
	unsigned char c5 = 5;
	unsigned char c6 = 6;
	e1_str += c1;
	e1_str += c2;
	e1_str += c3;
	e1_str += c4;
	e1_str += c5;
	e1_str += c6;
	Matrix e1(e1_str, 3);
	
	string e2_str = "";
	c1 = 7;
	c2 = 8;
	c3 = 9;
	c4 = 10;
	c5 = 11;
	c6 = 12;
	e2_str += c1;
	e2_str += c2;
	e2_str += c3;
	e2_str += c4;
	e2_str += c5;
	e2_str += c6;
	Matrix e2(e2_str, 2);
	
	Matrix e3 = dot(e1, e2);
	
	string e3_expected_str = "";
	c1 = 58;
	c2 = 64;
	c3 = 139;
	c4 = 154;
	e3_expected_str += c1;
	e3_expected_str += c2;
	e3_expected_str += c3;
	e3_expected_str += c4;
	Matrix e3_expected(e3_expected_str, 2);
	
	assert(e3.str() == e3_expected.str());
	
	string f1_str = e1_str + e2_str;
	c1 = 13;
	c2 = 14;
	c3 = 15;
	c4 = 16;
	f1_str += c1;
	f1_str += c2;
	f1_str += c3;
	f1_str += c4;
	
	Matrix f1(f1_str, 4);
	Matrix f2(e1_str.substr(0, 4), 1);
	Matrix f3 = dot(f1, f2);
	
	string f3_str = "";
	c1 = 30;
	c2 = 70;
	c3 = 110;
	c4 = 150;
	f3_str += c1;
	f3_str += c2;
	f3_str += c3;
	f3_str += c4;
	Matrix f3_expected(f3_str, 1);
	assert(f3.str() == f3_expected.str());
	
	// test matrix transpose
	Matrix pre_transpose("123456", 3);
	Matrix post_transpose = transpose(pre_transpose);
	assert(post_transpose.str() == "142536");
	assert(post_transpose.vertical_length() == pre_transpose.horizontal_length());
	assert(post_transpose.horizontal_length() == pre_transpose.vertical_length());
}

// =======================================
// Convert hex to base64
void challenge1() {
	string input = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	string result = ASCII_to_base64(hex_to_ASCII(input));
	string desired = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	
	assert(result == desired);
	
	string test1 = "light work.";
	string test2 = "light work";
	string test3 = "light wor";
	string test4 = "light wo";
	string test5 = "light w";
	string expected1 = "bGlnaHQgd29yay4=";
	string expected2 = "bGlnaHQgd29yaw==";
	string expected3 = "bGlnaHQgd29y";
	string expected4 = "bGlnaHQgd28=";
	string expected5 = "bGlnaHQgdw==";
	
	assert(ASCII_to_base64(test1) == expected1);
	assert(ASCII_to_base64(test2) == expected2);
	assert(ASCII_to_base64(test3) == expected3);
	assert(ASCII_to_base64(test4) == expected4);
	assert(ASCII_to_base64(test5) == expected5);
	
	assert(base64_to_ASCII(expected1) == test1);
	assert(base64_to_ASCII(expected2) == test2);
	assert(base64_to_ASCII(expected3) == test3);
	assert(base64_to_ASCII(expected4) == test4);
	assert(base64_to_ASCII(expected5) == test5);
	
	std::cout << "Challenge 1: " << result << "\n";
}

// Fixed XOR
void challenge2() {
	string a = "1c0111001f010100061a024b53535009181c";
	string b = "686974207468652062756c6c277320657965";
	string aa = hex_to_ASCII(a);
	string bb = hex_to_ASCII(b);
	string result = fixed_XOR(aa, bb);
	string desired = "746865206b696420646f6e277420706c6179";
	
	assert(ASCII_to_hex(result) == desired);	// encode
	assert(fixed_XOR(result, bb) == a);			// decode
	
	std::cout << "Challenge 2: " << result << "\n";
}

// Single-byte XOR cipher
void challenge3() {
	string input = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	string result = single_byte_XOR(hex_to_ASCII(input));
	string desired = "Cooking MC's like a pound of bacon";
	
	assert(result == desired);
	
	std::cout << "Challenge 3: " << result << "\n";
}

// Detect single character XOR
void challenge4() {
	vector<string> split_vect = split_file("C4input.txt");
	string result = detect_single_byte_XOR(split_vect);
	string desired = "Now that the party is jumping\n";
	
	assert(result == desired);
	
	std::cout << "Challenge 4: " << result;;
}

// Repeating key XOR
void challenge5() {
	string text = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	string key = "ICE";
	string result = repeating_key_XOR(text, key);
	string desired = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
	
	assert(result == desired);													// encode
	assert(text == hex_to_ASCII(repeating_key_XOR(hex_to_ASCII(result), key)));	// decode
	
	std::cout << "Challenge 5: " << result << "\n\n";
}

// Break repeating key XOR
void challenge6() {
	string a = "this is a test";
	string b = "wokka wokka!!!";
	assert(hamming_distance(a, b) == 37);
	
	vector<string> split_vect = split_file("C6input.txt");
	string input = "";
	for (int i = 0; i < split_vect.size(); i++) { input += split_vect[i]; }
	
	input = base64_to_ASCII(input);
	string result = break_repeating_key_XOR(input);
	string desired = "Terminator X: Bring the noise";
	assert(result == desired);
	
	std::cout << "Challenge 6: " << result << "\n\n";
	string deciphered = hex_to_ASCII(repeating_key_XOR(input, result));
	print_(deciphered);
}

// AES-128 in ECB mode
void challenge7() {
	// vector<string> split_vect = split_file("C7input.txt");
	// string input = "";
	// for (int i = 0; i < split_vect.size(); i++) { input += split_vect[i]; }
	// string key = "YELLOW SUBMARINE";
	// string result = AES_decrypt(input, key);
	
	// print_(result);
	
	Matrix test_key("Thats my Kung Fu", 4);
	
	// test key expansion
	Matrix test_key_expansion = get_column(test_key, 0);
	test_key_expansion = rot_word(test_key_expansion);
	Matrix rot_word_expected(hex_to_ASCII("73206754"), 1);
	assert(rot_word_expected.str() == test_key_expansion.str());
	Matrix sub_bytes_result = sub_bytes(Matrix(hex_to_ASCII("cf4f3c09"), 1));
	string sub_bytes_expected = hex_to_ASCII("8a84eb01");
	assert(sub_bytes_result.str() == sub_bytes_expected);
	
	// test sub-bytes and inverse sub-bytes
	string pre_sub = "19a09ae93df4c6f8e3e28d48be2b2a08";
	sub_bytes_result = sub_bytes(Matrix(hex_to_ASCII(pre_sub), 4));
	sub_bytes_expected = "d4e0b81e27bfb44111985d52aef1e530";
	assert(ASCII_to_hex(sub_bytes_result.str()) == sub_bytes_expected);
	Matrix inverse_sub_result = inverse_sub_bytes(sub_bytes_result);
	assert(inverse_sub_result.str() == hex_to_ASCII(pre_sub));
	
	sub_bytes_result = sub_bytes(test_key);
	sub_bytes_expected = "2045ef928fb73cb6b7b39d9f85b75a9d";
	assert(ASCII_to_hex(sub_bytes_result.str()) == sub_bytes_expected);
	
	// test shift and unshift rows
	Matrix shift_rows_result = shift_rows(sub_bytes_result);
	string shift_rows_expected = "2045ef92b73cb68f9d9fb7b39d85b75a";
	assert(ASCII_to_hex(shift_rows_result.str()) == shift_rows_expected);
	Matrix unshift_rows_result = unshift_rows(shift_rows_result);
	assert(ASCII_to_hex(unshift_rows_result.str()) == ASCII_to_hex(sub_bytes_result.str()));
	
	string pre_shift = hex_to_ASCII("d4e0b81e27bfb44111985d52aef1e530");
	shift_rows_result = shift_rows(Matrix(pre_shift, 4));
	shift_rows_expected = "d4e0b81ebfb441275d52119830aef1e5";
	assert(ASCII_to_hex(shift_rows_result.str()) == shift_rows_expected);
	unshift_rows_result = unshift_rows(shift_rows_result);
	assert(unshift_rows_result.str() == pre_shift);
	
	// test mix columns
	string mix_column_input = hex_to_ASCII("dbf201c6130a01c6532201c6455c01c6");
	Matrix mix_column_result(mix_column_input, 4);
	mix_column_result = mix_columns(mix_column_result);
	string mix_column_expected = "8e9f01c64ddc01c6a15801c6bc9d01c6";
	assert(ASCII_to_hex(mix_column_result.str()) == mix_column_expected);
	
	mix_column_input = hex_to_ASCII("d4e0b81ebfb441275d52119830aef1e5");
	mix_column_result = Matrix(mix_column_input, 4);
	mix_column_result = mix_columns(mix_column_result);
	mix_column_expected = "04e0482866cbf8068119d326e59a7a4c";
	assert(ASCII_to_hex(mix_column_result.str()) == mix_column_expected);
	
	// test matrix XOR
	test_key = Matrix(hex_to_ASCII("a088232afa54a36cfe2c397617b13905"), 4);
	Matrix XOR_result = matrix_XOR(mix_column_result, test_key);
	Matrix XOR_expected(hex_to_ASCII("a4686b029c9f5b6a7f35ea50f22b4349"), 4);
	assert(XOR_result.str() == XOR_expected.str());
	
	// test AES
	string plaintext = "Two One Nine Two";
	string key = "Thats my Kung Fu";
	string AES_result = AES_encrypt(plaintext, key);
	// string AES_expected_hex = "29c3505f571420f6402299b31a02d73ab3e46f11ba8d2b97c18769449a89e868";
	string AES_expected_hex = "29c3505f571420f6402299b31a02d73a";
	assert(ASCII_to_hex(AES_result) == AES_expected_hex);
	
	print_(ASCII_to_hex(AES_result));
}