/**********************************************************
* File: HuffmanEncoding.cpp
* --------------------------
* v.2 2015/12/26
* - compress() is changed
* - code formatted
* - code fields renamed
* - some decomposition is included
*
* Implementation of the functions from HuffmanEncoding.h.
*
**********************************************************/

#include "HuffmanEncoding.h"
#include "HuffmanTypes.h"

using namespace std;

/* Function: fileInput
 * -------------------
 * Defines if this file exist in project directory
 */
string fileInput(string promptText) {
    /* Until user dosen't make valid input - ask him */
    string result = "";
    while (true) {
        result = getLine(promptText);
        if (fileExists(result)) {
            break;
        }else{
            cout << "Unfortunately your input is failed" << endl;
        }
    }
    return result;
}

/* Function: modifyMap
 * -------------------
 * Modifies param frequenciesMap entries
 * for current char key.
 */
void modifyMap(MyMap<ext_char, int>& frequenciesMap,
               ext_char nextSymbol) {
    if (frequenciesMap.containsKey(nextSymbol)) {
        /* Modifies map entry - increments symbol value */
        int symbValue = frequenciesMap[nextSymbol];
        frequenciesMap.put(nextSymbol, ++symbValue);
    }else{
        /* Adds new map entry for this symbol */
        int symbApearence = 1;
        frequenciesMap.put(nextSymbol, symbApearence);
    }
}

/* Function: getFrequencyTable
 * Usage: MyMap<ext_char, int> freq = getFrequencyTable(file);
 * --------------------------------------------------------
 * This function will also set the frequency of the EOF
 * character to be 1, which ensures that any future encoding
 * tree built from these frequencies will have an encoding for
 * the EOF character.
 */
MyMap<ext_char, int> getFrequencyTable(ibstream& infileStream) {
    MyMap<ext_char, int> resultFrequenciesMap;
    ext_char nextChar;

    while ((nextChar = infileStream.get()) != EOF) {
        modifyMap(resultFrequenciesMap, nextChar);
    }

    resultFrequenciesMap.add(EOF, 1);
    return resultFrequenciesMap;
}

/* Function: buildNodesVector
 * Usage: buildNodesVector(vector, frequencyTable);
 * --------------------------------------------------------
 * Creates symbols Nodes from frequency table and adds them
 * to Nodes* queue
 */
void loadQueueBySymbolsNodes(MyPQueue<Node*>& nodesQueue, MyMap<ext_char, int>& frequenciesMap){
    for(ext_char key: frequenciesMap){
        Node* symbNode = new Node;
        /* Node constructor */
        symbNode->symbol = key;
        symbNode->symbAppearance = frequenciesMap[key];
        symbNode->leftChild = NULL;
        symbNode->rightChild = NULL;
        /* Add curent node to queue whith apearence priority */
        int symbAppearance = frequenciesMap[key];
        nodesQueue.enqueue(symbNode, symbAppearance);
    }
}

/* Function: buildEncodingTree
 * Usage: Node* tree = buildEncodingTree(nodesVector);
 * --------------------------------------------------------
 * Constructs a Huffman encoding tree from frequencies Nodes
 * and returns a pointer to the root.
 */
Node* buildEncodingTree(MyPQueue<Node*>& nodesQueue){
    Node* root = new Node;//this root will return copy of tree root from function
    while(!nodesQueue.isEmpty()){
        Node* nd1 = nodesQueue.dequeueMin();//remove min appearence node form vec
        if(!nodesQueue.isEmpty()){
            Node* nd2 = nodesQueue.dequeueMin();
            int sumAppear = nd1->symbAppearance + nd2->symbAppearance;
            Node* parent = new Node;
            parent->leftChild = nd1;
            parent->rightChild = nd2;
            parent->symbAppearance = sumAppear;
            parent->symbol = NOT_A_CHAR;
            nodesQueue.enqueue(parent, sumAppear);
        }else{
            root = nd1;
        }
    }
    return root;
}

/* Function: deleteTree
 * Usage: deleteTree(encodingTreeRoot);
 * --------------------------------------------------------
 * Deallocates all memory allocated for a given encoding
 * tree.
 */
void deleteTree(Node*& root) {
    /* Fork end */
    if (root->leftChild != NULL) {
        deleteTree(root->leftChild);
    }
    if (root->rightChild != NULL) {
        deleteTree(root->rightChild);
    }
    if ((root->leftChild == NULL) && (root->rightChild == NULL)) {
        root = NULL;
        delete root;
    }
}

/* Function: encodeTreeToFileHeader
 * --------------------------------
 * Encodes tree shape into cypherFile.
 * Makes depth traversing of coding tree, and writes:
 *
 * - 0 - if it's knot at this traverse, without symbols
 *
 * - 1 - if it's leaf at this traverse, without childs.
 *   It is followed by char byte code.
 */
void encodeTreeToFileHeader(Node* node, obstream& outfileStream) {
    /* This node is fork end
     * We should write '1' and char code then */
    if ((node->leftChild == NULL) && (node->rightChild == NULL)) {
        outfileStream.writeBit(1);
        outfileStream.put((node->symbol));
    }else{
        /* This node has childs - we should write just '0' */
        outfileStream.writeBit(0);
        if (node->leftChild != NULL) {
            encodeTreeToFileHeader((node->leftChild), outfileStream);
        }
        if (node->rightChild != NULL) {
            encodeTreeToFileHeader((node->rightChild), outfileStream);
        }
    }
}

/* Function: buildCypherTable
 * ---------------------------
 * Modifies param cypher table map
 * by entries [symbol][symbol Huffman code]
 * for this huffman tree.
 * Makes DFS from root to every symbol-leaf to
 * cocantenate currentCypher code value.
 *
 *
 * @param currentCypher        symbol
 * @param node                 root of Huffman tree
 * @param cypherTable          cypher table map to fill by entries
 */
void buildCodesTableForTree(string currentSymbolCode,
                             Node* node,
                             MyMap<ext_char, string>& charsCodesTable) {
    /* This node is fork end */
    if ((node->leftChild == NULL) && (node->rightChild == NULL)) {
        charsCodesTable.add(node->symbol, currentSymbolCode);
    }else{
        /* This node has childs */
        if (node->leftChild != NULL) {
            string leftCypher = currentSymbolCode + "0";
            buildCodesTableForTree(leftCypher, node->leftChild, charsCodesTable);
        }
        if (node->rightChild != NULL) {
            string rightCypher = currentSymbolCode + "1";
            buildCodesTableForTree(rightCypher, node->rightChild, charsCodesTable);
        }
    }
}

/* Function: writeCodeToStream
 * ---------------------------
 * Writes this symbol code to output stream
 *
 * @param symbCode   stores string of bit-code for some symbol
 */
void writeCodeToStream(string symbolCode, obstream& outfileStream) {
    for (int i = 0; i < symbolCode.length(); i++) {
        if (symbolCode[i] == '1') {
            outfileStream.writeBit(1);
        }else{
            outfileStream.writeBit(0);
        }
    }
}

/* Function: getCodeForChar
 * ---------------------------
 * Returns corresponding code for param char
 */
string getCodeForChar(char currentTextChar,
                    MyMap<ext_char, string>& charsCodesTable){
    return charsCodesTable[(ext_char)currentTextChar];
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
 *     without seeking the file anywhere.
 */
void encodeMainTextToFile(ibstream& infileStream,
                          Node* root,
                          obstream& outfileStream) {
    /* Cyphers table - [symb][Huffman code] */
    MyMap<ext_char, string> charsCodesTable;
    string currentCypher = "";//start code to concantenate
    /* Fills cypherTable by entries [symb][Huffman code] */
    buildCodesTableForTree(currentCypher, root, charsCodesTable);

    /* Infile stream translation process  */
    char textChar;
    string symbCode = "";
    while ((textChar = infileStream.get()) != EOF) {
        symbCode = getCodeForChar(textChar, charsCodesTable);
        writeCodeToStream(symbCode, outfileStream);
    }

    /* Write EOF */
    symbCode = getCodeForChar(EOF, charsCodesTable);
    writeCodeToStream(symbCode, outfileStream);//Main EOF
    writeCodeToStream(symbCode, outfileStream);//Fix problem of EOF decoding
}

/* Function: compress
 * Usage: compress(inputFile, cypherFile);
 * --------------------------------------------------------
 * Main entry point for the Huffman compressor.  Compresses
 * the inputFile, then writes the result to cypherFile.
 */
void compress(string inputFile, string cypherFile) {
    cout << "PROCESSING..." << endl;
    cout << "==========================================================" << endl;
    /* Huffman compression process */
    cout << "    - WAIT, FILE \"" << inputFile << "\" IS BEING CODED..." << endl;
    /* Prepare input stream object   */
    ifbstream infileStream;
    infileStream.open(inputFile.c_str());
    ofbstream outfileStream;
    outfileStream.open(cypherFile.c_str());

    /* Input file compression */
    /* Calculates the frequencies of each character within text */
    MyMap<ext_char, int> frequenciesTable = getFrequencyTable(infileStream);

    /* Priority queue for cypher tree creation */
    MyPQueue<Node*> nodesQueue;
    /* Creates nodes for each symbol and put them to queue */
    loadQueueBySymbolsNodes(nodesQueue, frequenciesTable);
    /* Main Huffman tree building */
    Node* root = buildEncodingTree(nodesQueue);
    /* ENCODE TREE INTO CYPHER FILE HEADER */
    encodeTreeToFileHeader(root, outfileStream);
    outfileStream.put(' ');//put some char to divide header from main text cypher cypher

    /* ENCODE FILE MAIN TEXT INTO CYPHER FILE */
    infileStream.rewind();
    encodeMainTextToFile(infileStream, root, outfileStream);

    /* Memory cleaning */
    deleteTree(root);

    infileStream.close();
    outfileStream.close();
    cout << "    - FILE'S CODING COMPLETE!" << endl;
}
