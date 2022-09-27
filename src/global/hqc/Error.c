#include "Error.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void HQC_PushError(const char* errorMessage) {
    HQC_PushErrorF(errorMessage);
}

void HQC_PushErrorF(char const* fmtErrorMessage, ...) {
    va_list ap;
    va_start(ap, fmtErrorMessage);
    printf("[HQC][ERR] ");
    printf(fmtErrorMessage, ap);
    va_end(ap);

    exit(7);
}