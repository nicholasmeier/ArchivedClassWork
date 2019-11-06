#include <iostream>
#include <string>
#include <string.h>
#include "MinHeap.h"
#include "Encoder.h"
#include "Decoder.h"

using namespace std;

void printHelp(){
	cout << "Invalid arguments." << endl;
	cout << "Type \"huffman [mode] [arguments]\" where mode and its arguments are one of the following:" << endl;
	cout << "-e [src] [dest]: encode the source file and place the result into the specified destination" << endl;
	cout << "-d [src] [dest]: decode the source file and place the result into the specified destination" << endl;
	exit(1);
}


int main (int argc, char** argv){
	//cout << argc << argv[0] << argv[1] << argv[2] << argv[3] << "\n";
	//cout << argv[1] << "\n";
	if (argc != 4){	printHelp(); }
	if (!(strcmp(argv[1], "-e"))){
		//cout << "Reached";
		Encoder * e = new Encoder(argv[2]);
		e->buildFrequencyTable();
		e->encode();
		e->writeEncodedFile(argv[3]);
	}
	
	return 0;
}
