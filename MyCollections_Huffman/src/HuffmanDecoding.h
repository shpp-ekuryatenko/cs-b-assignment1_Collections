/*******************************************************
* File: HuffmanDecoding.h
* --------------------------
* v.2 2015/12/23
* - decompress() is changed
* - code formatted
* - code fields renamed
*
* Definitions for the functions necessary to build a
* Huffman encoding system.
*
*******************************************************/

#ifndef HuffmanDecoding_Included
#define HuffmanDecoding_Included

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include "filelib.h"
#include "simpio.h"
#include "console.h"
#include "strlib.h"
#include "bstream.h"
#include "HuffmanDecoding.h"
#include "HuffmanTypes.h"


/* Function: readFileHeader
 * --------------------------------------------------------
 * Reads header of cyphered file and reconstructs huffman tree
 * for this cypher file
 */
void readFileHeader(ibstream& infileStream, Node* root);

/* Function: decodeFileToFile
 * --------------------------
 * Main cyphered text decoding process.
 */
void decodeFileToFile(ibstream &infileStream, Node* root, obstream &outfileStream);

/* Function: decompress
 * Usage: decompress(cypherFile, outFile);
 * --------------------------------------------------------
 * Decompresses cypherFile into outfile.
 */
void decompress(string cypherFile, string outFile);

#endif
