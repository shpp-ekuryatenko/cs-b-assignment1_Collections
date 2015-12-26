/**********************************************
 * File: HuffmanTypes.h
 * --------------------------
 * v.1 2015/11/10
 *
 * A file containing the types used by the Huffman
 * encoding program.
 */

#ifndef HuffmanTypes_Included
#define HuffmanTypes_Included

#include <stddef.h>
#include <string>
#include "strlib.h"

/* Type: ext_char
 * A type representing a character, a pseudo-eof, or nothing. */
typedef int ext_char;

/* Constant: PSEUDO_EOF - wasn't used in this programm
 * A constant representing the PSEUDO_EOF marker that you will
 * write at the end of your Huffman-encoded file.
 */
const ext_char PSEUDO_EOF = 256;

/* Constant: NOT_A_CHAR
 * A constant representing an extended character that does not
 * actually hold a value.	 When you are constructing your Huffman
 * encoding tree, you should set the characters in each internal
 * node to this value to explicitly mark that they are not being
 * used.
 */
const ext_char NOT_A_CHAR = 257;

/* Type: Node
 * A node inside a Huffman encoding tree.	 Each node stores four
 * values - the character stored here (or NOT_A_CHAR if the value
 * is not a character), pointers to the 0 and 1 subtrees, and the
 * weight of the tree.
 *
 * You are free to add fields to this class, but please *do not*
 * modify the existing fields here.	 Our testing code relies on
 * these fields being here in order to function correctly, and
 * Very Bad Things will happen if you change or reorder them.
 */
struct Node {
	/* A pointer to the 0 subtree. */
    Node* leftChild;

	/* A pointer to the 1 subtree. */
    Node* rightChild;

    /* Stores appearence for this node */
    int symbAppearance;

    /* Real symbol */
    ext_char symbol;

    Node ();

    Node (Node* left, Node* right, int app, ext_char symb);

    Node & operator=(const Node  & n);

    std::string toString();
};
#endif
