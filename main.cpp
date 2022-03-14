// clang++ main.cpp -o run functions.cpp

#include <iostream>
#include <assert.h>
#include "functions.h"

using std::string;

void challenge1();
void challenge2();
void challenge3();
void challenge4();
void challenge5();
void challenge6();
void challenge7();

int main() {
	challenge1();
	challenge2();
	challenge3();
	challenge4();
	challenge5();
	challenge6();
	challenge7();	// incomplete
	
	std::cout << "\n============================\n";
	std::cout << "All assertions passed.\n";
	
	return 0;
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
	string key = "YELLOW SUBMARINE";
}