// #include "Application.h"
#include "global/HQC.h"

#include <stdio.h>

int main(int argc, char** args) { 
    HQC_HVector arr = HQC_NewVector(sizeof(int));

    int a = 2, b = 5, c = 1, d = 90;
    HQC_VectorAppend(arr, &a);

    int* aa = (int*)HQC_VectorGet(arr, 0);
    *aa = 23;
    HQC_VectorAppend(arr, &a);
    HQC_VectorAppend(arr, &a);
    HQC_VectorAppend(arr, &d);

    HQC_VectorClear(arr);
    HQC_VectorAppend(arr, &a);

    for (int i = 0; i < HQC_VectorCount(arr); i++)
        printf("%d\n", *(int*)HQC_VectorGet(arr, i));

    for (int i = 0; i < HQC_VectorCount(arr); i++)
        HQC_VectorPop(arr);

    for (int i = 0; i < 1000; i++) {
        HQC_NewVector(sizeof(int));
    }

    return 0;
    // return ApplicationZuma_Start();
}