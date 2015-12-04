/********************************************************************************************
 * File: myMap.h
 * ----------------------
 * v.1 2015/11/30  - Huffman requirements
 * This file exports the template class, which maintains
 * structure for hash map structure.
 *
 * Current implementation of hashmap has raw study version,
 * it suits only Huffman compression requirements,
 * and could be improved in next versions.
 *
 * Possible improvements:
 * - clear  and deepCopy methods memory management;
 * - rehashing;
 * - iterator has to be bidirectional for STL purpose;
 * - hashCoding for user classes.
 ********************************************************************************************/
#ifndef _myMap_h
#define _myMap_h

#include <iostream>
#include <cstdlib>
#include <string>
#include "error.h"
#include "hashcode.h"


/*
 * Class: MyMap<KeyType, ValueType>
 * --------------------------------
 * Implements hash map structure with hash buckets
 * of linked lists implementation.
 *
 * Supports only pimitive types of keys - due to
 * "hashcode.h" implementation.
 */
template <typename KeyType, typename ValueType>
class MyMap {

    /* Forward reference */
    struct Pair;

/* -----------------------------------------------------------------------------------------//
 * Interface section
 * -----------------------------------------------------------------------------------------*/
public:  
    /*
     * Constructor: MyMap
     * Usage: MyMap<KeyType, ValueType> map;
     * -------------------------------------
     * Initializes a new empty map.
     */
    MyMap();

    /*
     * Destructor: ~MyMap
     * ----------------------
     * Frees any heap storage associated with this map.
     */
    virtual ~MyMap();

    /*
     * Method: get
     * Usage: ValueType value = map.get(key);
     * --------------------------------------
     * Returns the value associated with key in this map.
     * If key is not found, get returns the error message.
     */
    ValueType get(const KeyType& key) const;

    /*
     * Method: put
     * Usage: map.put(key, value);
     * ---------------------------
     * Associates key with value in this map.
     * If such key exist in map - udate it's value.
     */
    void put(const KeyType& key, const ValueType& value);

    /*
     * Method: clear
     * Usage: map.clear();
     * -------------------
     * Removes all Pair entries from this map.
     */
    void clear();

    /*
     * Method: containsKey
     * Usage: if (map.containsKey(key)) ...
     * ------------------------------------
     * Returns true if there is an entry for key
     * in this map.
     */
    bool containsKey(const KeyType& key) const;

    /*
     * Operator: []
     * Usage: map[key]
     * ---------------
     * Selects the value associated with key.
     * It's no modifiyng operator: if key is already
     * present in the map, this function
     * returns associated value.
     */
    ValueType operator [](const KeyType& key) const;

    /*
     * Method: copy constructor
     * -------------------------
     * The copy constructor provides deep copying to
     * pass MyMap objects by value, or to initiate
     * new objects.
     */
    MyMap(const MyMap<KeyType, ValueType>& src);

    /*
     * Method: assignment operator
     * ---------------------------
     * Provides deep copying to assign one existed
     * MyMap object to another.
     */
    MyMap& operator =(const MyMap& src);

    /*
     * Method: mapAll
     * Usage: map.mapAll(fn);
     * ----------------------
     * Iterates through the map entries and calls
     * method fn(key, value) for each one.
     */
    template <typename FunctorType>
    void mapAll(FunctorType fn) const;

/* -----------------------------------------------------------------------------------------//
 * Iterator support
 * ----------------
 * Implementation for the map object input iterator.
 * -----------------------------------------------------------------------------------------*/
    class iterator : public std::iterator<std::input_iterator_tag, KeyType> {
        /*
         * Implementation notes: MyMap iterator
         * ------------------------------------
         * The main principle is to maintain pointer
         * of the current Pair - cp, as a marker of current
         * iterator position.
         *
         * Iterator object hardly connected with current
         * MyMap object - mp, so they are created simultanously.
         * But we could create several iterator objects for
         * our goals.
         *
         * Iterator implemented as input iterator, because of
         * our goals had no needs change map by iterators.
         * We don't overload "=" operator for it.
         *
         * std::input_iterator_tag...  - Empty class to identify
         * the category of an iterator as an input iterator
         */
    private:

        const MyMap* mp;             /* Pointer to the map                      */
        int bucket;                  /* Index of current bucket                 */
        MyMap::Pair* cp;             /* Pointer to current cell in bucket chain */

    public:

        /*
         * Iterator constructor:
         * ----------------------
         * Defalut constuctor.
         * During MyMap object initiation, this
         * defalut iterator is created too.
         */
        iterator() : mp(NULL), bucket(0), cp(0) {
            /* Empty */
        }

        /*
         * Iterator constructor:
         * ----------------------
         * Implements begin and end iterators creation.
         *
         * Begin iterator points on the start element
         * of the container.
         * End iterator points on the first element
         * out of container.
         * While iterator object usage suposed in means
         * of MyMap trversing - this method constructs
         * real objects.
         *
         * @param mp    pointer to this MyMap object
         * @param end   true if the end iterator
         *              construction called
         */
        iterator(const MyMap* mp, bool end) {
            /* Tight connection with particular MyMap object. */
            this->mp = mp;
            if (end) {//If end flag - create end iterator
                /* Put iterator on the last map bucket */
                bucket = mp->nBuckets;
                /* In this bucket - put iterator on the external cell */
                cp = NULL;
            } else {//If end flag is false - create begin iterator
                /* Put iterator on the first map bucket */
                bucket = 0;
                /* This buckets cell stores pointer on the first list pair.
                 * Put iterator on it. */
                cp = mp->buckets[bucket];
                /* If this pair dosen't store anything - search
                 * next useful bucket. */
                bucket++;
                while ((cp == NULL) && (bucket < mp->nBuckets)) {
                    /* Check the pair binded with this bucket pair
                     * while we don't find first existed pair in map */
                    cp = mp->buckets[bucket];
                    bucket++;
                }
            }
        }

        /*
         * Method: copy constructor
         * ----------------------
         * Provides correct passing of iterator
         * by value, or new iterator initiation,
         * if it needs for current MyMap object.
         */
        iterator(const iterator& it) {
            mp = it.mp;
            bucket = it.bucket;
            cp = it.cp;
        }

        /*
         * Method: pre-increment operator ++
         * Usage: ++iter;
         * ---------------------------------
         * Moves iterator into the next map pair,
         * and then, returns this iterator reference.
         */
        iterator& operator++() {
            cp = cp->link;//Move this iterator to the next pair
            while ((cp == NULL) && (++bucket < mp->nBuckets)) {
                /* If the pair binded with this bucket isn't
                 * exist - move iterator to next bucket*/
                cp = mp->buckets[bucket];
            }
            return *this;//Get iterator reference
        }

        /*
         * Method: post-increment operator ++
         * Usage: iter++;
         * ---------------------------------
         * Returns copy of iterator value, which
         * is put on the prvious map pair. Before this,
         * current iterator is moved into
         * the next map pair.
         */
        iterator operator ++(int) {
            iterator copy(*this);
            operator++();
            return copy;
        }

        /*
         * Method: comparison operators
         * Usage: iter1 != iter2
         * ---------------------------
         * Returns true or false if
         * current iterators point into
         * the same map pair, due to it's
         * position in buckets ctructure.
         */
        bool operator ==(const iterator& rhs) {
            return mp == rhs.mp && bucket == rhs.bucket && cp == rhs.cp;
        }

        bool operator !=(const iterator& rhs) {
            return !(*this == rhs);
        }

        /*
         * Method: indirection Operator
         * Usage: KeyType key = *iter;
         * ---------------------------
         * Returns key value for pair
         * which iterator points currently
         */
        KeyType& operator *() {
            return cp->key;
        }

        /*
         * Method: member access operator
         * ------------------------------
         * Returns pointer on the key value for pair
         * which iterator points currently
         */
        KeyType* operator ->() {
            return &cp->key;
        }

        /* Friends */
        friend class MyMap;
    };

    /*
     * Method: begin
     * Usage: MyMap<KeyType, ValueType>::iterator iter = mmp.begin()
     * --------------------------------------------------------------
     * Returns iterator object pointed
     * on the start pair of the map.
     */
    iterator begin() const {
        return iterator(this, false);
    }

    /*
     * Method: end
     * Usage: if (iter == mmp.end()){...}
     * -----------------------------------
     * Returns iterator object pointed
     * on on the first element out of the map.
     *
     * It uses only for "end of map" checking
     * condition, to stop iteration etc.
     */
    iterator end() const {
        return iterator(this, true);
    }

/* -----------------------------------------------------------------------------------------//
 * Private section
 * -----------------------------------------------------------------------------------------*/
private:
    /*
     * Implementation notes: MyMap internal structure
     * -------------------------------------------------
     * The MyMap class is represented using a hash table that uses
     * bucket chaining to resolve collisions.
     * The user entries are stored in Pair - simple storage element.
     * Connection between pairs is realised by unidirected links to
     * next cell. New Pair is on the top of the list. Top is binded with bucket.
     * Buckets - it's high level array which divide map into several single lists.
     * For any user key hash code is calculated. This code
     * determines actual bucket which will bind this Pair.
     *
     * Buckets are stored in heap array. It make memory
     * management more complicated, but it was taken
     * to study heap arrays principles more.
     */

    /* Instance variables */

    /* Type: Pair
     * -----------
     * Main storage object for user values.
     */
    struct Pair{
         KeyType key;
         ValueType data;
         Pair* link;

         bool operator==(const Pair & n1){
                  return (key == n1.key)
                          && (data == n1.data)
                          && (link == n1.link);
         }
     };

    static const int INITIAL_BUCKET_COUNT = 30; //Initial buckets array length.
    Pair ** buckets;                //High level array of pointers to Pairs.
    int nBuckets;                   //Current buckets array length - for rehashing purpose.
    int numElements;                //Quantity of inputed user entries.

    /*
     * Private method: findCell
     * ------------------------
     * Finds Pair into the bucket list with such key.
     * Returns pointer on the Pair element with param key
     * in current bucket list. If such key entry isn't exist
     * in list - returns NULL.
     * Traverses through every element of this list.
     */
    Pair *findCell(int bucket, const KeyType& key) const{
        /* Takes pointer on the binded pair */
        Pair *cp = buckets[bucket];
        /* Starting from this pair - traverses through list */
        while((cp != NULL) && (cp->key != key)){
            cp = cp->link;
        }
        return cp;
    }

    /*
     * Private method: deepCopy
     * ------------------------
     * Implements deep copy process
     */
    void deepCopy(const MyMap& src);
};


/* -----------------------------------------------------------------------------------------//
 * Implementation section
 * -----------------------
 * As the class is templated, the implementation part has
 * to be written in the same header with interface.
 * -----------------------------------------------------------------------------------------*/

/*
 * Implementation notes: MyMap constructor
 * ---------------------------------------
 * At first heap array of Pair pointers is created -
 * buckets chain, a kind of top level array. There
 * are no objects to point them so,
 * the empty map at least posesses such memory resource.
 */
template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() {
    nBuckets = INITIAL_BUCKET_COUNT;
    buckets = new Pair*[nBuckets];
    for(int i = 0; i < nBuckets; i++){
        buckets[i] = NULL;
    }
    numElements = 0;
}

/*
 * Implementation notes: MyMap destructor
 * ---------------------------------------
 * At first it traverses through every bucket
 * and delete all Pair cells in current bucket list.
 * Then it deletes pointers array - buckets[].
 */
template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() {
    std::cout << "Destructor " << std::endl;
    for(int i = 0; i < nBuckets; i++){
        Pair * cp = buckets[i];
        while(cp != NULL){
            Pair * oldPair = cp;
            cp = cp->link;
            delete oldPair;
        }
    }
    /* Delete the top-level array. */
    delete[] buckets;
}

/*
 * Implementation notes: get
 * ---------------------------
 * Calls findCell function to find out if
 * this entry exist in map.
 * Makes error if no pair with such key.
 */
template <typename KeyType, typename ValueType>
ValueType MyMap<KeyType, ValueType>::get(const KeyType& key) const {
    int bucket = hashCode(key) % nBuckets;
    Pair * cp = findCell(bucket, key);
    if(cp == NULL)error("get: No pair for this key: ");
    return (cp == NULL) ? ValueType() : cp->data;
}

/*
 * Implementation notes: put
 * ---------------------------
 * If such key exist in map - udate it's value.
 * If not - bind new pair with buckets[] cell.
 */
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::put(const KeyType& key, const ValueType& value) {
    int bucket = hashCode(key) % nBuckets;
    /* Discover if such key exist in those bucket yet */
    Pair *cp = findCell(bucket, key);
    /* This key might already be here, in which case we should look up
     * the key/value pair for it and update the value.
     */
    if(cp == NULL){
        /* If we got here, the key isn't present and thus must be inserted.
         * New cell will be binded with buckets[] cell and her link will
         * be old cell in this list */

        /* Create a new cell to hold the key/value pair. */
        cp = new Pair;
        cp->key = key;
        /* Link for new entry is old link from buckets array cell */
        cp->link = buckets[bucket];
        /* Now buckets array cell points right on the new entry cell */
        buckets[bucket] = cp;
    }
    cp->data = value;//Upate value.
    //std::cout << "bucket " << bucket << ": [" << cp << "] - " << cp->link << std::endl;
    numElements++;
}

/*
 * Implementation notes: clear
 * ---------------------------
 * From bucket[0] through all buckets traverses
 * through all buckets lists to delete all Pair cells
 */
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() {
    for(int i = 0; i < nBuckets; i++){
        Pair * cp = buckets[i];
        while(cp != NULL){
            Pair * oldPair = cp;
            cp = cp->link;
            delete oldPair;
        }
    }
   numElements = 0;
}

/*
 * Implementation notes: containsKey
 * -----------------------------------------
 * Gets hashCode for such key, to obtain actual bucket in map.
 * Through this bucket list find Pair with such param key.
 */
template <typename KeyType, typename ValueType>
bool MyMap<KeyType, ValueType>::containsKey(const KeyType& key) const {
    int bucket = hashCode(key) % nBuckets;
    Pair *cp = findCell(bucket, key);
    if(cp == NULL){
        return false;
    }else{
        return true;
    }
}

/*
 * Implementation notes: MyMap [] selection
 * -----------------------------------------
 * Implement traditional sort of access due to
 * square brackets for the index.
 * Overloads [] to select and return element without
 * modification.
 */
template <typename KeyType, typename ValueType>
 ValueType MyMap<KeyType, ValueType>::operator [](const KeyType& key) const {
    return get(key);
}

/*
 * Implementation notes: copy constructor
 * --------------------------------------
 * The copy constructor provides deep copying to pass MyMap
 * objects by value, or to initiate new objects.
 */
template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap(const MyMap<KeyType, ValueType>& src) {
    deepCopy(src);
    //std::cout << "Copy constructor " << std::endl;
}

/*
 * Implementation notes: assignment operator
 * -----------------------------------------
 * Provides deep copying to assign one existed MyMap object
 * to another.
 * To avoid self deleting before copying it checks param map.
 *
 * This declaration could be internal class function, like this:
 * MyMap& operator =(const MyMap& src){...}
 */
template <typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>& MyMap<KeyType, ValueType>::operator =(const MyMap& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
        //std::cout << "Assignment oper " << std::endl;
    }
    return *this;
}

/*
 * Implementation notes: mapAll
 * ----------------------------
 * Traverses through the map lists and call
 * functor function to every entry.
 */
template <typename KeyType, typename ValueType>
template <typename FunctorType>
void MyMap<KeyType, ValueType>::mapAll(FunctorType fn) const {
    for (int i = 0; i < nBuckets; i++) {
        for (Pair* cp = buckets[i]; cp != NULL; cp = cp->link) {
            fn(cp->key, cp->data);
        }
    }
}

/*
 * Private function: deepCopy
 * -----------------------------------------
 * Copies data from src param into the current object.
 * All dynamic memory is reallocated to create a "deep copy"
 * of param array. Erases all current internal  fields.
 */
template <typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::deepCopy(const MyMap& src) {
    nBuckets = src.nBuckets;
    buckets = new Pair*[nBuckets];
    for(int i = 0; i < nBuckets; i++){
        /* Initiates array pointers */
        buckets[i] = NULL;
    }
    numElements = 0;//Put method will set right value.
    /* Run through the src map buckets and put elements to this */
    for (int i = 0; i < nBuckets; i++) {
        for (Pair * cp = src.buckets[i]; cp != NULL; cp = cp->link) {
            put(cp->key, cp->data);
        }
    }
}


#endif
