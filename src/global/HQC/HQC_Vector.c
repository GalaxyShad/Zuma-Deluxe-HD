#include "../HQC.h"

#define VECTOR_INITIAL_CAPACITY 8


typedef struct Vector {
    size_t capacity;

    size_t elementSize;
    void*  elements;
    size_t elementsCount;
} Vector;

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

static Vector* _CastToVector(HQC_VectorContainer vector) { 
    if (!vector)
        HQC_RaiseErrorHeaderFormat("HQC Vector", "Invalid Vector instance %p", vector);

    return (Vector*)vector; 
}


static void _Expand(Vector* vector) {
    vector->elementsCount++;

    if (vector->elementsCount <= vector->capacity)
        return;

    vector->capacity *= 2;
    vector->elements = HQC_Memory_Reallocate(vector->elements, vector->elementSize * vector->capacity);
}


static void _HandleOutOfRange(Vector* vector, int index) {
    if (index < 0)
        HQC_RaiseErrorHeaderFormat("HQC Vector", "Index of element cannot be less then 0");

    if (index >= vector->elementsCount)
        HQC_RaiseErrorHeaderFormat(
            "HQC Vector",
            "Index of element (%d) cannot be greater then count of elements in vector (%d)",
            index, vector->elementsCount
        );
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

HQC_VectorContainer HQC_Container_CreateVector(size_t elementSize) {
    Vector* vector = HQC_Memory_Allocate(sizeof(*vector));

    vector->capacity        = VECTOR_INITIAL_CAPACITY;
    vector->elementSize     = elementSize;
    vector->elements        = HQC_Memory_Allocate(vector->elementSize * vector->capacity);
    vector->elementsCount   = 0;

    return vector;
}


void HQC_Container_VectorAdd(HQC_VectorContainer vector, void* element) {
    Vector* vec = _CastToVector(vector);

    _Expand(vec);

    size_t memShift = (vec->elementsCount-1) * (vec->elementSize);

    HQC_Memory_Copy((char*)vec->elements + memShift, element, vec->elementSize);
}


void* HQC_Container_VectorGet(HQC_VectorContainer vector, int index) {
    Vector* vec = _CastToVector(vector);
    
    _HandleOutOfRange(vec, index);

    char* res = vec->elements;

    return (void*)(res + index * vec->elementSize);
}


size_t HQC_Container_VectorCount(HQC_VectorContainer vector) {
    Vector* vec = _CastToVector(vector);
    return vec->elementsCount;
}


void HQC_Container_FreeVector(HQC_VectorContainer vector) {
    Vector* vec = _CastToVector(vector);

    HQC_Memory_Free(vec->elements);

    vector = NULL;
}