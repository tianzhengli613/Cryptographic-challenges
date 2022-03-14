#include <iostream>
#include <sstream>
#include <assert.h>
#include <bitset>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include "functions.h"
#include "matrix.h"

using std::string;
using std::vector;

void print_(string input) { std::cout << input << "\n"; }
void print_(int input) { std::cout << input << "\n"; }

// Frequency scores of alphabetic characters and space
static std::map<char, double> english_freq = { 
	{'a', 0.0651738}, {'b', 0.0124248}, {'c', 0.0217339}, {'d', 0.0349835}, {'e', 0.1041442}, 
    {'f', 0.0197881}, {'g', 0.0158610}, {'h', 0.0492888}, {'i', 0.0558094}, {'j', 0.0009033}, 
    {'k', 0.0050529}, {'l', 0.0331490}, {'m', 0.0202124}, {'n', 0.0564513}, {'o', 0.0596302}, 
    {'p', 0.0137645}, {'q', 0.0008606}, {'r', 0.0497563}, {'s', 0.0515760}, {'t', 0.0729357}, 
	{'u', 0.0225134}, {'v', 0.0082903}, {'w', 0.0171272}, {'x', 0.0013692}, {'y', 0.0145984},
    {'z', 0.0007836}, {' ', 0.1918182}
    };
					
double freq_score(string text) {
	double score = 0;
	for (auto & c : text) {
			unsigned char temp = c;
			if (isalpha(temp)) { temp = tolower(temp); }
			score += english_freq[temp];
		}
	return score;
}

vector<string> split_file(string filename) {
	vector<string> result;
	
	string line;
	std::ifstream myfile(filename);
	// Read line by line
	if (myfile.is_open()) {
		while(getline(myfile, line)) { result.push_back(line); }
		myfile.close();
	}
	return result;
}

string base64_to_ASCII(string base64) {
	// base64 table (indices 0 - 63)
	string char_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
	
	string result = "";
	int buffer_count = 0;
	for (int i = 0; i < base64.size(); i += 4) {
		unsigned int block = char_table.find(base64[i]);
		for (int j = i + 1; j < i + 4; j++) {
			block <<= 6;
			// is padding, ie. '='
			if (char_table.find(base64[j]) != 64) { block |= char_table.find(base64[j]); }
			else { buffer_count++; }
		}
		
		unsigned char third = block;
		block >>= 8;
		unsigned char sec = block;
		block >>= 8;
		unsigned char first = block;
		
		result += first;
		if (buffer_count <= 1) { result += sec; }
		if (buffer_count == 0) { result += third; }
	}
	return result;
}

string ASCII_to_base64(string input) {
	// base64 table (indices 0 - 63)
	string char_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	
	int buffer_size = input.size() % 3;
	int size = input.size() - buffer_size;
	string result = "";
	for (int i = 0; i < size; i += 3) {
		// 24 bit block to be separated into 4 base64 characters
		unsigned int block = input[i] << 8;
		block |= input[i + 1];
		block <<= 8;
		block |= input[i + 2];
		
		// split
		unsigned char first = block >> 18;
		unsigned char sec = block >> 12;
		unsigned char third = block >> 6;
		unsigned char fourth = block;
		// turn off the 2 leftmost bits to become 6 bits
		sec &= 0b00111111;
		third &= 0b00111111;
		fourth &= 0b00111111;
		result += char_table[int(first)];
		result += char_table[int(sec)];
		result += char_table[int(third)];
		result += char_table[int(fourth)];
	}
	
	// buffer padding, either 1 or 2
	if (buffer_size) {
		unsigned int block = input[size];
		if (buffer_size == 1) {
			// 8 bit block + 4 for padding
			unsigned char first = block >> 2;
			unsigned char sec = block << 4;
			// turn off the 2 leftmost bits to become 6 bits
			sec &= 0b00111111;
			
			result += char_table[int(first)];
			result += char_table[int(sec)];
			result += "==";
		}
		else if (buffer_size == 2) {
			block <<= 8;
			block |= input[size + 1];
			// 16 bit block + 2 for padding + '='
			unsigned char first = block >> 10;
			unsigned char sec = block >> 4;
			unsigned char third = block << 2;
			// turn off the 2 leftmost bits to become 6 bits
			sec &= 0b00111111;
			third &= 0b00111111; 
			
			result += char_table[int(first)];
			result += char_table[int(sec)];
			result += char_table[int(third)];
			result += '=';
		}
	}
	return result;
}

string hex_to_ASCII(string hex) {
	string result = "";
	for (int i = 0; i < hex.size(); i += 2) {
		// split to hex substrings of 2
		string temp = hex.substr(i, 2);
		
		// convert
		unsigned char ch = stoul(temp, nullptr, 16);
		result += ch;
	}
	return result;
}

string ASCII_to_hex(string input) {
	string result = "";
	for (auto & c : input) {
		// convert
		int x = c;
		std::stringstream stream;
		stream << std::hex << x;
		string temp = stream.str();
		
		// pad with 0
		if (temp.size() < 2) { temp = "0" + temp; }
		result += temp;
	}
	return result;
}

string fixed_XOR(string a, string b) {
	assert(a.size() == b.size());
	
	string result = "";
	for (int i = 0; i < a.size(); i++) {
		// XOR and convert
		int xored = a[i] ^ b[i];
		std::stringstream stream;
		stream << std::hex << xored;
		string temp = stream.str();
		
		// pad with 0
		if (temp.size() < 2) { temp = "0" + temp; }
		result += temp;
	}
	return result;
}

string repeating_key_XOR(string input, string key) {
	// match key length to the input length by repeating
	string new_key = key;
	for (int i = 0; i < (input.size() - key.size()); i++) {
        new_key += new_key[i];
	}
	// perform fixed XOR, since they are now equal lengths
	return fixed_XOR(input, new_key);
}

string break_repeating_key_XOR(string input) {
	double smallest_score = 9999;
	int best_keysize = 0;
	// test keysizes from 2 to 40
	for (int keysize = 2; keysize <= 40; keysize++) {
		double score = 0;
		int counter = 0;
		for (int i = 0; i < input.size(); i += keysize) {
			if (i + (2 * keysize) <= input.size() - 1) { 
				score += hamming_distance(input.substr(i, keysize), input.substr(i + keysize, keysize)); 
				counter++;
			}
		}
		score /= counter;
		score /= keysize;
		
		if (score < smallest_score) {
			smallest_score = score;
			best_keysize = keysize;
		}
	}
	
	// split into the best keysize worth of blocks
	vector<string> blocks;
	int remainder = input.size() % best_keysize;
	int size = input.size() - remainder;
	for (int i = 0; i < size; i += best_keysize) {
		blocks.push_back(input.substr(i, i + best_keysize));
	}
	if (remainder) { blocks.push_back(input.substr(size, remainder)); }
	
	// transpose
	vector<string> transposed;
	for (int i = 0; i < best_keysize; i++) {
		string chunk = "";
		for (int j = 0; j < blocks.size(); j++) { chunk += blocks[j][i]; }
		transposed.push_back(chunk);
	}
	
	// single char XOR each transposed block
	string keys;
	for (auto & block : transposed) { keys += get_single_byte_XOR_key(block);  }
	return keys;
}

string get_single_byte_XOR_key(string input) {
	double largest_score = 0.0;
	string best_key;
	// generate candidates from 255 possible keys
	for (int i = 0; i <= 255; i++) {
		string key = "";
		unsigned char c = i;
		key += c;
		string candidate = hex_to_ASCII(repeating_key_XOR(input, key));
		double score = freq_score(candidate);
		
		// find the largest score
		if (score > largest_score) {
			largest_score = score;
			best_key = key;
		}
	}
	return best_key;
}

string single_byte_XOR(string input) {
	string key = get_single_byte_XOR_key(input);
	return hex_to_ASCII(repeating_key_XOR(input, key));
}

string detect_single_byte_XOR(vector<string> input) {
	// split file by lines into a vector
	// vector<string> split_vect = split_file(filename);
	vector<string> decoded;
	double largest_score = 0;
	int index;
	
	// decode each line
	for (int i = 0; i < input.size(); i++) {
		string current_line = single_byte_XOR(hex_to_ASCII(input[i]));
		double score = freq_score(current_line);
		// find the largest score
		if (score > largest_score) {
			largest_score = score;
			index = i;
		}
		decoded.push_back(current_line);
	}
	return decoded[index];
}

int hamming_distance(string a, string b) {
	assert(a.size() == b.size());
	
	int length = a.size();
	int dist = 0;
    
    for (int i = 0; i < length; i++) {
		// XOR
        unsigned char c = a[i] ^ b[i];
		while (c) {
			dist += c & 1;
			c >>= 1;
		}
    }
	return dist;
}

string AES_decrypt(string input, string key) {
	Matrix key_(key);
	
	// undo add round key (XOR)
	
	
	return "";
}