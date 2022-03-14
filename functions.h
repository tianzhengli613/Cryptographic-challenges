#include <iostream>
#include <vector>

using std::string;
using std::vector;

// ==========================================================================================
// 'input' implies that the text that is fed is already decrypted into ASCII form, 
// rather than encrypted in hex or something else
// ==========================================================================================

void print_(string input);
void print_(int input);

double freq_score(string text);

vector<string> split_file(string filename);

string base64_to_ASCII(string base64);
string ASCII_to_base64(string input);
string hex_to_ASCII(string hex);
string ASCII_to_hex(string input);

string fixed_XOR(string a, string b);
string repeating_key_XOR(string input, string key);
string break_repeating_key_XOR(string input);

string get_single_byte_XOR_key(string input);
string single_byte_XOR(string input);
string detect_single_byte_XOR(vector<string> input);

int hamming_distance(string a, string b);

string AES_decrypt(string input, string key);