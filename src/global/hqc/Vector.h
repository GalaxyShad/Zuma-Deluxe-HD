#pragma once

#include <stdint.h>

#include "Typedefs.h"

typedef HQC_UInt32 HQC_HVector;

HQC_HVector HQC_NewVector(size_t sizeOfItem);
void        HQC_VectorAppend(HQC_HVector vector, const void* item);
void        HQC_VectorRemoveAt(HQC_HVector vector, HQC_UInt32 index);
void        HQC_VectorPop(HQC_HVector vector);
const void* HQC_VectorGet(HQC_HVector vector, HQC_UInt32 index);
void        HQC_VectorClear(HQC_HVector vector);
HQC_UInt32  HQC_VectorCount(HQC_HVector vector);
void        HQC_DestroyVector(HQC_HVector vector);