// clang++ main.cpp -o run functions.cpp matrix.cpp

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
	// challenge7();	// incomplete
	
	test_matrices();
	
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
	// =====================================
	
	// Matrix XOR
	Matrix b("YELLOW SUBMARINE", 8);
	Matrix c = matrix_XOR(b, a);
	c = matrix_XOR(c, a);
	assert(c.str() == b.str());
	
	// test shift and unshift rows
	Matrix d("1234567812345678", 4);
	d = shift_rows(d);
	assert(d.str() == "1234856734126785");
	d = unshift_rows(d);
	assert(d.str() == "1234567812345678");
	
	// Matrix e("1234", 4);
	
	// // https://en.wikipedia.org/wiki/Rijndael_S-box
	// // convert to ASCII
	// string rijndael = "637c777bf26b6fc53001672bfed7ab76ca82c97dfa5947f0add4a2af9ca472c0b7fd9326363ff7cc34a5e5f171d8311504c723c31896059a071280e2eb27b27509832c1a1b6e5aa0523bd6b329e32f8453d100ed20fcb15b6acbbe394a4c58cfd0efaafb434d338545f9027f503c9fa851a3408f929d38f5bcb6da2110fff3d2cd0c13ec5f974417c4a77e3d645d197360814fdc222a908846eeb814de5e0bdbe0323a0a4906245cc2d3ac629195e479e7c8376d8dd54ea96c56f4ea657aae08ba78252e1ca6b4c6e8dd741f4bbd8b8a703eb5664803f60e613557b986c11d9ee1f8981169d98e949b1e87e9ce5528df8ca1890dbfe6426841992d0fb054bb16";
	// Matrix rijndael_Sbox(rijndael, 16);
	// rijndael_Sbox.display();
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
	
	assert(result == desired);							// encode
	assert(fixed_XOR(hex_to_ASCII(result), bb) == a);	// decode
	
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
	vector<string> split_vect = split_file("C7input.txt");
	string input = "";
	for (int i = 0; i < split_vect.size(); i++) { input += split_vect[i]; }
	string key = "YELLOW SUBMARINE";
	string result = AES_decrypt(input, key);
	
	print_(result);
}