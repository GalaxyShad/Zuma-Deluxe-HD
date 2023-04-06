#include "../HQC.h"

#include <SDL2/SDL.h>
#include <stdarg.h>

void HQC_RaiseErrorFormat(const char* format, ...) {
    va_list args;
    va_start(args, format);
    HQC_RaiseErrorHeaderFormat(NULL, format, args);
    va_end(args);
}


void HQC_RaiseErrorHeaderFormat(const char* header, const char* format, ...) {
    va_list args;
    va_start(args, format);

    char errMsg[512];

    sprintf(errMsg, format, args);

    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        header ? header : "Fatal",
        errMsg,
        NULL
    );

    va_end(args); 

    exit(1);
}