/**********************************************************
 * File: HuffmanTypes.cpp
 *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *
 **********************************************************/

#include "HuffmanTypes.h"

using namespace std;

Node::Node (){
   leftChild = NULL;
   rightChild = NULL;
   symbAppearance = 0;
   symbol = '*';
}

Node::Node (Node* left, Node* right, int app, ext_char symb){
   leftChild = left;
   rightChild = right;
   symbAppearance = app;
   symbol = symb;
}

Node & Node::operator=(const Node  & n){
   leftChild = n.leftChild;
   rightChild = n.rightChild;
   symbAppearance = n.symbAppearance;
   symbol = n.symbol;
   return *this;
}

string Node::toString(){
  return  "(" + charToString((char)symbol) + ", " + integerToString(symbAppearance) + ")";
}

