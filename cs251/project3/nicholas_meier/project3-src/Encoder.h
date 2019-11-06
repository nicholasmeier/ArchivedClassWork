#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>

#include "TreeNode.h"
#include "MinHeap.h"
#include "HuffTree.h"

#define MAX_VALS 256
using namespace std;

class Encoder
{
	private:
		unsigned frequency_table[MAX_VALS];
		string input_path;
		string inputstr;
		MinHeap * mh;
		int unique = 0;
		
		// You need to add more class memeber and methods
	public:
		//test_file_path is the input (decoded) file
		Encoder(string file_path){
			input_path = file_path;
			mh = new MinHeap();
		}; 
		
		//Fills up the frequency_table array where frequency_table[i] 
		//will contain the frequency of char with ASCII code i		
		void buildFrequencyTable(){
			ifstream file(input_path.c_str());
			stringstream buffer;
			buffer << file.rdbuf();
			inputstr = buffer.str();
			for (int i = 0; i < inputstr.length(); i++){
				frequency_table[inputstr[i]] = 0;
			}
			for (int i = 0; i < inputstr.length(); i++){
				frequency_table[inputstr[i]]++;
			}

		}; 

		//Builds the min heap and run the encoding algorithm
		void encode(){
			for (int i = 0; i < MAX_VALS; i++){
				if (frequency_table[i] != 0){
					TreeNode * t = new TreeNode(i, frequency_table[i]);
					mh->insert(t);
					unique++;
				}
			}
		};

		//Generates the encoded file and save it as output_file_path
		void writeEncodedFile(string output_file_path){
			HuffTree * hf = new HuffTree();
			hf->buildTree(mh);
			hf->generateCodes();
			string out = "";
			/*
			for (int j = 0; j < MAX_VALS; j++){
				//Need to add the code to add frequency list to the output	
			}*/
			for (int i = 0; i < inputstr.length(); i++){
				out.append(hf->getCharCode(inputstr[i]));
			}
			
			ofstream output(output_file_path, ios::binary);
			
			unsigned char bit_buffer;
			int bits_written = 0;
			for (int j = 0; j < MAX_VALS; j++){
				if (frequency_table[j] != 0){
					string code = hf->getCharCode(j);
					for (int i = 0; i < code.length(); i++){
						bit_buffer = bit_buffer << 1;
						bit_buffer = bit_buffer | (code[i] - '0');
						bits_written++;
						if (bits_written == 8){
							output.write((const char*)bit_buffer, sizeof(const char));
							bits_written = 0;
							bit_buffer = 0;
						}else{
							if (i == code.length()-1 && bits_written < 8){
								unsigned char buff;
								output.write((const char*)buff, sizeof(const char));
							}
						}
					}
				}
			}
			/*
			bitset<sizeof(unsigned long) * 8> bits(out);
			unsigned long binary_out = bits.to_ulong();
			output.write((const char*)&binary_out, sizeof(unsigned long));
			*/
			

			/* //Old writing code
			vector<char> bits;
			unsigned char byte = 0;
			for (int j = 0; j < out.length(); j += 8){
				string sub = out.substr(j, 8);
				for (int i = 0; i < sub.length(); i++){
					if (sub[i] == '1'){ byte |= 1 << (7-i); }
				}
				bits.push_back(byte);
			}
			ofstream OutFile;
			OutFile.open(output_file_path, ios::out | ios::binary);
			for (auto & b : bits) {
				//tFile.write(b, 8);
				std::cout.write(reinterpret_cast<const char*>(&b), sizeof(b));;
			}*/
		};

		~Encoder();
};
#endif
