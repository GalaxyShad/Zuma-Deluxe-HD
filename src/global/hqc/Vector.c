#include "Vector.h"

#include "Error.h"

#include <stdlib.h>
#include <string.h>

#define PAGE_VECTORS 128

typedef struct HQC_Vector {
    HQC_UInt32 mCount;
    void* mItems;
    size_t mItemSize;
} HQC_Vector;


static HQC_UInt32 _HQC_VectorHandlesCount = 0;
static HQC_Vector _HQC_Vectors[PAGE_VECTORS]; 


static void _HQC_VectorCheckIndexRange(HQC_HVector vector, HQC_UInt32 index) {
    HQC_Vector* v = &_HQC_Vectors[vector];

    if (index >= v->mCount)
        HQC_PushErrorF("[Vector Hook #%x] Index %d out of Range ", vector, index);
}


HQC_HVector HQC_NewVector(size_t sizeOfItem) {
    HQC_UInt32 index = _HQC_VectorHandlesCount;

    if (index >= PAGE_VECTORS) 
        HQC_PushError("Vector table hooks overflow");

    _HQC_VectorHandlesCount++;

    HQC_Vector* vector = &_HQC_Vectors[index];
    
    vector->mCount = 0;
    vector->mItems = NULL;
    vector->mItemSize = sizeOfItem;

    return index;
}


void HQC_VectorAppend(HQC_HVector vector, const void* item) {
    HQC_Vector* v = &_HQC_Vectors[vector];

    v->mCount++;
    v->mItems = realloc(v->mItems, v->mItemSize * v->mCount);
    if (!v->mItems) 
        HQC_PushErrorF("[Vector Hook #%x] Cannot allocate memmory", vector);

    memcpy(v->mItems + (v->mCount-1) * v->mItemSize, item, v->mItemSize);
}


void HQC_VectorRemoveAt(HQC_HVector vector, HQC_UInt32 index) {
    _HQC_VectorCheckIndexRange(vector, index);

    HQC_Vector* v = &_HQC_Vectors[vector];
    
    if (v->mCount == 1) {
        HQC_VectorClear(vector);
        return;
    }

    for (int i = index; i < v->mCount-1; i++) {
        memcpy(
            v->mItems + v->mItemSize * i,
            v->mItems + v->mItemSize * (i + 1),
            v->mItemSize
        );
    }

    v->mCount--;

    v->mItems = realloc(v->mItems, v->mItemSize * v->mCount);

    if (!v->mItems) 
        HQC_PushErrorF("[Vector Hook #%x] Cannot reallocate memmory", vector);
}


void HQC_VectorPop(HQC_HVector vector) {
    HQC_VectorRemoveAt(vector, _HQC_Vectors[vector].mCount - 1);
}


const void* HQC_VectorGet(HQC_HVector vector, HQC_UInt32 index) {
    _HQC_VectorCheckIndexRange(vector, index);

    HQC_Vector* v = &_HQC_Vectors[vector];

    return v->mItems + index * v->mItemSize;
}

void HQC_VectorClear(HQC_HVector vector) {
    HQC_Vector* v = &_HQC_Vectors[vector];

    free(v->mItems);
    v->mItems = NULL;
    v->mCount = 0; 
}


HQC_UInt32 HQC_VectorCount(HQC_HVector vector) {
    return _HQC_Vectors[vector].mCount;
}


void HQC_DestroyVector(HQC_HVector vector) {
    HQC_Vector* v = &_HQC_Vectors[vector];

    HQC_VectorClear(vector);
    v->mItemSize = 0; 
}

