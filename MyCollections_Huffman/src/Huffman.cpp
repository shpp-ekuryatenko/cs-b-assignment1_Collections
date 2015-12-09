#include <iostream>
#include "simpio.h"
#include "console.h"
#include "bstream.h"
#include "HuffmanEncoding.h"
#include "HuffmanDecoding.h"
#include "HuffmanTypes.h"
#include "strlib.h"
#include <string>


using namespace std;

/* Huffman compression process */
string getCompressedFile(string inputFile){
    string cypherFile = "Cyphered_" + inputFile;
    cout << "WAIT, FILE \"" << inputFile << "\" IS BEING CODED..." << endl;

    /* Prepares streams */
    ifbstream infile1;
    infile1.open(inputFile.c_str());
    ofbstream outfile1;
    outfile1.open(cypherFile.c_str());

    /* Input file compression */
    compress(infile1, outfile1);
    infile1.close();
    outfile1.close();
    cout << "\"" << inputFile << "\" IS CODED TO \""
         << cypherFile << "\" INTO PROJECT BUILD FOLDER" << endl;
    cout << endl;
    return cypherFile;
}

/* Huffman decompression process */
string getDecompressedFile(string cypherFile){
    string decompressedFile = "";
    decompressedFile = stringReplace(cypherFile, "Cyphered_", "Decoded_", -1); 
    cout << "WAIT, CYPHER FILE \"" << cypherFile << "\" IS BEING DECODED..." << endl;

    /* Prepares streams */
    ifbstream infile2;
    infile2.open(cypherFile.c_str());
    infile2.rewind();
    ofbstream outfile2;
    outfile2.open(decompressedFile.c_str());

    /* Cypher file decompression */
    decompress(infile2,outfile2);
    infile2.close();
    outfile2.close();
    cout << "\"" << cypherFile << "\" IS DECODED TO \""
         << decompressedFile << "\" INTO PROJECT BUILD FOLDER" << endl;
    return cypherFile;
}

int main() {
   /* Huffman text file compression-decompression process
    * --------------------------------------------------------
    * Input file is coded due to Huffman compression rules
    * into cypherFile. After then cypher file is decompressed
    * into outFile.
    *
    * Input files are saved into res project folder, or you could
    * use fileInput function for console input
    * Output files will be saved into project build folder
    *
    * After decompression this programm is done
    */

   string inputFile = fileInput("Enter file name to compress: ");
   //string inputFile = "singleChar";
   cout << "-------------------------------------------------------" << endl;

   /* Huffman compression process */
   string cypherFile = getCompressedFile(inputFile);
   cout << "-------------------------------------------------------" << endl;

   /* Huffman decompression process */
   string outFile = getDecompressedFile(cypherFile);
   cout << "-------------------------------------------------------" << endl;

   return 0;
}
