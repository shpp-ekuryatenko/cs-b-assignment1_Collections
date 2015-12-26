/*******************************************************
* File: HuffmanEncoding.h
* --------------------------
* v.2 2015/12/26
* - compress() is changed
* - code formatted
* - code fields renamed
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


/* Function: fileInput
 * -------------------
 * Defines if this file exist in project directory
 */
string fileInput(string promptText);

/* Function: getFrequencyTable
 * Usage: MyMap<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Calculates the frequencies of each character within
 * infileStream text and stores
 * the result as a MyMap from ext_chars to the number of times
 * that the character appears.
 */
MyMap<ext_char, int> getFrequencyTable(ibstream &infileStream);

/* Function: loadQueueBySymbolsNodes
 * --------------------------------------------------------
 * Creates symbols Nodes from frequency table and adds them
 * to Nodes* queue
 */
void loadQueueBySymbolsNodes(MyPQueue<Node*>& mpq, MyMap<ext_char, int> &frequenciesMap);

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(nodesQueue);
 * --------------------------------------------------------
 * Constructs a Huffman encoding tree from frequencies Nodes
 * and returns a pointer to the root.
 */
Node* buildEncodingTree(MyPQueue<Node *> &nodesQueue);

/* Function: deleteTree
 * Usage: deleteTree(encodingTree);
 * --------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */
void deleteTree(Node* &root);

/* Function: encodeTreeToFileHeader
 * --------------------------------
 * Encodes tree shape into cypherFile.
 * Makes depth traversing of coding tree, and writes:
 */
void encodeTreeToFileHeader(Node* node, obstream &outfileStream);

/* Function: encodeMainTextToFile
 * Usage: encodeMainTextToFile(sourceFileStream,
 *                             encodingTree,
 *                             cypheredFileStream);
 * --------------------------------------------------------
 * Encodes chars from  infileStream file text to outfileStream
 * file.
 */
void encodeMainTextToFile(ibstream &infileStream, Node* encodingTree, obstream &outfileStream);

/* Function: compress
 * Usage: compress(inputFile, cypherFile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the inputFile, then writes the result to cypherFile.
 */
void compress(string inputFile, string cypherFile);


#endif
