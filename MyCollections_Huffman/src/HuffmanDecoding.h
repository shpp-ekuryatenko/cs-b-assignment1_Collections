/*******************************************************
 * File: HuffmanEncoding.h
 *
 * Definitions for the functions necessary to build a
 * Huffman encoding system.
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
 * -------------------------
 * Reads header of cyphered file and reconstructs huffman tree
 * for this cypher file
 */
void readFileHeader(ibstream& infile, Node* root);

/* Function: decodeFileToFile
 * -------------------------
 * Main cypher text decoding process */
void decodeFileToFile(ibstream &infile, Node* root, obstream &outfile);

/* Function: decompress
 * Usage: decompress(infile, outfile);
 * --------------------------------------------------------
 * Main Huffman decompressor.
 * Decompresses the file whose contents are specified by the
 * input ibstream, then writes the decompressed version of
 * the file to the stream specified by outfile.
 */
void decompress(ibstream& infile, obstream &outfile);

#endif
