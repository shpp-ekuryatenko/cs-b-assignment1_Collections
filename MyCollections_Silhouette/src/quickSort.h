/********************************************************************************************
* File: quickSort.h
* -----------------
* v.1 2015/12/25 - code reformatted
*
* This file implements ascend MyVector quickSort
* function  for primitives.
********************************************************************************************/
#ifndef _quickSort_h
#define _quickSort_h

#include "MyVector.h"

using namespace std;

template <typename ValueType>
int partition(MyVector<ValueType>& vec, int start, int finish){
    int pivot = vec[start];//Choose pivot for current sorting
    int lh = start + 1;    //Initiates left hand and right hand marker positions
    int rh = finish;
    while (true) {         //Main sorting cycle
        while ((lh < rh) && (vec[rh] >= pivot)) { //Main indexes movement
            rh--;
        }

        while ((lh < rh) && (vec[lh] < pivot)) {
            lh++;
        }

        /* End of main cycle */
        if (lh >= rh) {
            break; //Indexes meet at same vector cell
        }

        int tmp = vec[lh]; //Sorting exchange of markers cells values
        vec[lh] = vec[rh];
        vec[rh] = tmp;
    }

    /* Pivot placing process */
    /* Case 1 - start index cell stores the less value  */
    if (pivot <= vec[lh]) {
        return start;//start index is boundary to return
    }
    /* Case 2 - start index cell dosen't store the lowest value: pivot > vec[lh] */
    vec[start] = vec[lh];//Exchanges values in satart cell and last lh marker cell
    vec[lh] = pivot;
    return lh;           //Return lh marker index as boundary
}

template <typename ValueType>
void ascendQuickSort(MyVector<ValueType>& vec, int start, int finish){
    if (start < finish) {
        int boundary = partition(vec, start, finish);
        ascendQuickSort(vec, start, boundary - 1);
        ascendQuickSort(vec, boundary + 1, finish);
    }
}
#endif
