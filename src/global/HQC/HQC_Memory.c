#include "../HQC.h"

#include <string.h>
#include <stdlib.h>

// Allocator
void* HQC_Memory_Allocate(size_t memSize) {
    void* mem = malloc(memSize);

    if (mem == NULL) 
        HQC_RaiseErrorHeaderFormat(
            "HQC_Memory_Allocate",
            "Cannot allocate memmory with size %lld",
            memSize
        );
    
    return mem;
}


void* HQC_Memory_Copy(void* dst, void* src, size_t size) {
    return memcpy(dst, src, size);
}


void* HQC_Memory_Reallocate(void* src, size_t newMemSize) {
    void* mem = realloc(src, newMemSize);

    if (mem == NULL) 
        HQC_RaiseErrorHeaderFormat(
            "HQC_Memory_Reallocate",
            "Cannot reallocate memmory with size %lld",
            newMemSize
        );
    
    return mem;
}


void HQC_Memory_Free(void* pmem) {
    if (pmem == NULL) return;
    free (pmem);
}