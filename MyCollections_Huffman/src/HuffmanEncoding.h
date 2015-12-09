/*******************************************************
 * File: HuffmanEncoding.h
 *
 * Definitions for the functions necessary to build a
 * Huffman encoding system.
 *******************************************************/

#ifndef HuffmanEncoding_Included
#define HuffmanEncoding_Included

#include <fstream>
#include <iostream>
#include <iomanip>
#include "filelib.h"
#include <string>
#include "simpio.h"
#include <stdlib.h>
#include "console.h"
#include "strlib.h"
#include "bstream.h"
#include "HuffmanEncoding.h"
#include "HuffmanTypes.h"
#include "myPQueue.h"
#include "myMap.h"

/* User input:
 * Defines if this file exist in project directory */
string fileInput(string promptText);

/* Function: getFrequencyTable
 * Usage: Map<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Given an input stream containing text, calculates the
 * frequencies of each character within that text and stores
 * the result as a Map from ext_chars to the number of times
 * that the character appears.
 */
MyMap<ext_char, int> getFrequencyTable(ibstream &infile);

/* Function: buildNodesVector
 * Usage: buildNodesVector(vector, frequencyTable);
 * --------------------------------------------------------
 * Creates symbols Nodes from table and adds them to Nodes* vector */
void loadQueue(MyPQueue<Node*>& mpq, MyMap<ext_char, int> &fileMap);

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(frequency);
 * --------------------------------------------------------
 * Given a map from extended characters to frequencies,
 * constructs a Huffman encoding tree from those frequencies
 * and returns a pointer to the root.
 *
 * This function can assume that there is always at least one
 * entry in the map, since the EOF character will always
 * be present */
Node* buildEncodingTree(MyPQueue<Node *> &mpq);

/* Function: freeTree
 * Usage: freeTree(encodingTree);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.  */
void deleteTree(Node* &node);

/* Encodes tree shape into cypher
 * Makes depth traversing of coding tree, and writes
 * - 0 - if it's knot at this traverse, without symbols
 * - 1 - if it's leaf at this traverse, without childs. It is followed by char byte code  */
void encodeTreeToFileHeader(Node* node, obstream &outstr);

/* Function: encodeMainTextToFile
 * Usage: encodeMainTextToFile(sourceFile, encodingTree, cypheredFile);
 * --------------------------------------------------------
 * Encodes the given file using the encoding specified by the
 * given encoding tree, then writes the result one bit at a
 * time to the specified output file.
 *
 * This function can assume the following:
 *
 *   - The encoding tree was constructed from the given file,
 *     so every character appears somewhere in the encoding
 *     tree.
 *
 *   - The output file already has the encoding table written
 *     to it, and the file cursor is at the end of the file.
 *     This means that you should just start writing the bits
 *     without seeking the file anywhere.  */
void encodeMainTextToFile(ibstream &infile, Node* encodingTree, obstream &outfile);

/* Function: compress
 * Usage: compress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the file whose contents are specified by the input
 * ibstream, then writes the result to outfile. */
void compress(ibstream& infile, obstream& outfile);

#endif
