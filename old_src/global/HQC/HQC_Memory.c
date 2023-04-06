#include "../HQC.h"

// Allocator
void* HQC_Memory_Allocate(size_t memSize) {
    void* mem = malloc(memSize);

    if (mem == NULL) 
        HQC_RaiseErrorHeaderFormat(
            "HQC_Allocate",
            "Cannot allocate memmory with size %lld",
            memSize
        );
    
    return mem;
}


void HQC_Memory_Free(void* pmem) {
    if (pmem == NULL) return;
    free (pmem);
}