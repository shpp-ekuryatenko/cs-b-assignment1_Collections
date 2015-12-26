/********************************************************************************************
* File: myStack.h
* ----------------------
* v.2 2015/12/26 - Modified
* - code reformatted
*
* This file implements simple stack structure.
********************************************************************************************/

#ifndef _myStack_h
#define _myStack_h

#include "error.h"
#include "vector.h"

/*
 * Class: MyStack<ValueType>
 * -------------------------
 * Implements stack structure based on the internal Vector.
 * The top of the stack is the last added vector value.
 */
template <typename ValueType>
class MyStack {
/* -----------------------------------------------------------------------------------------//
 * Interface section
 * -----------------------------------------------------------------------------------------*/

public:

    /*
     * Constructor: MyStack
     * Usage: MyStack<ValueType> stack;
     * ------------------------------
     * Initializes a new empty stack.
     */
    MyStack();

    /*
     * Destructor: ~Stack
     * ------------------
     * Frees any heap storage associated with this stack.
     */
    virtual ~MyStack();

    /*
     * Method: push
     * Usage: stack.push(value);
     * -------------------------
     * Pushes the specified value onto the top of this stack.
     */
    void push(const ValueType& value);

    /*
     * Method: isEmpty
     * Usage: if (stack.isEmpty()) ...
     * -------------------------------
     * Returns true if this stack contains no elements.
     */
    bool isEmpty() const;

    /*
     * Method: pop
     * Usage: ValueType top = stack.pop();
     * -----------------------------------
     * Removes the top element from this stack and returns it.  This
     * method signals an error if called on an empty stack.
     */
    ValueType pop();

    /*
     * Method: peek
     * Usage: ValueType top = stack.peek();
     * ------------------------------------
     * Returns the value of top element from this stack, without removing it.
     * This method signals an error if called on an empty stack.
     */
    ValueType peek() const;

    /*
     * Method: size
     * Usage: int n = stack.size();
     * ----------------------------
     * Returns the number of values in this stack.
     */
    int size() const;

/* -----------------------------------------------------------------------------------------//
 * Private section
 * -----------------------------------------------------------------------------------------*/
private:
    /*
     * Implementation notes: myStack internal structure
     * --------------------------------------------------
     * The Stack is internally managed using an internal
     * Vector structure. So, there are no needs in memory
     * management or deep coping problems
     * The top of the stack is the last added to vector value.
     */

    /* Instance variables */

    Vector<ValueType> elements;     /* Main internal storage structure */
};

/* -----------------------------------------------------------------------------------------//
 * Implementation section
 * -----------------------
 * As the class is templated, the implementation part has
 * to be written in the same header with interface.
 * -----------------------------------------------------------------------------------------*/

/*
 * Implementation notes: myStack constructor
 * ------------------------------------------
 * Internal array "elements" is declared yet as a class field
 * so there are no needs to initiate some other fields.
 */
template <typename ValueType>
MyStack<ValueType>::MyStack() {
}

/*
 * Implementation notes: myStack destructor
 * ----------------------------------------
 * Internal array "elements" is deleted due to Vector class methods,
 * so there are no needs to control any else memory.
 */
template <typename ValueType>
MyStack<ValueType>::~MyStack() {
}

template <typename ValueType>
void MyStack<ValueType>::push(const ValueType &value) {
    elements.add(value);
}

template <typename ValueType>
bool MyStack<ValueType>::isEmpty() const {
    return elements.size() == 0;
}

/*
 * Implementation notes: pop
 * --------------------------
 * Removes the last element from vector, so vector size management
 * is controled by vector class methods.
 */
template <typename ValueType>
ValueType MyStack<ValueType>::pop() {
    if (isEmpty()) {
        error("MyStack::pop: Attempting to pop an empty stack");
    }
    ValueType top = elements[elements.size() - 1];
    elements.remove(elements.size() - 1);
    return top;
}

template <typename ValueType>
ValueType MyStack<ValueType>::peek() const {
    if (isEmpty()) {
        error("Stack::peek: Attempting to peek at an empty stack");
    }
    return elements.get(elements.size() - 1);
}

template <typename ValueType>
int MyStack<ValueType>::size() const {
    return elements.size();
}
#endif
