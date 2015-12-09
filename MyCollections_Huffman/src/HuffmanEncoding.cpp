/**********************************************************
 * File: HuffmanEncoding.cpp
 *
 * Implementation of the functions from HuffmanEncoding.h.
 *
 **********************************************************/

#include "HuffmanEncoding.h"
#include "HuffmanTypes.h"

#include "hashcode.h"

using namespace std;

/* User input:
 * Defines if this file exist in project directory */
string fileInput(string promptText){
    /* Until user dosen't make valid input - ask him */
    string result = "";
    while(true){
        result = getLine(promptText);
        if(fileExists(result)){
            break;
        }else{
            cout << "Unfortunately your input is failed" << endl;
        }
    }
    return result;
}

/* Build map of symbols appearance in file
 * Is used in getFrequencyTable()  */
void modifyMap(MyMap<ext_char, int>& mainMap,
               ext_char nextSymbol){
    if(mainMap.containsKey(nextSymbol)){
        /* Modifies map entry - increments symbol value */
        int symbValue = mainMap[nextSymbol];
        mainMap.put(nextSymbol, ++symbValue);
    }else{
        /* Adds new map entry for this symbol */
        int symbApearence = 1;
        mainMap.put(nextSymbol, symbApearence);
    }
}

/* Function: getFrequencyTable
 * Usage: Map<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * Given an input stream containing text, calculates the
 * frequencies of each character within that text and stores
 * the result as a Map from ext_chars to the number of times
 * that the character appears.
 *
 * This function will also set the frequency of the EOF
 * character to be 1, which ensures that any future encoding
 * tree built from these frequencies will have an encoding for
 * the EOF character.  */
MyMap<ext_char, int> getFrequencyTable(ibstream& infile) {
    MyMap<ext_char, int> resultMap;
    ext_char nextChar;
    while ((nextChar = infile.get()) != EOF) {
        modifyMap(resultMap, nextChar);
    }
    resultMap.add(EOF, 1);
    return resultMap;
}

/* Function: buildNodesVector
 * Usage: buildNodesVector(vector, frequencyTable);
 * --------------------------------------------------------
 * Creates symbols Nodes from table and adds them to Nodes* priority queue */
void loadQueue(MyPQueue<Node*>& mpq, MyMap<ext_char, int> &fileMap){
    for(ext_char key: fileMap){
        Node* symbNode = new Node;
        /* Node constructor */
        symbNode->symbol = key;
        symbNode->symbAppearance = fileMap[key];
        symbNode->leftChild = NULL;
        symbNode->rightChild = NULL;
        /* Add curent node to queue whith apearence priority */
        int symbAppearance = fileMap[key];
        mpq.enqueue(symbNode, symbAppearance);
    }
}

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
Node* buildEncodingTree(MyPQueue<Node*>& mpq){
    Node* root = new Node;//this root will return copy of tree root from function
    while(!mpq.isEmpty()){
        Node* nd1 = mpq.dequeueMin();//remove min appearence node form vec
        if(!mpq.isEmpty()){
            Node* nd2 = mpq.dequeueMin();
            int sumAppear = nd1->symbAppearance + nd2->symbAppearance;
            Node* parent = new Node;
            parent->leftChild = nd1;
            parent->rightChild = nd2;
            parent->symbAppearance = sumAppear;
            parent->symbol = NOT_A_CHAR;
            mpq.enqueue(parent, sumAppear);
        }else{
            root = nd1;
        }
    }
    return root;
}

/* Function: freeTree
 * Usage: freeTree(encodingTree);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */
void deleteTree(Node* &node){
    /* Fork end */
    if(node->leftChild != NULL){
       deleteTree(node->leftChild);
    }
    if(node->rightChild != NULL){
        deleteTree(node->rightChild);
    }
    if((node->leftChild == NULL) && (node->rightChild == NULL)){
       node = NULL;
       delete node;
    }
}

/* Encodes tree shape into cypher
 * Makes depth traversing of coding tree, and writes
 * - 0 - if it's knot at this traverse, without symbols
 * - 1 - if it's leaf at this traverse, without childs. It is followed by char byte code
 */
void encodeTreeToFileHeader(Node* node, obstream& outstr){
    /* This node is fork end
     * We should write '1' and char code then */
    if((node->leftChild == NULL) && (node->rightChild == NULL)){
        outstr.writeBit(1);
        outstr.put((node->symbol));
    }else{
        /* This node has childs - we should write just '0' */
        outstr.writeBit(0);
        if(node->leftChild != NULL){
            encodeTreeToFileHeader((node->leftChild), outstr);
        }
        if(node->rightChild != NULL){
            encodeTreeToFileHeader((node->rightChild), outstr);
        }
    }
}

/* Builds cypher table map from string
 * key - symbol
 * value - symbol code  */
void buildCypherTable(string currentCypher, Node* node, MyMap<ext_char, string>& cypherTable){
    /* This node is fork end */
    if((node->leftChild == NULL) && (node->rightChild == NULL)){
        cypherTable.add(node->symbol, currentCypher);
    }else{
        /* This node has childs */
        if(node->leftChild != NULL){
            string leftCypher = currentCypher + "0";           
            buildCypherTable(leftCypher , node->leftChild, cypherTable);
        }
        if(node->rightChild != NULL){
            string rightCypher = currentCypher + "1";           
            buildCypherTable(rightCypher , node->rightChild, cypherTable);
        }
    }
}

/* @symbCode - stores bits of some symbol bit-code
 * Function writes this symbol code to output stream  */
void writeCodeToStream(string symbCode,  obstream& ofbs){
    for(int i = 0; i < symbCode.length(); i++){
        if(symbCode[i] == '1'){
            ofbs.writeBit(1);
        }else{
            ofbs.writeBit(0);
        }
    }
}

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
void encodeMainTextToFile(ibstream& infile,
                        Node* root,
                        obstream& ofbs){
    /* Cyphers table - [symb][Huffman code] */
    MyMap<ext_char, string> cypherTable;
    string currentCypher = "";//start code to concantenate
    /* Fills cypherTable by entries [symb][Huffman code] */
    buildCypherTable(currentCypher , root, cypherTable);

    /* Infile stream translation process  */
    ext_char textChar;
    string symbCode = "";
    while ((textChar = infile.get()) != EOF) {
        symbCode = cypherTable[(ext_char)textChar];
        writeCodeToStream(symbCode, ofbs);
    }
    /* Write EOF */
    symbCode = cypherTable[EOF];
    writeCodeToStream(symbCode, ofbs);//Main EOF
    writeCodeToStream(symbCode, ofbs);//Fix problem of EOF decoding
}

/* Function: compress
 * Usage: compress(infile, outfile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the file whose contents are specified by the input
 * ibstream, then writes the result to outfile. */
void compress(ibstream& infile, obstream& outfile) {
    /* Calculates the frequencies of each character within text */
    MyMap<ext_char, int> frequenciesTable = getFrequencyTable(infile);

    /* Priority queue for cypher tree creation */
    MyPQueue<Node*> mpq;
    /* Creates nodes for each symbol and put them to queue */
    loadQueue(mpq, frequenciesTable);
    /* Main Huffman tree building */
    Node* root = buildEncodingTree(mpq);
    /* ENCODE TREE INTO CYPHER FILE HEADER */
    encodeTreeToFileHeader(root, outfile);
    outfile.put(' ');//put some char to divide header from main text cypher

    /* ENCODE FILE MAIN TEXT INTO CYPHER FILE */
    infile.rewind();
    encodeMainTextToFile(infile, root, outfile);

    /* Memory cleaning */
    deleteTree(root);
}
