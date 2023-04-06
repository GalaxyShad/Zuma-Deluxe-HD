#include "../HQC.h"

#include <string.h>


// Strings
size_t HQC_StringSize(const char* src) {
    if (src == NULL) HQC_RaiseErrorFormat("[HQC_StringLength] Bad string");

    size_t counter = 0;
    for (int i = 0; i < 32687; i++) {
        counter++;
        if (src[i] == '\0') return counter;
    }

    HQC_RaiseErrorFormat("[HQC_StringLength] Buffer overflow. No null terminator found");
}


size_t HQC_StringLength(const char* src) {
    return HQC_StringSize(src) - 1;
}


const char*  HQC_StringClone(const char* src) {
    size_t size = HQC_StringLength(src);

    return (const char*)HQC_Memory_Allocate(size);
}


const char*  HQC_StringConcat(const char* strA, const char* strB) {
    size_t sizeStrA = HQC_StringSize(strA) - 1;
    size_t sizeStrB = HQC_StringSize(strB) - 1;

    char* newString = HQC_Memory_Allocate(sizeStrA+sizeStrB+1);

    for (int i = 0; i < sizeStrA; i++) newString[i] = strA[i];

    for (int i = 0; i < sizeStrB+1; i++) newString[sizeStrA+i] = strB[i];

    return newString;
}