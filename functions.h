#include <iostream>
#include <vector>

using std::string;
using std::vector;

void print_(string input);
void print_(int input);

double freq_score(string text);

vector<string> split_file(string filename);

string base64_to_ASCII(string input);
string ASCII_to_base64(string input);
string hex_to_ASCII(string hex);
string ASCII_to_hex(string input);

// takes in ASCII text and returns hex string
string fixed_XOR(string a, string b);
string repeating_key_XOR(string text, string key);
string break_repeating_key_XOR(string filename);

// takes in hex string to decipher into ASCII
string single_byte_XOR(string input);
string detect_single_char_XOR(string filename);

int hamming_distance(string a, string b);