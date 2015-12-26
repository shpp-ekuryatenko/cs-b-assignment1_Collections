/**********************************************************
 * File: Huffman.cpp
 * ----------------------
 * v.2 2015/12/23 - decomposition in main
 * - compress() is changed
 * - decompress() is changed
 *
 * Program makes Huffman principle compression and
 * decompression for user .txt file.
 **********************************************************/
#include <iostream>
#include "simpio.h"
#include "console.h"
#include "bstream.h"
#include "HuffmanEncoding.h"
#include "HuffmanDecoding.h"
#include "HuffmanTypes.h"

using namespace std;

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
    * After decompression this programm is done.
    *
    * Suposed, it will be simple .txt file.
    * Any other file types weren't tesetd.
    */

   //string inputFile = fileInput("Enter file name: ");
   string inputFile = "tomSawyer.txt";
   string cypherFile = "Cyphered_" + inputFile;
   string outFile = "Decoded_" + inputFile;

   /* Huffman compression process */
   compress(inputFile, cypherFile);

   /* Huffman decompression process */
   decompress(cypherFile, outFile);

   return 0;
}
